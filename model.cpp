#include "model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

//Création du modèle à partir du fichier trouvé par "path"
Model::Model(const char *path) {
	ifstream file;
	vertices.clear();
	faces.clear();
	
	//Ouverture du fichier
	file.open(path);
	//Si le fichier ne s'ouvre pas on affiche une erreur
	if (!file.is_open()) { 
		cerr << "Erreur lors de l'ouverture du fichier";
		return; 
	}

	//Initialisation de la ligne
	string line;	
	
	//Tant que l'on est pas à la fin du fichier
	while (!file.eof()) {
		//On récupère la ligne
		getline(file, line);

		//Opérateur de chaîne
		istringstream issLine(line.c_str());
		//Caractère à enlever
		char goaway;
		
		//Si la ligne commence par "v "
		if (!line.compare(0, 2, "v ")) {
			//Création d'un objet sommet
			Vertice v;

			//On enlève le premier caractère
			issLine >> goaway;

			//On ajoute les coordonnées du sommet
			for (int i = 0; i < 3; i++) {
				issLine >> v.coords[i];
			}

			//On ajoute le sommet à la liste des sommets
			vertices.push_back(v);

		//Si la ligne commence par "f "
		} else if (!line.compare(0, 2, "f ")) {
			//Création d'un objet face
			Face f;
			//Entier pour récupérer le sommet et pour les entiers à ne pas prendre
			int sommet, goawayint;

			//On enlève le premier caractère
			issLine >> goaway;

			//On ajoute les sommets à la face
			for (int i = 0; i < 3; i++) {
				issLine >> sommet >> goaway >> goawayint >> goaway >> goawayint;
				f.sommets[i] = sommet - 1;
			}

			//On ajoute la face à la liste des faces
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

//Renvoie le sommet à l'indice "index"
Vertice Model::getVerticeAt(int index) {
	return vertices[index];
}

//Renvoie le nombre de faces
int Model::numberOfFaces() {
	return faces.size();
}

//Renvoie la face à l'indice "index"
Face Model::getFaceAt(int index) {
	return faces[index];
}