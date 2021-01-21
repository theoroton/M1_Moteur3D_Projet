#include "model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdio>

using namespace std;

Model::Model(const char *path) {
	ifstream file;
	
	file.open(path);
	if (!file.is_open()) { 
		cerr << "Erreur lors de l'ouverture du fichier";
		return; 
	}

	string line;
	float x, y, z;
	
	while (!file.eof()) {
		getline(file, line);

		istringstream issLine(line.c_str());
		char goaway;
		
		if (!line.compare(0, 2, "v ")) {
			issLine >> goaway;
			issLine >> x >> y >> z;

			Vertice3 vertice = { x,y,z };
			vertices.push_back(vertice);
		}
	}

}

int Model::numberOfVertices() {
	return vertices.size();
}

Vertice3 Model::getVerticeAt(int index) {
	return vertices[index];
}