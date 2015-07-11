/**
  Aluno: Lemuel Roberto
  Número USP: 7277728
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTEX 100
#define MAX_EDGE 200

/**
  Notação para identificar os ponteiros link1, link2 e link3:
  *) No grafo:
    *) Vértice:
      -element: representa o elemento que está armazenado no vértice
      -link1: aponta para o próximo vértice na lista de vértice da estrutura Graph
      -link2: aponta para a primeira aresta que une este vértice (v1) à um outro vértice (v?)
      -link3: não é utilizado na representação de vértice (aponta sempre para NULL)
    *) Aresta:
      -element: representa o peso da aresta
      -link1: aponta para a próxima aresta (que une o mesmo vértice v1 à um outro vértice (v?))
      -link2: aponta para o vértice v1 da aresta
      -link3: aponta para o vértice v2 da aresta
  *) No mapa de vértices:
      -element: representa o ID do vértice
      -link1: aponta para o próximo vértice do mapa (como eu insiro sempre no fim, o mapa está sempre ordenado)
      -link2: aponta para o vértice no grafo
      -link3: não é utilizado na representação de vértice (aponta sempre para NULL)
  *) No mapa de arestas:
    -element: representa o ID da aresta
    -link1: aponta para a próxima aresta do mapa (como eu insiro sempre no fim, o mapa está sempre ordenado)
    -link2: aponta para a aresta v1-v2 no mapa
    -link3: aponta para a aresta v2-v1 no mapa
*/

typedef struct Node {
  int element;
  struct Node *link1;
  struct Node *link2;
  struct Node *link3;
} Node;

typedef struct {
  Node root;
  Node *end;

  int quantVertex;
  int quantEdge;
} Graph;

typedef struct {
  Node root;
  Node *end;

  int nextID;
} Map;

void startGraph (Graph *g) {
  g->quantVertex = 0;
  g->quantEdge = 0;

  g->root.element = -1;
  g->root.link1 = NULL;
  g->root.link2 = NULL;
  g->root.link3 = NULL;

  g->end = NULL;
}

void startMap (Map *map) {
  map->nextID = 1; /* o primeiro ID seŕa 1 */

  map->root.element = -1;
  map->root.link1 = NULL;
  map->root.link2 = NULL;
  map->root.link3 = NULL;

  map->end = NULL;
}

int addVertexMap (Map *map, Node *vertex) {
  Node *newNode = (Node *) malloc (sizeof (Node));

  if (newNode != NULL) {
    newNode->element = map->nextID++; /* armazeno o ID do vértice */
    newNode->link1 = NULL; /* defino que o próximo no mapa de vértices é NULL */
    newNode->link2 = vertex; /* aponta para o vertice no grafo */
    newNode->link3 = NULL; /* sempre NULL */

    if (map->end == NULL) {
      map->root.link1 = newNode;
    } else {
      map->end->link1 = newNode;
    }

    map->end = newNode;

    return 1;
  } else {
    return 0;
  }
}

int searchVertex (Map *map, int ID, Node **result) {
  Node *aux = map->root.link1;

  while (aux != NULL) {
    if (aux->element == ID) {
      *result = aux->link2; /* retorno o vértice no grafo */

      return 1; /* no trabalho proposto sempre retornará 1 poque não será passado ID inválido */
    } else if (aux->element > ID) {
      return 0;
    }

    aux = aux->link1;
  }

  return 0;
}

int addEdgeMap (Map *map, Node *edge1, Node *edge2) {
  Node *newNode = (Node *) malloc (sizeof (Node));

  if (newNode != NULL) {
    newNode->element = map->nextID++; /* armazeno o ID do vértice */
    newNode->link1 = NULL; /* defino que o próximo no mapa de arestas é NULL */
    newNode->link2 = edge1; /* aponta para a aresta v1-v2 no grafo */
    newNode->link3 = edge2; /* aponta para a aresta v2-v1 no grafo */

    if (map->end == NULL) {
      map->root.link1 = newNode;
    } else {
      map->end->link1 = newNode;
    }

    map->end = newNode;

    return 1;
  } else {
    return 0;
  }
}

