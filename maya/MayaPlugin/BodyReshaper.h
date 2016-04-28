#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

#include "../EncodeModel/EncodeModel.h"

class BodyReshaper : public MPxCommand
{
public:
	BodyReshaper();
	virtual ~BodyReshaper();
	static void* creator(); //固定的
	static MSyntax newSyntax(); 
	MStatus doIt( const MArgList& args );
};

class MorphShape : public MPxCommand
{

public:
	MorphShape();
	virtual ~MorphShape();
	static void* creator(); //固定的
	static MSyntax newSyntax(); 
	MStatus doIt( const MArgList& args );

	static EncodeModel em;
	static const char *heightFlag, *heightLongFlag;
	static const char *weightFlag, *weightLongFlag = "-weight";
	static const char *fileFlag = "-f", *fileLongFlag = "-file";

};