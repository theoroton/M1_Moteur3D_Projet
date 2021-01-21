#ifndef MODEL_H
#define MODEL_H

#include <vector>

struct Vertice3 {
	float x;
	float y;
	float z;
};

class Model {
public:
	Model(const char *path);
	int numberOfVertices();
	Vertice3 getVerticeAt(int index);
private:
	std::vector<Vertice3> vertices;
};

#endif