int searchEdgeMap (Map *map, int ID, Node **edge1, Node **edge2) {
  Node *aux = map->root.link1;

  while (aux != NULL) {
    if (aux->element == ID) {
      *edge1 = aux->link2; /* retorno a aresta v1-v2 no grafo */
      *edge2 = aux->link3; /* retorno a aresta v2-v1 no grafo */

      return 1; /* no trabalho proposto sempre retornará 1 poque não será passado ID inválido */
    } else if (aux->element > ID) {
      return 0;
    }

    aux = aux->link1;
  }

  return 0;
}

int searchEdgeGraph (Graph *g, Node *vertex1, Node *vertex2, Node **edge) {
  Node *aux = vertex1->link2; /* aux é a primeira aresta v1-v? */

  while (aux != NULL) {
    if (aux->link2 == vertex2) {
      *edge = aux->link2;

      return 1;
    }
    aux = aux->link1;
  }

  return 0; /* a aresta não foi encontrada (não acontece neste trabalho) */
}

Node* insertVertex (Graph *g, int element, Map *map) {
  Node *newNode = (Node *) malloc (sizeof (Node));

  if (newNode != NULL) {
    newNode->element = element; /* armazeno o elemento passado */
    newNode->link1 = NULL; /* defino que o próximo na lista de vértices é NULL */
    newNode->link2 = NULL; /* lista de adjacentes vazia */
    newNode->link3 = NULL; /* sempre NULL */

    if (g->end == NULL) {
      g->root.link1 = newNode;
    } else {
      g->end->link1 = newNode;
    }

    g->end = newNode;

    g->quantVertex++; /* incremento a quantidade de vértices do grafo */

    addVertexMap (map, newNode);

    return newNode;
  } else {
    return NULL;
  }
}

int insertEdge (Graph *g, int v1, int v2, int weight, Map *vmap, Map *emap, Node **result1, Node **result2) {
  Node *vertex1 = NULL; /* representa o vértice v1 */
  Node *vertex2 = NULL; /* representa o vértice v2 */

  /* busca pelos vértices v1 e v2 */
  searchVertex (vmap, v1, &vertex1);
  searchVertex (vmap, v2, &vertex2);

  Node *newNode1 = (Node *) malloc (sizeof (Node)); /* aresta v1-v2 */
  Node *newNode2 = (Node *) malloc (sizeof (Node)); /* aresta v2-v1 */

  if (newNode1 != NULL && newNode2 != NULL) {
    /* insiro a aresta v1-v2 (newNode1) */
    /* crio a aresta */
    newNode1->element = weight; /* armazena o peso da aresta */
    newNode1->link1 = vertex1->link2; /* o próximo de newNode1 é a primeira aresta adjacente (vou inserir no início) */
    newNode1->link2 = vertex1; /* aponta para o vértice v1 */
    newNode1->link3 = vertex2; /* aponta para o vértice v2 */

    vertex1->link2 = newNode1; /* a primeira aresta de v1 é newNode1 */

    if (result1 != NULL)
      *result1 = newNode1; /* retorno a aresta v1-v2 */

    /* insiro a aresta v2-v1 (newNode2) */
    /* crio a aresta */
    newNode2->element = weight;
    newNode2->link1 = vertex2->link2;
    newNode2->link2 = vertex2;
    newNode2->link3 = vertex1;

    vertex2->link2 = newNode2;

    if (result2 != NULL)
      *result2 = newNode2;

    g->quantEdge++; /* incremento a quantidade de arestas do grafo */

    /* insiro a aresta criada, no mapa de arestas */
    addEdgeMap (emap, newNode1, newNode2);

    return 1;
  } else {
    return 0;
  }
}

