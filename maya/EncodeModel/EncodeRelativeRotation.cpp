
#include "EncodeModel.h"

//---model: vertex coordinates of input model---
//---faces: indices of vertices forming mesh triangles---
//---temp: vertex coordinates template model---
//---neigh: indices of neighboring faces---
MatrixXd EncodeRelativeRotation( MatrixXd model, MatrixXd faces, MatrixXd temp, MatrixXd neigh )
{
	//15 degrees of freedom for each face
	int step = 15; 
	//row vector storing encoded model
	MatrixXd result = MatrixXd::Zero( 1, faces.rows() * step ); 

	vector< Eigen::Matrix3d > Rs( faces.rows() );
	int at = 0;

	for (int i = 0; i < faces.rows(); ++i)
	{
		//--handle each face, row vector---
		Matrix3d V = faces.block< 1, 3 >( i, 0 );

		//---origin of current face on template model picked from three vertices---
		Vector3d Origin = temp.block( V( 0, 0 ), 0, 1, 3 ).transpose();//<1, temp.cols()>(temp(V[0], 0));

		//---column vector of one edge of current face on template model---
		Vector3d E1 = temp.block( V( 0, 1 ), 0, 1, 3 ).transpose() - Origin;

		//---column vector of the other edge of current face on template model---
		Vector3d E2 = temp.block( V( 0, 2 ), 0, 1, 3 ).transpose() - Origin;

		//---normal vector of current face on template model---
		Vector3d N = E1.cross( E2 ).normalized();

		//---matrix storing first edge, second edge, and normal on template model---
		Matrix3d Src(3, 3);
		Src.block< 3, 1 >( 0, 0 ) = E1;
		Src.block< 3, 1 >( 0, 1 ) = E2;
		Src.block< 3, 1 >( 0, 2 ) = N;

		//---origin coordinates of current face on input model---
		MatrixXd T0 = model.block< 1, 3 >( V( 0, 0 ), 0 ).transpose(); 

		//---column vector of one edge of current face on input model---
		MatrixXd T1 = model.block< 1, 3 >( V( 0, 1 ), 0 ).transpose() - T0;

		//---column vector of the other edge of current face on input model---
		MatrixXd T2 = model.block< 1, 3 >( V( 0, 2 ), 0 ).transpose() - T0;

		//---normal vector of current face on input model---
		Vector3d Tn = T1.cross( T2 ).normalized();

		//---matrix storing first edge, second edge, and normal on input model---
		Matrix3d Tgt(3, 3);
		Tgt.block< 3, 1 >( 0, 0 ) = T1;
		Tgt.block< 3, 1 >( 0, 1 ) = T2;
		Tgt.block< 3, 1 >( 0, 2 ) = Tn;

		Matrix3d T = Tgt * Src.inverse();
		JacobiSVD< Matrix3d > svd( T, Eigen::ComputeThinU | Eigen::ComputeThinV );
		Matrix3d U1 = svd.matrixU();
		Matrix3d V1 = svd.matrixV();
		Matrix3d S1 = Matrix3d::Zero();
		Vector3d sv = svd.singularValues();
		Matrix3d R = U1 * V1.transpose();

		if (R.determinant() < 0) {
			R = R * -1.0;
		}

		Rs[ i ] = R;
		S1 = R.inverse() * T;
		at = at + 9;

		result(0, at) = S1(0, 0);
		result(0, at + 1) = S1(0, 1);
		result(0, at + 2) = S1(0, 2);
		result(0, at + 3) = S1(1, 1);
		result(0, at + 4) = S1(1, 2);
		result(0, at + 5) = S1(2, 2);

		at = at + 6;
	}

	for (int i = 0; i < faces.rows(); i++)
	{
		Matrix3d delta = Rs[i].inverse() * Rs[neigh(i, 0)];
		result.block< 1, 3 >( 0, i * step ) = ToRotVec( delta );
		delta = Rs[i].inverse() * Rs[neigh(i, 1)];
		result.block< 1, 3 >( 0, i * step + 3 ) = ToRotVec( delta );
		delta = Rs[i].inverse() * Rs[neigh(i, 2)];
		result.block< 1, 3 >( 0, i * step + 6 ) = ToRotVec( delta );
	}

	return result;
}