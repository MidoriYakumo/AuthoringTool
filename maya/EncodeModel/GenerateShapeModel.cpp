/*****
%
%[coeffs, avg, eigvals, projected] = genshapemodel(models, replacefaces)
%
%   models - a k x n * 3 matrix containing k training models
%
% Copyright 2009 (c)Nils Hasler hasler@mpi-inf.mpg.de
% please cite
% N.Hasler, C.Stoll, M.Sunkel, B.Rosenhahn, H. - P.Seidel: A Statistical Model of Human Pose and Body Shape,
% Computer Graphics Forum(Proc.Eurographics 2009), Munich, Germany, March 2009.
*****/
#include"GenerateShapeModel.h"
/*
 function[coeffs, avg, eigvals, projected, limits] = genshapemodel(models, replacefaces, method)

models = models';

[M, N] = size(models);

avg = mean(models, 2);

if exist('replacefaces', 'var') && ~isempty(replacefaces)
if size(replacefaces, 1) == 1
replacefaces = replacefaces';
end
these = reshape(repmat(replacefaces * 10, 1, 10) + repmat(-9:0, length(replacefaces), 1), length(replacefaces) * 10, 1);
models(these, :) = repmat(avg(these), 1, N);
end

if ~exist('method', 'var')
method = 1;
end

 
for i = 1:N
models(:, i) = (models(:, i) - avg);  

 end

switch method
case 0
 
		[u, S, coeffs] = svd(models','econ');
			% calculate the variances
			S = diag(S);
eigvals = S;

projected = coeffs' * models;

case 1

	[P, eigvals] = eig(models'*models/(M-1));
		[eigvals, I] = sort(diag(eigvals), 'descend');
P = P(:, I);
 
coeffs = (models / sqrt(M - 1))*P*diag(1. / sqrt(eigvals)); 
 
projected = (models'*coeffs)';
 
 end

	 limits = std(projected, 0, 2);

*/
