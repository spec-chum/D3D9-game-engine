#include "plane.h"
#include "matrix.h"

CPlane::CPlane(const CVector &point, const CVector &normal)
{
	n = normal;
	d = -(normal * point);
}

CPlane::CPlane(const CVector &point1, const CVector &point2, const CVector &point3)
{
	n = CVector::Cross(point2 - point1, point3 - point1);
	n.Normalise();

	d = -(n * point1);
}

float CPlane::DistanceFromPoint(const CVector &point) const
{
	return((n * point) + d);
}

void CPlane::Normalise()
{
	// No need to get sqrt here as RSqrt calls Rcp anyway, no point in calling it twice
	float magnitude = n.GetSqrMagnitude();

	// Avoid zero length, just return original plane if so
	if(CMaths::FComp(magnitude, 0))
		return;

	// Get the rcp square root so we can use a faster multiply
	magnitude = CMaths::RSqrt(magnitude);

	n *= magnitude;
	d *= magnitude;
}

CPlane CPlane::Normalise(const CPlane &p)
{
	// No need to get sqrt here as RSqrt calls Rcp anyway, no point in calling it twice
	float magnitude = p.n.GetSqrMagnitude();

	// Avoid zero length, just return original plane if so
	if(CMaths::FComp(magnitude, 0))
		return p;

	// Get the rcp square root so we can use a faster multiply
	magnitude = CMaths::RSqrt(magnitude);

	return(CPlane(p.n * magnitude, p.d * magnitude));
}

CPlane CPlane::Transform(const CPlane &plane, const CMatrix &mat)
{
	// Get the inverse of the input matrix
	CMatrix invtran = CMatrix::Inverse(mat);

	// Now multiply the plane (using vectors) by the inverse transposed matrix
	// Extract the values for the vectors from the matrix (transposed column major)
	CVector vX(invtran.e[0][0], invtran.e[0][1], invtran.e[0][2], invtran.e[0][3]);
	CVector vY(invtran.e[1][0], invtran.e[1][1], invtran.e[1][2], invtran.e[1][3]);
	CVector vZ(invtran.e[2][0], invtran.e[2][1], invtran.e[2][2], invtran.e[2][3]);
	CVector vW(invtran.e[3][0], invtran.e[3][1], invtran.e[3][2], invtran.e[3][3]);

	CVector vN = plane.n;

	// Make sure w == plane.d (needed for DotW to function correctly)
	vN.w = plane.d;

	// Use SSE optimised dot product
	return(CPlane(CVector(CVector::DotW(vN, vX), CVector::DotW(vN, vY), CVector::DotW(vN, vZ)), CVector::DotW(vN, vW)));
}