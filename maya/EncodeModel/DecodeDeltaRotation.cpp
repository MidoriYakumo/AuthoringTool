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

function [features, A] = decdeltarot(input, neigh, direction, landmarks, weight, replacefaces, replacewith, usemex)

if size(input,2) == 1
input = input';
end

step = 15;
N = size(neigh, 1);
M = size(input, 1);

compressed = (N * step ~= size(input, 2));

if ~exist('direction', 'var') || isempty(direction)
direction = [0;0;0];
end

if ~exist('landmarks','var') || isempty(landmarks)
landmarks = 1;
end

if ~exist('weight','var') || isempty(weight)
weight = ones(size(landmarks));
elseif length(weight) == 1
weight = ones(size(landmarks)) * weight;
end

features = zeros(M, 10*N);

if exist('replacefaces','var') && ~isempty(replacefaces)
if compressed
replacefaces = sort(replacefaces);
at = 1;
k = 1;
for i = 1:N
if i == replacefaces(k)
if neigh(i,1) > i
input(:, at:at+2) = repmat(replacewith(at:at+2), M,1);
at = at + 3;
end
if neigh(i,2) > i
input(:, at:at+2) = repmat(replacewith(at:at+2), M,1);
at = at + 3;
end
if neigh(i,3) > i
input(:, at:at+2) = repmat(replacewith(at:at+2), M,1);
at = at + 3;
end
input(1, (i-1)*10+5:(i-1)*10+10) = repmat(replacewith(at:at+5), M,1);
at = at + 6;
k = k + 1;
else
if neigh(i,1) > i
at = at + 3;
end
if neigh(i,2) > i
at = at + 3;
end
if neigh(i,3) > i
at = at + 3;
end
at = at + 6;
end
end
else
these = (repmat(replacefaces,1,step)-1)*step + repmat(1:step,length(replacefaces),1);
input(:,these) = repmat(replacewith(1,these), M,1);
end
end

if ~exist('usemex', 'var')
usemex = 0;
end

for m = 1:M
if usemex
if exist('direction','var')
[rhs, A] = decdelta(input(m, :), neigh, direction);
else
[rhs, A] = decdelta(input(m, :), neigh);
end
if compressed
at = 1;
for i = 1:N
if neigh(i,1) > i
at = at + 3;
end
if neigh(i,2) > i
at = at + 3;
end
if neigh(i,3) > i
at = at + 3;
end
features(m, (i-1)*10+5:(i-1)*10+10) = input(m, at:at+5);
at = at + 6;
end
else
for i = 1:N
features(m, (i-1)*10+5:(i-1)*10+10) = input(m, (i-1)*step+10:(i-1)*step+15);
end
end
else
A = sparse(4*N,3*N);
row = 1;

if compressed
at = 1;
for i = 1:N
n = neigh(i,1);
if n > i
delta = fromrotvec(input(m, at:at+2)');
A(row:row+2,(n-1)*3+1:(n-1)*3+3) = -eye(3);
A(row:row+2,(i-1)*3+1:(i-1)*3+3) = delta';
row = row + 3;
at = at + 3;
end

n = neigh(i,2);
if n > i
delta = fromrotvec(input(m, at:at+2)');
A(row:row+2,(n-1)*3+1:(n-1)*3+3) = -eye(3);
A(row:row+2,(i-1)*3+1:(i-1)*3+3) = delta';
row = row + 3;
at = at + 3;
end

n = neigh(i,3);
if n > i
delta = fromrotvec(input(m, at:at+2)');
A(row:row+2,(n-1)*3+1:(n-1)*3+3) = -eye(3);
A(row:row+2,(i-1)*3+1:(i-1)*3+3) = delta';
row = row + 3;
at = at + 3;
end
features(m, (i-1)*10+5:(i-1)*10+10) = input(m, at:at+5);
at = at + 6;
end
else
for i = 1:N
n = neigh(i,1);
if n > i
delta = fromrotvec(input(m, (i-1)*step+1:(i-1)*step+3)');
A(row:row+2,(n-1)*3+1:(n-1)*3+3) = -eye(3);
A(row:row+2,(i-1)*3+1:(i-1)*3+3) = delta';
row = row + 3;
end

n = neigh(i,2);
if n > i
delta = fromrotvec(input(m, (i-1)*step+4:(i-1)*step+6)');
A(row:row+2,(n-1)*3+1:(n-1)*3+3) = -eye(3);
A(row:row+2,(i-1)*3+1:(i-1)*3+3) = delta';
row = row + 3;
end

n = neigh(i,3);
if n > i
delta = fromrotvec(input(m, (i-1)*step+7:(i-1)*step+9)');
A(row:row+2,(n-1)*3+1:(n-1)*3+3) = -eye(3);
A(row:row+2,(i-1)*3+1:(i-1)*3+3) = delta';
row = row + 3;
end
features(m, (i-1)*10+5:(i-1)*10+10) = input(m, (i-1)*step+10:(i-1)*step+15);
end
end

rhs = zeros(size(A,1),3);

for i = 1:length(landmarks)
A(row:row+2,(landmarks(i)-1)*3+1:(landmarks(i)-1)*3+3) = eye(3)*weight(i);
rhs(row:row+2,1:3) = fromrotvec(direction(i,:)')'*weight(i);
row = row + 3;
end
end

At = A';
Rs = (At*A) \ (At*rhs);

for i = 0:N-1
[U, S, V] = svd(Rs(i*3+1:i*3+3,:)');
x = U * V';
features(m, i*10+1:i*10+3) = torotvec(x);
end
end



*/