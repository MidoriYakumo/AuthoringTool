
#include <fstream>
using std::fstream;

#include <ios>
using std::ios;

#include "EncodeModel.h"

void ReadObj(std::string filename, MatrixXd &ret_vertices, MatrixXi &ret_faces )
{
	vector<vector<double>> vertices;
	vector<vector<int>> faces;
	double v1, v2, v3;
	int f1, f2, f3;
	fstream fin(filename, ios::in);
	string input;

	while (fin >> input)
	{
		if (input == "v" || input == "V")
		{
			fin >> v1 >> v2 >> v3;
			vertices.push_back(vector<double>());
			vertices.back().push_back(v1);
			vertices.back().push_back(v2);
			vertices.back().push_back(v3);
		}
		if (input == "f" || input == "F")
		{
			fin >> f1 >> f2 >> f3;
			faces.push_back(vector<int>());
			faces.back().push_back(f1);
			faces.back().push_back(f2);
			faces.back().push_back(f3);
		}
	}

	ret_vertices.resize( vertices.size(), 3 );
	for( int i = 0; i < vertices.size(); ++i ){
		ret_vertices( i, 0 ) = vertices[ i ][ 0 ];
		ret_vertices( i, 1 ) = vertices[ i ][ 1 ];
		ret_vertices( i, 2 ) = vertices[ i ][ 2 ];
	}

	ret_faces.resize( faces.size(), 3 );
	for( int i = 0; i < faces.size(); ++i ){
		ret_faces( i, 0 ) = faces[ i ][ 0 ];
		ret_faces( i, 1 ) = faces[ i ][ 1 ];
		ret_faces( i, 2 ) = faces[ i ][ 2 ];
	}
}