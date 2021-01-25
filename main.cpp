#include "tgaimage.h"
#include "model.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

//Couleurs
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

//Taille de l'image
const int width = 800;
const int height = 800;

//Modèle à afficher
Model *modele;

//Affichage d'un segment [(x0,y0) , (x1,y1)] de couleur 'color' sur l'image 'image'
void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
	cout << x0 << "-" << y0 << "  " << x1 << "-" << y1 << endl;

	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1)) {
		swap(x0, y0);
		swap(x1, y1);
		steep = true; 
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int derror2 = abs(dy) * 2;
	int error2 = 0;
	int y = y0;

	if (steep) {
		for (int x = x0; x <= x1; ++x) {
			image.set(y, x, color);
			error2 += derror2;
			if (error2 > dx) {
				y += (y1 > y0 ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}
	else {
		for (int x = x0; x <= x1; ++x) {
			image.set(x, y, color);
			error2 += derror2;
			if (error2 > dx) {
				y += (y1 > y0 ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}
}

//Affichage d'une face "face" de couleur 'color' sur l'image 'image'
void drawFace(Face face, TGAImage &image, TGAColor color) {

	//Pour les 3 côtés de la face
	for (int j = 0; j < 3; j++) {
		//On récupère les sommets 2 à 2 (0-1, 1-2, 2-0)
		Vertice v0 = modele->getVerticeAt(face.sommets[j]);
		Vertice v1 = modele->getVerticeAt(face.sommets[(j + 1) % 3]);

		//On récupère les coordonnées des sommets
		int x0 = (v0.coords[0] + 1) * width / 2;
		int y0 = (v0.coords[1] + 1) * height / 2;
		int x1 = (v1.coords[0] + 1) * width / 2;
		int y1 = (v1.coords[1] + 1) * height / 2;

		//On dessine la ligne entre les 2 sommets
		drawLine(x0, y0, x1, y1, image, color);
	}
}

int main(int argc, char** argv) {
	//Création de l'image
	TGAImage image(width, height, TGAImage::RGB);

	//Création du modèle à afficher
	modele = new Model("obj/african_head.obj");
	
	//Pour chaque face du modèle
	for (int i = 0; i < modele->numberOfFaces(); i++) {
		//On dessine la face
		drawFace(modele->getFaceAt(i), image, white);
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

