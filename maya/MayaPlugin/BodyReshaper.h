#pragma once

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

class BodyReshaper : public MPxCommand
{
public:
	BodyReshaper();
	virtual ~BodyReshaper();
	static void* creator() { return new BodyReshaper(); }
	static MSyntax newSyntax();
	MStatus doIt( const MArgList& args );
};