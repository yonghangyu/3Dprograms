#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
using namespace Eigen;
using namespace std;
typedef Eigen::Triplet<double> T;
class LoadMatrixVector {
public:
	int cols, rows;
	int lineNum;
	string line;
public:
	vector<double> convert(string line);
	void FillMatrix(SparseMatrix<double> &A,string fileMatrix);
	void FillVector(VectorXd &b, string fileVector);


};
