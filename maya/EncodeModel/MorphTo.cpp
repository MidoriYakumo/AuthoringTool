#include "EncodeModel.h"

MatrixXd Estsem(MatrixXd obj, MatrixXd means, MatrixXd B, MatrixXd K)
{
	MatrixXd sem = obj.transpose() * B;
	MatrixXd part_sem = sem.block(0, 0, 1, means.cols());
	MatrixXd meaning = part_sem.cwiseQuotient(K) + means;
	return meaning;
}

MatrixXd MorphTo(MatrixXd start, MatrixXd target, MatrixXd subjects, MatrixXd semvals) 
{
	//---gensembasis---
	subjects.transposeInPlace();

	MatrixXd means(1, semvals.cols());
	for (int i = 0; i < semvals.cols(); ++i)
	{
		double sub_sum = 0;
		for (int j = 0; j < semvals.rows(); ++j)
		{
			sub_sum += semvals(j, i);
		}
		means(0, i) = sub_sum / semvals.rows();
	}
	MatrixXd Temp1(semvals.rows()/* * subjects.rows()*/, means.cols());
	for (int i = 0; i < semvals.rows(); i++)
	{
		//Temp1.block<subjects.rows(), subjects.cols()>( i * subjects.rows() , 0) = subjects;
		//Temp1.block(i * subjects.rows(), 0, subjects.rows(), subjects.cols()) = subjects;
		Temp1.row( i ) = means;
	}
	semvals = semvals - Temp1;

	MatrixXd X = PInv(subjects) * semvals;
	MatrixXd B1(subjects.rows(), subjects.cols());
	B1.block(0, 0, subjects.rows(), X.cols()) = X;
	B1.block(0, X.cols(), subjects.rows(), subjects.cols() - semvals.cols()) = MatrixXd::Identity(subjects.cols(), subjects.cols() - semvals.cols());
	MatrixXd B = Ortho(B1, 0);
	MatrixXd D = X.array().square();
	MatrixXd D1 = D.colwise().sum();
	MatrixXd K = D1.cwiseSqrt().cwiseInverse();
	//MatrixXd Ssem = subjects * B;
	//---end gensembasis---

	MatrixXd Temp2(means.rows() * target.rows(), means.cols());

	for (int i = 0; i < target.rows(); i++)
	{
		Temp2.block(i * means.rows(), 0, means.rows(), means.cols()) = means;
	}
	target = target - Temp2;

	MatrixXd Final(start.rows(), target.rows());
	//MatrixXd Est(target.rows(), target.cols());

	Final.setZero();
	//Est.setZero();

	MatrixXd Sem;
	MatrixXd TempTarget;
	MatrixXd BInv = B.inverse();
	
	for (int i = 0; i < target.rows(); ++i)
	{
		Sem = start.transpose() * B;
		TempTarget = target.row(i);
		Sem.block(0, 0, 1, target.cols()) = TempTarget.cwiseProduct(K);
		Final.col(i) = (Sem * BInv).transpose();
		//Est.row(i) = Estsem(Final.col(i), means, B, K);
	}

	return Final;
}
