#ifndef _MAP_H_
#define _MAP_H_

#include "config.h"

// map unit represents a concrete cube postion and style.
// follow OpenGl coordinate system.
typedef union {
  unsigned int value;
  struct {
    // [0, CHUNK_SIZE]
    unsigned char x;
    unsigned char y;
    unsigned char z;
    unsigned char w;
  } u;
} MapUnit;

// chunk is CHUNK_SIZE^3 big cube containing MapUnits.
typedef struct {
  int dx;
  int dy;
  int dz;
  int size;
  int pos; // used data marker, -1 for not used
  MapUnit* data;
} MapChunk;

void delete_chunk(MapChunk*);
MapChunk* create_random_chunk(int, int, int);

#endif /* _MAP_H_ */
