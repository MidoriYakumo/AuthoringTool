#include "EncodeSimple.h"

#include "EncodeModel.h"

#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <Eigen/Geometry> 
#include <vector>
#include <Eigen/SVD>
 

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
	vector<Eigen::Matrix3d> Rs(faces.rows());
	vector<int> V(faces.cols());
	int at = 0;
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
		N(0, 0) = E1(1, 0) * E2(2, 0) - E1(2, 0) * E2(1, 0);
		N(1, 0) = E1(2, 0) * E2(0, 0) - E1(0, 0) * E2(2, 0);
		N(2, 0) = E1(0, 0) * E2(1, 0) - E1(1, 0) * E2(0, 0);
		N = N / N.norm();
		MatrixXd Src(3, 3);
		Src(0, 0) = E1(0, 0); Src(0, 1) = E2(0, 0); Src(0, 2) = N(0, 0);
		Src(1, 0) = E1(1, 0); Src(1, 1) = E2(1, 0); Src(1, 2) = N(1, 0);
		Src(2, 0) = E1(2, 0); Src(2, 1) = E2(2, 0); Src(2, 2) = N(2, 0);

		MatrixXd T0 = m.block(V[0], 0, 1, m.cols()).transpose();
		MatrixXd T1 = m.block(V[1], 0, 1, m.cols()).transpose() - T0;
		MatrixXd T2 = m.block(V[2], 0, 1, m.cols()).transpose() - T0;
		Tn(0, 0) = T1(1, 0) * T2(2, 0) - T1(2, 0) * T2(1, 0);
		Tn(1, 0) = T1(2, 0) * T2(0, 0) - T1(0, 0) * T2(2, 0);
		Tn(2, 0) = T1(0, 0) * T2(1, 0) - T1(1, 0) * T2(0, 0);
		Tn = Tn / Tn.norm();
		MatrixXd Tgt(3, 3);
		Tgt(0, 0) = T1(0, 0); Tgt(0, 1) = T2(0, 0); Tgt(0, 2) = Tn(0, 0);
		Tgt(1, 0) = T1(1, 0); Tgt(1, 1) = T2(1, 0); Tgt(1, 2) = Tn(1, 0);
		Tgt(2, 0) = T1(2, 0); Tgt(2, 1) = T2(2, 0); Tgt(2, 2) = Tn(2, 0);
		MatrixXd T = Tgt * Src.inverse();
		MatrixXd R = T;

		Eigen::JacobiSVD<MatrixXd> svd(R, Eigen::ComputeThinU | Eigen::ComputeThinV);
		MatrixXd U1 = svd.matrixU();
		MatrixXd V1 = svd.matrixV();
		int length = svd.singularValues().rows();
		MatrixXd S1(length, length);
		for (int i = 0; i < length; i++)
		{
			S1(i, i) = svd.singularValues()(i,0);
		}
		R = U1 * V1.transpose();
		if (R.determinant() < 0) { R = R * -1; }
		Rs[i] = R;
		S1 = R.inverse() * T;
		at = at + 9;

		result(0, at) = S1(0, 0);
		result(0, at + 1) = S1(0, 1);
		result(0, at + 2) = S1(0, 2);
		result(0, at + 3) = S1(1, 1);
		result(0, at + 4) = S1(1, 2);
		result(0, at + 5) = S1(2, 2);

		at = at + 6;
	}

	for (int i = 0; i < faces.rows(); i++)
	{
		MatrixXd delta = Rs[i].inverse() * Rs[neigh(i, 0)];
		result(i * step) = ToRotVec(delta)(0);
		delta = Rs[i].inverse() * Rs[neigh(i, 1)];
		result(i * step + 3) = ToRotVec(delta);
		delta = Rs[i].inverse() * Rs[neigh(i, 2)];
		result(i * step + 6) = ToRotVec(delta);
	}

}