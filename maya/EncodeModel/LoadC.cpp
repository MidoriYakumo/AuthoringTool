
#include "EncodeModel.h"

MatrixXd LoadC(){

	MatrixXd C;
	fstream fin( "./data/C.dat", ios::in );
	fstream fout( "out.txt", ios::out | ios::binary );
	float f;

	for( int i = 0; i < 1064; ++i ){
		cout << "flag" << endl;
		for( int j = 0; j < 193410; ++j ){
			fin >> f;
			fout.write( reinterpret_cast< const char * >( &f ), sizeof( float ) );
		}
	}

	return MatrixXd( 1, 1 );
}