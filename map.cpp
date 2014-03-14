#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"
#include "game_object.h"

void mapchunk_set_unit(MapChunk*, int, int, int, int);
MapChunk* create_and_init_chunk(int, int, int);
void chunk_grow(MapChunk*);

// helper test function
// param: origin of chunk, one chunk as 1 unit
MapChunk* create_random_chunk(int x, int y, int z) {
  MapChunk* mc = create_and_init_chunk(x, y, z);
  // the grass ground
  for (int i = 0; i < 64; i++) {
    for (int j = 192; j < 256; j++) {
      mapchunk_set_unit(mc, i, 0, j, CUBE_1);
    }
  }

  // generate 1024 random units
  for (int i = 0; i < 500; i++) {
    mapchunk_set_unit(mc,
		      rand() % 256, rand() % 255 + 1, rand() % 256,
		      rand() % OBJECT_COUNT);
  }
  return mc;
}

// (0, 0, 0) is at the left-back-bottom corner of chunck, same coord-system as OpenGL.
void mapchunk_set_unit(MapChunk* mc, int x, int y, int z, int w) {
  assert(0 <= x && x < 256);
  assert(0 <= y && y < 256);
  assert(0 <= z && z < 256);
  assert(0 <= w && w < 256);
  // TODO(later): check if already exists at (x, y, z)
  // unsigned int u = (w << 24) | (z << 16) | (y << 8) | x;

  // see if chunk has enough space to set
  if (mc->pos >= mc->size) {
    chunk_grow(mc);
  }
  // mc->data[mc->pos++].value = u;
  MapUnit* mu = &(mc->data[mc->pos++]);
  mu->u.x = (unsigned char)x;
  mu->u.y = (unsigned char)y;
  mu->u.z = (unsigned char)z;
  mu->u.w = (unsigned char)w;
}

void delete_chunk(MapChunk* mc) {
  free(mc->data);
  free(mc);
}

#define INIT_CHUNK_DATA_SIZE 65536
MapChunk* create_and_init_chunk(int x, int y, int z) {
  MapChunk* mc = (MapChunk*) malloc(sizeof(MapChunk));
  mc->data = (MapUnit*) malloc(INIT_CHUNK_DATA_SIZE * sizeof(MapUnit));
  mc->size = INIT_CHUNK_DATA_SIZE;
  mc->pos = 0;
  mc->dx = x;
  mc->dy = y;
  mc->dz = z;
  return mc;
}

void chunk_grow(MapChunk* mc) {
  mc->size += 1000;
  mc->data = (MapUnit*) realloc(mc->data, mc->size * sizeof(MapUnit));
}
