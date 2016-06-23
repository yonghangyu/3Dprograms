#include"Parameterization.h"
#include <fstream>
ofstream debug("debug.txt");

void Param::ReadMesh(string filename) {// mesh initialization

	if (!OpenMesh::IO::read_mesh(mesh, filename))
	{
		std::cerr << "read error\n";
		exit(1);
	}
}

void Param::verticesNum() {
	
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++) {
		vertexNum++;
	}

}
void Param::EntryCalculation() {
	

	//create a list to store the adjacent vertices's iterator of the center vertex
	vector<MyMesh::VertexVertexIter> adjacentList;

	// the position of the element in the coefficient matrix
	int i;
	int j;
	// the  size of the adjacentlist
	int length;
	//the weight sum for all 
	double weight_sum;


	// loop for every vertex on the mesh
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++) {

		i = v_it->idx();// get the index of the current vertex handle

		adjacentList.clear();
		weight_sum = 0;






		Vector3d centerPoint, currentPoint, formerPoint, latterPoint;

		centerPoint = Vector3d(mesh.point(*v_it).data()[0], mesh.point(*v_it).data()[1], mesh.point(*v_it).data()[2]);


		if (mesh.is_boundary(*v_it)) {
			
			tripletList.push_back(T(i, i, 1));
		}

		else {

			// loop for one ring of current center vertex
			for (MyMesh::VertexVertexIter vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); vv_it++) {

				adjacentList.push_back(vv_it);

			}

			length = adjacentList.size();

			for (int k = 0; k < length; k++) {

				j = adjacentList[k]->idx();
				currentPoint = Vector3d(mesh.point(*adjacentList[k]).data()[0], mesh.point(*adjacentList[k]).data()[1], mesh.point(*adjacentList[k]).data()[2]);


				if (k == 0) {

					formerPoint = Vector3d(mesh.point(*adjacentList[length - 1]).data()[0], mesh.point(*adjacentList[length - 1]).data()[1], mesh.point(*adjacentList[length - 1]).data()[2]);
					latterPoint = Vector3d(mesh.point(*adjacentList[1]).data()[0], mesh.point(*adjacentList[1]).data()[1], mesh.point(*adjacentList[1]).data()[2]);

				}
				else {

					formerPoint = Vector3d(mesh.point(*adjacentList[k - 1]).data()[0], mesh.point(*adjacentList[k - 1]).data()[1], mesh.point(*adjacentList[k - 1]).data()[2]);
					latterPoint = Vector3d(mesh.point(*adjacentList[(k + 1) % length]).data()[0], mesh.point(*adjacentList[(k + 1) % length]).data()[1], mesh.point(*adjacentList[(k + 1) % length]).data()[2]);
				}
				Vector3d  v1 = centerPoint - formerPoint;
				Vector3d  v2 = currentPoint - formerPoint;

				Vector3d  v3 = centerPoint - latterPoint;
				Vector3d  v4 = currentPoint - latterPoint;
				// make sure that this function has error,basically algorithm is ok,so maybe the function i use is false
				// ,especially the parameter passed to the funcction so some times the things confusing me is not the algorithm 
				//,but the detail,so just practice and make a conclusion
				double cos_alpha = (v1.dot(v2)) / (v1.norm()*v2.norm());
				double cos_beta = v3.dot(v4) / (v3.norm()*v4.norm());

				double cot_alpha = cos_alpha / pow((1 - pow(cos_alpha, 2)), 1.0 / 2);// when get the square root ,must use float type  
				double cot_beta = cos_beta / pow((1 - pow(cos_beta, 2)), 1.0 / 2);//,otherwise it will get 0 power,or directly use the sqrt() function ,so in the future , try to use the obvious func 
				double weight = (cot_alpha + cot_beta) / 2;
				
				//m.insert(i, j) = weight;
				weight_sum += weight;

				tripletList.push_back(T(i, j, weight));

			}

			//weight_sum = -weight_sum;

			tripletList.push_back(T(i, i, -weight_sum));


		}
	}
}

		/*for (int i = 0; i < vertexNum;i++) {
			for (int j = 0; j < vertexNum;j++) {
				if (i != j) {
					m.coeffRef(i, i)+=-m.coeff(i,j); 
				}		
			}
			tripletList.push_back(T(i,i,m.coeff(i,i)));
			//m(i, i) = -m(i, i);
		}*/


	

	


