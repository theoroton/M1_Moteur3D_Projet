#include "structures.h"
#include <vector>
#include <iostream>

using namespace std;

// Constructeur d'une matrice avec ro lignes et co colonnes
Matrix::Matrix(int ro, int co) {
	rows = ro;
	columns = co;
	for (int i = 0; i < ro; i++) {
		matr.push_back(vector<float>(co, 0));
	}
}

// Méthode qui crée une matrice identité de dimension dim
Matrix Matrix::identityMatrix(int dim) {
	Matrix idMatrix(dim, dim);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if (i == j) {
				idMatrix.matr[i][j] = 1;
			}
		}
	}
	return idMatrix;
}

// Multiplication matriciel
Matrix Matrix::operator*(Matrix mat2) {
	Matrix resMatrix(rows, mat2.columns);
	float res;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < mat2.columns; j++) {
			res = 0;
			for (int k = 0; k < columns; k++) {
				res += matr[i][k] * mat2.matr[k][j];
			}
			resMatrix.matr[i][j] = res;
		}
	}

	return resMatrix;
}

// Transposition de matrice
Matrix Matrix::transpose() {
	Matrix transMatrix(columns, rows);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			transMatrix.matr[j][i] = matr[i][j];
		}
	}

	return transMatrix;
}

// Inverse de matrice
Matrix Matrix::inverse() {
	float s0 = matr[0][0] * matr[1][1] - matr[1][0] * matr[0][1];
	float s1 = matr[0][0] * matr[1][2] - matr[1][0] * matr[0][2];
	float s2 = matr[0][0] * matr[1][3] - matr[1][0] * matr[0][3];
	float s3 = matr[0][1] * matr[1][2] - matr[1][1] * matr[0][2];
	float s4 = matr[0][1] * matr[1][3] - matr[1][1] * matr[0][3];
	float s5 = matr[0][2] * matr[1][3] - matr[1][2] * matr[0][3];

	float c5 = matr[2][2] * matr[3][3] - matr[3][2] * matr[2][3];
	float c4 = matr[2][1] * matr[3][3] - matr[3][1] * matr[2][3];
	float c3 = matr[2][1] * matr[3][2] - matr[3][1] * matr[2][2];
	float c2 = matr[2][0] * matr[3][3] - matr[3][0] * matr[2][3];
	float c1 = matr[2][0] * matr[3][2] - matr[3][0] * matr[2][2];
	float c0 = matr[2][0] * matr[3][1] - matr[3][0] * matr[2][1];

	// Should check for 0 determinant
	float invdet = 1.0 / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);

	Matrix inv(rows, columns);

	inv.matr[0][0] = (matr[1][1] * c5 - matr[1][2] * c4 + matr[1][3] * c3) * invdet;
	inv.matr[0][1] = (-matr[0][1] * c5 + matr[0][2] * c4 - matr[0][3] * c3) * invdet;
	inv.matr[0][2] = (matr[3][1] * s5 - matr[3][2] * s4 + matr[3][3] * s3) * invdet;
	inv.matr[0][3] = (-matr[2][1] * s5 + matr[2][2] * s4 - matr[2][3] * s3) * invdet;

	inv.matr[1][0] = (-matr[1][0] * c5 + matr[1][2] * c2 - matr[1][3] * c1) * invdet;
	inv.matr[1][1] = (matr[0][0] * c5 - matr[0][2] * c2 + matr[0][3] * c1) * invdet;
	inv.matr[1][2] = (-matr[3][0] * s5 + matr[3][2] * s2 - matr[3][3] * s1) * invdet;
	inv.matr[1][3] = (matr[2][0] * s5 - matr[2][2] * s2 + matr[2][3] * s1) * invdet;

	inv.matr[2][0] = (matr[1][0] * c4 - matr[1][1] * c2 + matr[1][3] * c0) * invdet;
	inv.matr[2][1] = (-matr[0][0] * c4 + matr[0][1] * c2 - matr[0][3] * c0) * invdet;
	inv.matr[2][2] = (matr[3][0] * s4 - matr[3][1] * s2 + matr[3][3] * s0) * invdet;
	inv.matr[2][3] = (-matr[2][0] * s4 + matr[2][1] * s2 - matr[2][3] * s0) * invdet;

	inv.matr[3][0] = (-matr[1][0] * c3 + matr[1][1] * c1 - matr[1][2] * c0) * invdet;
	inv.matr[3][1] = (matr[0][0] * c3 - matr[0][1] * c1 + matr[0][2] * c0) * invdet;
	inv.matr[3][2] = (-matr[3][0] * s3 + matr[3][1] * s1 - matr[3][2] * s0) * invdet;
	inv.matr[3][3] = (matr[2][0] * s3 - matr[2][1] * s1 + matr[2][2] * s0) * invdet;

	return inv;
}

// Récupére la ligne de la matrice d'indice index
vector<float> Matrix::operator[](int index) {
	return matr[index];
}

// Met à jour la ligne ro et la colonne co de la matrice avec la valeur val
void Matrix::set(int ro, int co, float val) {
	matr[ro][co] = val;
}

// Retourne le nombre de lignes de la matrice
int Matrix::nbRows() {
	return rows;
}

// Retourne le nombre de colonnes de la matrice
int Matrix::nbColumns() {
	return columns;
}