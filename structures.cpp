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