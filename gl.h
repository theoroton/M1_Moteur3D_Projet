#ifndef __GL_H__
#define __GL_H__

#include "tgaimage.h"
#include "structures.h"

Matrix viewport(int x, int y, int w, int h, int d);
Matrix projection(float cam);
Matrix modelview(Vec camera, Vec center, Vec up);

Matrix vectorToMatrix(Vec vec);
Vec matrixToVector(Matrix mat);

void drawTriangle(Vec* vertices, Vec* textures, float* intensities, float** z_buffer, TGAImage& image, TGAImage& texture);

#endif