/** Implementaçãoo de Matriz Esparsa para um trabalho de
	Algoritmos e Estruturas de Dados I - professor Fernando Paulovich

	Autores:
    Lemuel Roberto
    Marcelo Petrucelli
**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/** Importante:
    O valor de DIV deve ser sempre menor que o numero de linhas ou colunas do menor caso teste.

    O valor de MIN_TO_PART nao pode ser menor que o de DIV. Uma vez que o valor de DIV ira dividir o numero de linhas e/ou colunas da matriz,
    devemos levar em consideraSSao que o aumento de desempenho depende da discrepancia entre o valor de MIN_TO_PART e o valor de DIV **/

#define DIV 10 /* Define em quantas partes sera dividida as linhas e colunas da matriz */
#define MIN_TO_PART 200 /* Define qual deve ser o minimo numero de linhas e colunas para que estas sejam divididas */

typedef struct
{
    int valor;
} DADO;

typedef struct CELULA
{
    int linha;
    int coluna;

    DADO dado;

    struct CELULA *proximo;
    struct CELULA *abaixo;

    /** A ideia de "fim_pointer" eh ter um ponteiro em cada CELULA sentinela que aponte para o ultimo elemento correspondete a ela.

        Por exemplo, a CELULA sentinela que esta na coluna de sentinela, ou seja, marca uma linha da matriz, tera seu "fim_pointer" apontando
        para a ultima CELULA dessa linha, o mesmo vale para uma CELULA sentinela que marca coluna.

        Com isso, se a inserSSao for ordenada como esta na especificaSSao do trabalho, ela se dará de forma muito mais rapida **/

    struct CELULA *fim_pointer;
} CELULA;

typedef struct
{
    CELULA sentinela_mae;

    CELULA *fim_horizontal;
    CELULA *fim_vertical;

    /** A ideia desses vetores eh dividir para conquistar. Iremos dividir as linhas e colunas da matriz em DIV partes.
        Cada indice do vetor correspondera a um bloco de sentinelas. Essa implementaSSao nos permitira percorrer a matriz
        de forma mais otimizada.

        Por exemplo, se a a matriz tem dimensao 1000 x 1000 e preciso ir para a coluna 573, ao inves de percorrer sentinela a
        sentinela fazemos a seguinte conta:

        (int) pos = (573)/(1000/10) = 5 e acessamos o vetor "marcador_coluna" na posicao pos, marcador_coluna[5], e agora temos que percorrer
        apenas 73 sentinelas ao inves de 573.

        Eh importante resaltar que dividiremos a matriz APENAS se esta tiver dimensao (linha e/ou coluna) acima de MIN_TO_PART. P-) **/

    CELULA *marcador_linha[DIV];
    CELULA *marcador_coluna[DIV];
} MATRIZ;

/** funções para manipular as matrizes **/
void CriarMatriz (MATRIZ *matriz);
int CriarSentinelas (MATRIZ *matriz, int nlinhas, int ncolunas);
int InserirCelula (MATRIZ *matriz, int linha, int coluna, DADO *dado);
void LimparMatriz (MATRIZ *matriz);

/** FnSSoes para realizar operaSSoes entre matrizes **/
int SomarMatriz (MATRIZ *matriz1, MATRIZ *matriz2);
int MultiplicarMatriz (MATRIZ *matriz1, MATRIZ *matriz2);

int main (void)
{
    MATRIZ matriz1, matriz2;

    CriarMatriz (&matriz1);
    CriarMatriz (&matriz2);

    char op, str[7];
    int i=0, c, c1, c2;
    DADO dado;

    scanf("%c", &op);
    while (scanf(" %s %d %d", str, &c1, &c2) != EOF)
    {
        if (isdigit(str[0]))
        {
            c = atoi(str);
            dado.valor = c2;

            switch (i)
            {
            case 1:
                if ( !(InserirCelula (&matriz1, c, c1, &dado)) )
                    return 1;
                break;

            case 2:
                if ( !(InserirCelula (&matriz2, c, c1, &dado)) )
                    return 1;
                break;

            default:
                return 1;
            }
        }
        else
        {
            switch (i)
            {
            case 0:
                if ( !(CriarSentinelas (&matriz1, c1, c2)) )
                    return 1;
                i++;
                break;

            case 1:
                if ( !(CriarSentinelas (&matriz2, c1, c2)) )
                    return 1;
                i++;
                break;

            default:
                return 1;
            }
        }
    }

    switch (op)
    {
    case'a':
    case'A':
        if (!SomarMatriz (&matriz1, &matriz2))
            return 1;
        break;

    case'm':
    case'M':
        if (!MultiplicarMatriz (&matriz1, &matriz2))
            return 1;
        break;
    }

    LimparMatriz (&matriz1);
    LimparMatriz (&matriz2);

    return 0;
}

