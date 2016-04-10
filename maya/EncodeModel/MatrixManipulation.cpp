// conversion between rotation matrix and rotation vector representation
#include "EncodeModel.h"

/*
function r = torotvec(R)

RRt = R - R';
r = [RRt(2,3);RRt(3,1);RRt(1,2)];
len = norm(r);
if len == 0
return
end
r = r * (atan2(len, R(1,1)+R(2,2)+R(3,3)-1) / len);
*/

Vector3d ToRotVec(Matrix3d R)
{
	Matrix3d RRt = R - R.transpose();
	Vector3d r = { RRt( 1, 2 ), RRt( 2, 0 ), RRt( 0, 1 ) };
	double len = r.norm();

	if ( len == 0 ){
		return Vector3d::Zero();
	}

	r = r * ( atan2( len, R( 0, 0 ) + R( 1, 1 ) + R( 2, 2 ) - 1 ) / len );

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

Matrix3d FromRotVec( Vector3d r )
{
	Vector3d A = { 1., 1., 1. };
	Matrix3d R = Matrix3d::Identity();

	if ( r != A ){

		double theta = r.norm();

		r = -r / theta;
		R << 0, -r( 2 ), r( 1 ), r( 2 ), 0, -r( 0 ), -r( 1 ), r( 0 ), 0;
		R = R * sin( theta ) + ( R * R ) * ( 1 - cos( theta ) ) + Matrix3d::Identity();
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
/*
MatrixXd Ortho( MatrixXd R, int start = 0 ){

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
*/
	 
void MySM3Block( SparseMatrix< double > &sm, int row, int col, Matrix3d &dm3 ){

	vector< Triplet< double > > vtd;

	for( int i = 0; i < 3; ++i ){
		for( int j = 0; j < 3; ++j ){
			if( abs( dm3( i, j ) ) < 1e-6 ) continue;
			vtd.push_back( Triplet< double >( row + i, col + j, dm3( i, j ) ) );
		}
	}

	sm.setFromTriplets( vtd.begin(), vtd.end() );
}