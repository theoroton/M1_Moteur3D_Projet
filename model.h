#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

//Structure repr�sentant un sommet
struct Vertice {
	union {
		float coords[3];
		struct {
			float x, y, z;
		};
	};

	int operator[] (int index) { return coords[index]; }
};

//Structure repr�sentant une face
struct Face {
	int sommets[3];
};

class Model {

public:
	//Cr�ation du mod�le � partir du fichier trouv� par "path"
	Model(const char *path);
	//Renvoie le nombre de sommets
	int numberOfVertices();
	//Renvoie le sommet � l'indice "index"
	Vertice getVerticeAt(int index);
	//Renvoie le nombre de faces
	int numberOfFaces();
	//Renvoie la face � l'indice "index"
	Face getFaceAt(int index);

private:
	//Liste des sommets
	std::vector<Vertice> vertices;
	//Liste des faces
	std::vector<Face> faces;
};

#endif