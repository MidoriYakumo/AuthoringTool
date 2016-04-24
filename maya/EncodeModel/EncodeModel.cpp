
#include "EncodeModel.h"

void EncodeModel::LoadFaces(){

	fstream fin( "./data/faces.dat", ios::in );
	int a;

	faces = MatrixXi( 12894, 3 );
	for( int i = 0; i < 12894; ++i ){
		for( int j = 0; j < 3; ++j ){
			fin >> a;
			faces( i, j ) = a - 1;
		}
	}
}

void EncodeModel::LoadNeigh(){

	fstream fin( "./data/neigh.dat", ios::in );
	int a;

	neigh = MatrixXi( 12894, 3 );
	for( int i = 0; i < 12894; ++i ){
		for( int j = 0; j < 3; ++j ){
			fin >> a;
			neigh( i, j ) = a - 1;
		}
	}
}

void EncodeModel::LoadReconmean(){

	fstream fin( "./data/reconmean.dat", ios::in );
	double a, b, c;

	reconmean = MatrixXd( 6449, 3 );
	for( int i = 0; i < 6449; ++i ){
		fin >> a >> b >> c;
		reconmean( i, 0 ) = a;
		reconmean( i, 1 ) = b;
		reconmean( i, 2 ) = c;
	}
}

void EncodeModel::LoadC(){
	
	fstream fin( "./data/C.dat", ios::in | ios::binary );
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
	
	fstream fin( "./data/coeffs.dat", ios::in | ios::binary );
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
	
	fstream fin( "./data/avg.dat", ios::in );
	float f;

	avg = MatrixXd( 193410, 1 );
	for( int i = 0; i < 193410; ++i ){
		fin >> f;
		avg( i ) = f;
	}
}

void EncodeModel::LoadProjected(){

	fstream fin( "./data/projected.dat", ios::in );
	double d;

	projected = MatrixXd( 1064, 1064 );
	for( int i = 0; i < 1064; ++i ){
		for( int j = 0; j < 1064; ++j ){
			fin >> d;
			projected( j, i ) = d;
		}
	}
}

void EncodeModel::LoadSemdata(){

	fstream fin( "./data/semdata.dat", ios::in );
	double d;

	semdata = MatrixXd( 1064, 25 );
	for( int i = 0; i < 25; ++i ){
		for( int j = 0; j < 1064; ++j ){
			fin >> d;
			semdata( j, i ) = d;
		}
	}
}