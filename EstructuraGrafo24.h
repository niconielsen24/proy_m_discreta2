#ifndef   ESTRUCTURAGRAFO24_H
#define ESTRUCTURAGRAFO24_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t u32;
typedef u32 color;

struct GrafoSt{
  u32 num_vertices;
  u32 num_lados;
  u32 delta;
  u32 *grados_vertices;
  color *colores;
  u32 **vecinos;
};

#endif