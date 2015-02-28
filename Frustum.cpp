#include "frustum.h"

void CFrustum::Compute(CViewport *pViewport)
{
    CMatrix mat, MatView, MatProj;

	// Get the Projection and view matricies
	pViewport->GetProjection(MatProj);
	pViewport->GetView(MatView);

	// Multiply em to convert to clip space
	mat = CMatrix::Multiply(MatView, MatProj);

	m_Planes[LEFT].n.x		= mat.e[0][3] + mat.e[0][0];
	m_Planes[LEFT].n.y		= mat.e[1][3] + mat.e[1][0];
	m_Planes[LEFT].n.z		= mat.e[2][3] + mat.e[2][0];
	m_Planes[LEFT].d		= mat.e[3][3] + mat.e[3][0];
	m_Planes[LEFT].Normalise();

	m_Planes[RIGHT].n.x		= mat.e[0][3] - mat.e[0][0];
	m_Planes[RIGHT].n.y		= mat.e[1][3] - mat.e[1][0];
	m_Planes[RIGHT].n.z		= mat.e[2][3] - mat.e[2][0];
	m_Planes[RIGHT].d		= mat.e[3][3] - mat.e[3][0];
	m_Planes[RIGHT].Normalise();

	m_Planes[BOTTOM].n.x	= mat.e[0][3] + mat.e[0][1];
	m_Planes[BOTTOM].n.y	= mat.e[1][3] + mat.e[1][1];
	m_Planes[BOTTOM].n.z	= mat.e[2][3] + mat.e[2][1];
	m_Planes[BOTTOM].d		= mat.e[3][3] + mat.e[3][1];
	m_Planes[BOTTOM].Normalise();

	m_Planes[TOP].n.x		= mat.e[0][3] - mat.e[0][1];
	m_Planes[TOP].n.y		= mat.e[1][3] - mat.e[1][1];
	m_Planes[TOP].n.z		= mat.e[2][3] - mat.e[2][1];
	m_Planes[TOP].d			= mat.e[3][3] - mat.e[3][1];
	m_Planes[TOP].Normalise();

	m_Planes[FRONT].n.x		= mat.e[0][3] + mat.e[0][2];
	m_Planes[FRONT].n.y		= mat.e[1][3] + mat.e[1][2];
	m_Planes[FRONT].n.z		= mat.e[2][3] + mat.e[2][2];
	m_Planes[FRONT].d		= mat.e[3][3] + mat.e[3][2];
	m_Planes[FRONT].Normalise();

	m_Planes[BACK].n.x		= mat.e[0][3] - mat.e[0][2];
	m_Planes[BACK].n.y		= mat.e[1][3] - mat.e[1][2];
	m_Planes[BACK].n.z		= mat.e[2][3] - mat.e[2][2];
	m_Planes[BACK].d		= mat.e[3][3] - mat.e[3][2];
	m_Planes[BACK].Normalise();
}

BOOL CFrustum::PointIsInside(const CVector &Point) const
{
	float d;

	d = m_Planes[FRONT].DistanceFromPoint(Point);
	if(d < 0)
		return false;
	
	d = m_Planes[BACK].DistanceFromPoint(Point);
	if(d < 0)
		return false;

	d = m_Planes[LEFT].DistanceFromPoint(Point);
	if(d < 0)
		return false;

	d = m_Planes[RIGHT].DistanceFromPoint(Point);
	if(d < 0)
		return false;

	d = m_Planes[TOP].DistanceFromPoint(Point);
	if(d < 0)
		return false;

	d = m_Planes[BOTTOM].DistanceFromPoint(Point);
	if(d < 0)
		return false;

	// Point must be inside if we reached here
	return true;
}

BOOL CFrustum::SphereIsInside(const CVector &Point, const float Radius) const
{
	float d;

	d = m_Planes[FRONT].DistanceFromPoint(Point);
	if(d < -Radius)
		return false;
	
	d = m_Planes[BACK].DistanceFromPoint(Point);
	if(d < -Radius)
		return false;

	d = m_Planes[LEFT].DistanceFromPoint(Point);
	if(d < -Radius)
		return false;

	d = m_Planes[RIGHT].DistanceFromPoint(Point);
	if(d < -Radius)
		return false;

	d = m_Planes[TOP].DistanceFromPoint(Point);
	if(d < -Radius)
		return false;

	d = m_Planes[BOTTOM].DistanceFromPoint(Point);
	if(d < -Radius)
		return false;

	// Sphere must be inside if we reached here
	return true;
}