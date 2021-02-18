#include "model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

//Cr�ation du mod�le � partir du fichier trouv� par "path"
Model::Model(const string path) {
	ifstream file;

	// R�cup�ration de la texture du mod�le
	textureImg.read_tga_file((path + "_diffuse.tga").c_str());
	textureImg.flip_vertically();

	// R�cup�ration de la texture normal du mod�le
	normalImg.read_tga_file((path + "_nm.tga").c_str());
	normalImg.flip_vertically();

	// R�cup�ration de la texture spec du mod�le
	specImg.read_tga_file((path + "_spec.tga").c_str());
	specImg.flip_vertically();
	
	//Ouverture du fichier
	file.open((path + ".obj").c_str());
	//Si le fichier ne s'ouvre pas on affiche une erreur
	if (!file.is_open()) { 
		cerr << "Erreur lors de l'ouverture du fichier";
		return; 
	}

	//Initialisation de la ligne
	string line;	
	
	//Tant que l'on est pas � la fin du fichier
	while (!file.eof()) {
		//On r�cup�re la ligne
		getline(file, line);

		//Op�rateur de cha�ne
		istringstream issLine(line.c_str());
		//Caract�re � enlever
		char goaway;
		
		//Si la ligne commence par "v "
		if (!line.compare(0, 2, "v ")) {
			//Cr�ation d'un sommet
			Vec v;

			//On enl�ve le premier caract�re
			issLine >> goaway;

			//On ajoute les coordonn�es du sommet
			for (int i = 0; i < 3; i++) {
				issLine >> v.coords[i];
			}

			//On ajoute le sommet � la liste des sommets
			vertices.push_back(v);

		//Si la ligne commence par "vt "
		} else if (!line.compare(0, 3, "vt ")) {
			//Cr�ation d'une texture
			Vec t;

			//On enl�ve les 2 premiers caract�res
			issLine >> goaway >> goaway;

			//On ajoute les coordonn�es de la texture
			for (int i = 0; i < 3; i++) {
				issLine >> t.coords[i];
			}

			//On ajoute la texture � la liste des textures
			textures.push_back(t);

		} else if (!line.compare(0, 3, "vn ")) {
			//Cr�ation d'un vecteur normal
			Vec n;

			//On enl�ve les 2 premiers caract�res
			issLine >> goaway >> goaway;

			//On ajoute les coordonn�es du vecteur normal
			for (int i = 0; i < 3; i++) {
				issLine >> n.coords[i];
			}

			//On ajoute le vecteur normal � la liste des vecteurs normaux
			normals.push_back(n);

		//Si la ligne commence par "f "
		} else if (!line.compare(0, 2, "f ")) {
			//Cr�ation d'un objet face
			Face f;
			//Entier pour r�cup�rer le sommet et pour les entiers � ne pas prendre
			int sommet, texture, normal;

			//On enl�ve le premier caract�re
			issLine >> goaway;

			//On ajoute les sommets � la face
			for (int i = 0; i < 3; i++) {
				issLine >> sommet >> goaway >> texture >> goaway >> normal;
				f.vertices[i] = sommet - 1;
				f.textures[i] = texture - 1;
				f.normals[i] = normal - 1;
			}

			//On ajoute la face � la liste des faces
			faces.push_back(f);
		}
	}

	//On ferme le fichier
	file.close();
}

//Renvoie le nombre de sommets
int Model::numberOfVertices() {
	return vertices.size();
}

//Renvoie le sommet � l'indice "index"
Vec Model::getVerticeAt(int index) {
	return vertices[index];
}

//Renvoie le nombre de textures
int Model::numberOfTextures() {
	return textures.size();
}

//Renvoie la texture � l'indice "index"
Vec Model::getTextureAt(int index) {
	return textures[index];
}

//Renvoie le nombre de vecteurs normaux
int Model::numberOfNormals() {
	return normals.size();
}

//Renvoie le vecteur normal � l'indice "index"
Vec Model::getNormalAt(int index) {
	return normals[index];
}

//Renvoie le nombre de faces
int Model::numberOfFaces() {
	return faces.size();
}

//Renvoie la face � l'indice "index"
Face Model::getFaceAt(int index) {
	return faces[index];
}

//Renvoie la couleur de l'image texture � la position (u,v)
TGAColor Model::getColorAtTextureImg(float u, float v) {
	return textureImg.get(u * textureImg.get_width(), v * textureImg.get_height());
}

//Renvoie la couleur de l'image normal � la position (u,v)
TGAColor Model::getColorAtNormalImg(float u, float v) {
	return normalImg.get(u * normalImg.get_width(), v * normalImg.get_height());
}

//Renvoie la couleur de l'image spec � la position (u,v)
TGAColor Model::getColorAtSpecImg(float u, float v) {
	return specImg.get(u * specImg.get_width(), v * specImg.get_height());
}