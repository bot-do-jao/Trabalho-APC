//autor: Joao Pedro Nobrega Fernandes
//matricula: 211029361
//objetivo: construir um sistema funcional de companhia aerea, com menu, estrutura de loops, checagem de dados e salvamento em arquivos auxiliares

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#define TAM_MAX 100
#define CPF_MAX 16

typedef struct{

    char nomeVoo[TAM_MAX];
    char passageiros[TAM_MAX];
    int dia, mes, ano, hora, min;
    int status;
} Voo;

typedef struct
{
    char numReserva[TAM_MAX];
    char numVoo[TAM_MAX];
    char CPF[CPF_MAX];
    char nome[TAM_MAX];
    char sexo;
    int dia, mes, ano;
    int status;
} Reserva;

int checaDigito(char string[], int inicio, int fim)
{ // verifica se a string e de digitos
    for (int i = inicio; i < fim; i++)
    {
        if (string[i] < 48 || string[i] > 57){
            // olha a tabela ASCII
            return 0; 
        }
    }
    return 1; 
}

int checaVoo(char numero[])
{ 
    int tamanho;
    tamanho = strlen(numero); // determina o tamanho correto
    if (tamanho == 8)
    { 
        if (numero[0] == 'J' && numero[1] == 'E' && numero[2] == 'B' && numero[3] == '-' && checaDigito(numero, 4, 7) == 1){
            // checa a estrutura do voo
            return 0; 
        }
        else{
            return 1; 
        }
    }else{
        return 1; 
    }
}

int checaReserva(char numero[])
{ 
    int tamanho;
    tamanho = strlen(numero);
    if (tamanho == 9)
    {
        if (numero[0] == 'G' && numero[1] == 'B' && numero[2] == '-' && checaDigito(numero, 3, 9) == 1){
         //checa a estrutura da reserva
            return 0;
        }
        return 1;
    }else{
        return 1;
    }
}


int checaCPF(char numero[])
{
    int formatoCPF, i, j = 0;
    int cpfNum[12], penultimo = 0, ultimo = 0;
    

    formatoCPF = strlen(numero);//checagem do formato do CPF
    if (formatoCPF != 14) 
    {
        return 0;
    }else if (isdigit(numero[0]) == 0 || isdigit(numero[1]) == 0 || isdigit(numero[2]) == 0 || numero[3] != '.' || isdigit(numero[4]) == 0 || isdigit(numero[5]) == 0 || isdigit(numero[6]) == 0 || numero[7] != '.' || isdigit(numero[8]) == 0 || isdigit(numero[9]) == 0 || isdigit(numero[10]) == 0 || numero[11] != '-' || isdigit(numero[12]) == 0 || isdigit(numero[13]) == 0)
    {
        return 0;
    }else 
    {
        
        for ( i = 0; i < 14; i++)
        {
            if (i != 3 && i != 7 && i != 11)
            {
                cpfNum[j] = numero[i] - '0'; 
                j = j + 1;//conversao do formato
            }
        }
        
        if (cpfNum[0] == cpfNum[1] && cpfNum[1] == cpfNum[2] && cpfNum[2] == cpfNum[3] && cpfNum[3] == cpfNum[4] && cpfNum[4] == cpfNum[5] && cpfNum[5] == cpfNum[6] && cpfNum[6] == cpfNum[7] && cpfNum[7] == cpfNum[8] && cpfNum[8] == cpfNum[9] && cpfNum[9] == cpfNum[10])
        {
            return 0;//caso pra todos os digitos serem iguais
        }
        
        for ( i = 0; i < 9; i++){
            penultimo = penultimo + cpfNum[i]*(10-i); //determina o penultimo digito do cpf
        }

        penultimo = (penultimo*10) % 11; 

        if (penultimo == 10){
            penultimo = 0 ;
        }
        if (penultimo != cpfNum[9])
        {
            return 0;
        }else{
            
            for ( i = 0; i < 10; i++){
                ultimo = ultimo + cpfNum[i]*(11-i);//determina o ultimo digito do cpf
            }

            ultimo = (ultimo*10) % 11;//Resto da divisão por 11

            if (ultimo == 10)
            {
                ultimo = 0 ;
            }
            if (ultimo == cpfNum[10]){
                return 1;
            }
            else
            {
                return 0;
            }        
        }
    }
    
}

int ChecaNascimento(int dia, int mes, int ano)
{

    if (ano > 2021 || ano < 1910)
    {
        return 0;
    }

    if (mes <= 0 || mes >= 13 || dia <= 0 || dia > 31 || ano == 0)
    { // checagem da data caso simples
        return 0;
    }

    if ((ano % 4 != 0 || (ano % 100 == 0 && ano % 400 != 0)) && mes == 2 && dia > 28)
    {   //caso fevereiro ano bissexto
        return 0;
    }

    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && (dia > 30))
    {   //caso meses de 30 dias
        return 0;
    }

    if (mes == 2 && dia > 29)
    {   // caso fevereiro padrao
        return 0;
    }
    return 1;
}

int checaSexo(char numero)
{
    if (numero == 'F')
    {
        return 0; // caso seja feminino
    }
    if (numero == 'M')
    {
        return 0; // caso seja masculino
    }
    return 1; 
}

