#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include <cmath>
#include <ctgmath>

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

	void normalize() {
		float a = std::sqrt((x * x) + (y * y) + (z * z));
		x = x / a;
		y = y / a;
		z = z / a;
	}

	void toImageSize(int width, int height) {
		x = int((x + 1.) * width / 2. + .5);
		y = int((y + 1.) * height / 2. + .5);
	}
};

//Structure représentant une face
struct Face {
	union {
		int sommets[3];
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
};

#endif