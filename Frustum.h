#ifndef _cfrustum_h
#define _cfrustum_h

#include "plane.h"
#include "viewport.h"

__declspec(align(16)) class CFrustum
{
	CPlane m_Planes[6];
	CVector m_vVectors[8];

public:
	enum PLANESIDES
	{
		FRONT = 0,
		BACK,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
	};		

	CFrustum() {};

	void Compute(CViewport *pViewport);
	BOOL PointIsInside(const CVector &Point) const;
	BOOL SphereIsInside(const CVector &Point, const float Radius) const;
};

#endif