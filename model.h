#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

struct Vertice3 {
	float x;
	float y;
	float z;
};

struct Face {
	int v1, v2, v3;
};

class Model {
public:
	Model(const char *path);
	int numberOfVertices();
	Vertice3 getVerticeAt(int index);
	int numberOfFaces();
	Face getFaceAt(int index);
private:
	std::vector<Vertice3> vertices;
	std::vector<Face> faces;
};

#endif