int buscaCpf(FILE *arquivo, char busca[TAM_MAX])
{
    Reserva dados;
    fseek(arquivo, 0, SEEK_SET); 

    while (!feof(arquivo))
    {
        fread(&dados, sizeof(Reserva), 1, arquivo);

        if (strstr(busca, dados.CPF) != NULL)
        {
            return 1; // caso CPF repetido
        }
    }
    
}

int buscaNome(FILE *arquivo, char busca[TAM_MAX])
{
    Reserva dados;
    fseek(arquivo, 0, SEEK_SET); 

    while (!feof(arquivo))
    {
        fread(&dados, sizeof(Reserva), 1, arquivo);

        if (strstr(busca, dados.nome) != NULL)
        {
            return 1; // caso CPF repetido
        }
    }
    
}

int buscaDeDados(FILE *arquivo, char busca[TAM_MAX])
{
    Reserva dados;
    char controle[TAM_MAX]; // controle de tamanho

    fseek(arquivo, 0, SEEK_SET); 

    while (!feof(arquivo))
    {
        fread(&controle, sizeof(Reserva), 1, arquivo);
        if (strcmp(busca, controle) == 0) // comparacao das strings
        {
            return 1; 
        }
    }
    return 0; 
}

int buscaVoo(FILE *arquivo, char busca[TAM_MAX])
{
    Voo numVoo;
    char controle[TAM_MAX]; // controle de tamanho do voo

    fseek(arquivo, 0, SEEK_SET); 

    while (!feof(arquivo))
    {
        fread(&controle, sizeof(Voo), 1, arquivo);
        if (strstr(controle, busca) != NULL) // comparacao das strings
        {
            return 1; 
        }
    }
    return 0; 
}

int checaPorcentagem(FILE *arquivo, FILE *arquivo2, char busca[TAM_MAX])
{
    Reserva dados;
    Voo voo;
    int contPassageiros = 0, passageiros, porcentagem;

    fseek(arquivo2, 0, SEEK_SET);
    while (fread(&dados, sizeof(Reserva), 1, arquivo2) != 0)
    {
        if (strcmp(dados.numVoo, busca) == 0)//checa e conta o numero de reservas
        {
            contPassageiros++;
        }
    }

    fseek(arquivo, 0, SEEK_SET);
    while (fread(&voo, sizeof(Voo), 1, arquivo) != 0)
    {
        if (strstr(voo.nomeVoo, busca) != NULL)
        {
            passageiros = atof(voo.passageiros);
            porcentagem = (float)contPassageiros / passageiros; //calcula a porcentagem de ocupacao
        }
    }
    return porcentagem; //retorna a porcentagem
}

FILE *abreArquivo(char abre[TAM_MAX], char funcao[TAM_MAX])
{ 
    FILE *arquivo;
    arquivo = fopen(abre, funcao);

    if (arquivo == NULL)
    { // caso nao abra ou ja exista, feche o programa
        system("cls");
        printf("Por favor, aguarde a criacao do arquivo\n");
        fopen(abre, "w");
    }

    return arquivo; 
}

void abreArquivoAlt()
{
    FILE *arquivo;
    FILE *arquivo2;
    arquivo = abreArquivo("baseDeDadosVoo.bin", "r+b"); //  cria e abre o arquivo
    arquivo2 = abreArquivo("BaseDeDadosReserva.bin", "r+b");
    fclose(arquivo);
    fclose(arquivo2);
}

int checaData(int dia, int mes, int ano, int hora, int min)
{ 
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime); //pega a data do sistema

    if (ano < (tm.tm_year + 1900))
    {
        return 0;
    }
    //checa todos os casos de data
    if (ano == (tm.tm_year + 1900) && mes < (tm.tm_mon + 1))
    {
        return 0;
    }

    if (ano == (tm.tm_year + 1900) && mes == (tm.tm_mon + 1) && dia < tm.tm_mday)
    {
        return 0;
    }

    if (ano == (tm.tm_year + 1900) && mes == (tm.tm_mon + 1) && dia == tm.tm_mday && hora < tm.tm_hour)
    {
        return 0;
    }

    if (ano == (tm.tm_year + 1900) && mes == (tm.tm_mon + 1) && dia == tm.tm_mday && hora == tm.tm_hour && min < tm.tm_min)
    {
        return 0;
    }

    if (mes <= 0 || mes >= 13 || dia <= 0 || dia > 31 || ano == 0 || hora < 0 || min < 0 || hora >= 24 || min >= 60)
    { 
        return 0;
    }

    if ((ano % 4 != 0 || (ano % 100 == 0 && ano % 400 != 0)) && mes == 2 && dia > 28)
    {
        return 0;
    }

    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && (dia > 30))
    {
        return 0;
    }

    if (mes == 2 && dia > 29)
    {
        return 0;
    }

    return 1;
}

int checaCPFAlt(FILE *arquivo, char busca[TAM_MAX], char busca2[TAM_MAX])
{
    Reserva dados; //funcao para checar so o CPF ja existe no sistema
    fseek(arquivo, 0, SEEK_SET);
    while (!feof(arquivo))
    {
        fread(&dados, sizeof(Reserva), 1, arquivo);
        if (strcmp(busca, dados.CPF) == 0)
        {
            if (strcmp(busca2, dados.numVoo) == 0)
            {
                if (dados.status == 1)
                {
                    return 0;
                }
            }
        }
    }
    
}

