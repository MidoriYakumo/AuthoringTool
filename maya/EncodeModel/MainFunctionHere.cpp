/*******************************
%
% in this file some fundamental operations are described that can be performed with the supplied dataset
%
% 0. load supplied data

load pcamodel.mat

% data that is supplied in the matlab file :
%
% avg             average scan in relative rotation encoding
% projected       all scans projected into the PCA space(every column is one scan)
% coeffs          transformation matrix for returning from the PCA space
%                 relrot = coeffs * projected + avg
% C               inverse transformation
%                 projected = C * (relrot - avg)
% faces           for every face the indices of the corresponding vertices
% neigh           for every face the indices of the neighbouring faces
% poses           for every scan the number of the pose that was scanned
% subjects        for every scan the number of the subject
% reconmean       the template that was used for encoding the scans
% semdata         semantic data associated with the scans
% semdata_labels  human readable description of the columns of semdata



% 1. decode a model given in the PCA model and display it
%
% unproject from PCA space

p = coeffs * projected(:, 1) + avg;

% convert from relative rotation encoding to absolute rotation encoding

d = decdeltarot(p, neigh);

% fully reconstruct 3D model

r = reconstructmodel(d, faces, reconmean);

% show model

showmodel(r, faces, 'b');



% 2. encode model and project into PCA space
%
% encode

e = fullenc(r, faces, reconmean, neigh, 0);

% project into PCA space

p2 = C * (e' - avg);



	% 3. morph subject according to two semantic constraints(weight = 60kg, height = 170cm)

	morphed = morphto(projected(:, 1), [60, 200], projected, semdata(:, [6, 3]));

% show result

showmodel(reconstructmodel(decdeltarot(coeffs * morphed + avg, neigh), faces, reconmean), faces, 'g');



% 4. compute PCA on subset of complete database(only pose 0)
%
% extract pose = 0 scans

p0 = coeffs * projected(:, poses == 0) + repmat(avg, 1, 228);

% compute PCA

[coeffs0, avg0, eigvals0, projected0] = genshapemodel(p0');



	% 5. learn function for estimating / changing weight
	%
	% train function

	func = learn_optimal_estimator(projected0', semdata(poses==0, 6));

		% and estimate weight

		w = func(1) + p0(1, :) * func(2:end)

		% compare to measured value

		w - semdata(1, 6)

****************************************/


/**
#include <iostream>
#include "Eigen/Dense"
#include "pca.h"

using Eigen::MatrixXd;
using namespace std;


/*
* This is the main entry for the PCA program.
*/

#include "PCA_SquareMatrix.h"

int main()
{
	// The number of rows in the input matrix
	int rows = 3;

	// The number of columns in the input matrix
	int columns = 3;

	// Initialize the matrix
	MatrixXd m(rows, columns);

	// Set the matrix values.
	m << 1, 2, 4,
		10, 4, 5,
		100, 8, 4;

	// Compute the principal component and show results.
	cout << PCA::Compute(m) << endl;
}
**/