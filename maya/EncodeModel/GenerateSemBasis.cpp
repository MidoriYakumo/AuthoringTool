#include "EncodeModel.h"
#include <Eigen/SVD>
using Eigen::JacobiSVD;

//orthogonalize matrix A starting from column start
MatrixXd Ortho(MatrixXd R, int start) {

	int row = ( int )R.rows();
	int column = ( int )R.cols();
	MatrixXd Q = MatrixXd::Zero(row, column);

	for (int j = 0; j < start; ++j)
	{
		for (int i = 0; i < row; ++i)
		{
			Q(i, j) = R(i, j);
		}
	}
	
	for (int j = start; j < column; ++j)
	{

		MatrixXd Vtemp(row, 1);
		MatrixXd Vtemp2( row, 1 );

		for (int i = 0; i < row; ++i)
		{
			Vtemp(i, 0) = R(i, j);
		}

		if( j > 0 ){

			MatrixXd Qtemp = Q.block(0, 0, row, j);

			Vtemp2 = Vtemp - Qtemp * (Qtemp.transpose() * Vtemp);
		}else{
			Vtemp2 = Vtemp;
		}
		
		double len = Vtemp2.norm();

		Vtemp = Vtemp2 / len;
		Q.block(0, j, row, 1) = Vtemp;
	}
	
	return Q;
}

MatrixXd PInv(MatrixXd &m)
{
	double pinvtoler = 1.e-6; // choose your tolerance wisely!
	Eigen::JacobiSVD<MatrixXd> svd(m, Eigen::ComputeFullU | Eigen::ComputeFullV);
	MatrixXd singularValues = svd.singularValues();
	MatrixXd singularValues_inv = singularValues;
	for (int i = 0; i<m.cols(); ++i) {
		if (singularValues(i) > pinvtoler){
			singularValues_inv(i) = 1.0 / singularValues(i);
		}else{
			singularValues_inv(i) = 0;
		}
	}
	return svd.matrixV()*singularValues_inv.asDiagonal()*svd.matrixU().transpose();
}