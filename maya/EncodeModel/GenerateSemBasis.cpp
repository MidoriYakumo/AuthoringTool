#include "EncodeModel.h"
#include <Eigen/SVD>
using Eigen::JacobiSVD;

//orthogonalize matrix A starting from column start
MatrixXd Ortho(MatrixXd R, int start = 0) {
	int row = R.rows();
	int column = R.cols();
	MatrixXd Q(row, column);
	Q.setZero();
	for (int i = 0; i < row; ++i)
		for (int j = 0; j < start - 1; ++j)
		{
			Q(i, j) = R(i, j);
		}

	MatrixXd V(row, column - start + 1);
	V.setZero();
	//	vector<double> v(R.rows(), 0.0);

	for (int j = start - 1; j < column; ++j)
	{
		MatrixXd Vtemp(row, 1);
		MatrixXd Qtemp = Q.block(0, 0, row, j - 1);
		for (int i = 0; i < row; ++i)
		{
			V(i, j) = R(i, j);
			Vtemp(i, 1) = V(i, j);
		}
		Vtemp = Vtemp - Qtemp * (Qtemp.transpose() * Vtemp);
		Eigen::JacobiSVD<MatrixXd> svd(Vtemp, Eigen::ComputeThinU | Eigen::ComputeThinV);
		double len = svd.singularValues()[0];
		Vtemp = Vtemp / len;
		Q.block(0, start - 1, row, 1) = Vtemp;
	}
	R = Q;
	return R;
}

MatrixXd PInv(MatrixXd &PInvmat)
{
	Eigen::MatrixXd JT = PInvmat.transpose();
	Eigen::MatrixXd pseudo_inv_J = JT * (PInvmat * JT).inverse();

	//double  pinvtoler = 1.e-6; // choose your tolerance wisely!
	//Eigen::JacobiSVD<MatrixXd> svd(Vtemp, Eigen::ComputeThinU | Eigen::ComputeThinV);
	//double singularValues_inv = m_singularValues;
	//for (long i = 0; i<m_workMatrix.cols(); ++i) {
	//	if (m_singularValues(i) > pinvtoler)
	//		singularValues_inv(i) = 1.0 / m_singularValues(i);
	//	else singularValues_inv(i) = 0;
	//PInvmat = (m_matrixV*singularValues_inv.asDiagonal()*m_matrixU.transpose());
	return pseudo_inv_J;
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