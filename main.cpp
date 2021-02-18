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
#include <string>

using namespace std;

//Couleurs
const TGAColor white = TGAColor(255, 255, 255, 255);


//Taille de l'image
const int width = 1024;
const int height = 1024;
const int depth = 255;


//Modèle à afficher
Model *model;


//Vecteurs
Vec light = { 1,1,3 };
Vec camera = { 1,1,3 };
Vec center = { 0,0,0 };
Vec up = { 0,1,0 };


//Matrices
Matrix Viewport = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
Matrix Projection = projection((-1 / (center - camera).norm()));
Matrix Modelview = modelview(camera, center, up);


struct TextureShader : public IShader {
	// Récupération des vecteurs représentant les textures, les projection des sommets et les intensités aux sommets
	Vec* textures = new Vec[3];

	Matrix M = Projection * Modelview;
	Matrix MIT = (M.transpose()).inverse();

	virtual Vec vertex(Face f, int nbV) {
		// Récupération de la texture au sommet
		textures[nbV] = model->getTextureAt(f.textures[nbV]);

		// Sommet et projection dans le plan
		return matrixToVector(Viewport * Projection * Modelview * vectorToMatrix(model->getVerticeAt(f.vertices[nbV]))); // (formule 2)
	}

	virtual TGAColor getTexture(Vec bary) {
		// Récupération de la texture normale
		float u = bary[0] * textures[0].x + bary[1] * textures[1].x + bary[2] * textures[2].x;
		float v = bary[0] * textures[0].y + bary[1] * textures[1].y + bary[2] * textures[2].y;
		TGAColor colorNormal = model->getColorAtNormalImg(u, v);

		// Création vecteur normal
		Vec normal;
		for (int i = 0; i < 3; i++) {
			normal.coords[i] = (float)colorNormal[2-i] / 255.f * 2.f - 1.f;
		}

		Vec n = matrixToVector(MIT * vectorToMatrix(normal)).normalize();
		Vec l = matrixToVector(M * vectorToMatrix(light)).normalize();
		Vec r = (l - (n * 2 * (n * l))).normalize();

		
		// Calcul de l'intensité
		float diff = max(0.f, n * l);
		float spec = pow(max(r.z, 0.0f), model->getColorAtSpecImg(u, v)[0]);

		// Couleur
		TGAColor color = model->getColorAtTextureImg(u, v);

		//Coefficients
		float coeffAmbient = 5;
		float coeffDiffuse = 1;
		float coeffSpecular = 0.6;

		for (int i = 0; i < 3; i++) {
			color[i] = min<float>(coeffAmbient + color[i] * (coeffDiffuse * diff + coeffSpecular * spec), 255);
		}

		// Calcul de la couleur
		return color;
	}
};


void renderModel(string stringModel, IShader& shader, float** z_buffer, TGAImage& image) {

	// Création du modèle à afficher
	model = new Model(stringModel);

	// Face
	Face f;

	// Pour chaque Face du modèle
	for (int i = 0; i < model->numberOfFaces(); i++) {
		// On récupère la face
		f = model->getFaceAt(i);

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

}


int main(int argc, char** argv) {
	// Création de l'image
	TGAImage image(width, height, TGAImage::RGB);

	// Shader
	TextureShader shader;

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

	renderModel("obj/african_head/african_head", shader, z_buffer, image);
	renderModel("obj/african_head/african_head_eye_inner", shader, z_buffer, image);
	
	// Création de l'image TGA
	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}