int checaDataReserva(FILE *arquivo, char busca2[TAM_MAX])
{
    //checagem de todos os formatos de data
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime); // pega a data do sistema 
    Voo voo;
    fseek(arquivo, 1, SEEK_SET);
    while (!feof(arquivo))
    {
        fread(&voo, sizeof(Voo), 1, arquivo);
        if (strstr(voo.nomeVoo, busca2) != NULL)
        {
            if (voo.ano < (tm.tm_year + 1900))
            {

                return 0;
            }
            //checa todos os casos de data para a reserva
            if (voo.ano == (tm.tm_year + 1900) && voo.mes < (tm.tm_mon + 1))
            {
                return 0;
            }

            if (voo.ano == (tm.tm_year + 1900) && voo.mes == (tm.tm_mon + 1) && voo.dia < tm.tm_mday)
            {
                return 0;
            }

            if (voo.ano == (tm.tm_year + 1900) && voo.mes == (tm.tm_mon + 1) && voo.dia == tm.tm_mday && voo.hora < tm.tm_hour)
            {
                return 0;
            }

            if (voo.ano == (tm.tm_year + 1900) && voo.mes == (tm.tm_mon + 1) && voo.dia == tm.tm_mday && voo.hora == tm.tm_hour && voo.min < tm.tm_min)
            {
                return 0;
            }

            if (voo.mes <= 0 || voo.mes >= 13 || voo.dia <= 0 || voo.dia > 31 || voo.ano == 0 || voo.hora < 0 || voo.min < 0 || voo.hora >= 24 || voo.min >= 60)
            { 
                return 0;
            }

            if ((voo.ano % 4 != 0 || (voo.ano % 100 == 0 && voo.ano % 400 != 0)) && voo.mes == 2 && voo.dia > 28)
            {
                return 0;
            }

            if ((voo.mes == 4 || voo.mes == 6 || voo.mes == 9 || voo.mes == 11) && (voo.dia > 30))
            {
                return 0;
            }

            if (voo.mes == 2 && voo.dia > 29)
            {
                return 0;
            }

            return 1;
        }
    }
    return 1;
}

int checaStatusVoo(char numVoo[TAM_MAX], FILE *arquivo)
{
    Voo voo;

    while (!feof(arquivo))
    {
        fread(&voo, sizeof(Voo), 1, arquivo);
        if (strstr(voo.nomeVoo, numVoo) != NULL) // muda e avalia o status de um voo entre cancelado (0) e ativo (1)
        {
            printf("%d", voo.status);
            if (voo.status == 1)
            {
                return 1;
            }
            if (voo.status == 0)
            {
                return 0;
            }
        }
    }
    
}



void cadastVoo()
{ 
    Voo voo;
    FILE *arquivo;
    arquivo = abreArquivo("baseDeDadosVoo.bin", "r+b"); 
    int escolha;

    
    printf("Por favor, insira o voo que deseja cadastrar:\nSiga o formato JEB-XXXX\n");

    scanf("%s", &voo.nomeVoo); 

    if (checaVoo(voo.nomeVoo) == 1 ||  buscaVoo(arquivo, voo.nomeVoo) == 1)
    { //checa se o voo existe e/ou esta no sistema
        while (checaVoo(voo.nomeVoo) == 1 ||  buscaVoo(arquivo, voo.nomeVoo) == 1)
        {   
            printf("\nEsse voo ja existe ou eh invalido, tente novamente.\n");
            scanf("%s", &voo.nomeVoo);
            checaVoo(voo.nomeVoo);//loop ate a insercao correta dos dados
        }
    }
    printf("Insira a quatidade maxima de passageiros desse voo:\n");
    scanf("%s", &voo.passageiros); 

    if (checaDigito(voo.passageiros, 0, 2) == 0)
    { //checa se é uma entrada válida
        while (checaDigito(voo.passageiros, 0, 2) == 0)
        { 
            printf("\nNumero invalido, tente novamente.\n");
            scanf("%s", &voo.passageiros);
            checaDigito(voo.passageiros, 0, 2);//loop ate a insercao correta dos dados
        }
    }
    printf("Por favor, insira a data e hora desse voo:\nSiga o exemplo \"dd/mm/aaaa-mm:hh\"\n");
    scanf("%d/%d/%d-%d:%d", &voo.dia, &voo.mes, &voo.ano, &voo.hora, &voo.min); 

    if (checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 0)
    {
        while (checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 0)
        { 
            printf("\nData invalida, tente novamente.\n");
            scanf("%d/%d/%d-%d:%d", &voo.dia, &voo.mes, &voo.ano, &voo.hora, &voo.min);
        }
    }

    voo.status = 1;

    fwrite(&voo, sizeof(Voo), 1, arquivo); //escreve os dados no arquivo
    fclose(arquivo);                           
    printf("\nCadastro efetuado com sucesso. Deseja efetuar outro?\n1 - sim\n0 - nao\n"); 
    scanf("%d", &escolha); // variavel para escolha ou encerrar o sistema
    //loop de volta para o menu
    if (escolha == 1) 
    {
        system("cls"); //"reseta" e volta a funcao
        cadastVoo();
    }
    else{

        printf("Entendido.\n");
    
        system("cls"); 
    }
    
}

