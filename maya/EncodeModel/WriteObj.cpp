
#include "EncodeModel.h"

void WriteObj( std::string filename, MatrixXd &vertices, MatrixXi &faces ){

	fstream fout( filename, ios::out );

	for( int i = 0; i < vertices.rows(); ++i ){
		fout << "v " << vertices( i, 0 ) << " " << vertices( i, 1 ) << " " << vertices( i, 2 ) << endl;
	}

	for( int i = 0; i < faces.rows(); ++i ){
		fout << "f " << faces( i, 0 ) + 1 << " " << faces( i, 1 ) + 1 << " " << faces( i, 2 ) + 1 << endl;
	}
}
