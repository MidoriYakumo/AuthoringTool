
#include "EncodeModel.h"

MatrixXd Upright( MatrixXd &in ){
	
	MatrixXd inT = in.transpose();
	MatrixXd p = DoPCA( inT );
	Matrix3d M;

	M.col( 0 ) = p.col( 1 );
	M.col( 1 ) = p.col( 2 );
	M.col( 2 ) = p.col( 0 );

	if( M.determinant() < 0 ){
		M = -1. * M;
	}

	MatrixXd out = in * M;
	int landmarks[] = { 82, 4241, 984, 3299, 1115 };

	if( out( landmarks[ 4 ], 2 ) < out( landmarks[ 3 ], 2 ) ){
		
		Matrix3d R;

		R << 1, 0, 0, 0, -1, 0, 0, 0, -1;
		M = M * R;
		out = in * M;
	}
	
	if( out( landmarks[ 1 ], 0 ) < out( landmarks[ 2 ], 0 ) ){
		
		Matrix3d R;

		R << -1, 0, 0, 0, -1, 0, 0, 0, 1;
		M = M * R;
		out = in * M;
	}

	if( out( landmarks[ 0 ], 1 ) > out( landmarks[ 4 ], 1 ) ){
		
		Matrix3d R;

		R << -1, 0, 0, 0, 1, 0, 0, 0, -1;
		M = M * R;
		out = in * M;
	}

	for( int i = 0; i < out.cols(); ++i ){

		double mean = out.col( i ).mean();

		for( int j = 0; j < out.rows(); ++j ){
			out( j, i ) -= mean;
		}
	}

	return out;
}