/** CriarMatriz:
        Essa funSSao coloca um dado do tipo MATRIZ em um estado inicial **/

void CriarMatriz (MATRIZ *matriz)
{
    matriz->sentinela_mae.linha = -1;
    matriz->sentinela_mae.coluna = -1;

    matriz->sentinela_mae.abaixo = &(matriz->sentinela_mae);
    matriz->sentinela_mae.proximo = &(matriz->sentinela_mae);

    matriz->fim_vertical = &(matriz->sentinela_mae);
    matriz->fim_vertical->abaixo = &(matriz->sentinela_mae);
    matriz->fim_vertical->proximo = &(matriz->sentinela_mae);

    matriz->fim_horizontal = &(matriz->sentinela_mae);
    matriz->fim_horizontal->abaixo = &(matriz->sentinela_mae);
    matriz->fim_horizontal->proximo = &(matriz->sentinela_mae);

}

/** CriarSentinelas:
        Essa funSSao serve para a criaSSao de uma moldura (meia moldura) de CELULAS sentinelas para nossa matriz, ou seja,
        existira nlinhas linhas de CELULAS sentinelas e ncolunas colunas de CELULAS sentinelas  **/

int CriarSentinelas (MATRIZ *matriz, int nlinhas, int ncolunas)
{
    int i, j; /* Variaveis para controlar as interaSSoes */

    int separar_v = 0, separar_h = 0; /* Variavel para controlar se usarei as atalaias ou nao. Se "separar_v" eh 0 (FALSO) nao posso criar as atalaias, o mesmo vale para "separar_h" */

    if (nlinhas >= MIN_TO_PART) /* Se o numero de linhas da matriz eh igual ou superior a MIN_TO_PART crio as atalaias */
    {
        separar_v = 1;
        matriz->marcador_linha[0] = &(matriz->sentinela_mae);
    }

    if (ncolunas >= MIN_TO_PART) /* Se o numero de colunas da matriz eh igual ou superior a MIN_TO_PART crio as atalaias */
    {
        separar_h = 1;
        matriz->marcador_coluna[0] = &(matriz->sentinela_mae);
    }

    for (i = 0, j = 1; i < nlinhas; i = i + 1) /* Crio uma coluna de sentinelas, ou seja, crio a sentinela de cada linha da matriz */
    {
        CELULA *PTR_nova = (CELULA *) malloc (sizeof(CELULA));

        if (PTR_nova)
        {
            /* Insiro os indices da nova CELULA sentinela */
            PTR_nova->linha = i;
            PTR_nova->coluna = -1;
            PTR_nova->fim_pointer = &(matriz->sentinela_mae);

            /* FaSSo a ligaSSao vertical entre o "fim_vertical" atual e a nova CELULA sentinela */
            PTR_nova->abaixo = matriz->fim_vertical->abaixo;
            matriz->fim_vertical->abaixo = PTR_nova;

            /* FaSSo a ligaSSao horizontal (imponho circularidade horizontal a nova linha inserida) da nova CELULA sentinela */
            PTR_nova->proximo = PTR_nova;

            /* Ajusto o "fim_vertical" */
            matriz->fim_vertical = PTR_nova;

            if ( (i % (nlinhas / DIV) == 0) && (i != 0) && (separar_v == 1) )
            {
                matriz->marcador_linha[j] = PTR_nova;
                j = j + 1;
            }

        }
        else
        {
            return 0; /* Se uma CEULA sentinela nao foi alocada, retorno 0 */
        }
    }

    /* O processo executado abaixo eh analogo ao que foi feito para a criação das linhas da matrtiz (coluna de sentinelas) */
    for (i = 0, j = 1; i < ncolunas; i = i + 1) /* Crio uma linha de sentinelas, ou seja, crio a sentinela de cada coluna da matriz */
    {
        CELULA *PTR_nova = (CELULA *) malloc (sizeof(CELULA));

        if (PTR_nova)
        {
            PTR_nova->linha = -1;
            PTR_nova->coluna = i;
            PTR_nova->fim_pointer = &(matriz->sentinela_mae);

            PTR_nova->proximo = matriz->fim_horizontal->proximo;
            matriz->fim_horizontal->proximo = PTR_nova;

            PTR_nova->abaixo = PTR_nova;

            matriz->fim_horizontal = PTR_nova;

            if ( (i % (ncolunas / DIV) == 0) && (i != 0) && (separar_h == 1) )
            {
                matriz->marcador_coluna[j] = PTR_nova;
                j = j + 1;
            }
        }
        else
        {
            return 0; /* Se uma CEULA sentinela nao foi alocada, retorno 0 */
        }
    }

    return 1; /* Retorno 1 se todas as sentinelas foram alocadas e inseridas adequadamente */
}

