#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <Eigen/Geometry> 
#include <vector>

using Eigen::MatrixXd;

MatrixXd EncodeRelativeRotation(MatrixXd model, MatrixXd faces, MatrixXd temp, MatrixXd neigh);