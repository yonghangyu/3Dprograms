#include"drawScene.h"
#include"variable.h"

//compute the norm vector
drawScene::drawScene() {}

void drawScene::normVector(int i, GLdouble vertex[][3], int* faces, int n) {


	Eigen::Vector3d firstPoint;
	Eigen::Vector3d secondPoint;
	Eigen::Vector3d thirdPoint;

	firstPoint = Eigen::Vector3d(vertex[*(faces + n*i + 0)][0], vertex[*(faces + n*i + 0)][1], vertex[*(faces + n*i + 0)][2]);

	secondPoint = Eigen::Vector3d(vertex[*(faces + n*i + 1)][0], vertex[*(faces + n*i + 1)][1], vertex[*(faces + n*i + 1)][2]);

	thirdPoint = Eigen::Vector3d(vertex[*(faces + n*i + 2)][0], vertex[*(faces + n*i + 2)][1], vertex[*(faces + n*i + 2)][2]);


	Eigen::Vector3d fVector = secondPoint - firstPoint;
	Eigen::Vector3d sVector = secondPoint - thirdPoint;
	Eigen::Vector3d normal = fVector.cross(sVector);
	normal.normalize();

	//set the  surface normal 
	glNormal3d(normal.x(), normal.y(), normal.z());
	//std::cout << normal.x() << "";


}


