
#include "EncodeModel.h"

MatrixXd DecodeTranslation( MatrixXd &features, MatrixXi &faces, MatrixXd &temp ){
	
	SparseMatrix< double > M( ( int )temp.rows() + 1, ( int )temp.rows() );
	vector< Triplet< double > > vtd;
	MatrixXd dx = MatrixXd::Zero( temp.rows() + 1, 3 );

	M.reserve( 10 * ( int )temp.rows() );

	for( int i = 0; i < faces.rows(); ++i ){

		int at = i * 10;
		int v[ 3 ] = { faces( i, 0 ), faces( i, 1 ), faces( i, 2 ) };
		Matrix3d R = FromRotVec( features.block< 1, 3 >( 0, at ) );
		Matrix3d S = Matrix3d::Zero( 3, 3 ); 
		Matrix3d T;
		
		S.block< 1, 3 >( 0, 0 ) = features.block< 1, 3 >( 0, at + 4 );
		S.block< 3, 1 >( 0, 0 ) = features.block< 1, 3 >( 0, at + 4 );
		S.block< 1, 2 >( 1, 1 ) = features.block< 1, 2 >( 0, at + 7 );
		S.block< 2, 1 >( 1, 1 ) = features.block< 1, 2 >( 0, at + 7 );
		S( 2, 2 ) = features( at + 9 );
		T = R * S;
		
		Matrix3d tri;
		Matrix3d x;

		tri.block< 3, 1 >( 0, 0 ) = temp.block< 1, 3 >( v[ 0 ], 0 );
		tri.block< 3, 1 >( 0, 1 ) = temp.block< 1, 3 >( v[ 1 ], 0 );
		tri.block< 3, 1 >( 0, 2 ) = temp.block< 1, 3 >( v[ 2 ], 0 );
		x = T * tri;

		Vector3d p12 = x.col( 0 ) - x.col( 1 ); p12.normalize();
		Vector3d p13 = x.col( 0 ) - x.col( 2 ); p13.normalize();
		Vector3d p23 = x.col( 1 ) - x.col( 2 ); p23.normalize();
		Vector3d wts = { p13.dot( p23 ), -p12.dot( p23 ), p12.dot( p13 ) };
		Matrix3d wij3, _wij3; 
		MatrixXd WIJ;
		
		wij3 << 0, wts( 0 ) / sqrt( 1 - wts( 0 ) * wts( 0 ) ), 
			wts( 1 ) / sqrt( 1 - wts( 1 ) * wts( 1 ) ), 0, 0, 
			wts( 2 ) / sqrt( 1 - wts( 2 ) * wts( 2 ) ), 0, 0, 0;

		_wij3 = wij3;
		WIJ = wij3 = _wij3 + _wij3.transpose();
		WIJ( 0, 0 ) -= wij3.col( 0 ).sum();
		WIJ( 1, 1 ) -= wij3.col( 1 ).sum();
		WIJ( 2, 2 ) -= wij3.col( 2 ).sum();

		for( int j = 0; j < 3; ++j ){
			for( int k = 0; k < 3; ++k ){
				vtd.push_back( Triplet< double >( v[ j ], v[ k ], WIJ( j, k ) ) );
			}
		}

		Matrix3d tmp = WIJ * x.transpose();

		for( int j = 0; j < 3; ++j ){
			dx.row( v[ j ] ) = dx.row( v[ j ] ) + tmp.row( j );
		}
	}

	vtd.push_back( Triplet< double >( ( int )temp.rows(), 0, 1. ) );
	M.setFromTriplets( vtd.begin(), vtd.end() );
	
	SparseMatrix< double > Mt( M.transpose() );
	auto MtM( Mt * M );
	SimplicialLDLT< SparseMatrix< double > > solver;
	MatrixXd result;
	
	cout << "computing Mt..." << endl;
	solver.compute( MtM );
	cout << "solving for Mt * dx..." << endl;
	result = solver.solve( Mt * dx );

	return result;
}
