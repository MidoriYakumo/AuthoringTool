/***********%
% estimate semantic values of obj
% by encoding it into a semantic representation and then using the
% dependence matrix K to estimate the semantic values
***********/

/*
function meaning = estsem(obj, means, B, K)

sem = obj' * B;
meaning = sem(1:size(means, 2)) . / K + means;
*/