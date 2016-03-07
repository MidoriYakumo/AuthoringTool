
#include "LSystem.h"
#include "LSystemNode.h"
#include "cylinder.h"

#include <maya/MTime.h>
#include <maya/MTypes.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnStringData.h>

#define McheckErr(stat,msg)			\
	if ( MS::kSuccess != stat ) {	\
		cerr << msg;				\
		return MS::kFailure;		\
	}

MObject LSystemNode::angle;
MObject LSystemNode::size;
MObject LSystemNode::time;
MObject LSystemNode::grammar;
MObject LSystemNode::outputMesh;
MTypeId LSystemNode::id( 0x56789 );

MStatus LSystemNode::initialize(){

	MFnNumericAttribute angleAttr;
	MFnNumericAttribute sizeAttr;
	MFnUnitAttribute timeAttr;
	MFnTypedAttribute grammarAttr;
	MFnStringData grammarData;
	MFnTypedAttribute geometryAttr;

	MStatus returnStatus;

	LSystemNode::angle = angleAttr.create( "angle", "ag", 
		MFnNumericData::kDouble, 30.0, &returnStatus );
	McheckErr( returnStatus, "ERROR creating LSystemNode angle attribute\n" );

	LSystemNode::size = sizeAttr.create( "size", "sz", 
		MFnNumericData::kDouble, 1.0, &returnStatus );
	McheckErr( returnStatus, "ERROR creating LSystemNode size attribute\n" );

	LSystemNode::time = timeAttr.create( "time", "tm", 
		MFnUnitAttribute::kTime, 0.0, &returnStatus );
	McheckErr( returnStatus, "ERROR creating LSystemNode time attribute\n" );

	LSystemNode::grammar = grammarAttr.create( "grammar", "gmr", 
		MFnData::kString, grammarData.create( "F\nF->F[+F]F[-F]F" ), &returnStatus );
	McheckErr( returnStatus, "ERROR creating LSystemNode grammar attribute\n" );

	LSystemNode::outputMesh = geometryAttr.create( "outputMesh", "out", 
		MFnData::kMesh, &returnStatus );
	McheckErr( returnStatus, "ERROR creating LSystemNode outputMesh attribute\n" );
	geometryAttr.setStorable( false );

	returnStatus = addAttribute( LSystemNode::angle );
	McheckErr( returnStatus, "ERROR adding angle attribute\n" );

	returnStatus = addAttribute( LSystemNode::size );
	McheckErr( returnStatus, "ERROR adding size attribute\n" );

	returnStatus = addAttribute( LSystemNode::time );
	McheckErr( returnStatus, "ERROR adding time attribute\n" );

	returnStatus = addAttribute( LSystemNode::grammar );
	McheckErr( returnStatus, "ERROR adding grammar attribute\n" );

	returnStatus = addAttribute( LSystemNode::outputMesh );
	McheckErr( returnStatus, "ERROR adding outputMesh attribute\n" );

	returnStatus = attributeAffects( LSystemNode::angle, LSystemNode::outputMesh );
	McheckErr( returnStatus, "ERROR in angle attributeAffects\n" );

	returnStatus = attributeAffects( LSystemNode::size, LSystemNode::outputMesh );
	McheckErr( returnStatus, "ERROR in size attributeAffects\n" );

	returnStatus = attributeAffects( LSystemNode::time, LSystemNode::outputMesh );
	McheckErr( returnStatus, "ERROR in time attributeAffects\n" );

	returnStatus = attributeAffects( LSystemNode::grammar, LSystemNode::outputMesh );
	McheckErr( returnStatus, "ERROR in grammar attributeAffects\n" );

	return MS::kSuccess;
}

MStatus LSystemNode::compute( const MPlug &plug, MDataBlock &data ){
	
	MStatus returnStatus;

	if( plug == LSystemNode::outputMesh ){
		
		//---get time---
		MDataHandle timeData = data.inputValue( LSystemNode::time, &returnStatus );
		McheckErr( returnStatus, "Error getting time data handle\n" );
		MTime time = timeData.asTime();
		
		//---get angle---
		MDataHandle angleData = data.inputValue( LSystemNode::angle, &returnStatus );
		McheckErr( returnStatus, "Error getting angle data handle\n" );
		double angle = angleData.asDouble();

		//--get size---
		MDataHandle sizeData = data.inputValue( LSystemNode::size, &returnStatus );
		McheckErr( returnStatus, "Error getting size data handle\n" );
		double size = sizeData.asDouble();

		//---get grammar---
		MDataHandle grammarData = data.inputValue( LSystemNode::grammar, &returnStatus );
		McheckErr( returnStatus, "Error getting time data handle\n" );
		MString grammar = grammarData.asString();

		//---get geometry---
		MDataHandle geometryHandle = data.outputValue( LSystemNode::outputMesh, &returnStatus );
		McheckErr( returnStatus, "Error getting geometry data handle\n" );

		//---update geometry---
		MFnMeshData dataCreator;
		MObject geometryData = dataCreator.create( &returnStatus );
		McheckErr( returnStatus, "Error creating geometryData" );

		createMesh( time, angle, size, grammar, geometryData, returnStatus );
		McheckErr( returnStatus, "Error creating L-System" );

		geometryHandle.set( geometryData );
		data.setClean( plug );

	}else
		return MS::kUnknownParameter;

	return MS::kSuccess;
}

MObject LSystemNode::createMesh( 
	const MTime &time, const double angle, const double size, const MString grammar, 
	MObject &outData, MStatus &status ){

	LSystem system;
	std::vector<LSystem::Branch> branches;

	system.loadProgramFromString( grammar.asChar() );
	system.setDefaultAngle( angle );
	system.setDefaultStep( size );
	system.process( time.value(), branches );

	MPointArray points;
	MIntArray faceCounts;
	MIntArray faceConnects;
	
	for( auto e : branches ){
	
		MPoint start( e.first.n[ 0 ], e.first.n[ 2 ], e.first.n[ 1 ] );
		MPoint end( e.second.n[ 0 ], e.second.n[ 2 ], e.second.n[ 1 ] );
		CylinderMesh cylinder( start, end, 0.1 );

		cylinder.appendToMesh( points, faceCounts, faceConnects );
	}

	MFnMesh meshFS;
	MObject newMesh = meshFS.create( points.length(), faceCounts.length(), 
		points, faceCounts, faceConnects, outData, &status );

	return newMesh;
}
