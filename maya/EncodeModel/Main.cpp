
#include "EncodeModel.h"

void saveData(){

	fstream fin( "./data/semdata.dat", ios::in );
	fstream fout( "./data/semdata2.dat", ios::out | ios::binary );
	float a;

	for( int i = 0; i < 1064; ++i ){
		for( int j = 0; j < 25; ++j ){
			fin >> a;
			fout.write( reinterpret_cast< const char * >( &a ), sizeof( float ) );
		}
		cout << i << endl;
	}
}

void testEncode(){

	fstream fout( "out.txt", ios::out );
	MatrixXd vertices;
	MatrixXi faces;
	MatrixXd encoded;
	EncodeModel em;

	em.LoadReconmean();
	em.LoadNeigh();

	ReadObj( "../s1p0.obj", vertices, faces );
	encoded = EncodeRelativeRotation( vertices, faces, em.reconmean, em.neigh );

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
	EncodeModel em;
	double a;
	
	for( int i = 0; i < 193410; ++i ){
		fin >> a;
		encoded( i ) = a;
	}

	em.LoadNeigh();
	features = DecodeRelativeRotation( encoded, em.neigh );
	
	for( int i = 0; i < features.cols(); ++i ){
		fout << features( i ) << endl;
	}
}

void testDecodeTrans(){

	fstream fin( "./data/decodedrot.dat", ios::in );
	fstream fout( "out.txt", ios::out );
	MatrixXd features( 1, 128940 );
	MatrixXd modelout;
	EncodeModel em;
	double d;

	for( int i = 0; i < 128940; ++i ){
		fin >> d;
		features( i ) = d;
	}

	em.LoadFaces();
	em.LoadReconmean();
	modelout = DecodeTranslation( features, em.faces, em.reconmean );

	fout << modelout;
}

void testMorph(){

	fstream fout( "out1.txt", ios::out );
	MatrixXd morphed;
	MatrixXd target;
	MatrixXd semdata;
	EncodeModel em;

	em.LoadSemdata();
	em.LoadProjected();

	cout << "Morphing..." << endl;
	target = MatrixXd( 1, 2 );
	target << 80, 140;
	semdata = MatrixXd( 1064, 2 );
	semdata.col( 0 ) = em.semdata.col( 5 );
	semdata.col( 1 ) = em.semdata.col( 2 );
	morphed = MorphTo( em.projected.col( 0 ), target, em.projected, semdata );

	fout << morphed;
}

void testWhole(){

	//---encode---
	fstream fout( "out.txt", ios::out );
	MatrixXd vertices;
	MatrixXi faces;
	MatrixXd encoded;
	MatrixXd decodedrot;
	MatrixXd modelout;
	MatrixXd projected;
	MatrixXd morphed;
	MatrixXd unprojected;
	MatrixXd target;
	MatrixXd semdata;
	EncodeModel em;

	em.LoadReconmean();
	em.LoadNeigh();
	em.LoadAvg();
	em.LoadProjected();
	em.LoadSemdata();
	
	//---read model---
	cout << "Reading OBJ..." << endl;
	ReadObj( "../s1p4.obj", vertices, faces );

	//---encode---
	cout << "Encoding..." << endl;
	encoded = EncodeRelativeRotation( vertices, faces, em.reconmean, em.neigh );

	//---project into PCA space---
	cout << "Projecting..." << endl;
	em.LoadC();
	projected = em.C * ( encoded.transpose() - em.avg );
	em.C.resize( 0, 0 );

	//---morph to---
	cout << "Morphing..." << endl;
	target = MatrixXd( 1, 2 );
	target << 40, 130;
	semdata = MatrixXd( 1064, 2 );
	semdata.col( 0 ) = em.semdata.col( 5 );
	semdata.col( 1 ) = em.semdata.col( 2 );
	morphed = MorphTo( projected, target, em.projected, semdata );

	//---unproject from PCA space---
	cout << "Unprojecting..." << endl; 
	em.LoadCoeffs();
	unprojected = em.coeffs * morphed + em.avg;
	em.coeffs.resize( 0, 0 );

	//---decode---
	cout << "Decoding..." << endl;
	decodedrot = DecodeRelativeRotation( unprojected, em.neigh );
	modelout = DecodeTranslation( decodedrot, faces, em.reconmean );

	//---correct model rotation---
	modelout = Upright( modelout );

	//---write model---
	cout << "Writing OBJ..." << endl;
	WriteObj( "../s1p4_out.obj", modelout, faces );
}

void testOther(){

	MatrixXd a( 1064, 1064 );
	MatrixXd b( 1, 1064 );
	fstream fin( "out.txt", ios::in );
	fstream fin2( "in.txt", ios::in );
	float f;

	for( int i = 0; i < 1064; ++i ){
		for( int j = 0; j < 1064; ++j ){
			fin >> f;
			a( i, j ) = f;
		}
	}
	
	for( int i = 0; i < 1064; ++i ){
		fin2 >> f;
		b( i ) = f;
	}

	cout << b * a;
}

int main(){
	
	//saveData();
	//testOther();
	//testDecode();
	//testDecodeTrans();
	//testMorph();
	testWhole();

	return 0;
}
