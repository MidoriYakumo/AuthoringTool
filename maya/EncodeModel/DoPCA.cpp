/*
function[coeffs, eigvals, projected] = do_pca(models, N)

if ~exist('N', 'var')
N = size(models, 2);
end

avg = mean(models, 2);

% subtract off the mean for each dimension
for i = 1:N
models(:, i) = models(:, i) - avg;
end
% construct the matrix Y
%models = models / sqrt(N - 1);

% SVD does it all
[u, S, coeffs] = svd(models','econ');
	% calculate the variances
	S = diag(S);
eigvals = S.*S;
% project the original data
projected = coeffs' * models;

*/

#include "EncodeModel.h"

MatrixXd DoPCA( MatrixXd &model ){

	int N = model.cols();
	MatrixXd avg( model.rows(), 1 );

	for( int i = 0; i < avg.rows(); ++i ){
		avg( i ) = model.row( i ).mean();
	}

	for( int i = 0; i < avg.cols(); ++i ){
		model.col( i ) -= avg;
	}

	JacobiSVD< MatrixXd > svd( model.transpose(), Eigen::ComputeThinU | Eigen::ComputeThinV );

	return svd.matrixV();
}