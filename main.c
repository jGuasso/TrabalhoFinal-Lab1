#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INT_MAX 2147483647
#define TAM_MAX_LINHA 256
#define TAM_MAX_PALAVRA 50

int fim_de_frase(char caracter);
void limparpalavra(char palavra[]);
int retira(char linha[],char termo[]);
void substituir_palavra(char linha[],char novo[],int pos,int tam_antigo);
int tamanho_letras(char palavra[]);
int numero_extenso(char linha[],int pos, int num);
void espacosrepetidos(char linha[]);
int perguntar(char linha[],char copialinha[],char regra[]);
int sinalaberto_e_nao_fechados(char linha[], int pos,int primeiro);
int mesmo_sinal(char linha[], int pos,int primeiro);
int ehcaracter_especial(char c);
int ehmaiuscula(char c);
int ehdigito(char c);
int verifica_dicio(FILE *dicio, char palavra[]);
int nivel_semelhanca(char palavradicio[], char palavra[]);
int semelhanca(FILE *dicio, char palavra[], char linha[]);

//Função que verifica se o caracter encerra a frase
int fim_de_frase(char caracter){
    return (caracter=='.' || 
            caracter=='!' ||
            caracter=='?');
}

//Função que deixa a palavra somente com letras
void limparpalavra(char palavra[]){
    sscanf(palavra,"%[a-zA-Z]",palavra);
}

//Função que retira um termo de uma linha, independente do que está a sua volta
//Para retirar termos que podem estar no meio de palavras: "Bo@l%a" -> "Bola"
int retira(char linha[],char termo[]){

    int retorno=0;
    char *pos;
    int tam = strlen(termo);

    while ((pos = strstr(linha, termo)) != NULL) {
        memmove(pos, pos + tam, strlen(pos + tam) + 1);
        retorno=1;
    }
    return retorno;
}

//Função que substitui um termo antigo por um novo, a partir do seu tamanho e posicao
void substituir_palavra(char linha[],char novo[],int pos,int tam_antigo){
    char temp[TAM_MAX_LINHA];
    temp[0] = '\0';
    strncpy(temp,linha,pos);
    temp[pos] = '\0';
    strcat(temp,novo);
    strcat(temp,linha+pos+tam_antigo);
    strcpy(linha,temp);
}

//Função que verifica se todas as letras da palavra são do mesmo tamanho,
//desconsiderando se apenas o primeiro caracter for maiusculo
int tamanho_letras(char palavra[]){
    int tam, cont_maiusculas=0, cont_minusculas=0,caixa_alta;
    int retorno = 0;
    tam=strlen(palavra);
    int i;
    for ( i = 0; i < tam; i++)
    {
        if(palavra[i]>='A'&&palavra[i]<='Z') cont_maiusculas++;
        if(palavra[i]>='a'&&palavra[i]<='z') cont_minusculas++;
    }
    if(cont_minusculas>=cont_maiusculas) caixa_alta = 0;
    else caixa_alta = 1;
    
    if(caixa_alta){        
        for ( i = 0; i < tam; i++)
        {
            if(palavra[i]>='a'&&palavra[i]<='z'){
            palavra[i]-=32;
            retorno = 1;
            }
        }
    }
    else
    {
        for ( i = 1 ; i < tam; i++)
        {
            if(palavra[i]>='A'&&palavra[i]<='Z'){ 
            palavra[i]+=32;
            retorno = 1;
            }
        }
    }
    return retorno;
    
}