/** InserirCelula:
        Essa função insere uma nova CELULA a matriz, fazendo as ligações necessarias para que a matriz continue funcionando adequadamente **/

int InserirCelula (MATRIZ *matriz, int linha, int coluna, DADO *dado)
{
    if (dado->valor != 0 && linha > -1 && linha <= matriz->fim_vertical->linha && coluna > -1 && coluna <= matriz->fim_horizontal->coluna)
    {
        CELULA *PTR_nova = (CELULA *) malloc (sizeof(CELULA));

        if (PTR_nova)
        {
            /* Copio os indices e o dado passados a função, para a nova CEULA */
            PTR_nova->linha = linha;
            PTR_nova->coluna = coluna;
            PTR_nova->dado = *dado;

            /* Essa divisao entre PTR_aux_linha, PTR_aux_coluna e PTR_aux serve apenas para facilitar o entendimento do codigo e sua depuração */
            CELULA *PTR_aux_linha = NULL; /* Uso exclusivo para percorrer a coluna de sentinelas, ou seja, a coluna -1 de todas as linhas da matriz */
            CELULA *PTR_aux_coluna = NULL; /* Uso exclusivo para percorrer a linha de sentinelas, ou seja, a linha -1 de todas as colunas da matriz */
            CELULA *PTR_aux = NULL; /* Ponteiro auxiliar para percorrer as CELULAS interiores a matriz */

            /* Faço a ligação horizontal, ou seja percorro as linhas ate chegar a que eu preciso inserir a nova CELULA */

            if (matriz->fim_vertical->linha >= MIN_TO_PART) /* Acesso a atalaia correspondente a linha que quero inserir */
            {
                int pos = (int) (linha / ((matriz->fim_vertical->linha + 1) / DIV));
                PTR_aux_linha = matriz->marcador_linha[pos];
            }
            else
            {
                PTR_aux_linha = &(matriz->sentinela_mae);
            }


            while (PTR_aux_linha->abaixo != &(matriz->sentinela_mae) && PTR_aux_linha->linha < linha) /* Percorro as linhas da matriz, a partir da "sentinela_mae", ate que PTR_linha esteja na linha que devo inserir a nova CELULA */
            {
                PTR_aux_linha = PTR_aux_linha->abaixo;
            }


            if ( (PTR_nova->coluna > PTR_aux_linha->fim_pointer->coluna) && (PTR_aux_linha->fim_pointer != &(matriz->sentinela_mae)) )
            {
                PTR_aux = PTR_aux_linha->fim_pointer;

                /* Insiro a nova CELULA na matriz, fazendo as ligações horizontais necessarias */
                PTR_nova->proximo = PTR_aux->proximo;
                PTR_aux->proximo = PTR_nova;
                PTR_aux_linha->fim_pointer = PTR_nova;
            }
            else
            {
                PTR_aux = PTR_aux_linha;

                while (PTR_aux->proximo->coluna != matriz->sentinela_mae.coluna && PTR_aux->proximo->coluna < coluna) /* Percorro as colunas da linha em que estou atualmente, ate que PTR_aux fique imediatamente antes do local de InserSSao da nova CEULA */
                {
                    PTR_aux = PTR_aux->proximo;
                }

                if (PTR_aux->proximo->coluna == PTR_nova->coluna) /* Se ja existe uma CELULA com o mesmo indice da que iremos inserir agora, faço um "gap" entre PTR_aux e PTR_aux->proximo->proximo, ou seja, desfaço a ligação horizontal da CELULA, que existe na matriz, com mesmo indice da CELULA que pretendemos inserir */
                {
                    PTR_aux->proximo = PTR_aux->proximo->proximo;
                }

                /* Insiro a nova CELULA na matriz, fazendo as ligações horizontais necessarias */
                PTR_nova->proximo = PTR_aux->proximo;
                PTR_aux->proximo = PTR_nova;
            }

            /* Faço a ligação vertical, ou seja percorro as colunas ate chegar a que eu preciso inserir a nova CELULA */

            if (matriz->fim_horizontal->coluna >= MIN_TO_PART)  /* Acesso a atalaia correspondente a coluna que quero inserir */
            {
                int pos = (int) (coluna / ((matriz->fim_horizontal->coluna + 1) / DIV));
                PTR_aux_coluna = matriz->marcador_coluna[pos];
            }
            else
            {
                PTR_aux_coluna = &(matriz->sentinela_mae);
            }


            while ( (PTR_aux_coluna->proximo) != &(matriz->sentinela_mae) && (PTR_aux_coluna->coluna < coluna) ) /* Percorro as colunas matriz, a partir da "sentinela_mae", ate que PTR_coluna esteja na coluna que devo inserir a nova CELULA */
            {
                PTR_aux_coluna = PTR_aux_coluna->proximo;
            }


            if ( (PTR_nova->linha > PTR_aux_coluna->fim_pointer->linha) && (PTR_aux_coluna->fim_pointer != &(matriz->sentinela_mae)) )
            {
                PTR_aux = PTR_aux_coluna->fim_pointer;

                /* Faço as ligações verticais necessarias para que a nova CELULA esteja no lugar certo na matriz */
                PTR_nova->abaixo = PTR_aux->abaixo;
                PTR_aux->abaixo = PTR_nova;
                PTR_aux_coluna->fim_pointer = PTR_nova;
            }
            else
            {
                PTR_aux = PTR_aux_coluna;

                while (PTR_aux->abaixo->linha != matriz->sentinela_mae.linha && PTR_aux->abaixo->linha < linha) /* Percorro as linhas da coluna em que estou atualmente, ate que PTR_aux fique imediatamente acima do local de inserção da nova CELULA */
                {
                    PTR_aux = PTR_aux->abaixo;
                }

                if (PTR_aux->abaixo->linha == PTR_nova->linha) /* Se ja existe uma CELULA com o mesmo indice da que iremos inserir agora, façoo um "gap" entre PTR_aux e PTR_aux->abaixo->abaixo, ou seja, desfaço a ligação vertical da CELULA, que existe na matriz, com mesmo indice da CELULA que pretendemos inserir, e a removo (desaloco) da matriz */
                {
                    CELULA *PTR_rm = PTR_aux->abaixo;
                    PTR_aux->abaixo = PTR_aux->abaixo->abaixo;
                    free (PTR_rm); /* Desaloco a CELULA de mesmos indices que vou estou a inserir na matriz */
                }

                /* Façoo as ligações verticais necessarias para que a nova CELULA esteja no lugar certo na matriz */
                PTR_nova->abaixo = PTR_aux->abaixo;
                PTR_aux->abaixo = PTR_nova;
            }

            return 1; /* Retorno 1 se a nova CELULA foi alocada e inserida na matriz corretamente */
        }
    }

    return 0; /* Retorno 1 se a posição (linha ou coluna) informada nao pertence a matriz ou se foi impossivel alocar memoria para a nova CEULA */
}

