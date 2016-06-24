
#include"LoadMatrixVector.h"
#include <chrono>
#include<Eigen/SparseLU>
using namespace std::chrono;
int main()
{
	SparseMatrix<double> A;
	LoadMatrixVector object;
	//filling a sparse matrix
	object.FillMatrix(A, "A675.dat");
	VectorXd x(object.cols), b(object.cols);
	object.FillVector(b,"b675.dat");
	//numerical methods
	//cojugate gradient method
	ConjugateGradient<SparseMatrix<double>> cg;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	cg.compute(A);
	x = cg.solve(b);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();
	
	
	//std::cout << "Approximate solution: " << x << std::endl;
	cout << "cojugate gradient method :" << endl;
	cout << "Execution time:" << duration << endl;
	std::cout << "Iterations: " << cg.iterations() << std::endl;
	std::cout << "Estimation Error: " << cg.error() << std::endl;
	cout << endl;

	//BiCGSTAB method
	BiCGSTAB<SparseMatrix<double> > solver;
	t1 = high_resolution_clock::now();
	solver.compute(A);
	x = solver.solve(b);
	t2 = high_resolution_clock::now();
	duration = duration_cast<microseconds>(t2 - t1).count();

	cout << "BiCGSTAB method :" << endl;
	cout << "Execution time:" << duration << endl;
	std::cout << "#iterations:     " << solver.iterations() << std::endl;
	std::cout << "estimated error: " << solver.error() << std::endl;
	cout << endl;
	
	
	//sparseLU Method 
	SparseLU<SparseMatrix<double>>  lu;
	 // fill A and b;
		 // Compute the ordering permutation vector from the structural pattern of A
	 t1 = high_resolution_clock::now();
	 lu.compute(A);
	// Compute the numerical factorization 
	
	 //Use the factors to solve the linear system 
	 x = lu.solve(b);
	 t2 = high_resolution_clock::now();
	 duration = duration_cast<microseconds>(t2 - t1).count();
	 cout << "sparseLU Method " << endl;
	 cout << "Execution time:" << duration << endl;

	 system("pause");
	 return 0;
}