void cadastReserva()
{ 
    Reserva dados;
    Voo voo;
    FILE *arquivo;
    FILE *arquivo2;
    arquivo = abreArquivo("baseDeDadosReserva.bin", "r+b"); 
    arquivo2 = abreArquivo("BaseDeDadosVoo.bin", "r+b");
    int escolha;
    

    printf("Por favor, insira o numero de sua reserva:\nSiga o modelo GB-XXXXXX\n"); 
    scanf("%s", &dados.numReserva);

    if (checaReserva(dados.numReserva) == 1 || buscaDeDados(arquivo, dados.numReserva) == 1)
    { 
        while (checaReserva(dados.numReserva) == 1 || buscaDeDados(arquivo, dados.numReserva) == 1){
           //loop para colocacao de uma reserva valida ou nao repetida
            printf("Numero de reserva invalido ou repetido, tente novamente.\n");
            scanf("%s", &dados.numReserva);
        }
    }

    printf("Voos ativos no momento:\n");

    while (fread(&voo, sizeof(Voo), 1, arquivo2) != 0)
    {
        //busca e informa os voos ativos e cadastrados no sistema

        if (voo.status == 1 && checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 1){
            printf("%s\n", voo.nomeVoo);
        }
    }

    printf("Por favor, insira o voo que deseja reservar:\nSiga o formato JEB-XXXX\n");
    scanf("%s", &dados.numVoo); 
    if (buscaVoo(arquivo2, dados.numVoo) == 0 || checaPorcentagem(arquivo2, arquivo, dados.numVoo) == 1 || checaDataReserva(arquivo2, dados.numVoo) == 0 || checaStatusVoo(dados.numVoo, arquivo2) == 0)
    { //checa se o voo esta ativo e com espaco nele
        while (buscaVoo(arquivo2, dados.numVoo) == 0 || checaPorcentagem(arquivo2, arquivo, dados.numVoo) == 1 || checaDataReserva(arquivo2, dados.numVoo) == 0 || voo.status == 0)
        {
            printf("\nVoo invalido. Por favor tente novamente.\n");
            scanf("%s", &dados.numVoo);
        }
    }

    fflush(stdin);

    printf("Insira o seu CPF, seguindo o modelo XXX.XXX.XXX-XX\n");
    scanf("\n%[^\n]s", &dados.CPF); 


    while (checaCPF(dados.CPF) == 0 || checaCPFAlt(arquivo, dados.CPF, dados.numVoo) == 0)
    { // checa se o cpf e valido e/ou se ja esta cadastrado nesse voo
        printf("\nCPF invalido ou ja possui reserva ativa nesse voo. Tente novamente.\n");
        scanf("\n%[^\n]s", &dados.CPF);
    }

    fflush(stdin);

    printf("Insira seu nome completo, sem acentos:\n");
    gets(dados.nome);                           

    while (checaDigito(dados.nome, 0, 101) == 1) // checa se nao ha digitos no nome
    {
        printf("\nNome invalido. Tente novamente.\n");
        scanf("\n%[^\n]s", &dados.nome);
    }
    
    fflush(stdin);

    printf("Insira seu sexo.(M/F)\n");
    scanf("\n%c", &dados.sexo); 

    while (checaSexo(dados.sexo) == 1)
    { // erro no sexo
        printf("\nSexo invalido, tente novamente. As opcoes sao M ou F.\n");
        scanf("\n%c", &dados.sexo);
    }

    printf("Insira sua data de nascimento conforme o modelo dd/mm/aaaa\n");
    scanf("%d/%d/%d", &dados.dia, &dados.mes, &dados.ano);

    if (ChecaNascimento(dados.dia, dados.mes, dados.ano) == 0) 
    {
        while (ChecaNascimento(dados.dia, dados.mes, dados.ano) == 0)
        {
            printf("\nData invalida, tente novamente.\n");
            scanf("%d/%d/%d", &dados.dia, &dados.mes, &dados.ano);
        }
    }

    dados.status = 1;

    fwrite(&dados, sizeof(Reserva), 1, arquivo); 

    fclose(arquivo); 
    fclose(arquivo2);

    printf("Reserva efetuada com suceso! Deseja realizar outra?\n1 - sim\n0 - nao\n");
    scanf("%d", &escolha);
    //loop para a volta no menu
    if (escolha == 1)
    {
        system("cls");
        cadastReserva();
    }
    else{
        printf("Entendido.\n");
    
        system("cls"); 
    }
          
}

