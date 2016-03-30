#include "EncodeSimple.h"

#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <Eigen/Geometry> 
#include <vector>

using std::vector;

MatrixXd EncodeRelativeRotation(MatrixXd model, MatrixXd faces, MatrixXd temp, MatrixXd neigh)
{
	MatrixXd m = model;
	int step = 15;
	MatrixXd result(1, faces.rows() * step); //get the row number of the faces matrix
	result.setZero();
	MatrixXd N(3, 1);
	MatrixXd Tn(3, 1);
	N.setZero();
	Tn.setZero();
	vector<int> Rs1(faces.rows());
	vector<int> V(faces.cols());
	int at = 1;
	for (int i = 0; i < faces.rows(); ++i)
	{
		V[0] = faces(i, 0);
		V[1] = faces(i, 1);
		V[2] = faces(i, 2);
		MatrixXd Temp1 = temp.block(V[0], 0, 1, temp.cols());//<1, temp.cols()>(temp(V[0], 0));
		MatrixXd Origin = Temp1.transpose();
		MatrixXd Temp3 = temp.block(V[1], 0, 1, temp.cols());
		MatrixXd E1 = Temp3.transpose() - Origin;
		MatrixXd Temp5 = temp.block(V[2], 0, 1, temp.cols());
		MatrixXd E2 = Temp5.transpose() - Origin;
	}


}