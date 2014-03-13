// -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include <stdio.h>
#include "game_object.h"

// Every object has 6 faces: front, back, left, right, top, bottom
// the 256x256 texture is divided into 16x16 tiles.
// faces are represented as index of tile, which counted from bottom->up and left->right.
const int object_faces[256][6] = {
  {0, 0, 0, 0, 0, 0},
  {16, 16, 16, 16, 32, 0},
  {1, 1, 1, 1, 1, 1},
  {2, 2, 2, 2, 2, 2},
  {3, 3, 3, 3, 3, 3},
  {20, 20, 20, 20, 36, 4},
  {5, 5, 5, 5, 5, 5},
  {6, 6, 6, 6, 6, 6},
  {7, 7, 7, 7, 7, 7},
  {24, 24, 24, 24, 40, 8},
  {9, 9, 9, 9, 9, 9},
  {10, 10, 10, 10, 10, 10},
  {11, 11, 11, 11, 11, 11},
  {12, 12, 12, 12, 12, 12},
  {13, 13, 13, 13, 13, 13},
  {14, 14, 14, 14, 14, 14},
  {15, 15, 15, 15, 15, 15},

};

const GLfloat g_vertex_buffer_data[108] = {
  /* front */
  1.0f, -1.0f, 1.0f,
  -1.0f, -1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,

  1.0f, -1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,

  /* back */
  -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, 1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  1.0f, 1.0f, -1.0f,
  -1.0f, 1.0f, -1.0f,

  /* left */
  -1.0f, -1.0f, 1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f, 1.0f, -1.0f,

  -1.0f, -1.0f, 1.0f,
  -1.0f, 1.0f, -1.0f,
  -1.0f, 1.0f, 1.0f,

  /* right */
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, -1.0f,

  /* top */
  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, -1.0f,

  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, -1.0f,
  1.0f, 1.0f, -1.0f,

  /* bottom */
  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, 1.0f,

  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, 1.0f,
  1.0f, -1.0f, 1.0f,
};

void get_object_texture_uv(t_object object, GLfloat* uv_buf) {
  static const GLfloat TILE_W = 1.0f/16.0f;
  static const GLfloat face_uv_base[12] = {
    TILE_W, 0,
    0, 0,
    0, TILE_W,
    TILE_W, 0,
    0, TILE_W,
    TILE_W, TILE_W
  };
  const int* face_tiles = &object_faces[object][0];
  // printf("### texture %d ###\n", (int) object);
  for (int i = 0; i < 6; i++) {
    for (unsigned int j = 0; j < sizeof(face_uv_base)/sizeof(face_uv_base[0]);) {
      // uv.x
      // i * 12 means its the i-th face
      // (face_tiles[i] % 16) means which column it is in texture
      *(uv_buf + i * 12 + j) = (face_tiles[i] % 16) * TILE_W + face_uv_base[j];
      j++;
      // uv.y
      // (face_tiles[i] / 16) means which row it is in texture
      *(uv_buf + i * 12 + j) = (face_tiles[i] / 16) * TILE_W + face_uv_base[j];
      j++;
      // printf("   %f %f\n", *(uv_buf + i * 12 + j - 2), *(uv_buf + i * 12 + j - 1));
    }
  }
}
