#include"drawScene.h"
#include"variable.h"
#include<cmath>
#include <Eigen/core>
#include <Eigen/Geometry>
#include<vector>
using namespace std;
vector<double>  GaussList;
vector<double>  MeanList;
vector<vector<double>> meanColor;
vector<vector<double>> gaussColor;

#define M_PI 3.1415926;

#include <fstream>
ofstream debug("debug.txt");

//compute the norm vector
drawScene::drawScene() {}


int* drawScene:: RGBtoHSV(int red, int green, int blue) {// 8 bit representation instead of percenct   so  int (0--255)
	
	int *hsv = new int[3];
	int maximum = std::max(red,std::max(green,blue));
	int minimum = std::min(red,std::min(green,blue));

	//hue 
	if (maximum==minimum) {
		hsv[0] = 0;
	}
	else if (maximum==red) {
		hsv[0] = (int)(60 * (green - blue) / (maximum - minimum) + 360) % 360;

	}
	else if (maximum == green) {
		hsv[0] = (60 * (blue - red) / (maximum - minimum)) + 120;
	}
	else if (maximum == blue) {
		hsv[0] = (60 * (red - green) / (maximum - minimum)) + 240;
	}

	// saturation
	if (maximum == 0) {
		hsv[1] = 0;
	}
	else {
		hsv[1] = (255 * ((maximum - minimum) / maximum));
	}

	// value
	hsv[2] = maximum;

	return hsv;


}

int* drawScene::HSVtoRGB(int h,int s,int v) {

	float f;
	int i, p, q, t;
	int* rgb = new int[3];

	i = (int)floor(h / 60.0f) % 6;
	f = (float)(h / 60.0f) - (float)floor(h / 60.0f);
	p = (int)round(v * (1.0f - (s / 255.0f)));
	q = (int)round(v * (1.0f - (s / 255.0f) * f));
	t = (int)round(v * (1.0f - (s / 255.0f) * (1.0f - f)));

	switch (i) {
	case 0: rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
	case 1: rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
	case 2: rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
	case 3: rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
	case 4: rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
	case 5: rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
	}

	return rgb;

}
// color calculation



void drawScene::Info_Poly(MyMesh mesh) {

	int Count_Face = 0;

	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
		Count_Face++;
	}
	std::cout << std::endl;
	if (Count_Face == 4) {

		std::cout << "Geometry Information For Tetra:" << std::endl;
		std::cout << " the number of faces is " << Count_Face << std::endl;
	}
	else if (Count_Face==6) {

		std::cout << "Geometry Information For Cube:" << std::endl;
		std::cout << " the number of faces is " << Count_Face << std::endl;
	}
	else if (Count_Face == 8) {

		std::cout << "Geometry Information For Octa:" << std::endl;
		std::cout << " the number of faces is " << Count_Face << std::endl;
	}
	else if (Count_Face == 12) {

		std::cout << "Geometry Information For Dode:" << std::endl;
		std::cout << " the number of faces is " << Count_Face << std::endl;
	}
	else if (Count_Face == 20) {

		std::cout << "Geometry Information For Icos:" << std::endl;
		std::cout << " the number of faces is " << Count_Face << std::endl;
	}
	


	int Count_Vertex = 0;
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end();++v_it) {
		Count_Vertex++;
	}
	std::cout << " the number of vertices is " << Count_Vertex << std::endl;

	int HalfEdge_Count = 0;
	for (MyMesh::HalfedgeIter h_it = mesh.halfedges_begin(); h_it != mesh.halfedges_end(); ++h_it) {
		
		HalfEdge_Count++;
	}
	std::cout << " the number of halfedges is " << HalfEdge_Count << std::endl;

	int boundary_Edge = 0;
	for (MyMesh::HalfedgeIter h_it = mesh.halfedges_begin(); h_it != mesh.halfedges_end(); ++h_it) {
		if (mesh.is_boundary(*h_it)) {
			
			boundary_Edge++;
		}
		
	}
	std::cout << " the number of halfedges is " << boundary_Edge << std::endl;

	



}


