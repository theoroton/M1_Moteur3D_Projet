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

void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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

int main(int argc, char** argv) {
	TGAImage image(800, 800, TGAImage::RGB);

	modele = new Model("obj/african_head.obj");

	Vertice3 v;
	
	for (int i = 0; i < modele->numberOfVertices(); i++) {
		v = modele->getVerticeAt(i);
		int x = (v.x) * width / 2. + width/2;
		int y = (v.y) * height / 2. + height/2;
		image.set(x, y, white);
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

