#include "EstructuraGrafo24.h"
#include "APIG24.h"
#include <stdio.h>
#include <stdlib.h>

const unsigned int numero_grande = 4294967296 - 1;

Grafo ConstruirGrafo(char *filename)
{
  Grafo G = malloc(sizeof(struct GrafoSt));
  G->num_vertices = 0;
  G->num_lados = 0;
  G->delta = 0;
  G->grados_vertices = NULL;
  G->colores = NULL;
  G->vecinos = NULL;

  FILE *file = fopen(filename,"r");
  if (file == NULL) 
  {
   fprintf(stderr, "La fila no existe.\n");
   DestruirGrafo(G);
   fclose(file);
   return NULL;
  }

  char c;
  while (!feof(file) && (c = fgetc(file)) == 'c')
  {
    while (c != '\n')
    {
      c = fgetc(file);
    }
  }
  ungetc(c,file);

  u32 n, m;
  int res = fscanf(file, "p edge %u %u", &n, &m);
  if (res != 2){
    fprintf(stderr, "Formato Invalido.\n");
    fclose(file);
    DestruirGrafo(G);
    return NULL;
  }
  if (n > 1 && m > 0)
  {
    G->num_vertices = n;
    G->num_lados = m;
  }
  else
  {
    fprintf(stderr, "No es un grafo valido,\n");
    DestruirGrafo(G);
    fclose(file);
    return NULL;
  }

  printf("vertices: %u\naristas: %u\n",G->num_vertices,G->num_lados);

  G->colores = calloc(G->num_vertices, sizeof(color));
  G->grados_vertices = calloc(G->num_vertices, sizeof(u32));
  G->vecinos = calloc(G->num_vertices, sizeof(u32 *));
  
  u32 aristas_obtenidas = 0;
  u32 v, w;

  while (!feof(file) && aristas_obtenidas < G->num_lados)
  {
    fgetc(file);
    c = fgetc(file);
    if (c != 'e')
    {
      fprintf(stderr, "Formato Invalido. Despues de p edge n m debe haber m lineas que empiezan con e\n");
      DestruirGrafo(G);
      fclose(file);
      return NULL;
    }
    ungetc(c, file);

    res = fscanf(file, "e %u %u", &v, &w);
    if (res != 2)
    {
      fprintf(stderr, "Formato Invalido. la linea no sigue el formato e i j (con i, j < n)\n");
      DestruirGrafo(G);
      fclose(file);
      return NULL;
    }
    if (v > G->num_vertices - 1 || w > G->num_vertices - 1)
    {
      fprintf(stderr, "Vertice inexistente (0 <= v, w <= n)");
      DestruirGrafo(G);
      fclose(file);
      return NULL;
    }
    if (v == w)
    {
      fprintf(stderr, "Una arista no puede conectar un vertice consigo mismo.\n");
      DestruirGrafo(G);
      fclose(file);
      return NULL;
    }
    bool arista_repetida = false;
    for (u32 j = 0; j < G->grados_vertices[v]; j++)
    {
      arista_repetida |= G->vecinos[v][j] == w;
    }
    if (arista_repetida)
    {
      fprintf(stderr, "Arista repetida.\n");
      DestruirGrafo(G);
      fclose(file);
      return NULL;
    }
    if (G->grados_vertices[v] == 0)
      G->vecinos[v] = malloc(sizeof(u32));
    else
      G->vecinos[v] = realloc(G->vecinos[v], (G->grados_vertices[v] + 1) * sizeof(u32));
    if (G->grados_vertices[w] == 0)
      G->vecinos[w] = malloc(sizeof(u32));
    else
      G->vecinos[w] = realloc(G->vecinos[w], (G->grados_vertices[w] + 1) * sizeof(u32));
    G->vecinos[v][G->grados_vertices[v]] = w;
    G->vecinos[w][G->grados_vertices[w]] = v;
    G->grados_vertices[v]++;
    G->grados_vertices[w]++;
    aristas_obtenidas++;
  }
  
  for (u32 i = 0; i < G->num_vertices; i++)
  {
    printf("\nvecinos de %u:", i);
    for (u32 j = 0; j < G->grados_vertices[i]; j++)
      printf(" %u", G->vecinos[i][j]);
  }

  fclose(file);

  for (u32 vertice = 0; vertice < G->num_vertices; vertice++)
  {
    if (G->delta < G->grados_vertices[vertice])
    {
      G->delta = G->grados_vertices[vertice];
    }
  }

  printf("\n");
  for (u32 i = 0; i < G->num_vertices; i++)
    printf("grado vertice %u: %u\n", i, G->grados_vertices[i]);
  
  printf("\ndelta: %u", G->delta);

  return G;
}

void DestruirGrafo(Grafo G) {
  if (G != NULL) {
    for (unsigned int i = 0; i < G->num_vertices; i++) {
      if (G->vecinos[i] != NULL)
        free(G->vecinos[i]);
    }
    if (G->vecinos != NULL)
      free(G->vecinos);
    if (G->colores != NULL)
      free(G->colores);
    if (G->grados_vertices != NULL)
      free(G->grados_vertices);
    free(G);
    G = NULL;
  }
}

u32 Vecino(u32 j, u32 i, Grafo G) {
  if (i < G->num_vertices && j < G->grados_vertices[i] && G->vecinos[i][j]) return G->vecinos[i][j];
  else return numero_grande;
}

u32 NumeroDeVertices(Grafo G) { return G->num_vertices; }

u32 NumeroDeLados(Grafo G) { return G->num_lados; }

u32 Delta(Grafo G) { return G->delta; }

u32 Grado(u32 i, Grafo G) {
  u32 grado = 0;
  u32 num_vertices = NumeroDeVertices(G);
  if (i < num_vertices) {
    grado = G->grados_vertices[i];
  } else if (i >= num_vertices) {
    return 0;
  }
  return grado;
}

color Color(u32 i, Grafo G) {
  if (i < G->num_vertices)
    return G->colores[i];
  else
    return numero_grande;
}

void AsignarColor(color x, u32 i, Grafo G) {
  if (G != NULL) {
    if (i >= G->num_vertices) {
      printf("No se asigno ningun color\n");
    } else {
      G->colores[i] = x;
    }
  }
}

void ExtraerColores(Grafo G, color *Color) {
  if (G != NULL) {
    for (u32 i = 0; i < G->num_vertices; i++) {
      Color[i] = G->colores[i];
    }
  }
}

void ImportarColores(color *Color, Grafo G) {
  if (G != NULL) {
    for (u32 i = 0; i < G->num_vertices; i++) {
      G->colores[i] = Color[i];
    }
  }
}