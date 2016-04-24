
#include "EncodeModel.h"

void saveData(){

	fstream fin( "./data/coeffs1.dat", ios::in );
	fstream fout( "./data/coeffs2.dat", ios::out | ios::binary );
	float f;

	for( int i = 0; i < 1064; ++i ){
		for( int j = 0; j < 193410; ++j ){
			fin >> f;
			fout.write( reinterpret_cast< const char * >( &f ), sizeof( float ) );
		}
		cout << i << endl;
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

	fstream fin( "./data/encoded.dat", ios::in );
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

void testDecodeTrans(){

	fstream fin( "./data/decodedrot.dat", ios::in );
	fstream fout( "out.txt", ios::out );
	MatrixXd features( 1, 128940 );
	MatrixXd modelout;
	double d;

	for( int i = 0; i < 128940; ++i ){
		fin >> d;
		features( i ) = d;
	}

	EncodeModel em;

	em.LoadFaces();
	em.LoadReconmean();
	modelout = DecodeTranslation( features, em.faces, em.reconmean );

	fout << modelout;
}

void testMorph(){

	//---encode---
	fstream fout( "out.txt", ios::out );
	MatrixXd vertices;
	MatrixXi faces;
	MatrixXd encoded;
	MatrixXd decodedrot;
	MatrixXd modelout;
	MatrixXd projected;
	MatrixXd unprojected;
	EncodeModel em;

	em.LoadReconmean();
	em.LoadNeigh();
	em.LoadAvg();
	
	//---read model---
	cout << "Reading OBJ..." << endl;
	ReadObj( "../s1p0.obj", vertices, faces );

	//---encode---
	cout << "Encoding..." << endl;
	encoded = EncodeRelativeRotation( vertices, faces, em.reconmean, em.neigh );

	//---project into PCA space---
	cout << "Projecting..." << endl;
	em.LoadC();
	projected = em.C * ( encoded.transpose() - em.avg );
	em.C.resize( 0, 0 );

	//---unproject from PCA space---
	cout << "Unprojecting..." << endl;
	em.LoadCoeffs();
	unprojected = em.coeffs * projected + em.avg;
	em.coeffs.resize( 0, 0 );

	//---decode---
	cout << "Decoding..." << endl;
	decodedrot = DecodeRelativeRotation( unprojected, em.neigh );
	modelout = DecodeTranslation( decodedrot, faces, em.reconmean );

	//---correct model rotation---
	modelout = Upright( modelout );

	//---write model---
	cout << "Writing OBJ..." << endl;
	WriteObj( "../s1p0_out.obj", modelout, faces );
}

int main(){
	
	testMorph();
	//testDecode();
	//testDecodeTrans();
	//testMorph();

	return 0;
}
