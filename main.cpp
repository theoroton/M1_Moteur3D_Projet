#include "tgaimage.h"
#include "model.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

//Couleurs
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 128, 0, 255);

//Taille de l'image
const int width = 200;
const int height = 200;

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

void drawTopTriangle(Vertice haut, Vertice milieu, Vertice a, TGAImage &image, TGAColor color) {
	float pas1 = (haut.x - milieu.x) / (haut.y - milieu.y);
	float pas2 = (haut.x - a.x) / (haut.y - a.y);

	float x1 = haut.x;
	float x2 = haut.x;

	for (int y = haut.y; y >= a.y; y--) {

		drawLine(x1, y, x2, y, image, color);
		x1 -= pas1;
		x2 -= pas2;
	}
}

void drawBottomTriangle(Vertice bas, Vertice milieu, Vertice a, TGAImage& image, TGAColor color) {
	float pas1 = (bas.x - milieu.x) / (bas.y - milieu.y);
	float pas2 = (bas.x - a.x) / (bas.y - a.y);

	float x1 = bas.x;
	float x2 = bas.x;;

	for (int y = bas.y; y < a.y; y++) {

		drawLine(x1, y, x2, y, image, color);
		x1 += pas1;
		x2 += pas2;
	}
}

void drawTriangle(Vertice v0, Vertice v1, Vertice v2, TGAImage& image) {
	if (v0.y > v1.y) { swap(v0, v1); }
	if (v1.y > v2.y) { swap(v1, v2); }
	if (v0.y > v1.y) { swap(v0, v1); }

	Vertice a = { (v2.x + ((float)(v1.y - v2.y) / (float)(v0.y - v2.y)) * (v0.x - v2.x)), v1.y, 0 };

	TGAColor randColor = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
	drawTopTriangle(v2, v1, a, image, randColor);
	drawBottomTriangle(v0, v1, a, image, randColor);
}



int main(int argc, char** argv) {
	//Création de l'image
	TGAImage image(width, height, TGAImage::RGB);

	//Création du modèle à afficher
	modele = new Model("obj/african_head.obj");
	
	drawTriangle({ 10,70,0 }, { 50,160,0 }, { 70,80,0 }, image);
	drawTriangle({ 180,50,0 }, { 150,1,0 }, { 70,180,0 }, image);
	drawTriangle({ 180,150,0 }, { 120,160,0 }, { 130,180,0 }, image);

	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