void consultaVoo()
{ 
    FILE *arquivo;
    FILE *arquivo2;
    Voo voo;
    Reserva dados;
    char busca[TAM_MAX], status[TAM_MAX];
    arquivo = abreArquivo("baseDeDadosVoo.bin", "r+b"); // abre e cria o arquivo
    arquivo2 = abreArquivo("BaseDeDadosReserva.bin", "r+b");    
    int escolha, contPassageiros = 0, cont = 1;
    float porcentagem, passageiros;

    printf("Voos ativos no momento:\n");

    while (fread(&voo, sizeof(Voo), 1, arquivo) != 0) //busca do arquivo e mostra os voos ativos
    {
        if (voo.status == 1 && checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 1)
        {
            printf("%s\n", voo.nomeVoo);
        }
    }

    printf("Por favor, insira o numero do voo, conforme o modelo JEB-XXXX \n");
    scanf("%s", busca); 

    if (buscaVoo(arquivo, busca) == 0) // checa se o voo existe no sistema
    {
        while(buscaVoo(arquivo, busca) == 0){
            printf("Voo nao existente, tente novamente.\n");
            scanf("%s", busca);
        }
    }else{
        system("cls");
    }
    fseek(arquivo2, 0, SEEK_SET); 
    while (fread(&dados, sizeof(Reserva), 1, arquivo2))
    {
        if (strstr(dados.numVoo, busca) != 0 && dados.status != 0) {
            contPassageiros++;
            //conta o numero de reservas ativas no voo
        }
    }

    fseek(arquivo, 0, SEEK_SET); 

    while (fread(&voo, sizeof(Voo), 1, arquivo) != 0)
    {
        if (strstr(voo.nomeVoo, busca) != NULL) 
        {

            passageiros = atof(voo.passageiros); 
            porcentagem = (float)contPassageiros / passageiros;
            porcentagem = porcentagem * 100;

            if (checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 1)
            {
                if (voo.status == 1) 
                {
                    strcpy(status, "Ativo");
                }
                if (voo.status == 0)
                {
                    strcpy(status, "Cancelado");
                }
            }
            if (checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 0)
            {
                strcpy(status, "Voo expirado"); 
            }

            
            printf("\nNumero do Voo: %s \nStatus do Voo: %s\nData: %02d/%02d/%d-%02d:%02d\nPorcentagem de ocupacao: %.2f%%\n", 
            busca, status, voo.dia, voo.mes, voo.ano, voo.hora, voo.min, porcentagem); 
            printf("passageiros ativos por CPF:\n"); //mostra os dados do voo 

            fseek(arquivo2, 0, SEEK_SET); //busca e informa os passageiros (CPFs) que reservaram o voo
            while (fread(&dados, sizeof(Reserva), 1, arquivo2))
            {
                if (strstr(dados.numVoo, busca)) 
                {
                    if (dados.status != 0)
                    {
                        printf("%d - %s\n", cont, dados.CPF);
                    }
                }
                cont++;
            }

            fread(&voo, sizeof(Voo), 1, arquivo);
        }
    }

    printf("\nConsultar outro Voo?\n1 - sim\n0 - nao\n");
    scanf("%d", &escolha);

    if (escolha == 1)
    {
        system("cls");
        consultaVoo();
    }
    else{
        printf("Entendido.\n");
    
        system("cls");   
    }
    fclose(arquivo); 
    fclose(arquivo2);
    
}
void consultaReserva()
{
    Reserva dados;
    Voo voo;
    FILE *arquivo;
    FILE *arquivo2;
    arquivo = abreArquivo("baseDeDadosReserva.bin", "r+b"); 
    arquivo2 = abreArquivo("baseDeDadosVoo.bin", "r+b");
    char busca[TAM_MAX], status[TAM_MAX];
    int escolha;

    printf("Reservas ativas no momento:\n"); //busca mostra as reservas ativas no sistema, para qualquer voo

    while (fread(&dados, sizeof(Reserva), 1, arquivo) != 0 )
    {
        if (dados.status == 1 )
        {
            printf("%s\n", dados.numReserva);
        }
    }

    printf("Por favor, insira o numero da reserva conforme o modelo GB-XXXXXX\n");
    scanf("%s", busca); 

    while (buscaDeDados(arquivo, busca) == 0)
    {
        printf("Reserva nao encontrada, tente novamente.\n");
        scanf("%s", busca); 
    }
    if (buscaDeDados(arquivo, busca) == 1) // caso a reserva exista
    {
        
        fseek(arquivo, 0, SEEK_SET);
        while (fread(&dados, sizeof(Reserva), 1, arquivo) != 0) //le os dados do arquivo
        {
            if (strstr(dados.numReserva, busca) != NULL) //realiza a busca
            {
                fseek(arquivo, 0, SEEK_SET);
                while (fread(&voo, sizeof(Voo), 1, arquivo2) != 0)
                {
                    if (strstr(dados.numVoo, voo.nomeVoo) != NULL)
                    {
                        if (dados.status != 0 && voo.status != 0 && checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 1) {
                            // checa o status da reserva
                        
                            strcpy(status, "Ativa");
                        }
                        else{
                            strcpy(status, "Cancelada");
                        }
                    }
                }
                
                printf("Reserva: %s \nNumero do Voo : %s\nCPF: %s\nStatus da reserva : %s\nnome: %s\nsexo: %c\nData de nascimento: %02d/%02d/%d \n", 
                busca, dados.numVoo, dados.CPF, status, dados.nome, dados.sexo, dados.dia, dados.mes, dados.ano); //mostra os dados da reserva

                
                fclose(arquivo2);
                fclose(arquivo);
                    
                
            }

        }
    }
     
    printf("Deseja consultar mais reservas?\n1 - sim\n0 - nao\n");
    scanf("%d", &escolha);
    //loop de volta ao sistema
    if (escolha == 1)
    {
        system("cls");
        consultaReserva();
    }
    else{
        printf("Ok\n");
    
        system("cls");
    } 
    
}

