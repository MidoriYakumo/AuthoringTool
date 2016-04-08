
#include "EncodeModel.h"

MatrixXd LoadTemplate(){
	
	MatrixXd result( 6449, 3 );
	fstream fin( "reconmean.dat", ios::in );
	double a, b, c;
	
	for( int i = 0; i < 6449; ++i ){
		fin >> a >> b >> c;
		result( i, 0 ) = a;
		result( i, 1 ) = b;
		result( i, 2 ) = c;
	}

	return result;
}