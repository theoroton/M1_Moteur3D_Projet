#include "model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

Model::Model(const char *path) {
	ifstream file;
	vertices.clear();
	faces.clear();
	
	file.open(path);
	if (!file.is_open()) { 
		cerr << "Erreur lors de l'ouverture du fichier";
		return; 
	}

	string line;	
	
	while (!file.eof()) {
		getline(file, line);

		istringstream issLine(line.c_str());
		char goaway;
		
		if (!line.compare(0, 2, "v ")) {
			float x, y, z;

			issLine >> goaway;
			issLine >> x >> y >> z;

			vertices.push_back({x, y, z});

		} else if (!line.compare(0, 2, "f ")) {
			string sv1, sv2, sv3;
			int v1, v2, v3;
			issLine >> goaway;
			issLine >> sv1 >> sv2 >> sv3;
			v1 = stoi(sv1) - 1;
			v2 = stoi(sv2) - 1;
			v3 = stoi(sv3) - 1;

			faces.push_back({v1,v2,v3});
		}
	}

	file.close();
}

int Model::numberOfVertices() {
	return vertices.size();
}

Vertice3 Model::getVerticeAt(int index) {
	return vertices[index];
}

int Model::numberOfFaces() {
	return faces.size();
}

Face Model::getFaceAt(int index) {
	return faces[index];
}