void consultaPassageiro()
{
    FILE *arquivo;
    arquivo = abreArquivo("BaseDeDadosReserva.bin","r+b");
    
    Reserva dados;
    char busca[TAM_MAX], status[TAM_MAX], statusbusca[TAM_MAX];
    int escolha, cont = 1;


    printf("CPFs cadastrados no sistema:\n"); //busca mostra os CPFs com cadastro no sistema

    while (fread(&dados, sizeof(Reserva), 1, arquivo) != 0 )
    {
        if (dados.status == 1 )
        {
            printf("%s\n", dados.CPF);
        }
    }


    printf("Por favor, insira o CPF da busca, conforme o modelo XXX.XXX.XXX-XX\n");
    scanf("%s", busca);

    if (buscaCpf(arquivo, busca) == 0)//checa se o CPF possui reserva ativa no sistema
    {
        printf("\nCPF invalido. Tente novamente.\n");
        
        getch();
    }

    if (buscaCpf(arquivo, busca) == 1)
    {
        
        fseek(arquivo, 0, SEEK_SET);
        while (fread(&dados, sizeof(Reserva), 1, arquivo) != 0)
        {
            if (strstr(dados.CPF, busca) != NULL)
            {
                fseek(arquivo, 0, SEEK_SET);
                if (dados.status == 1)
                {
                    strcpy(status, "Ativa");
                }
                if (dados.status == 0)
                {
                    strcpy(status, "Cancelada");
                }

                printf("\n");
                printf("nome: %s\nCPF: %s\nsexo: %c\nData de nascimento: %02d/%02d/%d\n", 
                dados.nome, dados.CPF, dados.sexo, dados.dia, dados.mes, dados.ano); //mostra os dados do passageiro
                

                fseek(arquivo, 0, SEEK_SET);
                while (!feof(arquivo))
                {   //abre o arquivo, puxa a string do cpf
                    fread(&dados, sizeof(Reserva), sizeof(Reserva), arquivo); 
                    if (strstr(dados.CPF, busca) != NULL)
                    {
                        fseek(arquivo, 0, SEEK_SET); //dentro da string do cpf compara o voo cadastrado com o dado do sistema
                        do
                        {
                            fread(&statusbusca, sizeof(Reserva), 1, arquivo);
                            if (strstr(dados.numReserva, statusbusca) != NULL)
                            {
                                if (dados.status == 1) //checa o valor do status do voo buscado e retorna ativo ou cancelado
                                {
                                    printf("%d- %s -> Ativa\n", cont, dados.numReserva);
                                }
                                if (dados.status == 0)
                                {
                                    printf("%d- %s -> Cancelada \n", cont, dados.numReserva);
                                }
                            }
                        }while (!feof(arquivo));
                        cont++;
                    }
                }
            }
            fread(&dados, sizeof(Reserva), 1, arquivo);
        }
    }
    fclose(arquivo); 

    printf("Deseja consultar mais passageiros?\n1 - sim\n0 - nao\n");
    scanf("%d", &escolha);
    //loop de retorno ao sistema
    if (escolha == 1)
    {
        system("cls");
        consultaPassageiro();
    }
    else{
        printf("Ok\n");
    
        system("cls"); 
    }
    
}

