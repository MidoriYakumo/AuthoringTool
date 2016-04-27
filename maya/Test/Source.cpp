
#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
using namespace std;

int main(){

	MatrixXd a( 2, 3 ), b;
	a << 1, 2, 3, 4, 5, 6;

	a = a.cwiseSqrt().cwiseInverse();

	cout << a << endl;

	return 0;
}
