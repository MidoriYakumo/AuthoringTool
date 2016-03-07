#pragma once

#include <maya/MPlug.h>
#include <maya/MPxNode.h>
#include <maya/MStatus.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

class LSystemNode : public MPxNode{
public:
					LSystemNode(){};
	virtual			~LSystemNode(){};
	virtual MStatus	compute( const MPlug &plug, MDataBlock &data );
	static void*	creator() { return new LSystemNode; }
	static MStatus	initialize();

	static MObject	angle;
	static MObject	size;
	static MObject	time;
	static MObject	grammar;
	static MObject	outputMesh;
	static MTypeId	id;

private:
	MObject createMesh( 
		const MTime &time, const double angle, const double size, const MString grammar, 
		MObject &outData, MStatus &status );
};