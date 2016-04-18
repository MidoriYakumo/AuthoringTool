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

#include <Eigen/Sparse>
using Eigen::SparseMatrix;

//---input: row vector of input model in RR represention---
//---neigh: indices of neighboring faces---
//---return: row vector of input model in translation invariant encoding---
MatrixXd DecodeRelativeRotation( MatrixXd &input, MatrixXi &neigh ){

	//if( input.cols() == 1 ){
	//	input.transposeInPlace();
	//}

	int step( 15 );
	int N( neigh.rows() );
	Vector3d direction = Vector3d::Zero();
	//int landmarks( 1 );
	//MatrixXd weight( 1, 1 ); weight << 1;
	MatrixXd features = MatrixXd::Zero( 1, 10 * N );
	SparseMatrix< double > A( ( N * 9 >> 1 ) + 3, 3 * N );
	int row( 0 );

	for( int i = 0; i < N; ++i ){
		
		int n( neigh( i, 0 ) );

		if( n > i ){

			Matrix3d delta = FromRotVec( input.block< 1, 3 >( 0, i * step ) );
			Matrix3d tmp1 = Matrix3d::Identity() * -1.;
			Matrix3d tmp2 = delta.transpose();

			MySM3Block( A, row, n * 3, tmp1 );
			MySM3Block( A, row, i * 3, tmp2 );
			row += 3;
		}

		n = neigh( i, 1 );

		if( n > i ){

			Matrix3d delta = FromRotVec( input.block< 1, 3 >( 0, i * step + 3 ) );
			Matrix3d tmp1 = Matrix3d::Identity() * -1.;
			Matrix3d tmp2 = delta.transpose();

			MySM3Block( A, row, n * 3, tmp1 );
			MySM3Block( A, row, i * 3, tmp2 );
			row += 3;
		}

		n = neigh( i, 2 );

		if( n > i ){

			Matrix3d delta = FromRotVec( input.block< 1, 3 >( 0, i * step + 6 ) );
			Matrix3d tmp1 = Matrix3d::Identity() * -1.;
			Matrix3d tmp2 = delta.transpose();

			MySM3Block( A, row, n * 3, tmp1 );
			MySM3Block( A, row, i * 3, tmp2 );
			row += 3;
		}

		features.block< 1, 6 >( 0, i * 10 + 4 ) = input.block< 1, 6 >( 0, i * step + 9 );
	}

	MatrixXd rhs = MatrixXd::Zero( A.rows(), 3 );
	Matrix3d eye = Matrix3d::Identity();

	MySM3Block( A, row, 0, eye );
	rhs.block< 3, 3 >( row, 0 ) = FromRotVec( direction ).transpose();
	row += 3;

	cout << "Solving." << endl;
	SparseMatrix< double > At( A.transpose() );
	auto AtA( At * A );
	SimplicialLDLT< SparseMatrix< double > > solver;
	MatrixXd Rs;

	solver.compute( AtA );
	Rs = solver.solve( At * rhs );


	for( int i = 0; i < N; ++i ){

		MatrixXd tmp = Rs.block< 3, 3 >( i * 3, 0 );
		JacobiSVD< MatrixXd > svd( tmp, Eigen::ComputeThinU | Eigen::ComputeThinV );
		MatrixXd U = svd.matrixU();
		MatrixXd V = svd.matrixV();
		MatrixXd x = U * V.transpose();

		features.block< 1, 3 >( 0, i * 10 ) = ToRotVec( x );
	}

	return features;
}
