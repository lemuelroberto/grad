/**
	Lemuel Roberto
  Marcelo Petrucelli
**/

/**	Agradecimentos especiais:
		Ao Thiago Bellini por ver a falta de 2 casos na remoçao
		A Vaneça Lima por conseguir liberar toda memoria alocada
**/

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int chave;
    int valor;
} INFO;

typedef struct NO
{
    INFO info;
    struct NO *pai;
    struct NO *fesq;
    struct NO *fdir;
    char cor;
} NO;

typedef struct
{
    NO *raiz;
    NO nil; /* Marcador para o qual toda folha apontara para ele */
} ARVORE;

/** Prototipos das funçoes de criaçao e destruiçao da ARVORE **/
void CriarArvore (ARVORE *arvore);
void LimparArvore (ARVORE *arvore);

/** Prototipos das funçoes de inserçao e remoçao de novas INFOS na ARVORE**/
int Inserir (ARVORE *arvore, INFO *info);
void Remover(ARVORE *arvore, INFO *info);

/** Prototipos das funçoes de I/O **/
int ReceberEntrada (ARVORE *arvore);
void Imprimir (ARVORE *arvore);

int main (void)
{
    ARVORE arvore;

    CriarArvore (&arvore);
    ReceberEntrada (&arvore);
    LimparArvore (&arvore);

    return 0;
}

/** CriarArvore: inicia a estrutura ARVORE
        Parametros:
            *)ARVORE *arvore: recebe a estrutura a ser inicializada
**/

void CriarArvore (ARVORE *arvore)
{
    /* Ajusta o marcador "nil" */
    arvore->nil.cor = 'p';
    arvore->nil.fesq = &arvore->nil;
    arvore->nil.fdir = &arvore->nil;

    /* Ajusta a "raiz" */
    arvore->raiz = &arvore->nil;
    arvore->raiz->pai = &arvore->nil;
    arvore->raiz->fesq = &arvore->nil;
    arvore->raiz->fdir = &arvore->nil;

}

/** RotacaoEsquerda: faz a rotaçao sobre um NO para a esquerda
        Parametros:
            *)ARVORE *arvore: recebe a ARVORE que contem o NO a ser rotacionado
            *)NO *centro: recebe um ponteiro para o NO que sera o centro da rotaçao
**/

void RotacaoEsquerda (ARVORE *arvore, NO *centro)
{
    NO *PTR_aux = centro->fdir;
    centro->fdir = PTR_aux->fesq;
    PTR_aux->fesq->pai = centro;
    PTR_aux->pai = centro->pai;

    if (centro->pai == &arvore->nil)
    {
        arvore->raiz = PTR_aux;
    }
    else
    {
        if (centro == centro->pai->fesq)
        {
            centro->pai->fesq = PTR_aux;
        }
        else
        {
            centro->pai->fdir = PTR_aux;
        }
    }
    PTR_aux->fesq = centro;
    centro->pai = PTR_aux;
}

/** RotacaoEsquerda: faz a rotaçao sobre um NO para a direita
        Parametros:
            *)ARVORE *arvore: recebe a ARVORE que contem o NO a ser rotacionado
            *)NO *centro: recebe um ponteiro para o NO que sera o centro da rotaçao
**/

void RotacaoDireita (ARVORE *arvore, NO *centro)
{

    NO *PTR_aux = centro->fesq;
    centro->fesq = PTR_aux->fdir;
    PTR_aux->fdir->pai = centro;
    PTR_aux->pai = centro->pai;
    if (centro->pai == &arvore->nil)
    {
        arvore->raiz = PTR_aux;
    }
    else
    {
        if (centro == centro->pai->fdir)
        {
            centro->pai->fdir = PTR_aux;
        }
        else
        {
            centro->pai->fesq = PTR_aux;
        }
    }
    PTR_aux->fdir = centro;
    centro->pai = PTR_aux;
}

/** InserirAux: eça funçao rearranja a ARVORE paçada para que fique de acordo com todos os principios de uma Arvore Vermelha e Preta,
        ou seja, ela realiza rotaçoes (simples ou duplas) e recolore NOS para que nao violem nenhuma premiça
        Parametros:
            *)ARVORE *arvore: recebe a ARVORE na qual estamos inserindo o novo NO
            *)NO *no: recebe um ponteiro para o NO inserido em Inserir ()

    Observaçao: Esta funçao foi escrita com base na teoria e no pseudocodigo do livro de apoio citado na especificaçao do trabalho
**/