//Função que substitui números de 0 a 10 por sua escrita por extenso
int numero_extenso(char linha[],int pos, int num){
    char str[10];
    int tam;
    if(num<11){
        switch (num)
        {
        case 0:
            sprintf(str,"zero");
            tam=1;
            break;
        case 1:
            sprintf(str,"um");
            tam=1;
            break;
        case 2:
            sprintf(str,"dois");
            tam=1;
            break;
        case 3:
            sprintf(str,"tres");
            tam=1;
            break; 
        case 4:
            sprintf(str,"quatro");
            tam=1;
            break; 
        case 5:
            sprintf(str,"cinco");
            tam=1;
            break;
        case 6:
            sprintf(str,"seis");
            tam=1;
            break;
        case 7:
            sprintf(str,"sete");
            tam=1;
            break;
        case 8:
            sprintf(str,"oito");
            tam=1;
            break;
        case 9:
            sprintf(str,"nove");
            tam=1;
            break;
        case 10:
            sprintf(str,"dez");
            tam=2;
            break;
        }
        substituir_palavra(linha,str,pos,tam);
        return 1;
    }
    else return 0;

}

int ehcaracter_especial(char c){
    char especiais[6]={
        '@','\x25','#','&','*','$'};
    for (int i = 0; i < 6; i++)
    {
        if (c==especiais[i])
        {
            return 1;
        }
    }
    return 0;
}

//Função que remove espaços repetidos
void espacosrepetidos(char linha[]){
    int tam = strlen(linha),espacos;
    char copialinha[TAM_MAX_LINHA];
    strcpy(copialinha,linha);
    for (int i = 0; i < tam; i++)
    {
        espacos=0;
        for (int j = i;linha[j]==' '; j++)espacos++; 
        
        if(espacos>1)
        {
            substituir_palavra(linha,"",i,espacos-1);
            perguntar(copialinha,linha,
            "Correcao:Excessos de espacos em branco consecutivos (mais do que um) devem ser descartados.");
            tam--;
        }
    }
}

//Função para confirmar alterações
int perguntar(char linha[],char copialinha[],char regra[]){
    while (1)
    {
        char verificador;
        printf("\ntrecho original:\" %s \"\n",linha);
        printf("\ntrecho alterado:\" %s \"\n",copialinha);
        printf("\n%s\n",regra);
        printf("\nAplicar? (s/n)\n");
        while ((getchar()) != '\n'); //limpar o buffer
        scanf("%c",&verificador);
        if(verificador=='s'||verificador=='S'){
            strcpy(linha,copialinha);
            return 1;
        }
        if(verificador=='n'||verificador=='N'){
            strcpy(copialinha,linha);
            return 0;
        }
    }
}

//Verifica se todos sinais abertos são fechados
int sinalaberto_e_nao_fechados(char linha[], int pos,int primeiro){
    int pos_aberto,s_aberto=0;
    int tam = strlen(linha);
    char copialinha[TAM_MAX_LINHA];
    strcpy(copialinha,linha);
    for ( pos=pos; pos < tam; pos++)
    {
        //abertura
        if(linha[pos]=='('||
           linha[pos]=='{'||
           linha[pos]=='['
        )
        {
            if (s_aberto)
            {
                pos = sinalaberto_e_nao_fechados(linha,pos,0);
            }
            else
            {
                pos_aberto = pos;
                s_aberto=1;
            }
        }
        //acaba frase
        if(s_aberto &&
           (fim_de_frase(linha[pos]) ||
            pos == tam-1))
        {
            substituir_palavra(linha,"",pos_aberto,1);
            s_aberto=0;
            perguntar(copialinha,linha,
            "Correcao: Sinais abertos e nao fechados antes de um final de frase devem ser descartados. Da mesma forma, sinais fechados sem terem sido abertos devem ser descartados.");
            if(!primeiro)return (pos-1);
        }

        //fechamento
        if(linha[pos]==')'||
           linha[pos]=='}'||
           linha[pos]==']'
        )
        {
            if (s_aberto)
            {
                s_aberto=0;
                if (!primeiro)
                {
                    return(pos+1);
                }
            }
            else
            {
                substituir_palavra(linha,"",pos,1);
                perguntar(copialinha,linha,
                "Correcao: Sinais abertos e nao fechados antes de um final de frase devem ser descartados. Da mesma forma, sinais fechados sem terem sido abertos devem ser descartados.");

                pos--;
            }
        }

    }
    return 0;
}

