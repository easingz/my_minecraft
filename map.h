#ifndef _MAP_H_
#define _MAP_H_

// map unit represents a concrete cube postion and style.
typedef union {
    unsigned int value;
    struct {
	unsigned char x;
	unsigned char y;
	unsigned char z;
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
MapChunk* create_random_chunk();

#endif /* _MAP_H_ */
