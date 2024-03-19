#include "APIG24.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    char *filename = argv[argc-1];
    printf("%s\n",filename);
    Grafo G = ConstruirGrafo(filename);
    DestruirGrafo(G);
  return EXIT_SUCCESS;
}