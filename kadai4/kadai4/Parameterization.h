// -------------------- OpenMesh
#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Handles.hh>
// ----------------------------------------------------------------------------
typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;
#include <Eigen/core>
#include <Eigen/Geometry>
#include<Eigen/Sparse>
#include<vector>
#include<cmath>
#include<string>
#include <Eigen/SparseLU>
#define _USE_MATH_DEFINES
using namespace std;
using namespace Eigen;

typedef Eigen::Triplet<double> T;


class Param {

public:   
	      MyMesh mesh;
		  MyMesh Mesh_2D;
		  std::vector<T> tripletList;
		  VectorXd u;
		  VectorXd v;
		  int vertexNum;// the number of total verticices
		  int boundaryVertex;//the number of boundary vertices
		  VectorXd boundaryU;// the u parameter of boundary vertices
		  VectorXd boundaryV;//the v parameter of boundary vertices

         void ReadMesh(string filename);// read mesh 
		 void EntryCalculation();// calculate the entry of the coefficient matrix 
		 void boundaryCalculation();// calculate the boundary condition for the system 
		 void uvCalculation();// calculate the u,v parameter for the internal vertices
		 void outPut2DMesh();
		 void verticesNum();
		 Param();// default constructor

		 



};
