#include"Parameterization.h"

int main() {
	
	Param p;
	p.ReadMesh("camelhead.off");
	p.verticesNum();
	p.EntryCalculation();
	p.boundaryCalculation();
	p.uvCalculation();
	p.outPut2DMesh();
	system("pause");
	return 1;
}