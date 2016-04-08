
#include "EncodeModel.h"

MatrixXi LoadNeighbor(){

	MatrixXi result( 6449, 3 );
	fstream fin( "neigh.dat", ios::in );
	int a, b, c;

	for( int i = 0; i < 6449; ++i ){
		fin >> a >> b >> c;
		result( i, 0 ) = a;
		result( i, 1 ) = b;
		result( i, 2 ) = c;
	}
	
	return result;
}