void InserirAux (ARVORE *arvore, NO *no)
{
    /* Ajusta os NOS segundo suas cores ate estes nao violem nenhuma regra da Arvore Vermelha e Preta */
    while (no->pai->cor == 'v')
    {
        if (no->pai == no->pai->pai->fesq)
        {
            NO *PTR_aux = no->pai->pai->fdir;

            if (PTR_aux->cor == 'v')
            {
                no->pai->cor = 'p';
                PTR_aux->cor = 'p';
                no->pai->pai->cor = 'v';
                no = no->pai->pai;
            }
            else
            {
                /* Caso entre no "if" fara rotaçao dupla */
                if (no == no->pai->fdir)
                {
                    no = no->pai;
                    RotacaoEsquerda (arvore, no);
                }

                no->pai->cor = 'p';
                no->pai->pai->cor = 'v';
                RotacaoDireita (arvore, no->pai->pai);
            }
        }
        else
        {
            NO *PTR_aux = no->pai->pai->fesq;

            if (PTR_aux->cor == 'v')
            {
                no->pai->cor = 'p';
                PTR_aux->cor = 'p';
                no->pai->pai->cor = 'v';
                no = no->pai->pai;
            }
            else
            {
                if (no == no->pai->fesq)
                {
                    no = no->pai;
                    RotacaoDireita (arvore, no);
                }

                no->pai->cor = 'p';
                no->pai->pai->cor = 'v';
                RotacaoEsquerda (arvore, no->pai->pai);
            }
        }
    }

    arvore->raiz->cor = 'p';
}

/** Inserir: insere um NO da mesma maneira que se inserem em Arvores de Buscas Binarias,
        mas respeitando as regras de uma Arvore Vermelha e Preta. Apos inserir, chamo InserirAux para restaurar poçiveis violaçoes das premiças
        Parametros:
            *)ARVORE *arvore: recebe a ARVORE que recebera a nova INFO
            *)INFO *info: recebe INFO que sera incapsulada em um NO que sera efetivamente adicionado a ARVORE
        Retorno: retorno 1 caso NO tenha sido alocado e 0 (zero) caso contrario.
**/