void Param::boundaryCalculation() {
	
	
	
	boundaryU.resize(vertexNum);
	boundaryV.resize(vertexNum);

	boundaryU.setZero();
	boundaryV.setZero();
	//debug << boundaryU;

	
	
	MyMesh::HalfedgeIter start_h;
	

	for (MyMesh::HalfedgeIter h_it = mesh.halfedges_begin(); h_it != mesh.halfedges_end();h_it++) {
		
		if (mesh.is_boundary(*h_it)) {
			
			start_h = h_it;
			break;

		}
	}
	
	MyMesh::HalfedgeHandle heh, heh_init;
	heh_init = heh = *start_h;
	//heh=heh_init =mesh.halfedge_handle(vhandle) ;
	heh = mesh.next_halfedge_handle(heh);

	MyMesh::VertexHandle vh;
	vector<MyMesh::VertexHandle> boundaryList;
	vh = mesh.to_vertex_handle(heh_init);
	boundaryList.push_back(vh);

	while (heh!=heh_init) {

		vh = mesh.to_vertex_handle(heh);
		boundaryList.push_back(vh);
		heh = mesh.next_halfedge_handle(heh);

	}

	boundaryVertex = boundaryList.size(); //cout << boundaryVertex;

	

//	for (temp_half = start_half; temp_half != start_half;temp_half=mesh {}


	/*for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end();v_it++) {
		

		if (mesh.is_boundary(*v_it)) {

			boundaryVertex++;  
			boundaryList.push_back(v_it);

		}

		
	}*/

	int interval = boundaryVertex / 4;

	// 4 fixed points on the boundary in 2D region
	MyMesh::VertexHandle fix1 = boundaryList[0]; 
	MyMesh::VertexHandle fix2 = boundaryList[interval];
	MyMesh::VertexHandle fix3 = boundaryList[2*interval];
	MyMesh::VertexHandle fix4 = boundaryList[3*interval];
	
	//fix1
	boundaryU(fix1.idx()) = 0;
	boundaryV(fix1.idx()) = 0;
	//fix2
	//cout << fix2->idx();
	boundaryU(fix2.idx()) = 1;
	boundaryV(fix2.idx()) = 0;
	//fix3
	boundaryU(fix3.idx()) = 1;
	boundaryV(fix3.idx()) = 1;
	//fix4
	boundaryU(fix4.idx()) = 0;
	boundaryV(fix4.idx()) = 1;

	// calculate the length for each boundary vertex and calculate each actual interval length on the boundary
	double length_1, length_2, length_3, length_4;
	vector<double> lengthList;
	double lengthSum = 0; 
	int j;
	double ij_length;

	for (int i = 0; i < boundaryVertex;i++) {
		lengthList.push_back(lengthSum);

		if (i==boundaryVertex-1) {
			j = 0;
		}
		else {
			j = i + 1;
		}

		Vector3d point_i = Vector3d(mesh.point(boundaryList[i]).data()[0], mesh.point(boundaryList[i]).data()[1], mesh.point(boundaryList[i]).data()[2]);
		Vector3d point_j = Vector3d(mesh.point(boundaryList[j]).data()[0], mesh.point(boundaryList[j]).data()[1], mesh.point(boundaryList[j]).data()[2]);
		
		ij_length = (point_j - point_i).norm();
		if (i==interval) {
			length_1 = lengthSum;
			lengthSum = 0;
		}
		else if (i==2*interval) {
			length_2 = lengthSum;
			lengthSum = 0;
		}
		else if (i == 3*interval) {
			length_3 = lengthSum;
			lengthSum = 0;
		}
		
		lengthSum += ij_length;	
	}

	length_4 = lengthSum;
	
	for (int i = 0; i < boundaryVertex;i++) {
		if (i<interval+1) {
			lengthList[i] /= length_1;
		}
		else if (i>interval&&i<2*interval+1) {
			lengthList[i] /= length_2;
		}
		else if(i>2*interval&&i<3 * interval + 1){
			lengthList[i] /= length_3;
		}
		else{
			lengthList[i] /=length_4;
		}
	}

	double ratio;
	for (int i = 0; i < boundaryVertex;i++ ) {
		
		ratio = lengthList[i];
		//cout << ratio << endl;

		int index = boundaryList[i].idx();
		
		if (i<interval&&i>0) {
			
			boundaryU(index) = ratio;
			boundaryV(index) = 0;
		}
		else if (i>interval&&i<(2*interval)) {
		
			boundaryV(index) = ratio;
			boundaryU(index)= 1;
		}
		else if (i>(2*interval)&&i<(3 * interval) ) {
			
			boundaryU(index) = 1 - ratio;
			boundaryV(index)= 1;
		}
		else if (i>(3*interval)) {
			
			boundaryV(index) = 1 - ratio;
			boundaryU(index)= 0;
		}

	}
	for (int i = 0; i < boundaryVertex; i++) {

		cout << boundaryU(boundaryList[i].idx()) << endl;;
		

	}
	
	//cout << boundaryU;

}

void Param::uvCalculation() {
	
	SparseLU<SparseMatrix<double>> solver;
	//debug << m;
	SparseMatrix<double> mat(vertexNum, vertexNum);
	mat.setFromTriplets(tripletList.begin(), tripletList.end());
	/*for (int i = 0; i < tripletList.size();i++) {
		debug << tripletList[i].col()<<" "<< tripletList[i].row()<<" "<< tripletList[i].value();
		debug << endl;
	}*/
	
	//cout << mat.coeff(6756, 0) <<" "<< mat.coeff(0, 6756);
	solver.analyzePattern(mat);
	solver.factorize(mat);
	//debug << mat;
	//u.resize(vertexNum);
	u = solver.solve(boundaryU);
	
	
	v.resize(vertexNum);
	v = solver.solve(boundaryV);
	/*for (int i = 0; i < vertexNum;i++) {

		if (v(i)>1||u(i)>1) { cout << v(i)<<endl; }
		
	}*/
	/*for (int i = 0; i < vertexNum;i++) {
		if (boundaryU(i) == 1)
			cout << 1;
	}*/

	//debug << v;
	//cout << "u vector is" << u << endl;
	//debug << u;

	//debug << v;

}
void Param::outPut2DMesh() {

	MyMesh::VertexHandle* vhandle=new MyMesh::VertexHandle[vertexNum];
	vector<MyMesh::VertexHandle> face_vhandles;
	for (int i = 0; i < vertexNum;i++) {
		vhandle[i] = Mesh_2D.add_vertex(MyMesh::Point(u(i),v(i),0));
	}
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end();f_it++) {
		face_vhandles.clear();
		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid();fv_it++) {
			face_vhandles.push_back(vhandle[fv_it->idx()]);
		}
		Mesh_2D.add_face(face_vhandles);
	}

	try
	{
		if (!OpenMesh::IO::write_mesh(Mesh_2D, "output.off"))
		{
			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
			
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		
	}

	
}





Param::Param() {

	vertexNum = 0;
	boundaryVertex = 0;

	
}