void drawScene::CurCalculation(MyMesh mesh) {
	
	int minmaxCount = 0;

	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
	
		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {

			
			double cot_sum = 0;
			double angle = 0;
			double cot_alpha, cot_beta, cos;
			Eigen::Vector3d Gauss_sum=Eigen::Vector3d(0,0,0);
			double Voronoi_sum = 0;
			double Gauss_Cur, Mean_Cur;

			Eigen::Vector3d currentPoint;
			Eigen::Vector3d formerPoint;
			Eigen::Vector3d latterPoint;
			vector<Eigen::Vector3d> adjacentList;
			Eigen::Vector3d centerPoint;


		    centerPoint = Eigen::Vector3d(mesh.point(*fv_it).data()[0], mesh.point(*fv_it).data()[1], mesh.point(*fv_it).data()[2]);
			Eigen::Vector3d v_pi;
			Eigen::Vector3d v1, v2;
			int length = 0;
			for (MyMesh::VertexVertexIter vv_it = mesh.vv_iter(*fv_it); vv_it.is_valid(); ++vv_it) {
				Eigen::Vector3d temp = Eigen::Vector3d(mesh.point(*vv_it).data()[0], mesh.point(*vv_it).data()[1], mesh.point(*vv_it).data()[2]);
				adjacentList.push_back(temp);
				length++;
			}

			// calculate cot_sum and angle

			for (int j = 0; j < length; j++) {

				currentPoint = adjacentList[j];

				if (j == 0) {
					formerPoint = adjacentList[length - 1];
					latterPoint = adjacentList[1];
				}
				else if(j!=0) {

					formerPoint = adjacentList[j - 1];
					latterPoint = adjacentList[(j + 1) % length];
				}

				v1 = currentPoint - formerPoint;
				v2 = centerPoint - formerPoint;
				cos = (v1.dot(v2)) / (v1.norm()*v2.norm());
				cot_alpha = cos /pow((1 - pow(cos, 2)), 1.0 / 2);
				v1 = currentPoint - latterPoint; //cout << v1;
				v2 = centerPoint - latterPoint;
				cos = (v1.dot(v2)) / (v1.norm()*v2.norm());
				cot_beta = cos / pow((1 - pow(cos, 2)), 1.0 / 2);
				cot_sum = cot_alpha + cot_beta;
				Gauss_sum += cot_sum*(currentPoint - centerPoint);
				Voronoi_sum += cot_sum*pow(((currentPoint - centerPoint).norm()), 2);
				//theta calculation
				v1 = currentPoint - centerPoint;
				v2 = latterPoint - centerPoint;
				cos = (v1.dot(v2)) / (v1.norm()*v2.norm());
				angle += acos(cos);
			}
			Voronoi_sum = Voronoi_sum/ 8; // Area calculation
			Mean_Cur =(( 2*Gauss_sum / Voronoi_sum).norm())/2;
			Gauss_Cur =(2 * 3.14 - angle)/ Voronoi_sum;// m_pi?
			if (minmaxCount==0) {
				
				MaxGauss = Gauss_Cur;
				MinGauss = Gauss_Cur;
				MaxMean = Mean_Cur;
				MinMean = Mean_Cur;

			}

			else if (minmaxCount != 0) {
				
				if (Gauss_Cur > MaxGauss) {
					MaxGauss = Gauss_Cur;
				}
				if (Gauss_Cur<MinGauss)
				{
					MinGauss = Gauss_Cur;
				}
				if (Mean_Cur > MaxMean) {
					MaxMean = Mean_Cur;
				}
				if (Mean_Cur<MinMean)
				{
					MinMean = Mean_Cur;
				}


			}

			minmaxCount++;
			
			


			GaussList.push_back(Gauss_Cur);
			MeanList.push_back(Mean_Cur);

		}
	}
	//debug << "Max gauss:" << MaxGauss << endl;
	//debug << "Min gauss:" << MinGauss << endl;
	for (int i = 0; i < GaussList.size(); i++)
	{
		debug << GaussList[i] << endl;
	}
	cout << MaxGauss << endl;
	cout << MinGauss << endl;
}


