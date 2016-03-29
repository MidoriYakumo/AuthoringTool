/*
* Copyright 2009 (c) Nils Hasler hasler@mpi-inf.mpg.de
* please cite
* N. Hasler, C. Stoll, M. Sunkel, B. Rosenhahn, H.-P. Seidel: A Statistical Model of Human Pose and Body Shape,
* Computer Graphics Forum (Proc. Eurographics 2009), Munich, Germany, March 2009.
*/
#include <math.h>
#include "mex.h"

#define input       prhs[0]
#define faces       prhs[1]
#define template	prhs[2]
#define neigh       prhs[3]

#define sqr(a)      ((a) * (a))

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	mwSize N, N2;
	double *n, *in, *tris, *initpoints, *data;

	if (nrhs < 4)
		mexErrMsgTxt("Four input arguments required.");
	else if (nrhs > 5)
		mexErrMsgTxt("More than 5 input arguments.");
	else if (nlhs > 2)
		mexErrMsgTxt("Too many output arguments.");
	else if (nlhs == 0)
		mexErrMsgTxt("No output argument.");

	N = mxGetM(neigh);
	N2 = 2 * N;

	in = mxGetPr(input);
	tris = mxGetPr(faces);
	initpoints = mxGetPr(template);
	n = mxGetPr(neigh);

	A = mxCreateDoubleMatrix(N * 15, 1, mxREAL);
	data = mxGetPr(A);

	for (unsigned i = 0; i < N; i++) {
		/*triangle<float> *t = &triangles[i];*/
		unsigned t[3] = { tris[i], tris[N + i], tris[N2 + i] };

		src[0] = initpoints[t[1]] - initpoints[t[0]];
		src[1] = initpoints[t[1] + N] - initpoints[t[0] + N];
		src[2] = initpoints[t[1] + N2] - initpoints[t[0] + N2];
		src[3] = initpoints[t[2]] - initpoints[t[0]];
		src[4] = initpoints[t[2] + N] - initpoints[t[0] + N];
		src[5] = initpoints[t[2] + N2] - initpoints[t[0] + N2];
		/* cross product */
		src[6] = src[1] * src[5] - src[2] * src[4];
		src[7] = src[2] * src[3] - src[0] * src[5];
		src[8] = src[0] * src[4] - src[1] * src[3];
		/* normalise */
		double l = 1.0 / sqrt(sqr(src[6]) + sqr(src[7]) + sqr(src[8]));
		src[6] *= l; src[7] *= l; src[8] *= l;

		tgt[0] = points[t[1]] - points[t[0]];
		tgt[1] = points[t[1] + N] - points[t[0] + N];
		tgt[2] = points[t[1] + N2] - points[t[0] + N2];
		tgt[3] = points[t[2]] - points[t[0]];
		tgt[4] = points[t[2] + N] - points[t[0] + N];
		tgt[5] = points[t[2] + N2] - points[t[0] + N2];
		/* cross product */
		tgt[6] = tgt[1] * tgt[5] - tgt[2] * tgt[4];
		tgt[7] = tgt[2] * tgt[3] - tgt[0] * tgt[5];
		tgt[8] = tgt[0] * tgt[4] - tgt[1] * tgt[3];
		/* normalise */
		double l = 1.0 / sqrt(sqr(tgt[6]) + sqr(tgt[7]) + sqr(tgt[8]));
		tgt[6] *= l; tgt[7] *= l; tgt[8] *= l;

		m3x3<float> T = tgt / src;
		m3x3<float> R = T;
		R.polar();

		m3x3<float> S = R.solve(T);

		pt<float> rv = R.torotationvector();

		data[0] = rv.x;
		data[1] = rv.y;
		data[2] = rv.z;
		data[3] = 0;
		data[4] = S(0);
		data[5] = S(3);
		data[6] = S(6);
		data[7] = S(4);
		data[8] = S(7);
		data[9] = S(8);
	}

}
