#include <iostream>
#include <GLFW/glfw3.h>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include<vector>
// ----------------------------------------------------------------------------
typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;

class drawScene {


private:

	MyMesh mesh_Tetra;
	MyMesh mesh_Cube;
	MyMesh mesh_Octa;
	MyMesh mesh_Dode;
	MyMesh mesh_Icos;
	
	
public:

	drawScene();

	void drawTetrahegon(int type);
	void drawCube(int type);
	void drawOctahegron(int type);
	void drawDodecahedron(int type);
	void drawIcosahedron(int type);
	
	void  Info_Poly(MyMesh mesh);
	int* RGBtoHSV(int red, int green, int blue);
	int* HSVtoRGB(int h, int s, int v);
	void CurCalculation(MyMesh mesh);
	void colorCalculation();

};
