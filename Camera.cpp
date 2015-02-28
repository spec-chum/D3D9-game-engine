#include "camera.h"

// Set the constants
const int CCamera::STATIONARY = 0;
const int CCamera::FORWARDS = 1;
const int CCamera::BACKWARDS = 2;
const int CCamera::LEFT = 3;
const int CCamera::RIGHT = 4;
const float CCamera::ACCELERATION = 0.2f;

CCamera::CCamera(const LPDIRECT3DDEVICE9 pDevice, const int Width, const int Height)
{
	m_pDevice = pDevice;
	m_Width = Width;
	m_Height = Height;
	m_MiddleX = m_Width >> 1;
	m_MiddleY = m_Height >> 1;
	m_WalkDir = 0;
	m_StrafeDir = 0;
	m_fSpeed = 10.0f;			// Units per second
	m_fCurrentRotX = 0.0f;
	m_fWalkVel = 0;
	m_fStrafeVel = 0;
	m_fMaxVel = 1.0f;
	m_fSensitivity = 0.005f;
	m_bIsFlying = false;
	m_bMoved = false;

	// Set default values for camera's starting position
	m_vPosition	= CVector(0.0, 0.0, 0.0);
	m_vView		= CVector(0.0, 0.0, 1.0);
	m_vUpVector	= CVector(0.0, 1.0, 0.0);

	// *=HACK=* Offset the mouse slightly for the first frame, otherwise view vector doesn't get updated
	SetCursorPos(m_MiddleX - 1, m_MiddleY - 1);
}

void CCamera::Position(const CVector &Position, const CVector &View, const CVector &Up)
{
	m_vPosition	= Position;
	m_vView		= View;
	m_vUpVector	= Up;
}

void CCamera::Rotate()
{
	POINT MousePos;
	
	// Get mouse offset from centre of viewport
	GetCursorPos(&MousePos);

	// If mouse hasn't moved, no need to rotate anything, so just set moved to false and return
	if((MousePos.x == m_MiddleX) && (MousePos.y == m_MiddleY) ) 
	{
		m_bMoved = false;
		return;
	}

	// Reset mouse back to centre of viewport
	SetCursorPos(m_MiddleX, m_MiddleY);							

	const float AngleY = static_cast<float>(MousePos.x - m_MiddleX) * m_fSensitivity;		
	const float AngleX = static_cast<float>(m_MiddleY - MousePos.y) * m_fSensitivity;

	m_fCurrentRotX -= AngleX;

	// Get the direction vector
	m_vView -= m_vPosition;

	// Cap the x axis rotation so we can't do a full 360
	if(m_fCurrentRotX > 1.7f)
		m_fCurrentRotX = 1.7f;
	else if(m_fCurrentRotX < -1.7f)
		m_fCurrentRotX = -1.7f;
	else
	{
		// Find an axis perpendicular to the camera's direction vector and the up vector, and rotate around it
		// This represents the X axis for up and down rotations
		CMatrix::RotationAxis((m_vView ^ m_vUpVector).Normalise(), AngleX).TransformCoords(&m_vView, 1);
	}
	// Rotate around the up vector (Y axis) for left and right rotation
	CMatrix::RotationAxis(m_vUpVector, AngleY).TransformCoords(&m_vView, 1);

	// Restore view vector
	m_vView += m_vPosition;

	// We moved, so update move flag
	m_bMoved = true;
}

void CCamera::Walk()
{
	const float ACCELSPEED = ACCELERATION * m_fSpeed;

	if(m_WalkDir == FORWARDS)
	{	
		// Half the velocity if player trying to go in opposite direction
		if(m_PrevWalkDir == BACKWARDS)
			m_fWalkVel *= 0.5f;

		m_PrevWalkDir = FORWARDS;

		// Accelerate to full velocity (m_fMaxVel)
		if(m_fWalkVel < m_fMaxVel)
			m_fWalkVel += ACCELSPEED + 0.005f;		
	}
	else if(m_WalkDir == BACKWARDS)
	{	
		if(m_PrevWalkDir == FORWARDS)
			m_fWalkVel *= 0.5f;

		m_PrevWalkDir = BACKWARDS;

		if(m_fWalkVel > -m_fMaxVel)
			m_fWalkVel -= ACCELSPEED + 0.005f;
	}
	else
	{
		if(m_fWalkVel)
		{
			if(m_PrevWalkDir == FORWARDS)
			{
				if(m_fWalkVel > ACCELSPEED)
					m_fWalkVel -= ACCELSPEED;
				else
					m_fWalkVel = 0.0f;
			}
			else if(m_PrevWalkDir == BACKWARDS)
			{
				if(m_fWalkVel < -ACCELSPEED)
					m_fWalkVel += ACCELSPEED;
				else 
					m_fWalkVel = 0.0f;
			}
		}
	}
	
	if(m_StrafeDir == LEFT)
	{
		if(m_PrevStrafeDir == RIGHT)
			m_fStrafeVel *= 0.5f;

		m_PrevStrafeDir = LEFT;

		if(m_fStrafeVel < m_fMaxVel)
			m_fStrafeVel += ACCELSPEED + 0.005f;	

	}
	else if(m_StrafeDir == RIGHT)
	{	
		if(m_PrevStrafeDir == LEFT)
			m_fStrafeVel *= 0.5f;

		m_PrevStrafeDir = RIGHT;

		if(m_fStrafeVel > -m_fMaxVel)
			m_fStrafeVel -= ACCELSPEED + 0.005f;
	}
	else
	{
		if(m_fStrafeVel)
		{
			if(m_PrevStrafeDir == LEFT)
			{
				if(m_fStrafeVel > ACCELSPEED)
					m_fStrafeVel -= ACCELSPEED;
				else
					m_fStrafeVel = 0.0f;
			}
			else if(m_PrevStrafeDir == RIGHT)
			{
					if(m_fStrafeVel < -ACCELSPEED)
						m_fStrafeVel += ACCELSPEED;
					else
                        m_fStrafeVel = 0.0f;
			}
		}
	}

	// See if we moved, and update the vectors if so
	if(m_fStrafeVel || m_fWalkVel)
	{
		// Calculate the direction and strafe vectors
		CVector vDirection = m_vView - m_vPosition;
		CVector vStrafe = vDirection ^ m_vUpVector;

		// Keep camera's height level if not flying
		if(!m_bIsFlying)
			vDirection.y = 0;

		vDirection.Normalise();
		vStrafe.Normalise();

		// Multiply the vectors by speed and velocity
		vDirection *= m_fSpeed * m_fWalkVel;
		vStrafe *= m_fSpeed * m_fStrafeVel;

		CVector vDirStrafe = vDirection + vStrafe;

		m_vPosition += vDirStrafe;
		m_vView += vDirStrafe;

		// Make sure we update the moved flag in case mouse didn't move
		m_bMoved = true;
	}
}

void CCamera::SetVelocity(const float Vel)
{
	if(m_fWalkVel < -Vel)
		m_fWalkVel = -Vel;
	
	else if(m_fWalkVel > Vel)
			m_fWalkVel = Vel;

	if(m_fStrafeVel < -Vel)
		m_fStrafeVel = -Vel;
	
	else if(m_fStrafeVel > Vel)
			m_fStrafeVel = Vel;

	m_fMaxVel = Vel;
}

// Update, returns false if no movement occured, otherwise true
BOOL CCamera::Update(CViewport *pViewport)
{
	Rotate();
	Walk();

	// Set the new view matrix only if we moved
	if(m_bMoved)		
		pViewport->SetView(CMatrix::LookAtLH(m_vPosition, m_vView, m_vUpVector));

	return m_bMoved;
}