int SomarMatriz (MATRIZ *matriz1, MATRIZ *matriz2)
{
    if ( matriz1->fim_vertical->linha == matriz2->fim_vertical->linha
        && matriz1->fim_horizontal->coluna == matriz2->fim_horizontal->coluna )
    {
        printf("# %d %d\n", matriz1->fim_vertical->linha + 1, matriz1->fim_horizontal->coluna + 1);

        CELULA *PTR_aux1_linha = matriz1->sentinela_mae.abaixo; /* Uso exclusivo para percorrer a linha de sentinelas, ou seja, a linha -1 de todas as colunas da matriz1. Na verdade ela marca a linha que esta sendo percorrida na matriz1 */
        CELULA *PTR_aux1 = PTR_aux1_linha; /* Ponteiro auxliar para percorrer as CELULAS iteriores a matriz1 */

        CELULA *PTR_aux2_linha = matriz2->sentinela_mae.abaixo; /* Uso exclusivo para percorrer a coluna de sentinelas, ou seja, a coluna -1 de todas as linhas da matriz2. Na verdade ela marca a coluna que esta sendo percorrida na matriz2  */
        CELULA *PTR_aux2 = PTR_aux2_linha; /* Ponteiro auxliar para percorrer as CELULAS iteriores a matriz2 */

        while ( PTR_aux1_linha != &(matriz1->sentinela_mae) ) /* Percorro toda a matriz1 */
        {
            PTR_aux1 = PTR_aux1_linha->proximo;
            PTR_aux2 = PTR_aux2_linha->proximo;

            while ( PTR_aux1 != PTR_aux1_linha || PTR_aux2 != PTR_aux2_linha) /* Percorro todas as CELULAS das matrizes 1 e 2 */
            {
                /* Se existir CELULAS em matriz1 e matriz2 com mesmos indices, somo seus valores */
                if ( PTR_aux1->coluna == PTR_aux2->coluna )
                {
                    if ( (PTR_aux1->dado.valor + PTR_aux2->dado.valor) != 0 )
                    {
                        printf("%d %d %d\n", PTR_aux1_linha->linha, PTR_aux1->coluna,(PTR_aux1->dado.valor + PTR_aux2->dado.valor) );
                    }

                    PTR_aux1 = PTR_aux1->proximo;
                    PTR_aux2 = PTR_aux2->proximo;
                }

                /* Se a linha "PTR_aux1_linha" for vazia ou se ja terminei de percorre-la imprimo os valores das CELULAS da linha "PTR_aux2_linha" */
                else if (PTR_aux1 == PTR_aux1_linha)
                {
                    while (PTR_aux2 != PTR_aux2_linha)
                    {
                        printf("%d %d %d\n", PTR_aux2_linha->linha, PTR_aux2->coluna,PTR_aux2->dado.valor );
                        PTR_aux2 = PTR_aux2->proximo;
                    }
                }

                /* Se a linha "PTR_aux2_linha" for vazia ou se ja terminei de percorre-la imprimo os valores das CELULAS da linha "PTR_aux1_linha" */
                else if (PTR_aux2 == PTR_aux2_linha)
                {
                    while (PTR_aux1 != PTR_aux1_linha)
                    {
                        printf("%d %d %d\n", PTR_aux1_linha->linha, PTR_aux1->coluna,PTR_aux1->dado.valor );
                        PTR_aux1 = PTR_aux1->proximo;
                    }
                }

                /* Como estou andando uma CELULA na matriz1 e na matriz2 por vez, e ao mesmo tempo, quando um indice de uma matriz ultrapassar o indice da outra, continuo imprimindo e tratando isso */
                else
                {
                    if (PTR_aux1->coluna > PTR_aux2->coluna)
                    {
                        printf("%d %d %d\n", PTR_aux2_linha->linha, PTR_aux2->coluna,PTR_aux2->dado.valor );
                        PTR_aux2 = PTR_aux2->proximo;
                    }

                    else
                    {
                        printf("%d %d %d\n", PTR_aux1_linha->linha, PTR_aux1->coluna,PTR_aux1->dado.valor );
                        PTR_aux1 = PTR_aux1->proximo;
                    }
                }
            }

            PTR_aux1_linha = PTR_aux1_linha->abaixo;
            PTR_aux2_linha = PTR_aux2_linha->abaixo;
        }

        return 1;
    }

    return 0;
}

