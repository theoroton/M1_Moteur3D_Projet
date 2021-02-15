#include "tgaimage.h"
#include "model.h"
#include "structures.h"
#include "gl.h"

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


//Taille de l'image
const int width = 800;
const int height = 800;
const int depth = 255;


//Modèle à afficher
Model *modele;


//Vecteurs
Vec light = { 3,3,3 };
Vec camera = { 1,1,3 };
Vec center = { 0,0,0 };
Vec up = { 0,1,0 };


//Matrices
Matrix Viewport = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
Matrix Projection = projection((-1 / (center - camera).norm()));
Matrix Modelview = modelview(camera, center, up);


int main(int argc, char** argv) {
	// Création de l'image
	TGAImage image(width, height, TGAImage::RGB);

	// Création du modèle à afficher
	modele = new Model("obj/african_head/african_head.obj");

	// Création et récupération de la texture du modèle
	TGAImage texture;
	texture.read_tga_file("obj/african_head/african_head_diffuse.tga");
	texture.flip_vertically();


	// Normalisation du vecteur lumière
	light.normalize();
	
	// Initialisation z_buffer
	float** z_buffer = new float*[width];
	for (int i = 0; i < width; i++) {
		z_buffer[i] = new float[height];
	}

	// Initialisation de la valeur min du z_buffer
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// Inverse du max des float (!!! min donne le float minimum > 0)
			z_buffer[i][j] = -numeric_limits<float>::max();
		}
	}

	// Face
	Face f;

	// Pour chaque Face du modèle
	for (int i = 0; i < modele->numberOfFaces(); i++) {
		// On récupère la face
		f = modele->getFaceAt(i);

		// Récupération des vecteurs représentant les sommets
		Vec* vertices = new Vec[3];
		for (int j = 0; j < 3; j++) {
			vertices[j] = modele->getVerticeAt(f.vertices[j]);
		}

		// Récupération des vecteurs représentant les textures, les projection des sommets et les intensités aux sommets
		Vec *textures = new Vec[3];
		float *intensities = new float[3];

		// Pour chaque sommet de la face
		for (int j = 0; j < 3; j++) {
			// Récupération du sommet et projection dans le plan
			vertices[j] = matrixToVector(Viewport * Projection * Modelview * vectorToMatrix(vertices[j])); // (formule 2)
			// Récupération de la texture au sommet
			textures[j] = modele->getTextureAt(f.textures[j]);
			// Récupération de l'intensité au sommet
			intensities[j] = (modele->getNormalAt(f.normals[j])) * light;
		}

		// On dessine le triangle correspondant à la Face
		drawTriangle(vertices, textures, intensities, z_buffer, image, texture);

	}
	
	// Création de l'image TGA
	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}

