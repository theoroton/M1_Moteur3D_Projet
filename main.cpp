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


// Texture
TGAImage textureImg;
TGAImage normalImg;


struct TextureShader : public IShader {
	// Récupération des vecteurs représentant les textures, les projection des sommets et les intensités aux sommets
	Vec* textures = new Vec[3];
	float* intensities = new float[3];

	virtual Vec vertex(Face f, int nbV) {
		// Récupération de la texture au sommet
		textures[nbV] = modele->getTextureAt(f.textures[nbV]);
		// Récupération de l'intensité au sommet
		intensities[nbV] = (modele->getNormalAt(f.normals[nbV])) * light;

		// Sommet et projection dans le plan
		return matrixToVector(Viewport * Projection * Modelview * vectorToMatrix(modele->getVerticeAt(f.vertices[nbV]))); // (formule 2)
	}

	virtual TGAColor getTexture(Vec bary) {
		// Calcul de l'intensité
		float intensity = bary[0] * intensities[0] + bary[1] * intensities[1] + bary[2] * intensities[2];

		// Récupération de la texture
		float u = bary[0] * textures[0].x + bary[1] * textures[1].x + bary[2] * textures[2].x;
		float v = bary[0] * textures[0].y + bary[1] * textures[1].y + bary[2] * textures[2].y;
		TGAColor color = textureImg.get(u * textureImg.get_width(), v * textureImg.get_height());

		// Calcul de la couleur
		return color * intensity;
	}
};

int main(int argc, char** argv) {
	// Création de l'image
	TGAImage image(width, height, TGAImage::RGB);

	// Création du modèle à afficher
	modele = new Model("obj/african_head/african_head.obj");

	// Récupération de la texture du modèle
	textureImg.read_tga_file("obj/african_head/african_head_diffuse.tga");
	textureImg.flip_vertically();

	// Récupération de la texture normal du modèle
	normalImg.read_tga_file("obj/african_head/african_head_nm.tga");
	normalImg.flip_vertically();


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
	// Shader
	TextureShader shader;

	// Pour chaque Face du modèle
	for (int i = 0; i < modele->numberOfFaces(); i++) {
		// On récupère la face
		f = modele->getFaceAt(i);

		// Récupération des vecteurs représentant les sommets
		Vec* vertices = new Vec[3];

		// Pour chaque sommet de la face
		for (int j = 0; j < 3; j++) {
			// Récupération du sommet et projection dans le plan
			vertices[j] = shader.vertex(f, j); 
		}

		// On dessine le triangle correspondant à la Face
		drawTriangle(vertices, shader, z_buffer, image);

	}
	
	// Création de l'image TGA
	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}

