#include "LSystemCmd.h"
#include "LSystem.h"

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <list>

MString part1 = "curve -d 1 -p ";
//---start---
MString part2 = "-p ";
//---end---
MString part3 = "-k 0 -k 1 -name curve";
//---ID---
MString part4 = "; \ncircle -c 0 0 0 -nr 0 1 0 -sw 360 -r 0.1 -d 3 -ut 0 -tol 0.01 -s 8 -ch 1 -name nurbsCircle";
//---ID---
MString part5 = "; \nselect -r nurbsCircle";
//---ID---
MString part6 = " curve";
//---ID---
MString part7 = "; \nextrude -ch true -rn false -po 1 -et 2 -ucp 1 -fpt 1 -upn 1 -rotation 0 -scale 1 -rsp 1 \"nurbsCircle";
//---ID---
MString part8 = "\" \"curve";
//---ID---
MString part9 = "\"; \n";

LSystemCmd::LSystemCmd() : MPxCommand()
{
}

LSystemCmd::~LSystemCmd() 
{
}

const char *sizeFlag = "-s", *sizeLongFlag = "-size";
const char *degreeFlag = "-d", *degreeLongFlag = "-degree";
const char *grammarFlag = "-g", *grammarLongFlag = "-grammar";
const char *iterationFlag = "-i", *iterationLongFlag = "-iteration";

MSyntax LSystemCmd::newSyntax(){

	MSyntax syntax;

	syntax.addFlag( sizeFlag, sizeLongFlag, MSyntax::kDouble );
	syntax.addFlag( degreeFlag, degreeLongFlag, MSyntax::kDouble );
	syntax.addFlag( grammarFlag, grammarLongFlag, MSyntax::kString );
	syntax.addFlag( iterationFlag, iterationLongFlag, MSyntax::kUnsigned );

	return syntax;
}

MStatus LSystemCmd::doIt( const MArgList& args )
{
    //cout<<"Implement Me!"<<endl;

	//---
	MArgDatabase argData( syntax(), args );
	double size = 1.;
	double degree = 25.7;
	MString grammar = "F\nF->F[+F]F[-F]F";
	unsigned iteration = 1;

	if( argData.isFlagSet( sizeFlag ) )
		argData.getFlagArgument( sizeFlag, 0, size );
	if( argData.isFlagSet( degreeFlag) )
		argData.getFlagArgument( degreeFlag, 0, degree );
	if( argData.isFlagSet( grammarFlag ) )
		argData.getFlagArgument( grammarFlag, 0, grammar );
	if( argData.isFlagSet( iterationFlag ) )
		argData.getFlagArgument( iterationFlag, 0, iteration );
	
	LSystem system;

	system.loadProgramFromString( grammar.asChar() );
	system.setDefaultAngle( degree );
	system.setDefaultStep( size );
	
	//for (int i = 0; i < 1; i++)
	{
		//std::string insn = system.getIteration(i);
		//std::cout << insn << std::endl;

		std::vector<LSystem::Branch> branches;
		system.process( iteration, branches );

		MString cmd;
		int id = 0;

		for( auto e : branches ){
			++id;
			cmd += part1 + e.first.n[ 0 ] + " " + e.first.n[ 2 ] + " " + e.first[ 1 ] + " ";
			cmd += part2 + e.second.n[ 0 ] + " " + e.second.n[ 2 ] + " " + e.second.n[ 1 ] + " ";
			cmd += part3 + id + part4 + id + part5 + id + part6 + id + part7 + id + part8 + id + part9;
		}

		cout << cmd << endl;
		MGlobal::executeCommand( cmd );
	}
	//---

    return MStatus::kSuccess;
}