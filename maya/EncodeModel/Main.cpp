
#include "EncodeModel.h"

void saveData(){

	fstream fin( "in.txt", ios::in );
	fstream fout( "out.txt", ios::out );
	vector< int > v;
	int a;

	for( int i = 0; i < 12894 * 3; ++i ){
		fin >> a;
		v.push_back( a );
	}
	
	for( int i = 0; i < 12894; ++i ){
		fout << v[ i ] << " " << v[ i + 12894 ] << " " << v[ i + 25788 ] << endl;
	}
}

void testEncode(){

	fstream fout( "out.txt", ios::out );
	MatrixXd vertices;
	MatrixXi faces;
	MatrixXd temp;
	MatrixXi neigh;
	MatrixXd encoded;

	ReadObj( "../s1p0.obj", vertices, faces );
	temp = LoadTemplate();
	neigh = LoadNeighbor();
	encoded = EncodeRelativeRotation( vertices, faces, temp, neigh );

	for( int i = 0; i < encoded.cols(); ++i ){
		fout << encoded( i ) << endl;
	}
	
}

void testDecode(){

	fstream fin( "in.txt", ios::in );
	fstream fout( "out.txt", ios::out );
	MatrixXd encoded( 1, 193410 );
	MatrixXi neigh;
	MatrixXd features;
	double a;
	
	for( int i = 0; i < 193410; ++i ){
		fin >> a;
		encoded( i ) = a;
	}

	neigh = LoadNeighbor();
	features = DecodeRelativeRotation( encoded, neigh );

	for( int i = 0; i < features.cols(); ++i ){
		fout << features( i ) << endl;
	}

}

int main(){

	testDecode();

	return 0;
}
