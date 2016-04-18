
#include "BodyReshaper.h"
#include "../EncodeModel/EncodeModel.h"

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <fstream>
#include <ios>

MorphShape::MorphShape() : MPxCommand() {

}

MorphShape::~MorphShape() {

}

const char *fileFlag = "-f", *fileLongFlag = "-file";

void* MorphShape::creator(){ 

	return new MorphShape(); 
}

MSyntax MorphShape::newSyntax(){

	MSyntax syntax;

	syntax.addFlag( fileFlag, fileLongFlag, MSyntax::kString );  //文件名

	return syntax;
}

MStatus MorphShape::doIt( const MArgList& args ){

	MArgDatabase argData( syntax(), args ); //get the database of the arguments 调用一个command
	MString filename;

	if( argData.isFlagSet( fileFlag ) ) //数据库有没有提供这个参数 -f，有-f就取出-f后面跟着的值
		argData.getFlagArgument( fileFlag, 0, filename ); // -f , filename 

	

	return MStatus::kSuccess;
}