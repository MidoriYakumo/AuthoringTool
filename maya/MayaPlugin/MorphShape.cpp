#include "BodyReshaper.h"
#include "../EncodeModel/EncodeModel.h"

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <fstream>
#include <ios>

#pragma comment( lib, "../MayaPlugin/x64/Release/EncodeModel.lib" )

int MorphShape::id = 0;
EncodeModel MorphShape::em;
string MorphShape::pluginPath;
const char *MorphShape::heightFlag = "-h";
const char *MorphShape::heightLongFlag = "-height";
const char *MorphShape::weightFlag = "-w";
const char *MorphShape::weightLongFlag = "-weight";
const char *MorphShape::fileFlag = "-f";
const char *MorphShape::fileLongFlag = "-file";
const char *MorphShape::genderFlag = "-g";
const char *MorphShape::genderLongFlag = "-gender";
const char *MorphShape::breastFlag = "-bg";
const char *MorphShape::breastLongFlag = "-breastgirth";
const char *MorphShape::waistFlag = "-wg";
const char *MorphShape::waistLongFlag = "-waistgirth";
const char *MorphShape::hipFlag = "-hg";
const char *MorphShape::hipLongFlag = "-hipgirth";

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
	syntax.addFlag(genderFlag, genderLongFlag, MSyntax::kDouble);
	syntax.addFlag(breastFlag, breastLongFlag, MSyntax::kDouble);
	syntax.addFlag(waistFlag, waistLongFlag, MSyntax::kDouble);
	syntax.addFlag(hipFlag, hipLongFlag, MSyntax::kDouble);
	return syntax;
}

MStatus MorphShape::doIt( const MArgList& args ){

	MArgDatabase argData( syntax(), args ); //get the database of the arguments 调用一个command
	MString filename;
	double height = 0.0;
	double weight = 0.0;
	double gender = 1.;
	double breast = 1000.;
	double waist = 800.;
	double hip = 1000.;

	if (argData.isFlagSet(fileFlag)) //数据库有没有提供这个参数 -f，有-f就取出-f后面跟着的值
		argData.getFlagArgument(fileFlag, 0, filename); // -f , filename 

	if (argData.isFlagSet(heightFlag))
		argData.getFlagArgument(heightFlag, 0, height);

	if (argData.isFlagSet(weightFlag))
		argData.getFlagArgument(weightFlag, 0, weight);

	if (argData.isFlagSet(genderFlag))
		argData.getFlagArgument(genderFlag, 0, gender);

	if (argData.isFlagSet(breastFlag))
		argData.getFlagArgument(breastFlag, 0, breast);

	if (argData.isFlagSet(waistFlag))
		argData.getFlagArgument(waistFlag, 0, waist);

	if (argData.isFlagSet(hipFlag))
		argData.getFlagArgument(hipFlag, 0, hip);

	cout << "Gender: " << ( gender < .5 ? "Female" : "Male" ) << endl;
	cout << "Height: " << height << endl;
	cout << "Weight: " << weight << endl;
	cout << "Breast Girth: " << breast << endl;
	cout << "Waist Girth: " << waist << endl;
	cout << "Hip Girth " << hip << endl;
	 
	fstream fout("out.txt", ios::out);
	MatrixXd vertices;
	MatrixXi faces;
	MatrixXd encoded;
	MatrixXd decodedrot;
	MatrixXd modelout;
	MatrixXd projected;
	MatrixXd morphed;
	MatrixXd unprojected;
	MatrixXd target;
	MatrixXd semdata;
	EncodeModel em;

	em.LoadReconmean();
	em.LoadNeigh();
	em.LoadAvg();
	em.LoadProjected();
	em.LoadSemdata();

	//cout << "Model: " << filename.asChar() << endl;

	//---read model---
	cout << "Reading OBJ..." << endl;
	ReadObj( filename.asChar(), vertices, faces );

	//---encode---
	cout << "Encoding..." << endl;
	encoded = EncodeRelativeRotation(vertices, faces, em.reconmean, em.neigh);

	//---project into PCA space---
	cout << "Projecting..." << endl;
	em.LoadC();
	projected = em.C * (encoded.transpose() - em.avg);
	em.C.resize(0, 0);

	//---morph to---
	cout << "Morphing..." << endl;
	target = MatrixXd(1, 6);
	target << gender, weight, height, breast, waist, hip;
	semdata = MatrixXd(1064, 6);
	semdata.col(0) = em.semdata.col(1);
	semdata.col(1) = em.semdata.col(5);
	semdata.col(2) = em.semdata.col(2);
	semdata.col(3) = em.semdata.col(16);
	semdata.col(4) = em.semdata.col(17);
	semdata.col(5) = em.semdata.col(18);
	morphed = MorphTo(projected, target, em.projected, semdata);

	//---unproject from PCA space---
	cout << "Unprojecting..." << endl;
	em.LoadCoeffs();
	unprojected = em.coeffs * morphed + em.avg;
	em.coeffs.resize(0, 0);

	//---decode---
	cout << "Decoding..." << endl;
	decodedrot = DecodeRelativeRotation(unprojected, em.neigh);
	modelout = DecodeTranslation(decodedrot, faces, em.reconmean);

	//---correct model rotation---
	modelout = Upright(modelout);

	//---write model---
	cout << "Writing OBJ..." << endl;
	WriteObj( MorphShape::pluginPath + "/../out1.obj", modelout, faces );

	MString command;
	
	command = command + "file -import -type \"OBJ\" -namespace \"body"
		+ MorphShape::id + "\" -pr \"" + MorphShape::pluginPath.c_str()
		+ "/../out1.obj\";" + "setAttr \"body" + MorphShape::id 
		+ ":Mesh.rotateX\" -90; select -r body" + MorphShape::id 
		+ ":Mesh; setAttr \"body" + MorphShape::id + ":Mesh.translateX\" " 
		+ ( MorphShape::id + 1. ) + ";";
	++MorphShape::id;
	MGlobal::executeCommand( command );

	return MStatus::kSuccess;
}

void MorphShape::RotateX( MatrixXd &vertices ){

	for( int i = 0; i < vertices.rows(); ++i ){

		double tmp = vertices( i, 1 );

		vertices( i, 1 ) = vertices( i, 2 );
		vertices( i, 2 ) = -tmp;
	}
}