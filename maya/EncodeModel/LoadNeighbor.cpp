
#include "EncodeModel.h"

MatrixXi LoadNeighbor(){

	MatrixXi result( 12894, 3 );
	fstream fin( "./data/neigh.dat", ios::in );
	int a, b, c;

	for( int i = 0; i < 12894; ++i ){
		fin >> a >> b >> c;
		result( i, 0 ) = a-1;
		result( i, 1 ) = b-1;
		result( i, 2 ) = c-1;
	}
	
	return result;
}