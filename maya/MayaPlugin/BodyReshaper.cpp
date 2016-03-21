
#include "BodyReshaper.h"
#include "../AutomaticRigging/pinocchioApi.h"

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <fstream>
#include <ios>

BodyReshaper::BodyReshaper() : MPxCommand() {

}

BodyReshaper::~BodyReshaper() {

}

const char *fileFlag = "-f", *fileLongFlag = "-file";
//const char *degreeFlag = "-d", *degreeLongFlag = "-degree";
//const char *grammarFlag = "-g", *grammarLongFlag = "-grammar";
//const char *iterationFlag = "-i", *iterationLongFlag = "-iteration";

MSyntax BodyReshaper::newSyntax(){

	MSyntax syntax;

	syntax.addFlag( fileFlag, fileLongFlag, MSyntax::kString );
	//syntax.addFlag( degreeFlag, degreeLongFlag, MSyntax::kDouble );
	//syntax.addFlag( grammarFlag, grammarLongFlag, MSyntax::kString );
	//syntax.addFlag( iterationFlag, iterationLongFlag, MSyntax::kUnsigned );

	return syntax;
}

MStatus BodyReshaper::doIt( const MArgList& args ){

	MArgDatabase argData( syntax(), args );
	MString filename;
	//double degree = 25.7;
	//MString grammar = "F\nF->F[+F]F[-F]F";
	//unsigned iteration = 1;

	if( argData.isFlagSet( fileFlag ) )
		argData.getFlagArgument( fileFlag, 0, filename );
	//if( argData.isFlagSet( degreeFlag) )
	//	argData.getFlagArgument( degreeFlag, 0, degree );
	//if( argData.isFlagSet( grammarFlag ) )
	//	argData.getFlagArgument( grammarFlag, 0, grammar );
	//if( argData.isFlagSet( iterationFlag ) )
	//	argData.getFlagArgument( iterationFlag, 0, iteration );

	cout << "Automatic rigging: " << filename << endl;

	Mesh mesh( filename.asChar() );
	Skeleton skeleton = HumanSkeleton();
	PinocchioOutput output = autorig( skeleton, mesh );

	for( int i = 0; i < ( int )output.embedding.size(); ++i ){

		MString createJointCmd( "joint -p " );

		//output.embedding[ i ] = ( output.embedding[ i ] - mesh.toAdd ) / mesh.scale;
		output.embedding[ i ] = ( output.embedding[ i ] - Vector3( .5, .5, .5 ) ) * 2./* + Vector3( 0., 0., 1. )*/;
		createJointCmd = createJointCmd 
			+ output.embedding[ i ][ 0 ] + " " 
			+ output.embedding[ i ][ 1 ] + " "
			+ output.embedding[ i ][ 2 ] + " "
			+ " -n joint" + i + "; select -cl;";

		MGlobal::executeCommand( createJointCmd );
	}

	MString parentJointCmd = 
		"jointDisplayScale 0.1; ikHandleDisplayScale 0.2;\n"
		"select -r joint1; select -tgl joint0; parent;\n"
		"select -r joint2; select -tgl joint1; parent;\n"
		"select -r joint3; select -tgl joint0; parent;\n"
		"select -r joint4; select -tgl joint2; parent;\n"
		"select -r joint5; select -tgl joint4; parent;\n"
		"select -r joint6; select -tgl joint5; parent;\n"
		"select -r joint7; select -tgl joint6; parent;\n"
		"select -r joint8; select -tgl joint2; parent;\n"
		"select -r joint9; select -tgl joint8; parent;\n"
		"select -r joint10; select -tgl joint9; parent;\n"
		"select -r joint11; select -tgl joint10; parent;\n"
		"select -r joint12; select -tgl joint0; parent;\n"
		"select -r joint13; select -tgl joint12; parent;\n"
		"select -r joint14; select -tgl joint13; parent;\n"
		"select -r joint15; select -tgl joint0; parent;\n"
		"select -r joint16; select -tgl joint15; parent;\n"
		"select -r joint17; select -tgl joint16; parent;\n"
		"select -r joint0; select -tgl body:Mesh; SmoothBindSkin;\n"
		"setAttr skinCluster1.normalizeWeights 0;\n"
		"ikHandle -sj joint4 -ee joint6;\n"
		"ikHandle -sj joint8 -ee joint10;\n"
		"ikHandle -sj joint12 -ee joint14;\n"
		"ikHandle -sj joint15 -ee joint17;\n";

	MGlobal::executeCommand( parentJointCmd );

	for( int i = 0; i < ( int )mesh.vertices.size(); ++i ){

		Vector< double, -1 > v = output.attachment->getWeights(i);
		vector< double > weights( v.size() + 1, 0 );
		vector< int > parent{ 0, 0, 1, 0, 2, 4, 5, 6, 2, 8, 9, 10, 0, 12, 13, 0, 15, 16 };
		MString jointWeightCmd;

		jointWeightCmd = jointWeightCmd
			+ "select -r body:Mesh.vtx["
			+ i
			+ "]; skinPercent -normalize false -zeroRemainingInfluences true ";

		for( int j = 0; j < v.size(); ++j ) {

			double d = floor( 0.5 + v[j] * 10000. ) / 10000.;

			weights[ j + 1 ] += d * .5;
			weights[ parent[ j + 1 ] ] += d * .5;
		}

		for( int j = 0; j < weights.size(); ++j ){
			jointWeightCmd = jointWeightCmd + "-transformValue joint" + j + " " + weights[ j ] + " ";
		}

		jointWeightCmd = jointWeightCmd + "skinCluster1;";
		MGlobal::executeCommand( jointWeightCmd );
	}

	return MStatus::kSuccess;
}