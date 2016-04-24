#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <Eigen/Sparse>
using Eigen::SparseMatrix;
using Eigen::Triplet;
using Eigen::SparseSparseProduct;
using Eigen::SparseLU;
using Eigen::SimplicialLDLT;

#include <Eigen/Dense>
using Eigen::Vector3d;
using Eigen::Matrix3d;
using Eigen::MatrixXd;
using Eigen::MatrixXi;

#include <Eigen/SVD>
using Eigen::JacobiSVD;

#include <cmath>
using std::atan2;
using std::sin;
using std::cos;

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::fstream;

#include <ios>
using std::ios;

void ReadObj( std::string filename, MatrixXd &ret_vertices, MatrixXi &ret_faces );
void WriteObj( std::string filename, MatrixXd &vertices, MatrixXi &faces );

MatrixXd EncodeRelativeRotation( MatrixXd &model, MatrixXi &faces, MatrixXd &temp, MatrixXi &neigh);
MatrixXd DecodeRelativeRotation( MatrixXd &input, MatrixXi &neigh );
MatrixXd DecodeTranslation( MatrixXd &features, MatrixXi &faces, MatrixXd &temp );

Vector3d ToRotVec(Matrix3d R);
Matrix3d FromRotVec(Vector3d r);
MatrixXd Ortho( MatrixXd R, int start );
MatrixXd LoadTemplate();
MatrixXi LoadNeighbor();
MatrixXd LoadC();

void MySM3Block(vector< Triplet< double > > &vtd, int row, int col, Matrix3d &dm3);

 // GenSemBasis( MatrixXd subjects, MatrixXd semvals);
MatrixXd PInv(MatrixXd &PInvmat);
MatrixXd Estsem(MatrixXd obj, MatrixXd means, MatrixXd B, MatrixXd K);
MatrixXd MorphTo(MatrixXd start, MatrixXd target, MatrixXd subjects, MatrixXd semvals);
MatrixXd Upright( MatrixXd &in );
MatrixXd DoPCA( MatrixXd &in );

class EncodeModel{
public:
	
	void LoadFaces();
	void LoadNeigh();
	void LoadReconmean();
	void LoadC();
	void LoadCoeffs();
	void LoadAvg();
	void LoadProjected();
	void LoadSemdata();

	MatrixXi faces;
	MatrixXi neigh;
	MatrixXd reconmean;
	MatrixXd C;
	MatrixXd coeffs;
	MatrixXd avg;
	MatrixXd projected;
	MatrixXd semdata;
};