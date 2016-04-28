#include "BodyReshaper.h"
#include "../EncodeModel/EncodeModel.h"

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <fstream>
#include <ios>

const char *MorphShape::heightFlag = "-h";
const char *MorphShape::heightLongFlag = "-height";
const char *MorphShape::weightFlag = "-w";
const char *MorphShape::weightLongFlag = "-weight";
const char *MorphShape::fileFlag = "-f"; 
const char *MorphShape::fileLongFlag = "-file"; 

MorphShape::MorphShape() : MPxCommand()
{
}

MorphShape::~MorphShape() 
{
}

void* MorphShape::creator()
{ 
	return new MorphShape(); 
}

MSyntax MorphShape::newSyntax()
{
	MSyntax syntax;
	syntax.addFlag(fileFlag, fileLongFlag, MSyntax::kString );  //文件名
	syntax.addFlag(heightFlag, heightLongFlag, MSyntax::kDouble);
	syntax.addFlag(weightFlag, weightLongFlag, MSyntax::kDouble);
	return syntax;
}

MStatus MorphShape::doIt( const MArgList& args ){

	MArgDatabase argData( syntax(), args ); //get the database of the arguments 调用一个command
	MString filename;
	double height = 0.0;
	double weight = 0.0;

	if(argData.isFlagSet( fileFlag ) ) //数据库有没有提供这个参数 -f，有-f就取出-f后面跟着的值
		argData.getFlagArgument( fileFlag, 0, filename ); // -f , filename 

	if (argData.isFlagSet(heightFlag))
		argData.getFlagArgument(heightFlag, 0, height);

	if (argData.isFlagSet(weightFlag))
		argData.getFlagArgument(weightFlag, 0, weight);

	cout << "height" << height << endl;
	cout << "weight" << weight << endl;

	


	return MStatus::kSuccess;
}