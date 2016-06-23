#include <iostream>
#include <Eigen/core>
#include <Eigen/Geometry>
#include <GLFW/glfw3.h>



class drawScene {




public:


	drawScene();

	void normVector(int i, GLdouble vertex[][3], int* faces, int n);
	void drawTetrahegon(int type);
	void drawCube(int type);
	void drawOctahegron(int type);
	void drawDodecahedron(int type);
	void  drawIcosahedron(int type);
};
