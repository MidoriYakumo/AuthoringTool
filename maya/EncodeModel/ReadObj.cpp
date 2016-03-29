//#include<fstream>
//#include<string>
//#include<vector>

#include"ReadObj.h"

using std::fstream;
using std::string;
using std::vector;
using std::ios;

vector<vector<double>> vertices;
vector<vector<int>> faces;

void ReadObj(std::string filename)
{
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
}