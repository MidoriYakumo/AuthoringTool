#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>

#include "BodyReshaper.h"

MStatus initializePlugin( MObject obj ) //打钩
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj, "Figure Reshaper", "1.0", "1.0" );

    //---register command---
    status = plugin.registerCommand( "FigureReshaper", BodyReshaper::creator, BodyReshaper::newSyntax );
	if (!status){ status.perror( "registerCommand" ); return status; }
	
	//---create menu---
	MString cmd = "global string $gMainWindow; \n"
		"setParent $gMainWindow; \n"
		"menu -label \"Figure Reshaper\" myFigureReshaperMenu; \n"
		"setParent -menu myFigureReshaperMenu; \n"
		"menuItem -label \"Figure Reshaper\" -command \"source \\\""
		+ plugin.loadPath() + "/MyPluginDialog\\\"\"; \n";  //menu插进主界面？

	MGlobal::executeCommand( cmd, true );

    return status;
}

MStatus uninitializePlugin( MObject obj ) //不打钩
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj );

	//---deregister command---
    status = plugin.deregisterCommand( "FigureReshaper" );
	if (!status){ status.perror( "deregisterCommand" ); return status; }

	//---delete menu---
	MGlobal::executeCommand( "deleteUI myFigureReshaperMenu;" );

    return status;
}
