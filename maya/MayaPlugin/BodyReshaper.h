#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

#include "../EncodeModel/EncodeModel.h"
#include "../AutomaticRigging/pinocchioApi.h"

class BodyReshaper : public MPxCommand
{
public:
	BodyReshaper();
	virtual ~BodyReshaper();
	static void* creator(); //固定的
	static MSyntax newSyntax(); 
	MStatus doIt( const MArgList& args );

	void RotateMesh( Mesh &mesh );

	static const char *fileFlag, *fileLongFlag;
};

class MorphShape : public MPxCommand
{

public:
	MorphShape();
	virtual ~MorphShape();
	static void* creator(); //固定的
	static MSyntax newSyntax(); 
	MStatus doIt( const MArgList& args );

	void RotateX( MatrixXd &vertices );

	static int id;
	static string pluginPath;
	static EncodeModel em;
	static const char *fileFlag, *fileLongFlag;
	static const char *genderFlag, *genderLongFlag;
	static const char *heightFlag, *heightLongFlag;
	static const char *weightFlag, *weightLongFlag;
	static const char *breastFlag, *breastLongFlag;
	static const char *waistFlag, *waistLongFlag;
	static const char *hipFlag, *hipLongFlag;
};