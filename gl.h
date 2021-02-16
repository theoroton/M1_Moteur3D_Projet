#ifndef __GL_H__
#define __GL_H__

#include "tgaimage.h"
#include "structures.h"

Matrix viewport(int x, int y, int w, int h, int d);
Matrix projection(float cam);
Matrix modelview(Vec camera, Vec center, Vec up);

Matrix vectorToMatrix(Vec vec);
Vec matrixToVector(Matrix mat);

struct IShader {
	virtual ~IShader();
	virtual Vec vertex(Face f, int nbV) = 0;
	virtual TGAColor getTexture(Vec bary) = 0;
};

void drawTriangle(Vec* vertices, IShader& shader, float** z_buffer, TGAImage& image);

#endif