void drawScene::colorCalculation() {

	double ratio;
	double x, z;// interpolation color
	debug << "========================" << endl;
	debug << double(MaxGauss - MinGauss) << endl;
	for (int i = 0; i < GaussList.size(); i++) {
		// gausslist color calculation

		vector<double> gaussRGB, meanRGB;

		//if (GaussList[i] == MaxGauss) {
		//	gaussRGB.push_back(0);
		//	gaussRGB.push_back(0);
		//	gaussRGB.push_back(1);
		//}
		//else if (GaussList[i] == MinGauss) {
		//	gaussRGB.push_back(1);
		//	gaussRGB.push_back(0);
		//	gaussRGB.push_back(0);
		//}
		//else if (GaussList[i]<MaxGauss&&GaussList[i]>MinGauss) 
		{
			ratio = (GaussList[i] +10000) / (20000 + 10000);// GaussList[i]); //cout << ratio<<" ";
			//int* hsv=RGBtoHSV(255/(1+ratio),0,255*ratio/(1+ratio));
			//int* rgb = HSVtoRGB(hsv[0],hsv[1],hsv[2]);
			//gaussRGB.push_back(1 / (1 + ratio));

		//	debug << "Max gauss:" << MaxGauss << "__";
			//debug << "Min gauss:" << MinGauss << "__";
			//debug << "gauss:" << GaussList[i]<< endl; 
			//debug << ratio << endl;
			gaussRGB.push_back(ratio);
			gaussRGB.push_back(0);
			//gaussRGB.push_back(ratio / (1 + ratio));
			gaussRGB.push_back(1 - ratio);
		}
		//cout << gaussRGB[0] << " " << endl;
		gaussColor.push_back(gaussRGB);

	}
	cout << "" << endl;
}

