#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>
#include "structures.h"
#include "tgaimage.h"

class Model {

public:
	//Création du modèle à partir du fichier trouvé par "path"
	Model(const std::string path);
	//Renvoie le nombre de sommets
	int numberOfVertices();
	//Renvoie le sommet à l'indice "index"
	Vec getVerticeAt(int index);

	//Renvoie le nombre de textures
	int numberOfTextures();
	//Renvoie la texture à l'indice "index"
	Vec getTextureAt(int index);

	//Renvoie le nombre de vecteurs normaux
	int numberOfNormals();
	//Renvoie le vecteur normal à l'indice "index"
	Vec getNormalAt(int index);

	//Renvoie le nombre de faces
	int numberOfFaces();
	//Renvoie la face à l'indice "index"
	Face getFaceAt(int index);

	//Renvoie la couleur de l'image texture à la position (u,v)
	TGAColor getColorAtTextureImg(float u, float v);
	//Renvoie la couleur de l'image normal à la position (u,v)
	TGAColor getColorAtNormalImg(float u, float v);
	//Renvoie la couleur de l'image spec à la position (u,v)
	TGAColor getColorAtSpecImg(float u, float v);

private:
	//Liste des sommets
	std::vector<Vec> vertices;
	//Liste des textures
	std::vector<Vec> textures;
	//Liste des vecteurs normaux
	std::vector<Vec> normals;
	//Liste des faces
	std::vector<Face> faces;

	//Textures
	TGAImage textureImg;
	TGAImage normalImg;
	TGAImage specImg;
};

#endif