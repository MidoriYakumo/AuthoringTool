#include "EncodeModel.h"
#include <Eigen/SVD>
using Eigen::JacobiSVD;

//orthogonalize matrix A starting from column start
MatrixXd Ortho(MatrixXd R, int start) {

	int row = R.rows();
	int column = R.cols();
	MatrixXd Q = MatrixXd::Zero(row, column);

	for (int j = 0; j < start - 1; ++j)
	{
		for (int i = 0; i < row; ++i)
		{
			Q(i, j) = R(i, j);
		}
	}

	//MatrixXd V(row, column - start + 1);
	//V.setZero();
	//	vector<double> v(R.rows(), 0.0);

	for (int j = start; j < column; ++j)
	{

		MatrixXd Vtemp(row, 1);
		MatrixXd Vtemp2( row, 1 );

		for (int i = 0; i < row; ++i)
		{
			//V(i, j) = R(i, j);
			Vtemp(i, 0) = R(i, j);
		}

		if( j > 1 ){

			MatrixXd Qtemp = Q.block(0, 0, row, j - 1);

			Vtemp2 = Vtemp - Qtemp * (Qtemp.transpose() * Vtemp);
		}else{
			Vtemp2 = Vtemp;
		}
		
		//Eigen::JacobiSVD<MatrixXd> svd(Vtemp, Eigen::ComputeThinU | Eigen::ComputeThinV);
		//double len = svd.singularValues()[0];
		double len = Vtemp2.norm();

		Vtemp = Vtemp2 / len;
		Q.block(0, j, row, 1) = Vtemp;
	}
	
	return Q;
}

MatrixXd PInv(MatrixXd &m)
{
	//Eigen::MatrixXd JT = PInvmat.transpose();
	//Eigen::MatrixXd pseudo_inv_J = JT * (PInvmat * JT).inverse();

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

/*
MatrixXd GenSemBasis(MatrixXd subjects, MatrixXd semvals)
{
	subjects = subjects.transpose();
	MatrixXd means(1, subjects.cols());
	int sub_sum = 0;
	for (int i = 0; i < subjects.cols(); ++i)
	 {
		 for (int j = 0; j < subjects.rows(); ++j)
		 {
			 sub_sum += subjects(j, i);
		 }
		 means(1, i) = sub_sum / subjects.rows();
	 }
	MatrixXd Temp1(subjects.rows() * subjects.rows() , subjects.cols());
	for (int i = 0; i < subjects.rows(); i++)
	{
	//Temp1.block<subjects.rows(), subjects.cols()>( i * subjects.rows() , 0) = subjects;
		Temp1.block(i * subjects.rows(), 0, subjects.rows(), subjects.cols()) = subjects;
	}
	semvals = semvals - Temp1;
	MatrixXd X = PInv(subjects) * semvals;
	MatrixXd B1(subjects.rows(), subjects.cols());
	B1.block(0, 0, subjects.rows(), 2) = X;
	B1.block(0, 2, subjects.rows(), subjects.cols() - semvals.cols()) = MatrixXd::Identity(subjects.cols(), subjects.cols() - semvals.cols());
	MatrixXd B = Ortho(B1);	
	MatrixXd D = X.array().square();
	MatrixXd D1 = D.colwise().sum();
	MatrixXd K = D1.cwiseSqrt().cwiseInverse();
	MatrixXd Ssem = subjects * B;
} */