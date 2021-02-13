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
const int depth = 255;

//Modèle à afficher
Model *modele;

void drawTriangle(Vec *vertices, Vec *textures, float* intensities, float **z_buffer, TGAImage &image, TGAImage &texture) {
	// Récupération des sommets
	Vec A = vertices[0];
	Vec B = vertices[1];
	Vec C = vertices[2];

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
					// On enregistre le z dans le z_buffer
					z_buffer[x][y] = P.z;

					// Récupération de la texture
					float u = bary1 * textures[0].x + bary2 * textures[1].x + bary3 * textures[2].x;
					float v = bary1 * textures[0].y + bary2 * textures[1].y + bary3 * textures[2].y;
					TGAColor color = texture.get(u * texture.get_width(), v * texture.get_height());

					float intensity = bary1 * intensities[0] + bary2 * intensities[1] + bary3 * intensities[2];

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

// méthode qui transforme un vecteur 3D en matrice 4D en ajoutant 1 à la 4ième composante (formule 1)
Matrix vectorToMatrix(Vec vec) {
	Matrix mVec(4, 1);

	mVec.set(0, 0, vec.x);
	mVec.set(1, 0, vec.y);
	mVec.set(2, 0, vec.z);
	mVec.set(3, 0, 1);

	return mVec;
}

// Méthode qui transforme une matrice 4D mat en un vecteur 3D en divisant chaque composante par la 4ième composante (formule 3)
Vec matrixToVector(Matrix mat) {
	Vec vMat;

	vMat.x = mat[0][0] / mat[3][0];
	vMat.y = mat[1][0] / mat[3][0];
	vMat.z = mat[2][0] / mat[3][0];

	return vMat;
}

// Viewport
Matrix viewport(int x, int y, int w, int h) {
	Matrix vp = Matrix::identityMatrix(4);

	vp.set(0, 3, x + w / 2);
	vp.set(1, 3, y + h / 2);
	vp.set(2, 3, depth / 2);

	vp.set(0, 0, w / 2);
	vp.set(1, 1, h / 2);
	vp.set(2, 2, depth / 2);

	return vp;
}

Vec centralProjection(Vec point) {
	float cameraDist = 1 - (point.z / 3);
	return { point.x / cameraDist, point.y / cameraDist, point.z / cameraDist };
}

// LookAt
Matrix  lookAt(Vec camera, Vec center, Vec up) {
	Vec z = (center - camera).normalize();
	Vec x = (up ^ z).normalize();
	Vec y = (z ^ x).normalize();

	Matrix Minv = Matrix::identityMatrix(4);
	Matrix Tr = Matrix::identityMatrix(4);

	for (int i = 0; i < 3; i++) {
		Minv.set(0, i, x.coords[i]);
		Minv.set(1, i, y.coords[i]);
		Minv.set(2, i, z.coords[i]);
		Tr.set(i, 3, -center.coords[i]);
	}

	return (Minv * Tr);
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

	// Face
	Face f;
	// Couleur
	TGAColor color;
	// Vecteur lumière
	Vec light = { 0,0,1 };
	light.normalize();
	// Caméra
	Vec camera = { 0,0,3 };
	// Centre
	Vec center = { 0,0,0 };
	// Up
	Vec up = { 0,1,0 };
	// Matrice de la projection
	Matrix projection = Matrix::identityMatrix(4);
	projection.set(3, 2, (-1 / (center - camera).norm()));
	// ViewPort
	Matrix viewPort = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	// ModelView
	Matrix modelView = lookAt(camera, center, up);
	

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

		// Récupération des vecteurs représentant les sommets
		Vec* vertices = new Vec[3];
		for (int j = 0; j < 3; j++) {
			vertices[j] = modele->getVerticeAt(f.vertices[j]);
		}

		// On calcule le vecteur normal du triangle
		Vec normal = ((vertices[0] - vertices[1]) ^ (vertices[1] - vertices[2]));

		// On normalise ce vecteur
		normal.normalize();

		// On calcule l'intensité de la lumière
		float intensity = (normal * light);

		// Si l'intensité est supérieur à 0
		if (intensity > 0) {

			// Récupération des vecteurs représentant les textures, les projection des sommets et les intensités aux sommets
			Vec *textures = new Vec[3];
			float *intensities = new float[3];
			std::cout << f.vertices[0] << " - " << f.vertices[1] << " - " << f.vertices[2] << std::endl;
			for (int j = 0; j < 3; j++) {
				vertices[j] = matrixToVector(viewPort * projection * modelView * vectorToMatrix(vertices[j])); // (formule 2)
				textures[j] = modele->getTextureAt(f.textures[j]);
				Vec c = modele->getNormalAt(f.normals[j]);
				intensities[j] = (modele->getNormalAt(f.normals[j])) * light;
				cout << c.coords[0] << " " << c.coords[1] << " " << c.coords[2] << endl;
				cout << intensities[j] << endl;
			}
			cout << endl;

			// On dessine le triangle correspondant à la Face
			drawTriangle(vertices, textures, intensities, z_buffer, image, texture);
		}

	}

	std::cout << std::endl << light.x << " - " << light.y << " - " << light.z << std::endl;
	
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}

