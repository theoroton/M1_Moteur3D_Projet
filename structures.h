#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include <cmath>
#include <ctgmath>
#include <vector>

//Structure représentant un vecteur
struct Vec {
	union {
		float coords[3];
		struct {
			float x, y, z;
		};
	};

	Vec operator- (Vec v2) { 
		return { 
			v2.x - x, 
			v2.y - y, 
			v2.z - z 
		}; 
	}

	Vec operator^ (Vec v2) {
		return {
			y * v2.z - z * v2.y,
			z * v2.x - x * v2.z,
			x * v2.y - y * v2.x
		};
	}

	float operator*(Vec v2) {
		return (x * v2.x + y * v2.y + z * v2.z);
	}

	float norm() {
		return std::sqrt((x * x) + (y * y) + (z * z));
	}

	Vec normalize() {
		float a = norm();
		x = x / a;
		y = y / a;
		z = z / a;

		return { x,y,z };
	}
};

//Structure représentant une face
struct Face {
	union {
		int vertices[3];
		struct {
			int a, b, c;
		};
	};

	union {
		int textures[3];
		struct {
			int ta, tb, tc;
		};
	};
	
	union {
		int normals[3];
		struct {
			int na, nb, nc;
		};
	};
};

//Classe représentant une matrice
class Matrix {
public:
	Matrix(int ro, int co);
	static Matrix identityMatrix(int dim);
	Matrix operator* (Matrix mat2);
	std::vector<float> operator[](int index);
	void set(int ro, int co, float val);
	int nbRows();
	int nbColumns();

private:
	int rows;
	int columns;
	std::vector<std::vector<float>> matr;
};

#endif