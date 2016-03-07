#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSimple.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDGModifier.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStringArray.h>
#include <list>

#include "LSystemCmd.h"
#include "LSystemNode.h"

MStatus initializePlugin( MObject obj )
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj, "Liang", "1.0", "1.0");

    //---register command---
    status = plugin.registerCommand( "LSystemCmd", LSystemCmd::creator, LSystemCmd::newSyntax );
	if (!status){ status.perror("registerCommand"); return status; }

	status = plugin.registerNode( "LSystemNode", LSystemNode::id, LSystemNode::creator, LSystemNode::initialize );
	if (!status){ status.perror("registerNode"); return status; }

	//---register menu---
	MString cmd = "global string $gMainWindow; \n"
		"setParent $gMainWindow; \n"
		"menu -label \"L-System\" myLSystemMenu; \n"
		"setParent -menu myLSystemMenu; \n"
		"menuItem -label \"L-System Command\" -command \"source \\\""
		+ plugin.loadPath() + "/MyPluginDialog\\\"\"; \n"
		"menuItem -label \"L-System Node\" -command \"source \\\""
		+ plugin.loadPath() + "/MyCreateNode\\\"\"; \n";

	MGlobal::executeCommand( cmd, true );

    return status;
}

MStatus uninitializePlugin( MObject obj )
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj );

	//---deregister command---
    status = plugin.deregisterCommand( "LSystemCmd" );
	if (!status){ status.perror("deregisterCommand"); return status; }

	//---deregister node---
	status = plugin.deregisterNode( LSystemNode::id );
	if( !status ){ status.perror( "deregisterNode" ); return status; }

	//---restore menu---
	MGlobal::executeCommand( "deleteUI myLSystemMenu;" );

    return status;
}
