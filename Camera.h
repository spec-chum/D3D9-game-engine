#ifndef _ccamera_h
#define _ccamera_h

#include "vector3.h"
#include "viewport.h"

class CCamera 
{
public:
	LPDIRECT3DDEVICE9 m_pDevice;
	CVector m_vPosition, m_vView, m_vUpVector;
	float m_fWalkVel, m_fStrafeVel;
	float m_fSpeed, m_fCurrentRotX;
	float m_fMaxVel;
	float m_fSensitivity;
	int m_Width, m_Height, m_MiddleX, m_MiddleY;
	int m_WalkDir, m_PrevWalkDir;
	int m_StrafeDir, m_PrevStrafeDir;
	BOOL m_bIsFlying;
	BOOL m_bMoved;

	void Rotate(); 
	virtual void Walk();	// default suitable for testing purposes, has inertia and velocity but no jump/duck etc

public:
	CCamera(const LPDIRECT3DDEVICE9 pDevice, const int Width, const int Height);
	CCamera() {};
	
	static const int STATIONARY;
	static const int FORWARDS;
	static const int BACKWARDS;
	static const int LEFT;
	static const int RIGHT;
	static const float ACCELERATION;

	CVector GetPosition()	const	{ return m_vPosition; };
	CVector GetView()		const	{ return m_vView; };
	CVector GetUpVector()	const	{ return m_vUpVector; };

	float GetSpeed() { return m_fSpeed; };
	int GetWalkDirection() { return m_WalkDir; };
	int GetStrafeDirection() { return m_StrafeDir; };
	BOOL IsFlying() { return m_bIsFlying; };
	BOOL Update(CViewport *pViewport);
	void SetSpeed(const float Speed) { m_fSpeed = Speed; };
	void SetVelocity(const float Vel);
	void SetFlying(BOOL Flying) { m_bIsFlying = Flying; };
	void SetWalkDirection(const int Dir) { m_WalkDir = Dir; };
	void SetStrafeDirection(const int Dir) { m_StrafeDir = Dir; };
	void Position(const CVector &osition, const CVector &View, const CVector &Up);
	void SetSensitivity(const float Sensitivity) { m_fSensitivity *= Sensitivity; };
};

#endif
