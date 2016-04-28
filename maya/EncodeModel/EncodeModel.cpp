
#include "EncodeModel.h"

void EncodeModel::SetFilepath( string filepath ){
	this->filepath = filepath;
}

void EncodeModel::LoadFaces(){

	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/faces.dat", ios::in | ios::binary );
	int a;

	faces = MatrixXi( 12894, 3 );
	for( int i = 0; i < 12894; ++i ){
		for( int j = 0; j < 3; ++j ){
			fin.read( reinterpret_cast< char * >( &a ), sizeof( int ) );
			faces( i, j ) = a - 1;
		}
	}
}

void EncodeModel::LoadNeigh(){

	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/neigh.dat", ios::in | ios::binary );
	int a;

	neigh = MatrixXi( 12894, 3 );
	for( int i = 0; i < 12894; ++i ){
		for( int j = 0; j < 3; ++j ){
			fin.read( reinterpret_cast< char * >( &a ), sizeof( int ) );
			neigh( i, j ) = a - 1;
		}
	}
}

void EncodeModel::LoadReconmean(){

	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/reconmean.dat", ios::in | ios::binary );
	float a, b, c;

	reconmean = MatrixXd( 6449, 3 );
	for( int i = 0; i < 6449; ++i ){
		fin.read( reinterpret_cast< char * >( &a ), sizeof( float ) );
		fin.read( reinterpret_cast< char * >( &b ), sizeof( float ) );
		fin.read( reinterpret_cast< char * >( &c ), sizeof( float ) );
		reconmean( i, 0 ) = a;
		reconmean( i, 1 ) = b;
		reconmean( i, 2 ) = c;
	}
}

void EncodeModel::LoadC(){
	
	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/C.dat", ios::in | ios::binary );
	float f;

	C = MatrixXd( 1064, 193410 );
	for( int i = 0; i < 193410; ++i ){
		for( int j = 0; j < 1064; ++j ){
			fin.read( reinterpret_cast< char * >( &f ), sizeof( float ) );
			C( j, i ) = f;
		}
	}
}

void EncodeModel::LoadCoeffs(){
	
	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/coeffs.dat", ios::in | ios::binary );
	float f;

	coeffs = MatrixXd( 193410, 1064 );
	for( int i = 0; i < 1064; ++i ){
		for( int j = 0; j < 193410; ++j ){
			fin.read( reinterpret_cast< char * >( &f ), sizeof( float ) );
			coeffs( j, i ) = f;
		}
	}
}

void EncodeModel::LoadAvg(){
	
	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/avg.dat", ios::in | ios::binary );
	float f;

	avg = MatrixXd( 193410, 1 );
	for( int i = 0; i < 193410; ++i ){
		fin.read( reinterpret_cast< char * >( &f ), sizeof( float ) );
		avg( i ) = f;
	}
}

void EncodeModel::LoadProjected(){

	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/projected.dat", ios::in | ios::binary );
	float f;

	projected = MatrixXd( 1064, 1064 );
	for( int i = 0; i < 1064; ++i ){
		for( int j = 0; j < 1064; ++j ){
			fin.read( reinterpret_cast< char * >( &f ), sizeof( float ) );
			projected( i, j ) = f;
		}
	}
}

void EncodeModel::LoadSemdata(){

	fstream fin( "C:/Users/Liang Peng/Documents/GitHub/AuthoringTool/maya/EncodeModel/data/semdata.dat", ios::in | ios::binary );
	float f;

	semdata = MatrixXd( 1064, 25 );
	for( int i = 0; i < 25; ++i ){
		for( int j = 0; j < 1064; ++j ){
			fin.read( reinterpret_cast< char * >( &f ), sizeof( float ) );
			semdata( j, i ) = f;
		}
	}
}