int removeEdge (Graph *g, int ID, Map *vmap, Map *emap) {
  int weight;

  Node *edge1 = NULL;
  Node *edge2 = NULL;

  searchEdgeMap (emap, ID, &edge1, &edge2);

  if (edge1 == NULL || edge2 == NULL) {
    return -1;
  }

  weight = edge1->element;

  Node *aux = NULL; /* auxiliar para percorrer as arestas e realizar a remoção */

  /* removo a aresta v1-v2 */
  aux = edge1->link2->link2; /* aux aponta para a primeira aresta v1-v? */

  if (aux != edge1) { /* remoção de uma aresta qualquer */
    while (aux != NULL && aux->link1 != edge1) {
      aux = aux->link1;
    }

    Node *rm = aux->link1;
    aux->link1 = aux->link1->link1;
    /* limpo rm */
    rm->link1 = NULL;
    rm->link2 = NULL;
    rm->link3 = NULL;
    free (rm);
  } else { /* remoção da primeira aresta */
    if (aux->link1 == NULL) { /* tem apenas uma aresta */
      Node *rm = aux;
      edge1->link2->link2 = NULL; /* a lista de arestas v1-v? está vazia */
      /* limpo rm */
      rm->link1 = NULL;
      rm->link2 = NULL;
      rm->link3 = NULL;
      free (rm);
    } else { /* tem mais de uma aresta */
      Node *rm = aux;
      edge1->link2->link2 = aux->link1;
      /* limpo rm */
      rm->link1 = NULL;
      rm->link2 = NULL;
      rm->link3 = NULL;
      free (rm);
    }
  }

  /* removo a aresta v1-v2 */
  aux = edge2->link2->link2;

  if (aux != edge2) {
    while (aux != NULL && aux->link1 != edge2) {
      aux = aux->link1;
    }

    Node *rm = aux->link1;
    aux->link1 = aux->link1->link1;
    /* limpo rm */
    rm->link1 = NULL;
    rm->link2 = NULL;
    rm->link3 = NULL;
    free (rm);
  } else {
    if (aux->link1 == NULL) {
      Node *rm = aux;
      edge2->link2->link2 = NULL;
      /* limpo rm */
      rm->link1 = NULL;
      rm->link2 = NULL;
      rm->link3 = NULL;
      free (rm);
    } else {
      Node *rm = aux;
      edge2->link2->link2 = aux->link1;
      /* limpo rm */
      rm->link1 = NULL;
      rm->link2 = NULL;
      rm->link3 = NULL;
      free (rm);
    }
  }

  /* removo a aresta do mapa de arestas */
  aux = &emap->root;
  while (aux != NULL && aux->link1->element != ID) {
    aux = aux->link1;
  }

  Node *rm = aux->link1;
  aux->link1 = aux->link1->link1;

  if (aux->link1 == NULL) { /* retirei a aresta final do mapa */
    emap->end = aux;
  }
  if (aux == emap->end) { /* retirei a aresta final do mapa  */
    emap->end = NULL;
  }

  /* limpo rm */
  rm->link1 = NULL;
  rm->link2 = NULL;
  rm->link3 = NULL;
  free (rm);

  g->quantEdge--;

  return weight;
}

int findEdgeID (Node *edge, Map *emap) {
  Node *aux = emap->root.link1; /* aux aponta para a primeira aresta do mapa de arestas */

  while (aux != NULL) {
    if (aux->link2 == edge || aux->link3 == edge) {
      return aux->element; /* retorno a ID da aresta */
    }
    aux = aux->link1;
  }

  return -1; /* não encontrou (neste trabalho não acontece isto) */
}

int findVertexID (Node *vertex, Map *vmap) {
  Node *aux = vmap->root.link1; /* aux aponta para o primeiro vértice do mapa de vértices */

  while (aux != NULL) {
    if (aux->link2 == vertex) {
      return aux->element; /* retorno a ID do vértice */
    }
    aux = aux->link1;
  }

  return -1; /* não encontrou (neste trabalho não acontece isto) */
}

