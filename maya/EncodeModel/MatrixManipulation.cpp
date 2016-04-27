// conversion between rotation matrix and rotation vector representation
#include "EncodeModel.h"

/*
function r = torotvec(R)

RRt = R - R';
r = [RRt(2,3);RRt(3,1);RRt(1,2)];
len = norm(r);
if len == 0
return
end
r = r * (atan2(len, R(1,1)+R(2,2)+R(3,3)-1) / len);
*/

Vector3d ToRotVec(Matrix3d R)
{
	Matrix3d RRt = R - R.transpose();
	Vector3d r = { RRt( 1, 2 ), RRt( 2, 0 ), RRt( 0, 1 ) };
	double len = r.norm();

	if ( len == 0 ){
		return Vector3d::Zero();
	}

	r = r * ( atan2( len, R( 0, 0 ) + R( 1, 1 ) + R( 2, 2 ) - 1 ) / len );

	return r;
}

/*
R = fromrotvec(r)
convert rotation vector into rotation matrix
function R = fromrotvec(r)

if r == zeros(3,1)
R = eye(3);
else
theta = norm(r);
r = -r / theta;

R = [ 0    -r(3)  r(2); ...
r(3)  0    -r(1); ...
-r(2)  r(1)  0  ];
R = R * sin(theta) + (R * R) * (1 - cos(theta)) + eye(3);
end
*/

Matrix3d FromRotVec( Vector3d r )
{
	Vector3d A = { 0., 0., 0. };
	Matrix3d R = Matrix3d::Identity();

	if ( r != A ){

		double theta = r.norm();

		r = -r / theta;
		R << 0, -r( 2 ), r( 1 ), r( 2 ), 0, -r( 0 ), -r( 1 ), r( 0 ), 0;
		R = R * sin( theta ) + ( R * R ) * ( 1 - cos( theta ) ) + Matrix3d::Identity();
	}

	return R;
}
	 
void MySM3Block( vector< Triplet< double > > &vtd, int row, int col, Matrix3d &dm3 ){

	for( int i = 0; i < 3; ++i ){
		for( int j = 0; j < 3; ++j ){
			if( abs( dm3( i, j ) ) < 1e-6 ) continue;
			vtd.push_back( Triplet< double >( row + i, col + j, dm3( i, j ) ) );
		}
	}
}