void cancelaVoo()
{
    
    FILE *arquivo;
    FILE *arquivo2;
    FILE *temp, *tempAlt;
    int escolha, contador = 0, contadorAlt =0;
    char busca[TAM_MAX];
    arquivo = abreArquivo("baseDeDadosVoo.bin", "r+b"); 
    arquivo2 = abreArquivo("BaseDeDadosReserva.bin", "r+b");
    temp = abreArquivo("temporario.bin", "w+b"); 
    tempAlt = abreArquivo("temporarioAlt.bin", "w+b"); 
    Voo voo;
    Voo vooTemp;
    Reserva dados;
    Reserva dadosTemp;

    printf("Voos ativos no momento:\n");//mostra os voos ativos no sistema

    while (fread(&voo, sizeof(Voo), 1, arquivo) != 0)
    {
        if (voo.status == 1 && checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 1)
        {    
            printf("%s\n", voo.nomeVoo);
            contador++;
        }
    }

    printf("Digite o numero do Voo que deseja cancelar\n");
    scanf("%s", busca);

    if (buscaVoo(arquivo, busca) == 0)
    {
        printf("Voo nao existente no sistema. Tente novamente.\n");
        getch();
    }
    if (buscaVoo(arquivo, busca) == 1)
    {
        fseek(arquivo, 0, SEEK_SET);
        for (int i = 0; i <= contador; i++)
        {
            fread(&voo, sizeof(Voo), 1, arquivo);
            if (strstr(voo.nomeVoo,busca) == NULL)
            {
                fwrite(&voo, sizeof(Voo), 1, temp);//reescreve o arquivo em um temporario, caso o voo nao exista
            }else
            {
                strcpy(vooTemp.nomeVoo, voo.nomeVoo); //reescreve o arquivo e substitui o status do voo, caso ele exista
                strcpy(vooTemp.passageiros, voo.passageiros);
                vooTemp.dia = voo.dia;
                vooTemp.mes = voo.mes;
                vooTemp.ano = voo.ano;
                vooTemp.hora = voo.hora;
                vooTemp.min = voo.min;
                vooTemp.status = 0;                         
                fwrite(&vooTemp, sizeof(Voo), 1, temp);
            }  
        }
        fclose(arquivo);
        fclose(temp);
        remove("baseDeDadosVoo.bin"); //remove o arquivo original
        rename("temporario.bin", "baseDeDadosVoo.bin"); //transforma o temporario no original, renomeando-o

        fseek(arquivo2, 0, SEEK_SET);
        while(fread(&dados, sizeof(Reserva), 1, arquivo2)!= 0){
            contadorAlt++;
        }
        fseek(arquivo2, 0, SEEK_SET);
        for (int i = 0; i <= contadorAlt; i++)   
        {
            fread(&dados, sizeof(Reserva), 1, arquivo2); 
            if (strstr(dados.numVoo, busca) == NULL)     
            {
                fwrite(&dados, sizeof(Reserva), 1, arquivo2); //reescreve as reservas
            }
            if (strstr(dados.numVoo, busca) != NULL) // cancela as reservas ativas no voo cancelado
            {
                strcpy(dadosTemp.numReserva, dados.numReserva);
                strcpy(dadosTemp.numVoo, dados.numVoo);
                strcpy(dadosTemp.nome, dados.nome);
                strcpy(dadosTemp.CPF, dados.CPF);
                dadosTemp.sexo = dados.sexo;
                dadosTemp.dia = dados.dia;
                dadosTemp.mes = dados.mes;
                dadosTemp.ano = dados.ano;
                dadosTemp.status = 0;                         
                fwrite(&dadosTemp, sizeof(Reserva), 1, tempAlt); 
            }
        }
        fclose(arquivo2);
        fclose(tempAlt);
        remove("BaseDeDadosReserva.bin"); // remove o arquivo original
        rename("temporarioAlt.bin", "BaseDeDadosReserva.bin"); //renomeia o temporario e o transforma no original
        printf("Voo cancelado com sucesso!\n");

    }

    printf("Deseja cancelar outro Voo?\n1 - sim\n0 - nao\n");
    scanf("%d", &escolha);
    // loop de retorno ao menu
    if (escolha == 1)
    {
        system("cls");
        cancelaVoo();
    }
    else{
        
        printf("Ok\n");
        system("cls"); 
    }
        
}

void cancelaReserva()
{
    FILE *arquivo;
    FILE *temp;
    int escolha, contador = 0, contadorAlt =0;
    char busca[TAM_MAX];
    arquivo = abreArquivo("BaseDeDadosReserva.bin", "r+b");
    temp = abreArquivo("temporario.bin", "w+b"); 
    Reserva dados;
    Reserva dadosTemp;

   printf("Reservas ativas no momento:\n"); //busca e mostra as reservas ativas no sistema, para qualquer voo

    while (fread(&dados, sizeof(Reserva), 1, arquivo) != 0 )
    {
        if (dados.status == 1 )
        {
            printf("%s\n", dados.numReserva);
            contador++;
        }
    }

    printf("Digite o numero da Reserva que deseja cancelar\n");
    scanf("%s", busca);

    if (buscaDeDados(arquivo, busca) == 0)
    {
        printf("Reserva nao existente no sistema. Tente novamente.\n");
        getch();
    }
    if (buscaDeDados(arquivo, busca) == 1)
    {
        fseek(arquivo, 0, SEEK_SET);
        for (int i = 0; i < contador; i++)
        {
            fread(&dados, sizeof(Reserva), 1, arquivo); //reescreve as reservas
            if (strstr(dados.numReserva,busca) == NULL)
            {
                fwrite(&dados, sizeof(Reserva), 1, temp);
            }else //renomeia o temporario e o transforma no original
            {
                strcpy(dadosTemp.numReserva, dados.numReserva);
                strcpy(dadosTemp.numVoo, dados.numVoo);
                strcpy(dadosTemp.nome, dados.nome);
                strcpy(dadosTemp.CPF, dados.CPF);
                dadosTemp.sexo = dados.sexo;
                dadosTemp.dia = dados.dia;
                dadosTemp.mes = dados.mes;
                dadosTemp.ano = dados.ano;
                dadosTemp.status = 0;                         
                fwrite(&dadosTemp, sizeof(Reserva), 1, temp);
            }  
        }
        fclose(arquivo);
        fclose(temp);
        remove("BaseDeDadosReserva.bin");// remove o arquivo original
        rename("temporario.bin", "BaseDeDadosReserva.bin");//renomeia o temporario e o transforma no original
        printf("Reserva cancelada com sucesso!\n");

    }

    printf("Deseja cancelar outra Reserva?\n1 - sim\n0 - nao\n");
    scanf("%d", &escolha);
    //loop de retorno ao menu
    if (escolha == 1)
    {
        system("cls");
        cancelaReserva();
    }
    else{
        
        printf("Ok\n");
        system("cls"); 
    } 
    
}