void clearEdgeList (Graph *g, Node *firstEdge, Map *vmap, Map *emap) {
  Node *aux = firstEdge;

  Node *next = aux->link1;

  while (aux != NULL) {
    removeEdge (g, findEdgeID (aux, emap), vmap, emap);
    aux = next;
    if (next != NULL)
      next = next->link1;
  }
}

int removeVertex (Graph *g, int ID, Map *vmap, Map *emap) {
  int element;

  Node *vertex = NULL; /* vértice que será removido */

  searchVertex (vmap, ID, &vertex);

  if (vertex == NULL) {
    return -1;
  }

  element = vertex->element;

  /* remove as arestas que contém este vértice */
  if (vertex->link2 != NULL) {
    clearEdgeList (g, vertex->link2, vmap, emap);
  }

  /* remove o vértice */
  Node *aux = NULL;

  aux = &g->root; /* auxiliar para percorrer a lista de vértices */

  while (aux != NULL && aux->link1 != vertex) {
    aux = aux->link1;
  }

  Node *rm2 = aux->link1;
  aux->link1 = aux->link1->link1;

  if (aux->link1 == NULL) { /* o grafo tem apenas um vértice */
    g->end = aux;
  } else if (vmap->root.link1 == NULL) { /* o grafo está vazio */
    g->end = NULL;
  }

  /* limpo rm2 */
  rm2->link1 = NULL;
  rm2->link2 = NULL;
  rm2->link3 = NULL;
  free (rm2);

  aux = &vmap->root; /* auxiliar para percorrer o mapa */

  while (aux != NULL && aux->link1->element < ID) {
    aux = aux->link1;
  }

  Node *rm1 = aux->link1;
  aux->link1 = aux->link1->link1;

  if (aux->link1 == NULL) { /* retirei o vértice final do mapa */
    vmap->end = aux;
  }
  if (aux == vmap->end) { /* retirei o último vértice do mapa */
    vmap->end = NULL;
  }

  /* limpo rm1 */
  rm1->link1 = NULL;
  rm1->link2 = NULL;
  rm1->link3 = NULL;
  free (rm1);

  g->quantVertex--;

  return element;
}

void printGraph (Graph *g, Map *vmap, Map *emap) {
  /* impressão dos vértices */
  printf ("%d\n", g->quantVertex);

  Node *aux;

  aux = vmap->root.link1; /* aux é o primeiro vértice do mapa de vértices */

  while (aux != NULL) {
    printf ("%d %d\n", aux->element, aux->link2->element);

    aux = aux->link1;
  }

  /* impressão das arestas */
  printf ("%d\n", g->quantEdge);

  aux = emap->root.link1; /* aux é a primeira aresta do mapa de arestas */

  while (aux != NULL) {
    int ID1, ID2;

    ID1 = findVertexID (aux->link2->link2, vmap);
    ID2 = findVertexID (aux->link2->link3, vmap);

    printf ("%d %d %d %d\n", aux->element, ID1, ID2, aux->link2->element);

    aux = aux->link1;
  }
}

void changeElementVertex (int ID, int element, Map *vmap) {
  Node *vertex = NULL;

  searchVertex (vmap, ID, &vertex);

  vertex->element = element;
}

void changeWeightEdge (int ID, int weight, Map *emap) {
  Node *edge1 = NULL;
  Node *edge2 = NULL;

  searchEdgeMap (emap, ID, &edge1, &edge2);

  edge1->element = weight;
  edge2->element = weight;
}

void clearGraph (Graph *g, Map *vmap, Map *emap) {
  while (vmap->root.link1 != NULL) {
    removeVertex (g, vmap->root.link1->element, vmap, emap);
  }
}

int smallerWeight (int *weight, int *inTree) {
  int smaller = 0;

  int i;

  for (i = 1; i <= MAX_VERTEX; ++i) {
    if (inTree[i] == 0 && weight[i] < weight[smaller]) {
      smaller = i;
    }
  }
  return smaller;
}

