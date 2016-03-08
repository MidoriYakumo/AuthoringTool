
#include "BodyReshaper.h"

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>

BodyReshaper::BodyReshaper() : MPxCommand() {

}

BodyReshaper::~BodyReshaper() {

}

//const char *sizeFlag = "-s", *sizeLongFlag = "-size";
//const char *degreeFlag = "-d", *degreeLongFlag = "-degree";
//const char *grammarFlag = "-g", *grammarLongFlag = "-grammar";
//const char *iterationFlag = "-i", *iterationLongFlag = "-iteration";

MSyntax BodyReshaper::newSyntax(){

	MSyntax syntax;

	//syntax.addFlag( sizeFlag, sizeLongFlag, MSyntax::kDouble );
	//syntax.addFlag( degreeFlag, degreeLongFlag, MSyntax::kDouble );
	//syntax.addFlag( grammarFlag, grammarLongFlag, MSyntax::kString );
	//syntax.addFlag( iterationFlag, iterationLongFlag, MSyntax::kUnsigned );

	return syntax;
}

MStatus BodyReshaper::doIt( const MArgList& args ){

	//MArgDatabase argData( syntax(), args );
	//double size = 1.;
	//double degree = 25.7;
	//MString grammar = "F\nF->F[+F]F[-F]F";
	//unsigned iteration = 1;

	//if( argData.isFlagSet( sizeFlag ) )
	//	argData.getFlagArgument( sizeFlag, 0, size );
	//if( argData.isFlagSet( degreeFlag) )
	//	argData.getFlagArgument( degreeFlag, 0, degree );
	//if( argData.isFlagSet( grammarFlag ) )
	//	argData.getFlagArgument( grammarFlag, 0, grammar );
	//if( argData.isFlagSet( iterationFlag ) )
	//	argData.getFlagArgument( iterationFlag, 0, iteration );

	MGlobal::executeCommand( "" );

	return MStatus::kSuccess;
}