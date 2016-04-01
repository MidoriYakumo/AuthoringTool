#include "PCA_SquareMatrix.h"

#include <iostream>
#include "Eigen/Dense"


using Eigen::MatrixXd;
using namespace std;

/*
* This is the main entry for the PCA program.
*/
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