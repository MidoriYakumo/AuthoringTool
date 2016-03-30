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

#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::MatrixXi;

#include <Eigen/Sparse>
using Eigen::SparseMatrix;

void DecodeRelativeRotation( MatrixXd input, MatrixXi &neighbors ){

	if( input.cols() == 1 ){
		input.transposeInPlace();
	}

	int step( 15 );
	int N( neighbors.rows() );
	MatrixXd direction = MatrixXd::Zero( 3, 1 );
	int landmarks( 1 );
	MatrixXd weight = MatrixXd::Ones( landmarks );
	MatrixXd features = MatrixXd::Zero( 1, 10 * N );
	SparseMatrix< double > A( 4 * N, 3 * N );
	int row( 0 );

	for( int i = 0; i < N; ++i ){
		
		int n( neighbors( i, 1 ) );

		if( n > i ){

			MatrixXd delta = FromRotVec( input.block( 0, i * step, 1, 3 ).transpose() );

			A.block( row, ( n - 1 ) * 3, 3, 3 ) = MatrixXd::Ones( 3 ) * -1.0;
			A.block( row, i * 3, 3, 3 ) = delta.transpose();
			row += 3;
		}

		n = neighbors( i, 2 );

		if( n > i ){

			MatrixXd delta = FromRotVec( input.block( 0, i * step + 3, 1, 3 ).transpose() );

			A.block( row, ( n - 1 ) * 3, 3, 3 ) = MatrixXd::Ones( 3 ) * -1.0;
			A.block( row, i * 3, 3, 3 ) = delta.transpose();
			row += 3;
		}

		n = neighbors( i, 3 );

		if( n > i ){

			MatrixXd delta = FromRotVec( input.block( 0, i * step + 6, 1, 3 ).transpose() );

			A.block( row, ( n - 1 ) * 3 + 1, 3, 3 ) = MatrixXd::Ones( 3 ) * -1.0;
			A.block( row, i * 3, 3, 3 ) = delta.transpose();
			row += 3;
		}

		features.block( 0, i * 10 + 4, 1, 5 ) = input.block( 0, i * step + 9, 1, 5 );
	}

	MatrixXd rhs = MatrixXd::Zero( A.rows(), 3 );

	A.block( row, 0, 3, 3 ) = MatrixXd::Identity( 3, 3 );
	rhs.block( row, 0, 3, 3 ) = FromRotVec( direction.transpose() ).transpose();
	row += 3;

	MatrixXd At = A.transpose();
	MatrixXd Rs = ( At * A ).transpose() * ( At * rhs );

	for( int i = 0; i < N; ++i ){
		
		MatrixXd Rt = svd( )
	}
}
