#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::MatrixXi;

void ReadObj( std::string filename, MatrixXd &ret_vertices, MatrixXi &ret_faces );
void WriteObj( std::string filename, MatrixXd &vertices, MatrixXi &faces );

void EncodeRelativeRotation( MatrixXd &vertices, MatrixXi &faces, MatrixXd &temp, MatrixXi &neighbors );
void DecodeRelativeRotation( MatrixXd &input, MatrixXi &neighbors );

extern MatrixXd ToRotVec(const MatrixXd &R);
extern MatrixXd FromRotVec(MatrixXd &r);
MatrixXd Ortho(MatrixXd &R, int start = 1);
