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
	MatrixXd Temp1(subjects.rows() * subjects.rows(), subjects.cols());
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
	MatrixXd B = Ortho(B1, 0);
	MatrixXd D = X.array().square();
	MatrixXd D1 = D.colwise().sum();
	MatrixXd K = D1.cwiseSqrt().cwiseInverse();
	MatrixXd Ssem = subjects * B;
	MatrixXd Temp2(means.rows() * target.rows(), means.cols());
	for (int i = 0; i < target.rows(); i++)
	{
		Temp2.block(i * target.rows(), 0, means.rows(), means.cols()) = means;
	}
	target = target - Temp2;
	MatrixXd Final(start.size(), target.rows());
	Final.setZero();
	MatrixXd Est(target.rows(), target.cols());
	Est.setZero();

	MatrixXd Sem; Sem.setZero();
	MatrixXd TempTarget(1, target.cols()); TempTarget.setZero();
	for (int i = 0; i < target.rows(); ++i)
	{
		Sem = start.transpose() * B;
		TempTarget = target.row(i);
		Sem.block(0, 0, 1, target.cols()) = TempTarget.cwiseProduct(K);
		Final.col(i) = (Sem * B.inverse()).transpose();
		Est.row(i) = Estsem(Final.col(i), means, B, K);
	}
	return Final;
}