//draw tetrahegon
void drawScene::drawTetrahegon(int type) {
	//shading implementation




	GLdouble points[][3] = {

		{ -0.81649658, -0.47140452,0.33333333 },
		{ 0.81649658 ,-0.47140452,0.33333333 },
		{ 0.00000000,0.00000000,-1.00000000 },
		{ 0.00000000,0.94280904,0.33333333 }

	};

	int  face_index[4][3] = {
		0,1,3,
		0,3,2,
		0,2,1,
		1,2,3,
	};

	GLfloat colors[][3] = {
		{ 0,0,1 },
		{ 0,1,0 },
		{ 0,0,1 },
		{ 1,0,1 }
	};

	//set the material of the premitives
	float diffuse[] = { 0.8,0.8,0.2,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);


	if (type == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	else if (type == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (type == 3)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	glShadeModel(GL_FLAT);
	glPointSize(3);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 4; i++) {

		glColor3fv(colors[i]);

		normVector(i, points, &face_index[0][0], 3);

		for (int j = 0; j < 3; j++) {

			glVertex3dv(points[face_index[i][j]]);
		}
	}
	glEnd();

}
// draw cube

void drawScene::drawCube(int type) {

	GLdouble vertex[][3] = {
		{ -1,-1,-1 },//0
		{ 1,-1,-1 },//1
		{ 1,1,-1 },//2
		{ -1,1,-1 },//3
		{ -1,-1,1 },//4
		{ 1,-1,1 },//5
		{ 1,1,1 },//6
		{ -1,1,1 }//7
	};

	int face[][4] = {

		{ 1,2,6,5 },
		{ 2,3,7,6 },
		{ 4,5,6,7 },
		{ 0,4,7,3 },
		{ 0,1,5,4 },
		{ 0,3,2,1 }
		/*{ 4, 5, 6, 7 },
		{ 5, 1, 2, 6 },
		{ 0,1,2,3 },
		{0,4,7,3},
		{ 7, 3, 2, 6 },
		{ 0, 4, 5, 1 }*/

	};
	float color[][3] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 1.0, 0.0 },
		{ 1.0, 0.0, 1.0 },
		{ 0.0, 1.0, 1.0 }
	};

	GLfloat normal[][3] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ -1.0, 0.0, 0.0 },
		{ 0.0,-1.0, 0.0 },
		{ 0.0,0.0,-1.0 }
	};

	float diffuse[] = { 0.3,0.8,0.2,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

	int i;

	/* }Œ`‚Ì•`‰æ */

	glClear(GL_COLOR_BUFFER_BIT);

	if (type == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
	else if (type == 2)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (type == 3)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	glPointSize(3);
	glShadeModel(GL_FLAT);
	for (i = 0; i < 6; ++i) {

		glBegin(GL_QUADS);
		glColor3fv(color[i]);
		glNormal3fv(normal[i]);
		for (int j = 0; j < 4; j++) {
			
			glVertex3dv(vertex[face[i][j]]);
		}
		glEnd();
	}

	glFlush();


}
//draw octahegon

void drawScene::drawOctahegron(int type) {
	GLdouble vertex[][3] = {
		{ -0.70710678 ,-0.70710678 , 0.00000000 },
		{ -0.70710678 , 0.70710678 , 0.00000000 },
		{ 0.70710678 , 0.70710678 , 0.00000000 },

		{ 0.70710678, -0.70710678 , 0.00000000 },
		{ 0.00000000 , 0.00000000 ,-1.00000000 },
		{ 0.00000000 , 0.00000000 , 1.00000000 }
	};

	GLint faces[][3] = {
		{ 0,1,4 },
		{ 0,4,3 },
		{ 0,3,5 },
		{ 0,5,1 },
		{ 1,2,4 },
		{ 1,5,2 },
		{ 2,3,4 },
		{ 2,5,3 }

	};

	GLfloat colors[][3] = {
		{ 0,1,1 },
		{ 1,0,1 },
		{ 1,1,0 },
		{ 0,0.5,0.5 },
		{ 0.5,0,0.5 },
		{ 0.5,0.5,0 },
		{ 1,0,0 },
		{ 0,0,1 }

	};
	//set the material 
	float diffuse[] = { 0.8,0.1,0.2,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 125.0f);


	if (type == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	else if (type == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (type == 3)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	glShadeModel(GL_FLAT);
	glPointSize(3);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 8; i++) {
		glColor3fv(colors[i]);
		normVector(i, vertex, faces[0], 3);
		for (int j = 0; j < 3; j++) {
			glVertex3dv(vertex[faces[i][j]]);
		}
	}
	glEnd();

}

//³\“ñ–Ê‘Ì

void drawScene::drawDodecahedron(int type) {

	GLdouble vertex[][3] = {
		{ -0.35682209 ,-0.49112347 , 0.79465447 },
		{ 0.35682209, -0.49112347,  0.79465447 },
		{ 0.57735027, -0.79465447 , 0.18759247 },
		{ 0.00000000 ,-0.98224695, -0.18759247 },
		{ -0.57735027 ,-0.79465447 , 0.18759247 },
		{ -0.57735027 , 0.18759247 , 0.79465447 },
		{ 0.57735027  ,0.18759247 , 0.79465447 },
		{ 0.93417236 ,-0.30353100 ,-0.18759247 },
		{ -0.00000000 ,-0.60706200 ,-0.79465447 },
		{ -0.93417236, -0.30353100 ,-0.18759247 },
		{ -0.93417236 , 0.30353100  ,0.18759247 },
		{ 0.00000000  ,0.60706200 , 0.79465447 },
		{ 0.93417236 , 0.30353100 , 0.18759247 },
		{ 0.57735027 ,-0.18759247 ,-0.79465447 },
		{ -0.57735027, -0.18759247, -0.79465447 },
		{ -0.57735027 , 0.79465447 ,-0.18759247 },
		{ 0.00000000  ,0.98224695 , 0.18759247 },
		{ 0.57735027  ,0.79465447 ,-0.18759247 },
		{ 0.35682209 , 0.49112347, -0.79465447 },
		{ -0.35682209 , 0.49112347 ,-0.79465447 }

	};
	GLint faces[][5] = {
		{ 1, 2 ,7, 12, 6 },
		{ 1 ,6 ,11, 10, 5 },
		{ 1, 5, 4, 3 ,2 },
		{ 2 ,3, 8 ,13, 7 },
		{ 3, 4, 9, 14, 8 },
		{ 4 ,5 ,10, 15 ,9 },
		{ 6 ,12, 17 ,16 ,11 },
		{ 7 ,13 ,18, 17, 12 },
		{ 8 ,14, 19, 18, 13 },
		{ 9 ,15 ,20 ,19, 14 },
		{ 10, 11 ,16 ,20 ,15 },
		{ 16, 17, 18, 19 ,20 }

	};
	GLfloat colors[][3] = {

		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 0 },
		{ 0, 1, 1 },
		{ 0, 0, 0.5 },
		{ 0, 0.5, 0 },
		{ 0, 0.5, 0.5 },
		{ 0.5, 0, 0 },
		{ 0.5, 0, 1 },


	};


	//set the material of the premitives
	float diffuse[] = { 0.8,0.8,0.2,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0f);


	glClear(GL_COLOR_BUFFER_BIT);

	if (type == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	else if (type == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (type == 3)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	glShadeModel(GL_FLAT);
	for (int i = 0; i < 12; ++i) {
		glColor3fv(colors[i]);
		glPointSize(3);
		glBegin(GL_POLYGON);
		normVector(i, vertex, faces[0], 5);
		for (int j = 0; j < 5; j++) {
			glVertex3dv(vertex[faces[i][j] - 1]);
		}
		glEnd();

	}

	glFlush();

}

//³“ñ\–Ê‘Ì

void  drawScene::drawIcosahedron(int type) {
	GLdouble vertex[][3] = {
		{ -0.52573111, -0.72360680,  0.44721360 },
		{ -0.85065081 , 0.27639320 , 0.44721360 },
		{ -0.00000000 , 0.89442719 , 0.44721360 },
		{ 0.85065081 , 0.27639320 , 0.44721360 },
		{ 0.52573111, -0.72360680 , 0.44721360 },
		{ 0.00000000, -0.89442719, -0.44721360 },
		{ -0.85065081 ,-0.27639320, -0.44721360 },
		{ -0.52573111 , 0.72360680, -0.44721360 },
		{ 0.52573111 , 0.72360680, -0.44721360 },
		{ 0.85065081, -0.27639320 ,-0.44721360 },
		{ 0.00000000 , 0.00000000 , 1.00000000 },
		{ -0.00000000 , 0.00000000 ,-1.00000000 }
	};

	GLint faces[][3] = {

		{ 1, 2, 7 },
		{ 1, 7, 6 },
		{ 1, 6, 5 },
		{ 1 ,5 ,11 },
		{ 1 ,11, 2 },
		{ 2 ,3, 8 },
		{ 2 ,8, 7 },
		{ 2 ,11 ,3 },
		{ 3, 4 ,9 },
		{ 3 ,9 ,8 },
		{ 3, 11, 4 },
		{ 4 ,5 ,10 },
		{ 4 ,10, 9 },
		{ 4 ,11 ,5 },
		{ 5 ,6 ,10 },
		{ 6 ,7 ,12 },
		{ 6 ,12 ,10 },
		{ 7 ,8 ,12 },
		{ 8 ,9 ,12 },
		{ 9, 10 ,12 }

	};

	GLfloat colors[][3] = {

		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 0, 0, 0.5 },
		{ 0, 0.5, 0 },
		{ 0, 0.5, 0.5 },
		{ 0.5, 0, 0 },
		{ 0.5, 0, 1 },
		{ 0, 0, 0.5 },
		{ 0.5, 0, 1 },
		{ 0, 0.5, 0.5 },
		{ 0.5, 0, 1 },
		{ 0.3, 1, 1 },
		{ 1, 0, 1 },
		{ 0, 0, 1 },
		{ 0, 1, 1 }

	};

	//set the material of the premitives
	float diffuse[] = { 0.8,0.8,0.2,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);


	if (type == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	else if (type == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (type == 3)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	glPointSize(3);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; ++i) {
		normVector(i, vertex, faces[0], 3);
		glColor3fv(colors[i]);
		for (int j = 0; j < 3; j++) {
			glVertex3dv(vertex[faces[i][j] - 1]);
		}
	}
	glEnd();
	//glFlush();



}
// •\Ž¦‚ÌØ‚è‘Ö‚¦


