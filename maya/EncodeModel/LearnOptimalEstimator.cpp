/****
%
%[best_N, best_dim, best_msE, all_N, all_msE] = learn_optimal_estimator(X, Y, num_partitions, dim_min, dim_max, dim_step, breakfactor)
%
% use cross validation to train a linear function
%
% X(N x k) N input vectors organized in rows, e.g.projected'
% Y(k x 1) values of the function that is to be learned
% num_partitions  number of cross validation partitions
% dim_min         minimum number of coefficients used in the function
% dim_max         maximum number of coefficients used in the function
% dim_step        step size during the optimization
% breakfactor     when the current error > min_error * breakfactor  stop the optimization
%
****/