int Inserir (ARVORE *arvore, INFO *info)
{
    NO *PTR_aux_percurso = arvore->raiz;
    NO *PTR_aux_alvo = &arvore->nil;

    /* Encontro o local de inserçao */
    while (PTR_aux_percurso != &arvore->nil && info->chave != PTR_aux_percurso->info.chave)
    {
        PTR_aux_alvo = PTR_aux_percurso;

        if (info->chave < PTR_aux_percurso->info.chave)
        {
            PTR_aux_percurso = PTR_aux_percurso->fesq;
        }
        else if (info->chave > PTR_aux_percurso->info.chave)
        {
            PTR_aux_percurso = PTR_aux_percurso->fdir;
        }
    }

    /* Apesar de nao ter visto nos casos de teste, trato a inserçao de uma chave que ja existe na ARVORE,
        apenas substituindo o valor na INFO que ja existe, mesmo iço nao fazendo muito sentido por "chave" e "valor" açumirem o mesmo valor,
        mas se tiveçemos chaves repetidas faria toda diferença */
    if (PTR_aux_percurso == &arvore->nil)
    {
        NO *PTR_novo = (NO *) malloc (sizeof (NO));

        if (PTR_novo != NULL)
        {
            PTR_novo->info = *info;

            PTR_novo->pai = PTR_aux_alvo;

            if (PTR_aux_alvo == &arvore->nil)
            {
                arvore->raiz = PTR_novo;
            }
            else if (PTR_novo->info.chave < PTR_aux_alvo->info.chave)
            {
                PTR_aux_alvo->fesq = PTR_novo;
            }
            else
            {
                PTR_aux_alvo->fdir = PTR_novo;
            }

            PTR_novo->fesq = &arvore->nil;
            PTR_novo->fdir = &arvore->nil;
            PTR_novo->cor = 'v';

            /* Restauro poçiveis violaçoes de premiças na ARVORE */
            InserirAux (arvore, PTR_novo);

            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        PTR_aux_percurso->info = *info;
        return 1;
    }
}

/** RemoverAux: funçao auxiliar a Remover, que tem um papel similar a InserirAux
        Parametros:
            *)ARVORE *arvore: recebe a ARVORE da qual sera removido um NO
            *)NO *aux: ponteiro auxiliar para NO, que percorrera a ARVORE em busca de violaçoes e inconsistencias em NOS
**/

void RemoverAux(ARVORE *arvore, NO *aux)
{
    NO *aux_irm;

    /* Similarmente a Inserir, percorre a ARVORE em busca de incosistencias */
    while (aux->cor == 'p' && aux != arvore->raiz)
    {
        if (aux == aux->pai->fesq)
        {
            aux_irm = aux->pai->fdir;
            if (aux_irm->cor == 'v')
            {
                aux_irm->cor = 'p';
                aux->pai->cor = 'v';
                RotacaoEsquerda(arvore, aux->pai);
                aux_irm = aux->pai->fdir;
            }
            if (aux_irm->fdir->cor == 'p' && aux_irm->fesq->cor == 'p')
            {
                aux_irm->cor = 'v';
                aux = aux->pai;
            }
            else
            {
                if (aux_irm->fdir->cor == 'p')
                {
                    aux_irm->fesq->cor = 'p';
                    RotacaoDireita(arvore, aux_irm);
                    aux_irm = aux->pai->fdir;
                }
                aux_irm->cor = aux->pai->cor;
                aux->pai->cor = 'p';
                aux_irm->fdir->cor = 'p';
                RotacaoEsquerda(arvore, aux->pai);
                aux = arvore->raiz;
            }
        }
        else
        {
            aux_irm = aux->pai->fesq;
            if ( aux_irm->cor == 'v')
            {
                aux_irm->cor = 'p';
                aux->pai->cor = 'v';
                RotacaoDireita(arvore, aux->pai);
                aux_irm = aux->pai->fesq;
            }
            if ( aux_irm->fdir->cor == 'p' && aux_irm->fesq->cor == 'p')
            {
                aux_irm->cor = 'v';
                aux = aux->pai;
            }
            else
            {
                if ( aux_irm->fesq->cor == 'p')
                {
                    aux_irm->fdir->cor = 'p';
                    RotacaoEsquerda(arvore, aux_irm);
                    aux_irm = aux->pai->fesq;
                }
                aux_irm->cor = aux->pai->cor;
                aux->pai->cor = 'p';
                aux_irm->fdir->cor = 'p';
                RotacaoDireita(arvore, aux->pai);
                aux = arvore->raiz;
            }
        }
    }

    aux->cor = 'p';
}

/** Remover: remove um NO da ARVORErespeitando as regras de uma Arvore Vermelha e Preta. Apos remover, chamo InserirAux para restaurar poçiveis violaçoes das premiças
        Parametros:
            *)ARVORE *arvore: recebe a ARVORE que tera uma INFO apagada
            *)INFO *info: recebe a INFO que contem a "chave" procurada para ser removida
**/

void Remover(ARVORE *arvore, INFO *info)
{
    NO *aux_rem, *aux3, *aux2 = &arvore->nil, *aux = arvore->raiz;

    /* Busco pela "chave", caso encontrar eu a removo, senao apenas saio da funçao.
        Poderia ate retornar um inteiro para dizer se encontriei ou nao a chave buscada, ou ate mesmo um ponteiro para o NO removido,
        podendo ate liberar a memoria fora da funçao remover */
    while (aux != &arvore->nil && info->chave != aux->info.chave)
    {
        if (info->chave < aux->info.chave)
        {
            aux = aux->fesq;
        }
        else if (info->chave > aux->info.chave)
        {
            aux = aux->fdir;
        }
    }

    if (aux != &arvore->nil)
    {
        if (aux->fdir == &arvore->nil || aux->fesq == &arvore->nil)
        {
            aux_rem = aux;
        }
        else
        {
            aux_rem = aux->fesq;
            while (aux_rem->fdir != &arvore->nil)
                aux_rem = aux_rem->fdir;
        }
        if (aux_rem->fesq != &arvore->nil)
        {
            aux3 = aux_rem->fesq;
        }
        else
        {
            aux3 = aux_rem->fdir;
        }
        if (aux3 != &arvore->nil)
        {
            aux3->pai = aux_rem->pai;
        }
        else
        {
            aux2 = NULL;
            aux2 = (NO *) malloc(sizeof (NO));
            aux2->pai = aux_rem->pai;
            aux2->cor = 'p';
            aux2->fesq = &arvore->nil;
            aux2->fdir = &arvore->nil;
            aux3 = aux2;
        }
        if (aux_rem == arvore->raiz)
        {
            arvore->raiz = aux3;
        }
        else
        {
            if (aux_rem == aux_rem->pai->fesq)
                aux_rem->pai->fesq = aux3;
            else
                aux_rem->pai->fdir = aux3;
        }
        if (aux_rem != aux)
        {
            aux->info.chave = aux_rem->info.chave;
            aux->info.valor = aux_rem->info.valor;
        }
        if (aux_rem->cor == 'p')
        {
            RemoverAux(arvore, aux3);
        }

        if (aux2 != &arvore->nil)
        {
            if (aux2->pai->fesq == aux2)
                aux2->pai->fesq = &arvore->nil;
            else
                aux2->pai->fdir = &arvore->nil;
            free (aux2);
        }
        free (aux_rem);
    }
}

/** ReceberEntrada: recebe os dados da entrada padrao e os trata de maneira adequada, deliberando operaçoes de inserçao e remoçao
        Parametros:
            *)ARVORE *arvore: recebe a estrutura que recebera ou que perdera determinadas chaves, dependendo da operaçao escolhida
        Retorno: retorno 1 caso Inserir nao retorne 0 (zero), caso contrario retorno 0
**/

int ReceberEntrada (ARVORE *arvore)
{
    char operacao;
    int chave;

    INFO info;

    while (scanf (" %c %d", &operacao, &chave) != EOF)
    {
        switch (operacao)
        {
        case 'I':
        case 'i':
            info.chave = chave;
            info.valor = chave;
            if (!Inserir (arvore, &info))
            {
                return 0;
            }
            Imprimir (arvore);
            break;

        case 'R':
        case 'r':
            info.chave = chave;
            info.valor = chave;
            Remover(arvore, &info);
            Imprimir (arvore);
            break;

        default:
            break;
        }
    }

    return 1;
}

/** ImprimirAninhado: percorre a ARVORE em pre-ordem e imprimi o valor de cada INFO
        *)ARVORE *arvore: recebe a ARVORE que sera impreça
        *)NO *no: em cada instancia da recersividade eh um NO diferente, mas o primeiro NO eh arvore->raiz
**/

void ImprimirAninhado (ARVORE *arvore, NO *no)
{
    printf ("%d", no->info.valor);
    if (no->fesq != &arvore->nil)
    {
        printf (",{");
        ImprimirAninhado (arvore, no->fesq);
        printf ("}");
    }

    if (no->fdir != &arvore->nil)
    {
        printf (",{");
        ImprimirAninhado (arvore, no->fdir);
        printf ("}");
    }
}

/** Imprimir: imprimi, na saida padrao, com o auxilio de ImprimirAninhado, a ARVORE paçada
        Parametro:
            *)ARVORE *arvore: recebe a ARVORE que sera impreça
**/

void Imprimir (ARVORE *arvore)
{
    if(arvore->raiz != &arvore->nil)
    {
        printf ("{");
        ImprimirAninhado (arvore, arvore->raiz);
        printf ("}\n");
    }
}

/** LimparArvoreAux: funçao auxiliar, de LimparArvore, que percorre a arvore em pos-ordem recursivamente
        Parametros:
            *)ARVORE *arvore: recebe a ARVORE que sera limpada
            *)NO *no: em cada instancia da recersividade eh um NO diferente, mas o primeiro NO eh arvore->raiz
**/

void LimparArvoreAux (ARVORE *arvore, NO *no)
{
    if (no->fdir != &arvore->nil)
        LimparArvoreAux (arvore, no->fdir);

    if (no->fesq != &arvore->nil)
        LimparArvoreAux(arvore, no->fesq);

    if (no->fdir == &arvore->nil && no->fesq == &arvore->nil)
    {
        if(no->pai == &arvore->nil)
        {
            free(no);
        }
        else if(no->pai->fesq == no)
        {
            no->pai->fesq = &arvore->nil;
        	free (no);
        }
        else
        {
            no->pai->fdir = &arvore->nil;
            free(no);
        }
    }
}

/** LimparArvore: limpa uma ARVORE, ou seja, remove todo NO desta
        Parametro:
            *)ARVORE *arvore: recebe a ARVORE que sera limpada
**/

void LimparArvore (ARVORE *arvore)
{
    if (arvore->raiz != &arvore->nil)
    {
        LimparArvoreAux (arvore, arvore->raiz);
    }
}
