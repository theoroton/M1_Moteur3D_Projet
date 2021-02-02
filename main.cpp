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

void drawTriangle(Vec A, Vec B, Vec C, Vec *textures, float intensity, float **z_buffer, TGAImage &image, TGAImage &texture) {
	// Limites du triangle
	int xMax = max(A.x, max(B.x, C.x));
	int xMin = min(A.x, min(B.x, C.x));
	int yMax = max(A.y, max(B.y, C.y));
	int yMin = min(A.y, min(B.y, C.y));

	// Pour chaque x dans les limites du triangle
	for (int x = xMin; x <= xMax; x++) {

		// Pour chaque y dans les limites du triangle
		for (int y = yMin; y <= yMax; y++) {

			// Point P à tester
			Vec P = { (float) x, (float) y, 0. };

			// Vecteur x
			Vec Vx = { C.x - A.x, B.x - A.x, A.x - P.x };
			// Vecteur y
			Vec Vy = { C.y - A.y, B.y - A.y, A.y - P.y };

			// Produit scalaire du vecteur des x et du vecteur des y
			Vec u = Vx ^ Vy;

			// Coordonnées barycentriques
			float bary1 = 1 - (u.x + u.y) / u.z;
			float bary2 = u.y / u.z;
			float bary3 = u.x / u.z;

			// Si toutes les coordonnées sont positives
			if (bary1 >= 0 && bary2 >= 0 && bary3 >= 0) {
				// On calcule le z du point P
				P.z += A.z * bary1;
				P.z += B.z * bary2;
				P.z += C.z * bary3;

				// Si le z est supérieur au z enregistré
				if (P.z > z_buffer[x][y]) {
					// On enregistre le z
					z_buffer[x][y] = P.z;

					// Récupération de la texture
					float u = bary1 * textures[0].x + bary2 * textures[1].x + bary3 * textures[2].x;
					float v = bary1 * textures[0].y + bary2 * textures[1].y + bary3 * textures[2].y;
					TGAColor color = texture.get(u * texture.get_width(), v * texture.get_height());

					// Multiplication des composantes
					color.b = color.b * intensity;
					color.g = color.g * intensity;
					color.r = color.r * intensity;
					color.a = color.a * intensity;

					// On affiche le pixel
					image.set(x, y, color);
				}
			}
		}
	}
}



int main(int argc, char** argv) {
	// Création de l'image
	TGAImage image(width, height, TGAImage::RGB);

	// Création du modèle à afficher
	modele = new Model("obj/african_head/african_head.obj");

	// Création et récupération de la texture du modèle
	TGAImage texture;
	texture.read_tga_file("obj/african_head/african_head_diffuse.tga");
	texture.flip_vertically();

	Face f;
	// Couleur
	TGAColor color;
	// Vecteur lumière
	Vec light = { 0,0,-1 };

	// Initialisation z_buffer
	float** z_buffer = new float*[width];
	for (int i = 0; i < width; i++) {
		z_buffer[i] = new float[height];
	}

	// Initialisation de la valeur min
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// Inverse du max des float (!!! min donne le float minimum > 0)
			z_buffer[i][j] = -numeric_limits<float>::max();
		}
	}

	// Pour chaque Face
	for (int i = 0; i < modele->numberOfFaces(); i++) {
		// On récupère la face
		f = modele->getFaceAt(i);

		// On récupère les 3 sommets composants la Face
		Vec A = modele->getVerticeAt(f.a);
		Vec B = modele->getVerticeAt(f.b);
		Vec C = modele->getVerticeAt(f.c);

		// On calcule le vecteur normal du triangle
		Vec normal = ((A - B) ^ (A - C));

		// On normalise ce vecteur
		normal.normalize();

		// On calcule l'intensité de la lumière
		float intensity = -(normal * light);

		// Si l'intensité est supérieur à 0
		if (intensity > 0) {
			// On ajuste les coordonnées à la taille de l'image
			A.toImageSize(width, height);
			B.toImageSize(width, height);
			C.toImageSize(width, height);

			Vec *textures = new Vec[3];
			for (int j = 0; j < 3; j++) {
				textures[j] = modele->getTextureAt(f.textures[j]);
			}

			// On crée la couleur
			color = TGAColor(intensity * 255, intensity * 255, intensity * 255, 255);
			// On dessine le triangle correspondant à la Face
			drawTriangle(A, B, C, textures, intensity, z_buffer, image, texture);
		}
	}
	
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

