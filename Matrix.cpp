#include "matrix.h"

// Don't inline if in debug build
#ifdef _DEBUG
	#include "matrix.inl"
#endif

CMatrix (*CMatrix::ZeroMatrix)() = NULL;
CMatrix (*CMatrix::Transpose)(const CMatrix &Matrix) = NULL;
CMatrix (*CMatrix::Multiply)(const CMatrix &LH, const CMatrix &RH) = NULL;


