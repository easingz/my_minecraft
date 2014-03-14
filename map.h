#ifndef _MAP_H_
#define _MAP_H_

// map unit represents a concrete cube postion and style.
// follow OpenGl coordinate system.
typedef union {
  unsigned int value;
  struct {
    unsigned char x; // [0, 255]
    unsigned char y; // [0, 255]
    unsigned char z; // [0, 255]
    unsigned char w;
  } u;
} MapUnit;

// chunk is 256x256x256 big cube containing MapUnits.
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
