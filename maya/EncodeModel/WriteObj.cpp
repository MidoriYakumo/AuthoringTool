#include "WriteObj.h"

#include<fstream>
using std::fstream;

#include<ios>
using std::ios;

#include "EncodeModel.h"

void WriteObj( std::string filename, MatrixXd &vertices, MatrixXi &faces ){

}

//#include "mex.h"
//#include "matrix.h"
//
//#define filename	prhs[0]
//#define model		prhs[1]
//#define faces		prhs[2]
//#define normals		prhs[3]
//#define uvcoords	prhs[4]
//
//#define sqr(a)		((a) * (a))
//
//void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
//{
//	mwSize num;
//	double *d;
//	char *name;
//	FILE *fp;
//	unsigned i;
//
//	if (nrhs < 2)
//		mexErrMsgTxt("Two input argument required.");
//	else if (nlhs > 5)
//		mexErrMsgTxt("Too many output arguments. (> 4)");
//
//	name = mxArrayToString(filename);
//	fp = fopen(name, "w");
//	if (!fp)
//		mexErrMsgTxt("could not open file");
//	mxFree(name);
//
//	num = mxGetM(model);
//	d = mxGetPr(model);
//	for (i = 0; i < num; i++)
//		fprintf(fp, "v %f %f %f\n", d[i], d[i + num], d[i + num * 2]);
//
//	if (nrhs >= 5) {
//		num = mxGetM(uvcoords);
//		d = mxGetPr(uvcoords);
//		for (i = 0; i < num; i++)
//			fprintf(fp, "vt %f %f\n", d[i], d[i + num]);
//	}
//
//	if (nrhs >= 4) {
//		num = mxGetM(normals);
//		d = mxGetPr(normals);
//		for (i = 0; i < num; i++)
//			fprintf(fp, "vn %f %f %f\n", d[i], d[i + num], d[+num * 2]);
//	}
//
//	if (nrhs >= 3) {
//		num = mxGetM(faces);
//		d = mxGetPr(faces);
//		for (i = 0; i < num; i++)
//			fprintf(fp, "f %d %d %d\n", (int)d[i], (int)d[i + num], (int)d[i + num * 2]);
//	}
//
//	fclose(fp);
//
//	return;
//
//#if 0
//	//fp = fopen(mxGet
//	num = mxGetM(model);
//
//	if (mxGetM(projection) != 3 || mxGetN(projection) != 4)
//		mexErrMsgTxt("projection matrix must be 3 x 4");
//
//	count = mxCreateDoubleMatrix(1, 1, mxREAL);
//	plhs[0] = count;
//	cnt = mxGetPr(count);
//
//	proj = mxGetPr(projection);
//	m = mxGetPr(model);
//	f = mxGetPr(faces);
//	c1 = (unsigned char *)mxGetPr(image);
//
//	size.width = mxGetN(image);
//	size.height = mxGetM(image);
//	IplImage *img = cvCreateImage(size, IPL_DEPTH_8U, 1);
//	cvZero(img);
//
//	//	mexPrintf("w %d h %d, step %d numfaces %d\n", size.width, size.height, img->widthStep, numfaces);
//	//	for(i = 0; i < 10; i++)
//	//		mexPrintf("f[%d] %f\n", i, f[i]);
//
//	for (i = 0; i < numfaces; i++) {
//		int v = (int)(f[i] - 1);
//		double w = proj[2] * m[v] + proj[5] * m[v + numpoints] + proj[8] * m[v + numpoints * 2] + proj[11];
//		pts[0].x = (int)((proj[0] * m[v] + proj[3] * m[v + numpoints] + proj[6] * m[v + numpoints * 2] + proj[9]) / w + 0.5);
//		pts[0].y = (int)((proj[1] * m[v] + proj[4] * m[v + numpoints] + proj[7] * m[v + numpoints * 2] + proj[10]) / w + 0.5);
//		v = (int)(f[i + numfaces] - 1);
//		w = proj[2] * m[v] + proj[5] * m[v + numpoints] + proj[8] * m[v + numpoints * 2] + proj[11];
//		pts[1].x = (int)((proj[0] * m[v] + proj[3] * m[v + numpoints] + proj[6] * m[v + numpoints * 2] + proj[9]) / w + 0.5);
//		pts[1].y = (int)((proj[1] * m[v] + proj[4] * m[v + numpoints] + proj[7] * m[v + numpoints * 2] + proj[10]) / w + 0.5);
//		v = (int)(f[i + numfaces * 2] - 1);
//		w = proj[2] * m[v] + proj[5] * m[v + numpoints] + proj[8] * m[v + numpoints * 2] + proj[11];
//		pts[2].x = (int)((proj[0] * m[v] + proj[3] * m[v + numpoints] + proj[6] * m[v + numpoints * 2] + proj[9]) / w + 0.5);
//		pts[2].y = (int)((proj[1] * m[v] + proj[4] * m[v + numpoints] + proj[7] * m[v + numpoints * 2] + proj[10]) / w + 0.5);
//
//		cvFillConvexPoly(img, pts, 3, CV_RGB(0xFF, 0xFF, 0xFF), 8, 0);
//	}
//
//	//	cvSaveImage("/HPS/mocap2/nobackup/nils/hutrack/s2/test.png", img);
//
//	c2 = (unsigned char *)img->imageData;
//
//	if (nlhs == 1) {
//		for (i = 0; i < size.height; i++) {
//			for (col = 0; col < size.width; col++) {
//				//			c1[i + col * size.height] = c2[i * img->widthStep + col];
//				if (c1[i + col * size.height] != c2[i * img->widthStep + col]) {
//					c++;
//				}
//			}
//		}
//	}
//	else {
//		mxArray *out = mxCreateNumericMatrix(size.height, size.width, mxUINT8_CLASS, mxREAL);
//		unsigned char *o = mxGetPr(out);
//		plhs[1] = out;
//
//		for (i = 0; i < size.height; i++) {
//			for (col = 0; col < size.width; col++) {
//				o[i + col * size.height] = c1[i + col * size.height];
//				if (c1[i + col * size.height] != c2[i * img->widthStep + col]) {
//					o[i + col * size.height] = 128;
//					c++;
//				}
//			}
//		}
//	}
//
//	cvReleaseImage(&img);
//
//	*cnt = c;
//#endif
//}

/* compile line
*
* mex -largeArrayDims -I/HPS/home-0/hasler/local/include -L/HPS/home-0/hasler/local/lib -lcv -lcxcore silerror.c
*
*/

