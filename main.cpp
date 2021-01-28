#include "tgaimage.h"
#include "model.h"
#include "structures.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <ctgmath>

using namespace std;

//Couleurs
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 128, 0, 255);

//Taille de l'image
const int width = 800;
const int height = 800;

//Modèle à afficher
Model *modele;

//Affichage d'un segment [(x0,y0) , (x1,y1)] de couleur 'color' sur l'image 'image'
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

void drawTriangle(Vec A, Vec B, Vec C, TGAImage &image, TGAColor color) {
	int xMax = max(A.x, max(B.x, C.x));
	int xMin = min(A.x, min(B.x, C.x));
	int yMax = max(A.y, max(B.y, C.y));
	int yMin = min(A.y, min(B.y, C.y));

	Vec AB = A - B;
	Vec AC = A - C;

	for (int x = xMin; x <= xMax; x++) {

		for (int y = yMin; y <= yMax; y++) {

			Vec P = { (float) x, (float) y, 0. };
			Vec AP = A - P;

			float w1 = (A.x * AC.y + AP.y * AC.x - P.x * AC.y) / (AB.y * AC.x - AB.x * AC.y);
			float w2 = (AP.y - w1 * AB.y) / AC.y;

			if (w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1) {
				image.set(x, y, color);
			}
		}
	}
}



int main(int argc, char** argv) {
	//Création de l'image
	TGAImage image(width, height, TGAImage::RGB);

	//Création du modèle à afficher
	modele = new Model("obj/african_head.obj");

	Face f;
	TGAColor color;
	Vec light = { 0,0,-1 };

	for (int i = 0; i < modele->numberOfFaces(); i++) {
		f = modele->getFaceAt(i);

		Vec A = modele->getVerticeAt(f.sommets[0]);
		Vec B = modele->getVerticeAt(f.sommets[1]);
		Vec C = modele->getVerticeAt(f.sommets[2]);

		Vec normal = ((A - B) ^ (A - C));

		normal.normalize();

		float intensity = -(normal * light);

		if (intensity > 0) {
			A.toImageSize(width / 2, height / 2);
			B.toImageSize(width / 2, height / 2);
			C.toImageSize(width / 2, height / 2);

			color = TGAColor(intensity * 255, intensity * 255, intensity * 255, 255);
			drawTriangle(A, B, C, image, color);
		}
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

