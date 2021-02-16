#include "gl.h"

#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <ctgmath>

using namespace std;

IShader::~IShader() {};

// Viewport
Matrix viewport(int x, int y, int w, int h, int d) {
	Matrix vp = Matrix::identityMatrix(4);

	vp.set(0, 3, x + w / 2);
	vp.set(1, 3, y + h / 2);
	vp.set(2, 3, d / 2);

	vp.set(0, 0, w / 2);
	vp.set(1, 1, h / 2);
	vp.set(2, 2, d / 2);

	return vp;
}


// Projection
Matrix projection(float cam) {
	Matrix proj = Matrix::identityMatrix(4);

	proj.set(3, 2, cam);

	return proj;
}


// LookAt
Matrix modelview(Vec camera, Vec center, Vec up) {
	Vec z = (center - camera).normalize();
	Vec x = (up ^ z).normalize();
	Vec y = (z ^ x).normalize();

	Matrix mv = Matrix::identityMatrix(4);

	for (int i = 0; i < 3; i++) {
		mv.set(0, i, x.coords[i]);
		mv.set(1, i, y.coords[i]);
		mv.set(2, i, z.coords[i]);
		mv.set(i, 3, -center.coords[i]);
	}

	return mv;
}


// Méthode qui transforme un vecteur 3D en matrice 4D en ajoutant 1 à la 4ième composante (formule 1)
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


// Méthode qui calcule les coordonnées barycentriques
Vec barycenter(Vec A, Vec B, Vec C, Vec P) {
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

	// Retourne le vecteur du barycentre
	return { bary1, bary2, bary3 };
}


// Méthode pour afficher un triangle
void drawTriangle(Vec* vertices, IShader& shader, float** z_buffer, TGAImage& image) {

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
			Vec P = { (float)x, (float)y, 0. };

			// Calcul des coordonnées barycentriques
			Vec bary = barycenter(A, B, C, P);

			// Si toutes les coordonnées sont positives
			if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0) {
				// On calcule la composante z du point P
				P.z = A.z * bary[0] + B.z * bary[1] + C.z * bary[2];

				// Si le z est supérieur au z enregistré
				if (P.z >= z_buffer[x][y]) {
					// On enregistre le z dans le z_buffer
					z_buffer[x][y] = P.z;
					
					// Utilisation du shader
					TGAColor color = shader.getTexture(bary);

					// On affiche le pixel
					image.set(x, y, color);
				}
			}
		}
	}
}