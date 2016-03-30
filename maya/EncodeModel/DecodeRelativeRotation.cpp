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

void DecodeRelativeRotation( MatrixXd input, MatrixXi &neighbors ){

	if( input.cols() == 1 ){
		input.transposeInPlace();
	}

	int step( 15 );
	int N( neighbors.rows() );
	MatrixXd direction( 1, 3 ); direction.setZero();
	int landmarks( 1 );
	MatrixXd weight = MatrixXd::Ones( landmarks );

}