//verificia se o sinal de abertura e de fechamento são os mesmos
int mesmo_sinal(char linha[], int pos,int primeiro){
    int s_aberto=0;
    int tam = strlen(linha);
    char sinalabertura;
    char copialinha[TAM_MAX_LINHA];
    strcpy(copialinha,linha);
    for ( pos=pos; pos < tam; pos++)
    {
        //abertura
        if(linha[pos]=='('||
           linha[pos]=='{'||
           linha[pos]=='['
        )
        {
            if (s_aberto)
            {
                pos = mesmo_sinal(linha,pos,0);
            }
            else
            {
                sinalabertura = linha[pos];
                s_aberto=1;
            }
        }

        //fechamento
        if(linha[pos]==')'||
           linha[pos]=='}'||
           linha[pos]==']'
        )
        {
			switch (sinalabertura)
			{
			case '(':
				linha[pos]= ')';
				break;
			case '{':
				linha[pos]= '}';
				break;
			case '[':
				linha[pos]= ']';
				break;
			}
            perguntar(copialinha,linha,
            "Correcao: Os sinais de abertura e fechamento devem ser os mesmos.");
			s_aberto=0;
			if (!primeiro)
			{
				return(pos+1);
			}
        }

    }
    return 0;
}

int ehmaiuscula(char c){
    if(c>='A'&&
       c<='Z')
        return 1;
    else
        return 0;
}

int ehdigito(char c){
    if(c>=48&&
       c<=57)
        return 1;
    else
        return 0;
}

//verifica se a palavra consta no dicionário
int verifica_dicio(FILE *dicio, char palavra[]){

    if(ehmaiuscula(palavra[0])) return 1; //nomes pessoais e acrônimos

    char palavradicio[TAM_MAX_PALAVRA];
    fseek(dicio, 0, SEEK_SET);
    while (fgets(palavradicio,TAM_MAX_PALAVRA,dicio)!=0)
    {
        retira(palavradicio,"\n");
        if(strcmp(palavra,palavradicio)==0) return 1;

    }

    return 0;
}

//verifica quantas letras precisam ser retiradas da palavra para ficar igual à palavradicio
int nivel_semelhanca(char palavradicio[], char palavra[]){
    char copiapalavra[TAM_MAX_PALAVRA];
    int tam;
    int nivel=0;
    strcpy(copiapalavra,palavra);
    tam = strlen(palavra);
    for ( int i = 0; i < tam; i++)
    {
        if (tam<=0)return INT_MAX;
        
        if (palavra[i]!=palavradicio[i])
        {
            nivel++;
            for (int j = i; j < tam; j++)
            {
                palavra[j]=palavra[j+1];
            }
            i--;
            tam--;
        }
    }
    
    if (strcmp(palavra,palavradicio)!=0)
    {
        return INT_MAX;
    }
    return nivel;
}

//verifica se existe palavra semelhante no dicionario
int semelhanca(FILE *dicio, char palavra[], char linha[]){
    int pontos_palavras[3]={INT_MAX,INT_MAX,INT_MAX},pontos,i,j;
    char toppalavras[3][TAM_MAX_PALAVRA]={" "," "," "},palavradicio[TAM_MAX_PALAVRA],copiapalavra[TAM_MAX_PALAVRA];
    fseek(dicio, 0, SEEK_SET);
    while (fgets(palavradicio,TAM_MAX_PALAVRA,dicio)!=0)
    {
        retira(palavradicio,"\n");
        strcpy(copiapalavra,palavra);
        pontos=nivel_semelhanca(palavradicio,copiapalavra);
        for (i = 0; i < 3; i++)
        {  
            if(pontos<pontos_palavras[i]){
                for ( j = i; j < 2; j++)
                {
                    strcpy(toppalavras[j+1],toppalavras[j]);
                }
                pontos_palavras[i]=pontos;
                strcpy(toppalavras[i],palavradicio);
                break;
            }
        }
    }
    //se n tiver nenhum semelhante nem precisa perguntar
    if (pontos_palavras[0]==INT_MAX &&
        pontos_palavras[1]==INT_MAX &&
        pontos_palavras[2]==INT_MAX) return 0;
    
    while(1)
    {
        char verificador;
        printf("\nNo trecho:\n%s\n",linha);
        printf("\nDeseja alterar a palavra para alguma opcao existente no dicionario?\n");
        printf("\npalavra original:\" %s \"\n",palavra);
        printf("\nOpcoes:\n1:\" %s \"\n2:\" %s \"\n3:\" %s \"\n",toppalavras[0],toppalavras[1],toppalavras[2]);
        printf("\nDigite o numero da opcao ou digite 'n' para nao alterar\n");
        while ((getchar()) != '\n'); 
        scanf("%c",&verificador);
        switch (verificador)
        {
        case '1':
            strcpy(palavra,toppalavras[0]);
            return 1;
            break;
        case '2':
            strcpy(palavra,toppalavras[1]);
            return 1;
            break;
        case '3':
            strcpy(palavra,toppalavras[2]);
            return 1;
            break;
        case 'n':
        case 'N':
            return 0;
            break;
        }
    }
}

