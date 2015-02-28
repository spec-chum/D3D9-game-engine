#ifndef _cplane_h
#define _cplane_h

#include "vector3.h"

class CMatrix;

class CPlane
{
public:
	CVector n;
	float d;

	// Default constructor
	CPlane() {};

	// Explicit initialisation
	CPlane(const CVector &normal, const float distance) { n = normal; d = distance; };

	// Plane from point and normal
	CPlane(const CVector &point, const CVector &Normal);

	// Plane from 3 points
	CPlane(const CVector &point1, const CVector &point2, const CVector &point3);

	// Get distance from plane to a point
	float DistanceFromPoint(const CVector &point) const;

	// Normalises the plane
	void Normalise();

	// As above but a static function
	static CPlane Normalise(const CPlane &p);

	// Transforms the plain by the matrix
	static CPlane Transform(const CPlane &plane, const CMatrix &mat);
};

#endif