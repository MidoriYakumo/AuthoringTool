
#include <iostream>
#include <fstream>
#include <string>

#include "pinocchioApi.h"

int main(){

	string filename( "../s1p0.obj" );
	Mesh mesh( filename );
	Skeleton skeleton = HumanSkeleton();

	//---automatic rigging---
	PinocchioOutput output = autorig( skeleton, mesh );

	//---output skeleton---
	ofstream osSkeleton( "../skeleton.out" );

	for( int i = 0; i < ( int )output.embedding.size(); ++i ){
		output.embedding[ i ] = ( output.embedding[ i ] - mesh.toAdd ) / mesh.scale;
		osSkeleton << i << " " 
			<< output.embedding[i][0] << " " 
			<< output.embedding[i][1] << " " 
			<< output.embedding[i][2] << " " 
			<< endl;
	}

	//---output attachment---
	ofstream osAttachment( "../attachment.out" );

	for( int i = 0; i < ( int )mesh.vertices.size(); ++i ){
		Vector< double, -1 > v = output.attachment->getWeights(i);
		for( int j = 0; j < v.size(); ++j ) {
			double d = floor( 0.5 + v[j] * 10000. ) / 10000.;
			osAttachment << d << " ";
		}
		osAttachment << endl;
	}

	return 0;
}