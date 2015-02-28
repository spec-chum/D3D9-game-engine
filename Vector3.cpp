#include "vector3.h"

#ifdef _DEBUG
	#include "vector3.inl"
#endif

CVector (*CVector::Negate)(const CVector &v) = NULL;
CVector (*CVector::Multiply)(const CVector &v, const float Scalar) = NULL;
CVector (*CVector::Divide)(const CVector &v, const float Scalar) = NULL;
CVector (*CVector::Add)(const CVector &v1, const CVector &v2) = NULL;
CVector (*CVector::Subtract)(const CVector &v1, const CVector &v2) = NULL;
CVector (*CVector::Cross)(const CVector &v1, const CVector &v2) = NULL;
float (*CVector::Dot)(const CVector &v1, const CVector &v2) = NULL;
float (*CVector::DotW)(const CVector &v1, const CVector &v2) = NULL;
float (*CVector::SqrMagnitude)(const CVector &v) = NULL;
float (*CVector::Magnitude)(const CVector &v) = NULL;
