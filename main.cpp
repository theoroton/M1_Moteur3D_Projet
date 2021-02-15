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


//Mod�le � afficher
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
	// Cr�ation de l'image
	TGAImage image(width, height, TGAImage::RGB);

	// Cr�ation du mod�le � afficher
	modele = new Model("obj/african_head/african_head.obj");

	// Cr�ation et r�cup�ration de la texture du mod�le
	TGAImage texture;
	texture.read_tga_file("obj/african_head/african_head_diffuse.tga");
	texture.flip_vertically();


	// Normalisation du vecteur lumi�re
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

	// Pour chaque Face du mod�le
	for (int i = 0; i < modele->numberOfFaces(); i++) {
		// On r�cup�re la face
		f = modele->getFaceAt(i);

		// R�cup�ration des vecteurs repr�sentant les sommets
		Vec* vertices = new Vec[3];
		for (int j = 0; j < 3; j++) {
			vertices[j] = modele->getVerticeAt(f.vertices[j]);
		}

		// R�cup�ration des vecteurs repr�sentant les textures, les projection des sommets et les intensit�s aux sommets
		Vec *textures = new Vec[3];
		float *intensities = new float[3];

		// Pour chaque sommet de la face
		for (int j = 0; j < 3; j++) {
			// R�cup�ration du sommet et projection dans le plan
			vertices[j] = matrixToVector(Viewport * Projection * Modelview * vectorToMatrix(vertices[j])); // (formule 2)
			// R�cup�ration de la texture au sommet
			textures[j] = modele->getTextureAt(f.textures[j]);
			// R�cup�ration de l'intensit� au sommet
			intensities[j] = (modele->getNormalAt(f.normals[j])) * light;
		}

		// On dessine le triangle correspondant � la Face
		drawTriangle(vertices, textures, intensities, z_buffer, image, texture);

	}
	
	// Cr�ation de l'image TGA
	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}

