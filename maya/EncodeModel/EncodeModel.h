#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

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

#include <fstream>
using std::fstream;

#include <ios>
using std::ios;

void ReadObj( std::string filename, MatrixXd &ret_vertices, MatrixXi &ret_faces );
void WriteObj( std::string filename, MatrixXd &vertices, MatrixXi &faces );

MatrixXd EncodeRelativeRotation(MatrixXd model, MatrixXd faces, MatrixXd temp, MatrixXd neigh);
MatrixXd DecodeRelativeRotation( MatrixXd input, MatrixXi neigh );

Vector3d ToRotVec(Matrix3d R);
Matrix3d FromRotVec(Vector3d r);
MatrixXd Ortho( MatrixXd R, int start );

MatrixXd LoadTemplate();
MatrixXd LoadNeighbor();