// convert rotation matrix into rotation vector representation
#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <Eigen/Geometry> 
#include <vector>

using Eigen::MatrixXd;
using std::vector;

class MatrixManipulation
{
public:
	MatrixXd ToRotVec(const MatrixXd &R);
	MatrixXd FromRotVec(MatrixXd &r);
	MatrixXd Ortho(MatrixXd &R, int start = 1);
	MatrixXd Morphto(MatrixXd start, int target);

};

/*
r = torotvec(R)
convert rotation matrix into rotation vector representation

RRt = R - R';
r = [RRt(2,3);RRt(3,1);RRt(1,2)];
len = norm(r);
if len == 0
return
end
r = r * (atan2(len, R(1,1)+R(2,2)+R(3,3)-1) / len);
*/

MatrixXd MatrixManipulation::ToRotVec(const MatrixXd &R)
{
	MatrixXd Rt = R.transpose();
	MatrixXd RRt = R - Rt;
	MatrixXd r(3, 3);
	r.setZero();
	r(0, 0) = RRt(1, 2);
	r(1, 0) = RRt(2, 0);
	r(2, 0) = RRt(0, 1);
	Eigen::JacobiSVD<MatrixXd> svd(r, Eigen::ComputeThinU | Eigen::ComputeThinV);
	double len = svd.singularValues()[0];
	if (len == 0)  return Eigen::Matrix3d::Identity(); //yi cuo dian 1
	r = r * (std::atan2(len,R(0,0)+R(1,1)+R(2,2)-1));
	return r;
}

/*
R = fromrotvec(r)
convert rotation vector into rotation matrix
function R = fromrotvec(r)

if r == zeros(3,1)
R = eye(3);
else
theta = norm(r);
r = -r / theta;

R = [ 0    -r(3)  r(2); ...
r(3)  0    -r(1); ...
-r(2)  r(1)  0  ];
R = R * sin(theta) + (R * R) * (1 - cos(theta)) + eye(3);
end
*/
MatrixXd MatrixManipulation::FromRotVec(MatrixXd &r)
{
	MatrixXd A(3, 1);
	MatrixXd R(3, 3);
	R.setIdentity();
	A(0, 0) = 1; A(1, 0) = 1; A(2, 0) = 1;
	if (r == A) { return R.setIdentity();}
	else 
	{
		Eigen::JacobiSVD<MatrixXd> svd(r, Eigen::ComputeThinU | Eigen::ComputeThinV);
		double theta = svd.singularValues()[0];
		r = -r / theta;
		R(0, 0) = 0; R(0, 1) = -r(2, 0); R(0, 2) = r(1, 0);
		R(1, 0) = r(2, 0); R(1, 1) = 0; R(1, 2) = -r(0, 0);
		R(2, 0) = - r(1, 0); R(2, 1) = r(0, 0); R(2, 2) = 0;
		R = R * std::sin(theta) + (R * R) * (1 - std::cos(theta)) + Eigen::Matrix3d::Identity();
	}
	return R;
}

/*
% A = ortho(A)
% orthogonalize matrix A starting from column start
function A = ortho(A, start)

if ~exist('start','var')
start = 1;
end
n = size(A,2);
Q=zeros(size(A));
%R=zeros(n,n);
Q(:,1:start-1) = A(:,1:start-1);

for j = start:n
v = A(:,j);
%    x = Q(:,1:j-1)' * v;
v = v - Q(:,1:j-1) * (Q(:,1:j-1)' * v);
%    for i = 1:j-1
%        v = v-Q(:,i)'*v*Q(:,i);
%    end
%    Q(:,j) = v / R(j,j);
Q(:,j) = v / norm(v);
end
A = Q;
*/
MatrixXd MatrixManipulation::Ortho(MatrixXd &R, int start)
{
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
		MatrixXd Qtemp = Q.block(0, 0, row, j-1);
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

/*
%
% final = morphto(start, target, subjects, semvals)
%
% start vector, semantic target values, all subjects, and semantic values
%
% generates a semantic basis and morphs the start vector to the given
% target values.If target is a matrix several output poses are generated.
% One target per column.
%
% e.g.morph first scan to 60kg
%
% x = morphto(projected(:, 1), 60, projected, semvals(:, 6));
%
% Copyright 2009 (c)Nils Hasler hasler@mpi-inf.mpg.de
% please cite
% N.Hasler, C.Stoll, M.Sunkel, B.Rosenhahn, H. - P.Seidel: A Statistical Model of Human Pose and Body Shape,
% Computer Graphics Forum(Proc.Eurographics 2009), Munich, Germany, March 2009.
function[final, sem, means, B, K, est, Ssem] = morphto(start, target, subjects, semvals)

[B, Ssem, means, K] = gensembasis(subjects, semvals);

target = target - repmat(means, size(target, 1), 1);

%K = Ssem(:, 1 : size(target, 2)) \ (semvals - repmat(means, size(semvals, 1), 1));

final = zeros(length(start), size(target, 1));
est = zeros(size(target));

for i = 1:size(target, 1)
% transform to semantic representation
sem = start' * B;
% morph to semantic target values
sem(1:size(target, 2)) = target(i, :).*K;
% and transform back
final(:, i) = (sem / B)';

% estimate semantic values to check the error
est(i, :) = estsem(final(:, i), means, B, K);
end
*/
MatrixXd MatrixManipulation::Morphto(MatrixXd start, int target)
{

}
	 
 
