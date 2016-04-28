#include "BodyReshaper.h"
#include "../EncodeModel/EncodeModel.h"

#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <fstream>
#include <ios>

EncodeModel MorphShape::em;
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

	if (argData.isFlagSet(fileFlag)) //数据库有没有提供这个参数 -f，有-f就取出-f后面跟着的值
		argData.getFlagArgument(fileFlag, 0, filename); // -f , filename 

	if (argData.isFlagSet(heightFlag))
		argData.getFlagArgument(heightFlag, 0, height);

	if (argData.isFlagSet(weightFlag))
		argData.getFlagArgument(weightFlag, 0, weight);

	cout << "height" << height << endl;
	cout << "weight" << weight << endl;
	 
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

	//---read model---
	cout << "Reading OBJ..." << endl;
	ReadObj("../s1p4.obj", vertices, faces);

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
	target = MatrixXd(1, 2);
	target << weight, height;
	semdata = MatrixXd(1064, 2);
	semdata.col(0) = em.semdata.col(5);
	semdata.col(1) = em.semdata.col(2);
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
	WriteObj("../s1p4_out.obj", modelout, faces);

	return MStatus::kSuccess;
}