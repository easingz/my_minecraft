#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <GL/glew.h>

typedef enum {
    EMPTY,
    CUBE_1,
    CUBE_2,
    CUBE_3,
    CUBE_4,
    CUBE_5,
    CUBE_6,
    CUBE_7,
    CUBE_8,
    CUBE_9,
    CUBE_10,
    CUBE_11,
    CUBE_12,
    CUBE_13,
    CUBE_14,
    CUBE_15,
    CUBE_16,
    END,
} t_Object;

#define OBJECT_COUNT END

extern const int object_faces[256][6];
extern const GLfloat g_vertex_buffer_data[108];

void init_object_uvs();
void get_object_uvs(const t_Object, GLfloat**, int*);
void delete_object_uvs();

#endif /* _GAME_OBJECT_H_ */
