#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>

#include "BodyReshaper.h"

MStatus initializePlugin( MObject obj )
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj, "Body Reshaper", "1.0", "1.0");

    //---register command---
    status = plugin.registerCommand( "BodyReshaper", BodyReshaper::creator, BodyReshaper::newSyntax );
	if (!status){ status.perror( "registerCommand" ); return status; }
	
	//---create menu---
	MString cmd = "global string $gMainWindow; \n"
		"setParent $gMainWindow; \n"
		"menu -label \"Body Reshaper\" myBodyReshaperMenu; \n"
		"setParent -menu myBodyReshaperMenu; \n"
		"menuItem -label \"Body Reshaper\" -command \"source \\\""
		+ plugin.loadPath() + "/MyPluginDialog\\\"\"; \n";

	MGlobal::executeCommand( cmd, true );

    return status;
}

MStatus uninitializePlugin( MObject obj )
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj );

	//---deregister command---
    status = plugin.deregisterCommand( "BodyReshaper" );
	if (!status){ status.perror( "deregisterCommand" ); return status; }

	//---delete menu---
	MGlobal::executeCommand( "deleteUI myBodyReshaperMenu;" );

    return status;
}