void shortestPath (Graph *g, int v1, int v2, Map *vmap, Map *emap) {
  int inTree[MAX_VERTEX + 1];
  int parent[MAX_VERTEX + 1];
  int weight[MAX_VERTEX + 1];

  int i;

  for (i = 0; i <= MAX_VERTEX; ++i) {
    inTree[i] = 0;
    parent[i] = -1;
    weight[i] = INT_MAX;
  }

  Node *aux = NULL; /* auxiliar para percorrer os vértices do grafo (aux será sempre um vértice) */

  weight[v1] = 0; /* v1 tem o menor peso */

  Node *vertex2 = NULL; /* vértice de parada */

  searchVertex (vmap, v2, &vertex2);

  int IDAux; /* auxiliar para percorrer os vértices do grafo no Dikkstra */

  do {
    IDAux = smallerWeight (weight, inTree); /* ID do vértice de menor peso ainda não processado */
    inTree[IDAux] = 1; /* vértice processado */

    searchVertex (vmap, IDAux, &aux); /* aux recebe vértice (no grafo) de menor peso ainda não processado */

    Node *edge = aux->link2; /* aresta auxiliar para percorrer a lista de arestas Vx-V? */

    while (edge != NULL) { /* enquanto não passar por todas as arestas Vx-V? */
      int v = findVertexID (edge->link3, vmap); /* ID do vértice oposto */

      if (inTree[v] == 0 && weight[v] > edge->element + weight[IDAux]) {
        weight[v] = edge->element + weight[IDAux]; /* atualizo o peso do vértice */
        parent[v] = IDAux; /* atualizo o predecessor do vértice v */
      }
      edge = edge->link1;
    }
  } while (aux != vertex2);

  printf ("%d\n", weight[IDAux]); /* imprimo o peso do menor caminho entre v1 e v2 */

  int path[MAX_VERTEX];
  int pathLength = 0;

  int lastParent = IDAux;

  for (i = 0; lastParent != -1; ++i) {
    path[i] = lastParent;
    lastParent = parent[lastParent];
    pathLength++;
  }

  printf ("%d", path[pathLength - 1]);

  for (i = pathLength - 2; i >= 0 ; --i) {
    printf (" %d", path[i]);
  }
  printf ("\n");
}

int main (void) {
  Map vmap; /* mapa de vértices */
  Map emap; /* mapa de arestas */

  startMap (&vmap);
  startMap (&emap);

  Graph g;

  startGraph (&g);

  char command[3];

  do {
    scanf ("%s", command);

    if (strcmp (command, "CV") == 0) {

      int element;

      scanf (" %d", &element);

      insertVertex (&g, element, &vmap);
    } else if (strcmp (command, "DV") == 0) {
      int v;
      scanf ("%d", &v);

      removeVertex (&g, v, &vmap, &emap);
    } else if (strcmp (command, "CA") == 0) {
      int v1, v2, weight;

      scanf (" %d %d %d", &v1, &v2, &weight);

      if (v1 < v2) {
        insertEdge (&g, v1, v2, weight, &vmap, &emap, NULL, NULL);
      } else {
        insertEdge (&g, v2, v1, weight, &vmap, &emap, NULL, NULL);
      }
    } else if (strcmp (command, "DA") == 0) {

      int e;

      scanf ("%d", &e);

      removeEdge (&g, e, &vmap, &emap);
    } else if (strcmp (command, "TV") == 0) {
      int v;
      int element;

      scanf ("%d %d", &v, &element);

      changeElementVertex (v, element, &vmap);
    } else if (strcmp (command, "TA") == 0) {
      int e;
      int newWeight;

      scanf ("%d %d", &e, &newWeight);

      changeWeightEdge (e, newWeight, &emap);
    } else if (strcmp (command, "IG") == 0) {
      printGraph (&g, &vmap, &emap);
    } else if (strcmp (command, "CM") == 0) {
      int v1, v2;

      scanf ("%d %d", &v1, &v2);

      shortestPath (&g, v1, v2, &vmap, &emap);
    }
  } while (strcmp (command, "FM") != 0);

  clearGraph (&g, &vmap, &emap);

  return 0;
}