int MultiplicarMatriz (MATRIZ *matriz1, MATRIZ *matriz2)
{
    if (matriz1->fim_horizontal->coluna == matriz2->fim_vertical->linha)
    {
        printf("# %d %d\n", matriz1->fim_vertical->linha + 1, matriz2->fim_horizontal->coluna + 1);

        int valor_temp = 0; /* Armazena o resultado da multiplicação das linhas da matriz1 pelas colunas da matriz2 */

        CELULA *PTR_aux1_linha = matriz1->sentinela_mae.abaixo; /* Uso exclusivo para percorrer a linha de sentinelas, ou seja, a linha -1 de todas as colunas da matriz1. Na verdade ela marca a linha que esta sendo percorrida na matriz1 */
        CELULA *PTR_aux1 = PTR_aux1_linha->proximo; /* Ponteiro auxliar para percorrer as CELULAS iteriores a matriz1 */

        CELULA *PTR_aux2_coluna = matriz2->sentinela_mae.proximo; /* Uso exclusivo para percorrer a coluna de sentinelas, ou seja, a coluna -1 de todas as linhas da matriz2. Na verdade ela marca a coluna que esta sendo percorrida na matriz2  */
        CELULA *PTR_aux2 = PTR_aux2_coluna; /* Ponteiro auxliar para percorrer as CELULAS iteriores a matriz2 */

        while (PTR_aux1_linha != &matriz1->sentinela_mae) /* Percorro todas as CELULAS da matriz1, movendo PTR_aux1 para frente. Como ha circularidade, PTR_aux1 voltara ate a coluna de sentinelas, entao as matrizes 1 e 2 foram multiplicadas */
        {
            while (PTR_aux1_linha == PTR_aux1_linha->proximo && PTR_aux1_linha != &matriz1->sentinela_mae) /* Perorro as linhas da matriz1 ate encontrar uma linha que nao seja vazia */
            {
                PTR_aux1_linha = PTR_aux1_linha->abaixo;
                PTR_aux1 = PTR_aux1_linha->proximo;

                PTR_aux2_coluna = matriz2->sentinela_mae.proximo;
                PTR_aux2 = PTR_aux2_coluna;
            }

            if (PTR_aux1_linha == &matriz1->sentinela_mae) /* Verifico se ja terminamos de multiplicar a matriz1 pela matriz2 */
            {
                /* Terminamos de multiplicar a matriz1 pela matriz2. Se cair neste caso na primeira iteração de "while (PTR_aux1_linha != &matriz1->sentinela_mae)" siginifica que a matriz1 eh vazia */
                return 1;
            }

            while (PTR_aux2_coluna == PTR_aux2_coluna->abaixo && PTR_aux1_linha != &matriz1->sentinela_mae) /* Verifico se estou em uma coluna vazia da matriz2 */
            {
                PTR_aux1 = PTR_aux1_linha->proximo;

                PTR_aux2_coluna = PTR_aux2_coluna->proximo;
                PTR_aux2 = PTR_aux2_coluna;

                /* Se a matriz2 for vazia a partir de uma determinada coluna, quando chegar nessa coluna, ajusto os ponteiros "PTR_aux2_coluna" para " PTR_aux2_coluna = matriz2->sentinela_mae.proximo" e "PTR_aux1_linha" para "PTR_aux1_linha = PTR_aux1_linha->abaixo" */
                if (PTR_aux2_coluna == &matriz2->sentinela_mae)
                {
                    PTR_aux1_linha = PTR_aux1_linha->abaixo;
                    PTR_aux1 = PTR_aux1_linha->proximo;

                    PTR_aux2_coluna = matriz2->sentinela_mae.proximo;
                    PTR_aux2 = PTR_aux2_coluna;
                }
            }

            while (PTR_aux1->coluna > PTR_aux2->linha && PTR_aux2->abaixo != PTR_aux2_coluna) /* Percorro as linhas de cada coluna da matriz2, em busca de indices que permitam-me multiplicar as matrizes */
            {
                PTR_aux2 = PTR_aux2->abaixo;
            }

            if (PTR_aux1->coluna == PTR_aux2->linha && PTR_aux2 != PTR_aux2_coluna) /* Se os indices necessarios forem encontrados realizo a multiplicação */
            {
                valor_temp += PTR_aux1->dado.valor * PTR_aux2->dado.valor;
            }

            if (PTR_aux1->proximo == PTR_aux1_linha && valor_temp != 0) /* Se todas as colunas da atual linha da matriz1 e todas as linhas da atual coluna da matriz2 foram percorridas, posso inserir o novo DADO na matriz_mult, caso valor_temp seja valido (diferente de zero) */
            {
                printf ("%d %d %d\n", PTR_aux1_linha->linha, PTR_aux2_coluna->coluna, valor_temp); /* Imprimi na saida padrao os indices e o valor da CELULA originada da multiplicação das matrizes 1 e 2 */

                valor_temp = 0; /* Ao completar a multiplicação da atual linha da matriz1 pela atual coluna da matriz2, limpo valor_temp */
            }

            PTR_aux1 = PTR_aux1->proximo;

            if (PTR_aux2 == PTR_aux2_coluna || PTR_aux1 == PTR_aux1_linha) /* Se todas as linhas da coluna atual da matriz2 foram percorridas, passo para a proxima coluna da matriz2 ou mesmo se todas as colunas da linha atual da matriz1 foram percorridas */
            {
                PTR_aux2_coluna = PTR_aux2_coluna->proximo;
                PTR_aux2 = PTR_aux2_coluna;

                if (PTR_aux1 == PTR_aux1_linha) /* Verifico se terminei de percorrer a atual linha da matriz1 */
                {
                    /* Se todas as colunas da linha atual da matriz1 foram percorridas, e/ou todas as linhas da coluna atual da matriz2 foram percorridas, passo para a proxima linha da matriz1 e ajusto "PTR_aux2_coluna" para " PTR_aux2_coluna = matriz2->sentinela_mae.proximo" */
                    if (PTR_aux2_coluna == &matriz2->sentinela_mae)
                    {
                        PTR_aux1_linha = PTR_aux1_linha->abaixo;
                        PTR_aux1 = PTR_aux1_linha;

                        PTR_aux2_coluna = PTR_aux2_coluna->proximo;
                        PTR_aux2 = PTR_aux2_coluna;
                    }

                    PTR_aux1 = PTR_aux1_linha->proximo; /* Como PTR_aux1 esta na coluna de sentinelas, posso ir para a proxima CELULA da linha atual */
                }
            }
        }
        return 1; /* Retorno 1 se a multiplicação das matrizes 1 e 2 foi efetuada com sucesso */
    }

    return 0; /* Retorno 0 caso as matrizes 1 e 2 nao puderem ser multiplicadas devido as suas dimensoes*/
}