int main(int argc, char* argv[]){
    if (argc!=3)
    {
        printf("Erro: quantidade de argumentos invalida!\n");
        return 1;
    }
    char copiapalavra[TAM_MAX_PALAVRA],palavra[TAM_MAX_PALAVRA],
    dicio_filename[100],text_fileName[100],
    new_filename[100],linha[TAM_MAX_LINHA],copialinha[TAM_MAX_LINHA];

    FILE *texto;
    FILE *dicio;
    FILE *novo;

    int offset = 0;

    strcpy(text_fileName,argv[1]);
    strcpy(dicio_filename,argv[2]);

    //Cria o novo arquivo com a data e o horário
    time_t agora;
    time(&agora);
    struct tm *tempo_atual;
    tempo_atual = localtime(&agora);
    sprintf(new_filename,"%s_%02d_%02d_%04d_%02dh%02dm%02ds.txt",
           text_fileName,
           tempo_atual->tm_mday, 
           tempo_atual->tm_mon + 1, 
           tempo_atual->tm_year + 1900, 
           tempo_atual->tm_hour, 
           tempo_atual->tm_min, 
           tempo_atual->tm_sec);

    sprintf(text_fileName,"%s.txt",text_fileName);
    sprintf(dicio_filename,"%s.txt",dicio_filename);
    
    if((texto = fopen(text_fileName, "r+"))==NULL){
        printf("\nNao foi possivel abrir o seu arquivo de texto: %s\n",text_fileName);
        return 1;
    }
    dicio = fopen(dicio_filename, "a+");
    novo = fopen(new_filename,"w");

    int tamanho_palavra,i,inicio_frase=1;
    int pergunta=0;

    while (!feof(texto))
    {
        fgets(linha,TAM_MAX_LINHA,texto);
        strcpy(copialinha,linha);
        //REGRA 6
        for(offset=0;linha[offset]!='\0';offset++)
        {
            if (ehcaracter_especial(linha[offset]))
            {
                substituir_palavra(copialinha,"",offset,1);
                perguntar(linha,copialinha,
                "Correcao: Caracteres especiais ('@','#','$','\x25','&','*') devem ser descartados.");
            }
        }

        //REGRA 3
        sinalaberto_e_nao_fechados(linha,0,1);

        //REGRA 4
        mesmo_sinal(linha,0,1);

        //REGRA 2,8,9,10
        offset=0;
        while (linha[offset]!='\0')
        {
            while (//Se não for letra.
                   !((linha[offset] >='A'&&
                   linha[offset] <='Z') ||
                   (linha[offset] >='a'&&
                   linha[offset] <='z'))
                   &&
                   //Se for o ultimo caracter da linha.
                   linha[offset]!='\0'
                   )
                offset++;
            
            if(linha[offset]=='\0')break;

            sscanf(linha + offset, "%[A-Za-z]", palavra);
            strcpy(copiapalavra,palavra);
            tamanho_palavra = strlen(copiapalavra);

            if(tamanho_letras(copiapalavra)){
                substituir_palavra(copialinha,copiapalavra,offset,tamanho_palavra);
                if(perguntar(linha,copialinha,
                "Correcao: Em qualquer palavra, o tamanho da letra deve ser o mesmo para todas as letras."))
                    strcpy(palavra,copiapalavra);
                else
                    strcpy(copiapalavra,palavra);
            }

            if(!verifica_dicio(dicio,palavra)){//verifica se a palavra não está no dicionário
                //REGRA 9
                strcpy(copiapalavra,palavra);//copia a palavra
                strcpy(copialinha,linha);//copia a linha
                if(semelhanca(dicio,copiapalavra,linha)){//verifica se existe semelhança no dicionário
                    substituir_palavra(linha,copiapalavra,offset,tamanho_palavra);//substitui a palavra na posição
                    tamanho_palavra=strlen(copiapalavra);
                    offset+=tamanho_palavra;
                    strcpy(copialinha,linha);
                }
                else{
                    //REGRA 8
                    substituir_palavra(copialinha,"",offset,tamanho_palavra);//para retirar
                    printf("\n%s\n",copialinha);
                    if(!perguntar(linha,copialinha,"A palavra nao consta no dicionario, deseja retirar essa palavra do texto?"))
                    {   
                        offset+=tamanho_palavra;
                        while (1)
                        {
                            //REGRA 10
                            char verificador;
                            printf("\nDeseja adicionar \"%s\" ao dicionario?",palavra);
                            printf("\nAplicar(s/n)?\n");
                            while ((getchar()) != '\n'); //limpar o buffer
                            scanf("%c",&verificador);
                            if(verificador=='s'||verificador=='S'){
                                fputs("\n",dicio);
                                fputs(palavra,dicio);
                                break;
                            }
                            if(verificador=='n'||verificador=='N'){
                                break;
                            }
                        }
                    }
                }
            }
            else
                offset+=tamanho_palavra;
        }


        //REGRA 7
        espacosrepetidos(linha);
        strcpy(copialinha,linha);

        offset=0;
        //REGRA 5
        int numero;
        while (copialinha[offset]!='\0'){
            if (ehdigito(copialinha[offset])!=0)
            {
                sscanf(copialinha + offset,"%d",&numero);
                pergunta=numero_extenso(copialinha,offset,numero);
                if(pergunta){
                    perguntar(linha,copialinha,
                    "Correcao: Numeros naturais inferiores a 11 devem ser escritos por extenso.");
                     pergunta = 0;
                }
                while (numero>=10)
                {
                    offset++;
                    numero/=10;
                }
                
            }
            offset++;
        }

        offset=0;
        //REGRA 1
        while (sscanf(linha + offset, "%s", palavra) == 1){
            strcpy(copiapalavra,palavra);
            if(inicio_frase){
                if(copiapalavra[0]>='a' && copiapalavra[0]<='z'){
                    copiapalavra[0]-=32;
                    pergunta=1;
                    }
                inicio_frase=0;
            }
            tamanho_palavra = strlen(copiapalavra);

            for ( i = 0; i < tamanho_palavra; i++)copialinha[offset+i] = copiapalavra[i];
            if(pergunta){
                pergunta=0;
                perguntar(linha,copialinha,
                    "Correcao:Todas as frases devem iniciar com letra maiuscula.");
            }
            //verifica se acabou a frase e começou outra
            inicio_frase=fim_de_frase(copiapalavra[tamanho_palavra-1]);
            // Atualiza o offset para a próxima copiapalavra
            offset += tamanho_palavra;
            // Pula espaços em branco e virgulas
            while (linha[offset] == ' ' || 
                   linha[offset] == '\t'||
                   linha[offset] == ','
                   )
                offset++;
        }

        fputs(linha,novo);
    }
    fclose(dicio);
    fclose(texto);
    fclose(novo);
}