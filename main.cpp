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
Vec light = { 1,1,3 };
Vec camera = { 1,1,3 };
Vec center = { 0,0,0 };
Vec up = { 0,1,0 };


//Matrices
Matrix Viewport = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
Matrix Projection = projection((-1 / (center - camera).norm()));
Matrix Modelview = modelview(camera, center, up);


// Textures
TGAImage textureImg;
TGAImage normalImg;
TGAImage specImg;


struct TextureShader : public IShader {
	// R�cup�ration des vecteurs repr�sentant les textures, les projection des sommets et les intensit�s aux sommets
	Vec* textures = new Vec[3];

	Matrix M = Projection * Modelview;
	Matrix MIT = (M.transpose()).inverse();

	virtual Vec vertex(Face f, int nbV) {
		// R�cup�ration de la texture au sommet
		textures[nbV] = modele->getTextureAt(f.textures[nbV]);

		// Sommet et projection dans le plan
		return matrixToVector(Viewport * Projection * Modelview * vectorToMatrix(modele->getVerticeAt(f.vertices[nbV]))); // (formule 2)
	}

	virtual TGAColor getTexture(Vec bary) {
		// R�cup�ration de la texture normale
		float u = bary[0] * textures[0].x + bary[1] * textures[1].x + bary[2] * textures[2].x;
		float v = bary[0] * textures[0].y + bary[1] * textures[1].y + bary[2] * textures[2].y;
		TGAColor colorNormal = normalImg.get(u * normalImg.get_width(), v * normalImg.get_height());

		// Cr�ation vecteur normal
		Vec normal;
		for (int i = 0; i < 3; i++) {
			normal.coords[i] = (float)colorNormal[2-i] / 255.f * 2.f - 1.f;
		}

		Vec n = matrixToVector(MIT * vectorToMatrix(normal)).normalize();
		Vec l = matrixToVector(M * vectorToMatrix(light)).normalize();
		Vec r = (l - (n * 2 * (n * l))).normalize();

		
		// Calcul de l'intensit�
		float diff = max(0.f, n * l);
		float spec = pow(max(r.z, 0.0f), specImg.get(u * specImg.get_width(), v * specImg.get_height())[0]/1.f);

		// Couleur
		TGAColor color = textureImg.get(u * textureImg.get_width(), v * textureImg.get_height());

		for (int i = 0; i < 3; i++) color[i] = min<float>(5 + color[i] * (diff + .6 * spec), 255);

		// Calcul de la couleur
		return color;
	}
};

int main(int argc, char** argv) {
	// Cr�ation de l'image
	TGAImage image(width, height, TGAImage::RGB);

	// Cr�ation du mod�le � afficher
	modele = new Model("obj/african_head/african_head.obj");

	// R�cup�ration de la texture du mod�le
	textureImg.read_tga_file("obj/african_head/african_head_diffuse.tga");
	textureImg.flip_vertically();

	// R�cup�ration de la texture normal du mod�le
	normalImg.read_tga_file("obj/african_head/african_head_nm.tga");
	normalImg.flip_vertically();

	// R�cup�ration de la texture spec du mod�le
	specImg.read_tga_file("obj/african_head/african_head_spec.tga");
	specImg.flip_vertically();


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
	// Shader
	TextureShader shader;

	// Pour chaque Face du mod�le
	for (int i = 0; i < modele->numberOfFaces(); i++) {
		// On r�cup�re la face
		f = modele->getFaceAt(i);

		// R�cup�ration des vecteurs repr�sentant les sommets
		Vec* vertices = new Vec[3];

		// Pour chaque sommet de la face
		for (int j = 0; j < 3; j++) {
			// R�cup�ration du sommet et projection dans le plan
			vertices[j] = shader.vertex(f, j); 
		}

		// On dessine le triangle correspondant � la Face
		drawTriangle(vertices, shader, z_buffer, image);

	}
	
	// Cr�ation de l'image TGA
	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}

