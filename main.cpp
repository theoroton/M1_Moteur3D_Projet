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

//Mod�le � afficher
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
	
	Face f;
	
	for (int i = 0; i < modele->numberOfFaces(); i++) {
		f = modele->getFaceAt(i);
		Vertice3 v1 = modele->getVerticeAt(f.v1);
		Vertice3 v2 = modele->getVerticeAt(f.v2);
		Vertice3 v3 = modele->getVerticeAt(f.v3);
		drawLine((v1.x + 1) * width / 2, (v1.y + 1) * height / 2, (v2.x + 1) * width / 2, (v2.y + 1) * height / 2, image, white);
		drawLine((v2.x + 1) * width / 2, (v2.y + 1) * height / 2, (v3.x + 1) * width / 2, (v3.y + 1) * height / 2, image, white);
		drawLine((v3.x + 1) * width / 2, (v3.y + 1) * height / 2, (v1.x + 1) * width / 2, (v1.y + 1) * height / 2, image, white);
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

