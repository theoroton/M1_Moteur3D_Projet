#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

//Structure représentant un sommet
struct Vertice {
	union {
		float coords[3];
		struct {
			float x, y, z;
		};
	};

	int operator[] (int index) { return coords[index]; }
};

//Structure représentant une face
struct Face {
	int sommets[3];
};

class Model {

public:
	//Création du modèle à partir du fichier trouvé par "path"
	Model(const char *path);
	//Renvoie le nombre de sommets
	int numberOfVertices();
	//Renvoie le sommet à l'indice "index"
	Vertice getVerticeAt(int index);
	//Renvoie le nombre de faces
	int numberOfFaces();
	//Renvoie la face à l'indice "index"
	Face getFaceAt(int index);

private:
	//Liste des sommets
	std::vector<Vertice> vertices;
	//Liste des faces
	std::vector<Face> faces;
};

#endif