void deletaVoo()
{
    FILE *arquivo;
    FILE *arquivo2;
    FILE *temp, *tempAlt;
    int escolha, contador = 0, contadorAlt =0;
    char busca[TAM_MAX];
    arquivo = abreArquivo("baseDeDadosVoo.bin", "r+b"); 
    arquivo2 = abreArquivo("BaseDeDadosReserva.bin", "r+b");
    temp = abreArquivo("temporario.bin", "w+b"); 
    tempAlt = abreArquivo("temporarioAlt.bin", "w+b"); 
    Voo voo;
    Voo vooTemp;
    Reserva dados;
    Reserva dadosTemp;

    printf("Voos ativos no momento:\n");

    while (fread(&voo, sizeof(Voo), 1, arquivo) != 0)
    {
        if (voo.status == 1 && checaData(voo.dia, voo.mes, voo.ano, voo.hora, voo.min) == 1)
        {    
            printf("%s\n", voo.nomeVoo);
            contador++;
        }
    }

    printf("Digite o numero do Voo que deseja deletar\n");
    scanf("%s", busca);

    if (buscaVoo(arquivo, busca) == 0)
    {
        printf("Voo nao existente no sistema. Tente novamente.\n");
        getch();
    }
    if (buscaVoo(arquivo, busca) == 1)
    {
        fseek(arquivo, 0, SEEK_SET);
        for (int i = 0; i <= contador; i++)
        {
            fread(&voo, sizeof(Voo), 1, arquivo);
            if (strstr(voo.nomeVoo,busca) == NULL) //reescreve o arquivo sem o voo cancelado
            {
                fwrite(&voo, sizeof(Voo), 1, temp);
            }  
        }
        fclose(arquivo);
        fclose(temp);
        remove("baseDeDadosVoo.bin");
        rename("temporario.bin", "baseDeDadosVoo.bin");

        fseek(arquivo2, 0, SEEK_SET);
        while(fread(&dados, sizeof(Reserva), 1, arquivo2)!= 0){
            contadorAlt++;
        }
        fseek(arquivo2, 0, SEEK_SET);
        for (int i = 0; i < contadorAlt; i++)   
        {
            fread(&dados, sizeof(Reserva), 1, arquivo2); 
            if (strstr(dados.numVoo, busca) == NULL)     // reescreve todas as reservas caso nao haja reserva no voo deletado
            {
                fwrite(&dados, sizeof(Reserva), 1, arquivo2); 
            }
            if (strstr(dados.numVoo, busca) != NULL) 
            {
                strcpy(dadosTemp.numReserva, dados.numReserva); //cancela as reservas no voo deletado e reescreve o arquivo
                strcpy(dadosTemp.numVoo, dados.numVoo);
                strcpy(dadosTemp.nome, dados.nome);
                strcpy(dadosTemp.CPF, dados.CPF);
                dadosTemp.sexo = dados.sexo;
                dadosTemp.dia = dados.dia;
                dadosTemp.mes = dados.mes;
                dadosTemp.ano = dados.ano;
                dadosTemp.status = 0;                         
                fwrite(&dadosTemp, sizeof(Reserva), 1, tempAlt); 
            }
        }
        fclose(arquivo2);
        fclose(tempAlt);
        remove("BaseDeDadosReserva.bin");// remove o arquivo original
        rename("temporarioAlt.bin", "BaseDeDadosReserva.bin");//renomeia o temporario e o transforma no original
        printf("Voo deletado com sucesso!\n");

    }

    printf("Deseja deletar outro Voo?\n1 - sim\n0 - nao\n");
    scanf("%d", &escolha);
    //loop de retorno ao menu
    if (escolha == 1)
    {
        system("cls");
        deletaVoo();
    }
    else{
        
        printf("Ok\n");
        system("cls"); 
    } 
}

void menu()
{                         
    unsigned char escolha; 

    do
    {
        printf("Menu. digite o numero da opcao desejada:\n");
        printf("1) Cadastrar voo\n");
        printf("2) Cadastrar reserva\n3) Consultar voo\n");
        printf("4) Consultar reserva\n5) Consultar passageiro\n");
        printf("6) Cancelar voo\n7) Cancelar reserva\n");
        printf("8) Excluir voo\n9) Sair do programa\n");
        
        scanf("%c", &escolha); 

        switch (escolha)
        { 
        case '1':
            system("cls");
            cadastVoo(); 
            break;
        case '2':
            system("cls");
            cadastReserva();
            break;

        case '3':
            system("cls");
            consultaVoo();
            break;

        case '4':
            system("cls");
            consultaReserva();
            break;

        case '5':
            system("cls");
            consultaPassageiro();
            break;

        case '6':
            system("cls");
            cancelaVoo();
            break;

        case '7':
            system("cls");
            cancelaReserva();
            break;

        case '8':
            system("cls");
            deletaVoo();
            break;

        case '9': 
            printf("Ok\n");
            exit(0);
            break;

        default: 
            printf("Opcao invalida, tente novamente.\n");
            system("cls");
            break;
        }
    } while (1); 
}

int main()
{
    abreArquivoAlt();
    menu(); 
}