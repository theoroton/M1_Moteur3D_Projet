# Moteur 3D

Mon implémentation du moteur de rendu 3D : https://github.com/ssloy/tinyrenderer
Réalisé sur Visual Studio 2019

## Lesson 1 Bresenham’s Line Drawing Algorithm :

- Affichage des sommets qui composent le modèle : [code](https://github.com/theoroton/Moteur_3D/tree/a13d11dfe5d40e0547a01eb80f37cc059624d32e)
<img src="./images/l1_vertices.jpg" width="400" height="400" />

- Affichage des triangles qui composent le modèle : [code](https://github.com/theoroton/Moteur_3D/tree/8a531e86cccd345dc557f55c7f8d7ab09b562318)
<img src="./images/l1_faces.jpg" width="400" height="400" />

## Lesson 2 Triangle rasterization and back face culling :

- Remplissage des triangles simple (triangle coupé en 2 parties) : [code](https://github.com/theoroton/Moteur_3D/tree/3407e9ad9b1b9904dd252af66c51391d1ffc35c9)
<img src="./images/l2_triangle_standard.jpg"/>

- Remplissage des triangles avec coordonnées barycentriques (couleurs aléatoires) : [code](https://github.com/theoroton/Moteur_3D/tree/9c19667965ef45ac4df9b4200491011d8f5ca85d)
<img src="./images/l2_triangle_bary.jpg" width="400" height="400" />

- Ajout de la lumière sur le modèle : [code](https://github.com/theoroton/Moteur_3D/tree/ff83f128228e698509f99d57f049358bf93aa064)
<img src="./images/l2_light.jpg" width="400" height="400" />

## Lesson 3 Hidden faces removal (z buffer)

- Z buffer : [code](https://github.com/theoroton/Moteur_3D/tree/2d219f0ff74c3f18eb698ab0d9f2420fb47cc677)
<img src="./images/l3_zbuffer.jpg" width="400" height="400" />

- Affichage des textures : [code](https://github.com/theoroton/Moteur_3D/tree/394c523e92cc7ed7e8098dbdfcfb0b0392bfee90)
<img src="./images/l3_textures.jpg" width="400" height="400" />
