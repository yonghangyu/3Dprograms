#include"LoadMatrixVector.h"

vector<double> LoadMatrixVector::convert(string line) {
	
	int length = line.length();
	int end = length - 1;
	double  i, j;
	double v_ij;
	int count = 0;
	int index[2];
	string temp;
	for (int k = 0; k < length; k++) {
		if (line.at(k) != ' ') continue;

		else if (line.at(k) == ' '&&count == 0) {
			temp = line.substr(0, k);//why k  instead of k-1?
			i = atof(temp.c_str());
			count++;
			index[0] = k + 1;
		}
		else if (line.at(k) == ' '&&count == 1) {
			temp = line.substr(index[0], k - 1);
			j = atof(temp.c_str());
			index[1] = k + 1;
			break;
		}

	}
	temp = line.substr(index[1], end);
	v_ij = atof(temp.c_str());
	vector<double> ele;
	ele.push_back(i);
	ele.push_back(j);
	ele.push_back(v_ij);
	return ele;
}
void LoadMatrixVector::FillMatrix(SparseMatrix<double> &A,string fileMatrix) {
	//read file 

	
	ifstream myfile(fileMatrix);

	vector<T> tripletList;
	lineNum = 0;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			vector<double> temp = convert(line);
			if (lineNum == 0) {

				rows = temp[0];
				cols = temp[1];

			}
			else {
				tripletList.push_back(T(temp[0] - 1, temp[1] - 1, temp[2]));// file matrix number is starting from 1 to n
			}
			lineNum++;
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	A.resize(rows, cols);
	A.setFromTriplets(tripletList.begin(), tripletList.end());

}
void LoadMatrixVector::FillVector(VectorXd &b,string fileVector) {

	ifstream myfile_2(fileVector);
	lineNum = 0;
	if (myfile_2.is_open()) {
		while (getline(myfile_2, line)) {

			b(lineNum) = atof(line.c_str());
			lineNum++;

		}
		myfile_2.close();
	}
	else cout << "Unable to open file";

}
