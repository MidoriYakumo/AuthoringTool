/*
%
% [features, A] = decdeltarot(input, neigh, [direction], [landmarks], [weight], [replacefaces], [replacewith], [usemex])
%
% convert model in relative rotation encoding to one that uses absolute rotations
%
% input         input model in relative rotation encoding - generated, e.g., with fullenc
% neigh         (k x 3) for every triangle the indices of its neighbours
% direction     global rotations that are set (default [0;0;0])
% landmarks     triangle indices that are constrained (default 1)
% weight        weights for constrained triangles (default 1)
% replacefaces  indices of faces that are replaced before decoding (default [])
% replacewith   replace the faces with this data
% usemex        use mex file for decoding (default 0)
%
% e.g. fully decode model given by PCA coefficients and show it
%
% dec = decdeltarot(coeffs * projected(:,1) + avg, neigh);
% r = reconstructmodel(dec, faces, reconmean);
% showmodel(r, faces, 'b');
%

*/

#include "EncodeModel.h"

//---input: row vector of input model in RR represention---
//---neigh: indices of neighboring faces---
//---return: row vector of input model in translation invariant encoding---
MatrixXd DecodeRelativeRotation( MatrixXd input, MatrixXi neigh ){

	if( input.cols() == 1 ){
		input.transposeInPlace();
	}

	int step( 15 );
	int N( neigh.rows() );
	Vector3d direction = Vector3d::Zero();
	int landmarks( 1 );//
	MatrixXd weight( 1, 1 ); weight << 1;
	MatrixXd features = MatrixXd::Zero( 1, 10 * N );
	MatrixXd A( 4 * N, 3 * N );
	int row( 0 );

	for( int i = 0; i < N; ++i ){
		
		int n( neigh( i, 0 ) );

		if( n > i ){

			Matrix3d delta = FromRotVec( input.block< 1, 3 >( 0, i * step ) );

			A.block< 3, 3 >( row, n * 3 ) = Matrix3d::Ones() * -1.0;
			A.block< 3, 3 >( row, i * 3 ) = delta.transpose();
			row += 3;
		}

		n = neigh( i, 1 );

		if( n > i ){

			Matrix3d delta = FromRotVec( input.block< 1, 3 >( 0, i * step + 3 ) );

			A.block< 3, 3 >( row, n * 3 ) = Matrix3d::Ones() * -1.0;
			A.block< 3, 3 >( row, i * 3 ) = delta.transpose();
			row += 3;
		}

		n = neigh( i, 2 );

		if( n > i ){

			Matrix3d delta = FromRotVec( input.block< 1, 3 >( 0, i * step + 6 ) );

			A.block< 3, 3 >( row, n * 3 ) = Matrix3d::Ones() * -1.0;
			A.block< 3, 3 >( row, i * 3 ) = delta.transpose();
			row += 3;
		}

		features.block< 1, 6 >( 0, i * 10 + 4 ) = input.block< 1, 6 >( 0, i * step + 9 );
	}

	MatrixXd rhs = MatrixXd::Zero( A.rows(), 3 );

	A.block< 3, 3 >( row, 0 ) = Matrix3d::Identity();
	rhs.block< 3, 3 >( row, 0 ) = FromRotVec( direction ).transpose();
	row += 3;

	MatrixXd At = A.transpose();
	MatrixXd Rs = ( At * A ).inverse() * ( At * rhs );

	for( int i = 0; i < N; ++i ){
		
		Eigen::JacobiSVD< MatrixXd > svd( Rs, Eigen::ComputeThinU | Eigen::ComputeThinV );
		MatrixXd U = svd.matrixU();
		MatrixXd V = svd.matrixV();
		MatrixXd x = U * V.transpose();

		features.block< 1, 3 >( 0, i * 10 ) = ToRotVec( x );
	}

	return features;
}