//draw tetrahegon
void drawScene::drawTetrahegon(int type) {
	//shading implementation
	if(count_Tetra==0){
		
		if (!OpenMesh::IO::read_mesh(mesh_Tetra, "bunny.obj"))
		{
			std::cerr << "read error\n";
			exit(1);
		}
		

		

		//glScaled(10, 10, 10);

		
	}
	
	count_Tetra--;
	
	
	
	if (info_Tetra == 0) {
		Info_Poly(mesh_Tetra);
	}
	
	info_Tetra--;
	// curvature calculation
	if (curCount == 0) {
		CurCalculation(mesh_Tetra);
		colorCalculation();
	}

	curCount--;

	GLfloat colors[][3] = {
		{ 0,0,1 },
		{ 0,1,0 },
		{ 0,0,1 },
		{ 1,0,1 }
	};

	// Add face normals as default property
	mesh_Tetra.request_face_normals();
	// Add vertex normals as default property (ref. previous tutorial)
	mesh_Tetra.request_vertex_normals();

	// If the file did not provide vertex normals, then calculate them

	OpenMesh::IO::Options opt;
	if (!opt.check(OpenMesh::IO::Options::VertexNormal) &&
		mesh_Tetra.has_face_normals() && mesh_Tetra.has_vertex_normals())
	{
		// let the mesh update the normals
		mesh_Tetra.update_normals();
	}

	//set the material of the premitives
	float diffuse[] = { 01,0.0,1,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);


	if (type == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	else if (type == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (type == 3)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (isFlatOrSmooth == 1) {
		glShadeModel(GL_FLAT);
	}
	if (isFlatOrSmooth == 0) {
		glShadeModel(GL_SMOOTH);
	}
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScaled(10,10,10);*/


	//glPointSize(3);
	
	
	int k = 0;
	int countPoint = 0;

	for (MyMesh::FaceIter f_it = mesh_Tetra.faces_begin(); f_it != mesh_Tetra.faces_end();++f_it) {
		
		if (isFlatOrSmooth == 1) {
			glNormal3d(-mesh_Tetra.normal(*f_it).data()[0],-mesh_Tetra.normal(*f_it).data()[1], -mesh_Tetra.normal(*f_it).data()[2]);
		}
		
		//glColor3fv(colors[i]);
		//glColor3f(1,0,1);
		glBegin(GL_POLYGON);
		
		for (MyMesh::FaceVertexIter fv_it = mesh_Tetra.fv_iter(*f_it); fv_it.is_valid();++fv_it) {
			//MyMesh::VertexHandle vh = fv_it.handle();
		
			if (ColorCodingType==1) {//  why still yellow when no color added 
				int Valence = mesh_Tetra.valence(*fv_it);

				if (Valence <= 4 && Valence >= 2) {
					glColor3f(0, 0, 1);
					
				}
				else if (Valence <= 7 && Valence >= 5) {
					glColor3f(1, 1, 0);
					
				}
				else if (Valence >= 8) {
					
					glColor3f(1, 0, 0);
					
				}
			}
			
			else if (ColorCodingType==2) {
				
				//cout << ColorCodingType;
				
				glColor3d(gaussColor[countPoint][0], gaussColor[countPoint][1], gaussColor[countPoint][2]); //cout << gaussColor[countPoint][0] << " ";
				//glColor3f(1,0,0);
				countPoint++;

			}
			if (isFlatOrSmooth==0) { 
				//glNormal3fv(mesh.normal(vh).data());
				glNormal3d(-mesh_Tetra.normal(*fv_it).data()[0], -mesh_Tetra.normal(*fv_it).data()[1], -mesh_Tetra.normal(*fv_it).data()[2]);
			}
			
			glVertex3d(15*mesh_Tetra.point(*fv_it).data()[0],15* mesh_Tetra.point(*fv_it).data()[1], 15* mesh_Tetra.point(*fv_it).data()[2]);
		}

		glEnd();

		
	}

	
	

	

}
// draw cube

void drawScene::drawCube(int type) {

	if (count_Cube==0) {
		if (!OpenMesh::IO::read_mesh(mesh_Cube, "r03.obj"))
		{
			std::cerr << "read error\n";
			exit(1);
		}
	}
	count_Cube--;
	
	if (info_Cube == 0) {
		Info_Poly(mesh_Cube);
	}

	info_Cube--;


/*	if (curCount==0) {
		CurCalculation(mesh_Cube);
	}
	curCount--;*/

	float color[][3] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 1.0, 0.0 },
		{ 1.0, 0.0, 1.0 },
		{ 0.0, 1.0, 1.0 }
	};

	// Add face normals as default property
	mesh_Cube.request_face_normals();
	// Add vertex normals as default property (ref. previous tutorial)
	mesh_Cube.request_vertex_normals();

	// If the file did not provide vertex normals, then calculate them

	OpenMesh::IO::Options opt;
	if (!opt.check(OpenMesh::IO::Options::VertexNormal) &&
		mesh_Cube.has_face_normals() && mesh_Cube.has_vertex_normals())
	{
		// let the mesh update the normals
		mesh_Cube.update_normals();
	}
	

	float diffuse[] = { 0.3,0.8,0.2,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

	

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
	
	int i=0;

	if (isFlatOrSmooth == 1) {
		glShadeModel(GL_FLAT);
	}
	if (isFlatOrSmooth == 0) {
		glShadeModel(GL_SMOOTH);
	}

	glBegin(GL_QUADS);
	for (MyMesh::VertexIter v_it = mesh_Cube.vertices_begin(); v_it != mesh_Cube.vertices_end(); v_it++) {
		debug << *v_it << endl;
		for(MyMesh::VertexVertexIter vv_it = mesh_Cube.vv_iter(*v_it); vv_it.is_valid(); vv_it++){
			debug <<vv_it->idx() << " ";
		}
		debug << endl;
	}
	for (MyMesh::FaceIter f_it = mesh_Cube.faces_begin(); f_it != mesh_Cube.faces_end(); ++f_it) {
		
		if (isFlatOrSmooth == 1) {
			glNormal3d(mesh_Cube.normal(*f_it).data()[0], mesh_Cube.normal(*f_it).data()[1], mesh_Cube.normal(*f_it).data()[2]);
		}

		glColor3fv(color[i]);
		  
		for (MyMesh::FaceVertexIter fv_it = mesh_Cube.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
			if (isFlatOrSmooth == 0) {
				glNormal3d(mesh_Cube.normal(*fv_it).data()[0], mesh_Cube.normal(*fv_it).data()[1], mesh_Cube.normal(*fv_it).data()[2]);
			}
			glVertex3d(mesh_Cube.point(*fv_it).data()[0], mesh_Cube.point(*fv_it).data()[1], mesh_Cube.point(*fv_it).data()[2]);//why point?
		}
		i++;
	}
	glEnd();

	


}

//draw octahegon

void drawScene::drawOctahegron(int type) {
	if (count_Octa==0) {
		if (!OpenMesh::IO::read_mesh(mesh_Octa, "r02.obj"))
		{
			std::cerr << "read error\n";
			exit(1);
		}
	}
	count_Octa--;
	

	if (info_Octa == 0) {
		Info_Poly(mesh_Octa);
	}

	info_Octa--;
	
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

	// Add face normals as default property
	mesh_Octa.request_face_normals();
	// Add vertex normals as default property (ref. previous tutorial)
	mesh_Octa.request_vertex_normals();

	// If the file did not provide vertex normals, then calculate them

	OpenMesh::IO::Options opt;
	if (!opt.check(OpenMesh::IO::Options::VertexNormal) &&
		mesh_Octa.has_face_normals() && mesh_Octa.has_vertex_normals())
	{
		// let the mesh update the normals
		mesh_Octa.update_normals();
	}
	
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


	glPointSize(3);
	int i = 0;
	
	if (isFlatOrSmooth == 1) {
		glShadeModel(GL_FLAT);
	}
	if (isFlatOrSmooth == 0) {
		glShadeModel(GL_SMOOTH);
	}

		
	glBegin(GL_TRIANGLES);

		for (MyMesh::FaceIter f_it = mesh_Octa.faces_begin(); f_it != mesh_Octa.faces_end(); ++f_it) {
			if (isFlatOrSmooth == 1) {
				glNormal3d(mesh_Octa.normal(*f_it).data()[0], mesh_Octa.normal(*f_it).data()[1], mesh_Octa.normal(*f_it).data()[2]);
			}
			glColor3fv(colors[i]);

			for (MyMesh::FaceVertexIter fv_it = mesh_Octa.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
				if (isFlatOrSmooth == 0) {
					glNormal3d(mesh_Octa.normal(*fv_it).data()[0], mesh_Octa.normal(*fv_it).data()[1], mesh_Octa.normal(*fv_it).data()[2]);
				}
				glVertex3d(mesh_Octa.point(*fv_it).data()[0], mesh_Octa.point(*fv_it).data()[1], mesh_Octa.point(*fv_it).data()[2]);//why point?
			}
			i++;
		}

	glEnd();
}

//³\“ñ–Ê‘Ì

void drawScene::drawDodecahedron(int type) {
	if (count_Dode==0) {
		if (!OpenMesh::IO::read_mesh(mesh_Dode, "r04.obj"))
		{
			std::cerr << "read error\n";
			exit(1);
		}
	}
	count_Dode--;

	if (info_Dode == 0) {
		Info_Poly(mesh_Dode);
	}

	info_Dode--;

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

	// Add face normals as default property
	mesh_Dode.request_face_normals();
	// Add vertex normals as default property (ref. previous tutorial)
	mesh_Dode.request_vertex_normals();

	// If the file did not provide vertex normals, then calculate them

	OpenMesh::IO::Options opt;
	if (!opt.check(OpenMesh::IO::Options::VertexNormal) &&
		mesh_Dode.has_face_normals() && mesh_Dode.has_vertex_normals())
	{
		// let the mesh update the normals
		mesh_Dode.update_normals();
	}


	//set the material of the premitives
	float diffuse[] = { 0.8,0.8,0.2,1.0 };
	float specular[] = { 0.9,0.9,0.9,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0f);

	glPointSize(3);
	glClear(GL_COLOR_BUFFER_BIT);

	if (type == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	else if (type == 2)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (type == 3)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (isFlatOrSmooth == 1) {
		glShadeModel(GL_FLAT);
	}
	if (isFlatOrSmooth == 0) {
		glShadeModel(GL_SMOOTH);
	}

	int i = 0;
	for (MyMesh::FaceIter f_it = mesh_Dode.faces_begin(); f_it != mesh_Dode.faces_end(); ++f_it) {
		if (isFlatOrSmooth == 1) {
			glNormal3d(mesh_Dode.normal(*f_it).data()[0], mesh_Dode.normal(*f_it).data()[1], mesh_Dode.normal(*f_it).data()[2]);
		}
		glColor3fv(colors[i]);
		glBegin(GL_POLYGON);
		for (MyMesh::FaceVertexIter fv_it = mesh_Dode.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
			if (isFlatOrSmooth == 0) {
				glNormal3d(mesh_Dode.normal(*fv_it).data()[0], mesh_Dode.normal(*fv_it).data()[1], mesh_Dode.normal(*fv_it).data()[2]);
			}
			glVertex3d(mesh_Dode.point(*fv_it).data()[0], mesh_Dode.point(*fv_it).data()[1], mesh_Dode.point(*fv_it).data()[2]);//why point?
		}
		glEnd();
		i++;
	}


}

//³“ñ\–Ê‘Ì

void  drawScene::drawIcosahedron(int type) {

	if (count_Icos==0) {

		if (!OpenMesh::IO::read_mesh(mesh_Icos, "r05.obj"))
		{
			std::cerr << "read error\n";
			exit(1);
		}

	}
	count_Icos--;
	
	
	if (info_Icos == 0) {
		Info_Poly(mesh_Icos);
	}

	info_Icos--;
	// Add face normals as default property
	mesh_Icos.request_face_normals();
	// Add vertex normals as default property (ref. previous tutorial)
	mesh_Icos.request_vertex_normals();

	// If the file did not provide vertex normals, then calculate them

	OpenMesh::IO::Options opt;
	if (!opt.check(OpenMesh::IO::Options::VertexNormal) &&
		mesh_Icos.has_face_normals() && mesh_Icos.has_vertex_normals())
	{
		// let the mesh update the normals
		mesh_Icos.update_normals();
	}


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
	
	glPointSize(3);
	int i = 0;
	if (isFlatOrSmooth == 1) {
		glShadeModel(GL_FLAT);
	}
	if (isFlatOrSmooth == 0) {
		glShadeModel(GL_SMOOTH);
	}
	std::cout << isFlatOrSmooth << std::endl;
	
		for (MyMesh::FaceIter f_it = mesh_Icos.faces_begin(); f_it != mesh_Icos.faces_end(); ++f_it) {
			if (isFlatOrSmooth == 1) {
				//glShadeModel(GL_FLAT);
				glNormal3d(mesh_Icos.calc_face_normal(*f_it).data()[0], mesh_Icos.calc_face_normal(*f_it).data()[1], mesh_Icos.calc_face_normal(*f_it).data()[2]);
				
			}

			glColor3fv(colors[i]);
			glBegin(GL_POLYGON);

			for (MyMesh::FaceVertexIter fv_it = mesh_Icos.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {

				if (isFlatOrSmooth == 0) {
					//glShadeModel(GL_SMOOTH);

				glNormal3d(mesh_Icos.normal(*fv_it).data()[0], mesh_Icos.normal(*fv_it).data()[1], mesh_Icos.normal(*fv_it).data()[2]);
				
				}


				glVertex3d(mesh_Icos.point(*fv_it).data()[0], mesh_Icos.point(*fv_it).data()[1], mesh_Icos.point(*fv_it).data()[2]);//why point?
			}
			glEnd();
			i++;
		}
	





}

// •\Ž¦‚ÌØ‚è‘Ö‚¦


