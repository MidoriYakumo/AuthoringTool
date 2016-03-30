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