/** LimparMatriz:
        Essa funSSao desaloca toda memoria alocada dinamicamente por outras funções **/

void LimparMatriz (MATRIZ *matriz)
{
    CELULA *PTR_aux_linha = matriz->sentinela_mae.abaixo;
    CELULA *PTR_aux = NULL;
    CELULA *PTR_rm = NULL;

    while ( PTR_aux_linha != &(matriz->sentinela_mae) ) /* Desaloco todas as CELULAS internas a matriz */
    {
        PTR_aux = PTR_aux_linha->proximo;
        while ( PTR_aux != PTR_aux_linha )
        {
            PTR_rm = PTR_aux;
            PTR_aux = PTR_aux->proximo;

            free(PTR_rm);
        }
        PTR_aux_linha = PTR_aux_linha->abaixo;
    }

    PTR_aux = matriz->sentinela_mae.proximo;
    while ( PTR_aux != &(matriz->sentinela_mae) ) /* Desaloco a linha de CELULAS sentinelas */
    {
        PTR_rm = PTR_aux;
        PTR_aux = PTR_aux->proximo;

        free(PTR_rm);
    }

    PTR_aux_linha = matriz->sentinela_mae.abaixo;
    while ( PTR_aux_linha != &(matriz->sentinela_mae) ) /* Desaloco a coluna de CELULAS sentinelas */
    {
        PTR_rm = PTR_aux_linha;
        PTR_aux_linha = PTR_aux_linha->abaixo;

        free(PTR_rm);
    }
}
