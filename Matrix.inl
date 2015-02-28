MAT_INLINE CMatrix::CMatrix(const float e00, const float e01, const float e02, const float e03,
							const float e10, const float e11, const float e12, const float e13,
							const float e20, const float e21, const float e22, const float e23, 
							const float e30, const float e31, const float e32, const float e33)
{	
	e[0][0] = e00; e[0][1] = e01; e[0][2] = e02; e[0][3] = e03;
	e[1][0] = e10; e[1][1] = e11; e[1][2] = e12; e[1][3] = e13;
	e[2][0] = e20; e[2][1] = e21; e[2][2] = e22; e[2][3] = e23;
	e[3][0] = e30; e[3][1] = e31; e[3][2] = e32; e[3][3] = e33;
}

MAT_INLINE CMatrix::CMatrix(const float Value)
{
	// Set all elements to Value

	// Need this if() here cos you can't get a function pointer to a constructor
	if(g_USE_SSE)
	{		
		__asm
		{
			movss xmm0, [Value]
			shufps xmm0, xmm0, 0

			// ecx = *this, so no need to explicitly mov it into a reg
			movaps [ecx], xmm0
			movaps [ecx+0x10], xmm0
			movaps [ecx+0x20], xmm0
			movaps [ecx+0x30], xmm0
		}
	}
	else
	{
		// This creates horrible code, with fld's and fstp's all over thr shop, not good mr compiler :/
		/*
		e[0][0] = e[0][1] = e[0][2] = e[0][3] = 
		e[1][0] = e[1][1] = e[1][2] = e[1][3] = 
		e[2][0] = e[2][1] = e[2][2] = e[2][3] = 
		e[3][0] = e[3][1] = e[3][2] = e[3][3] = Value;
		*/
		
		__asm
		{
			mov eax, [Value]
			mov edi, ecx
			mov ecx, 16
			rep stosd
		}
	}
}

MAT_INLINE CMatrix::operator D3DXMATRIX*() const
{
	return(D3DXMATRIX*)this;
}

MAT_INLINE CMatrix::operator const D3DXMATRIX*() const
{
	return(const D3DXMATRIX*)this;
}

MAT_INLINE CMatrix CMatrix::operator*(const CMatrix &m) const
{
	return(Multiply(*this, m));
}

MAT_INLINE CMatrix& CMatrix::operator*=(const CMatrix &m)
{
	return *this = *this * m;
}

MAT_INLINE float CMatrix::Determinant(const CMatrix &m)
{
	return(	(m.e[0][0] * m.e[1][1] - m.e[0][1] * m.e[1][0]) * (m.e[2][2] * m.e[3][3] - m.e[2][3] * m.e[3][2]) -
			(m.e[0][0] * m.e[1][2] - m.e[0][2] * m.e[1][0]) * (m.e[2][1] * m.e[3][3] - m.e[2][3] * m.e[3][1]) +
			(m.e[0][0] * m.e[1][3] - m.e[0][3] * m.e[1][0]) * (m.e[2][1] * m.e[3][2] - m.e[2][2] * m.e[3][1]) +
			(m.e[0][1] * m.e[1][2] - m.e[0][2] * m.e[1][1]) * (m.e[2][0] * m.e[3][3] - m.e[2][3] * m.e[3][0]) -
			(m.e[0][1] * m.e[1][3] - m.e[0][3] * m.e[1][1]) * (m.e[2][0] * m.e[3][2] - m.e[2][2] * m.e[3][0]) +
			(m.e[0][2] * m.e[1][3] - m.e[0][3] * m.e[1][2]) * (m.e[2][0] * m.e[3][1] - m.e[2][1] * m.e[3][0]));
}

MAT_INLINE CMatrix CMatrix::Translation(const float x, const float y, const float z)
{
	return(CMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1));	
}

MAT_INLINE CMatrix CMatrix::LookAtLH(const CVector &Eye, const CVector &At, const CVector &Up)
{
	/*
	formula:

	zaxis = normalised(At - Eye)
	xaxis = normalised(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x           yaxis.x           zaxis.x          0
	xaxis.y           yaxis.y           zaxis.y          0
	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) 1
	*/

	CVector zAxis = (At - Eye).Normalise();
	CVector xAxis = (Up ^ zAxis).Normalise();
	CVector yAxis = zAxis ^ xAxis;

	return(CMatrix(	xAxis.x,		yAxis.x,		zAxis.x,		0,
					xAxis.y,		yAxis.y,		zAxis.y,		0,
					xAxis.z,		yAxis.z,		zAxis.z,		0,
					-(xAxis*Eye),	-(yAxis*Eye),	-(zAxis*Eye),	1));
}

MAT_INLINE CMatrix CMatrix::RotationAxis(const CVector &Axis, const float Angle)
{
	// Creates a rotation matrix around an arbitrary axis

	float sin, cos;

	CMaths::SinCos(Angle, sin, cos);

	// Pre-calc some values
	const float omCos = 1.0f - cos;
	const float xCos = Axis.x * omCos;
	const float yCos = Axis.y * omCos;
	const float zCos = Axis.z * omCos;
	const float xx = Axis.x * xCos;
    const float xy = Axis.x * yCos;
	const float xz = Axis.x * zCos;
	const float yy = Axis.y * yCos;
	const float yz = Axis.y * zCos;
	const float zz = Axis.z * zCos;

	return(CMatrix(	cos + xx,				Axis.z * sin + xy,		-Axis.y * sin + xz,	0,
					-Axis.z * sin + xy,		cos + yy,				Axis.x * sin + yz,	0,
					Axis.y * sin + xz,		-Axis.x * sin + yz,		cos + zz,			0,
					0,						0,						0,					1));
}

MAT_INLINE CMatrix CMatrix::Identity()
{
	return(CMatrix(	1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1));
}

MAT_INLINE CMatrix CMatrix::PerspectiveFovLH(const float FOV, const float Aspect, const float zNear, const float zFar)
{
	/*	
		w       0       0               0
		0       h       0               0
		0       0       zf/(zf-zn)      1
		0       0       -zn*zf/(zf-zn)  0

		where:

		h is the view space height. It is calculated from 
		h = cot(fovY/2);

		w is the view space width. It is calculated from
		w = h / Aspect.

	*/

	// Calc h and w, and precalc zFar / (zFar-zNear)
	const float h = CMaths::CoTan(FOV * 0.5f);
	const float w = h / Aspect;
	const float q = zFar / (zFar - zNear);

	return(CMatrix(	w,		0,		0,			0,
					0,		h,		0,			0,
					0,		0,		q,			1,
					0,		0,	   -q * zNear,	0));
}

MAT_INLINE CMatrix CMatrix::YawPitchRoll(const float Yaw, const float Pitch, const float Roll)
{
	return(RotationZ(Roll) * RotationX(Pitch) * RotationY(Yaw));
}

MAT_INLINE CMatrix CMatrix::Scaling(const float sx, const float sy, const float sz)
{
	return(CMatrix(sx, 0, 0, 0,
					0,sy, 0, 0,
					0, 0,sz, 0,
					0, 0, 0, 1));
}

MAT_INLINE CMatrix CMatrix::RotationX(const float Angle)
{
	float s, c;

	CMaths::SinCos(Angle, s, c);

	return(CMatrix(	1, 0, 0, 0,
					0, c, s, 0, 
					0,-s, c, 0,
					0, 0, 0, 1));
}

MAT_INLINE CMatrix CMatrix::RotationY(const float Angle)
{	
	float s, c;

	CMaths::SinCos(Angle, s, c);

	return(CMatrix(	c, 0,-s, 0,
					0, 1, 0, 0,
					s, 0, c, 0,
					0, 0, 0, 1));
}

MAT_INLINE CMatrix CMatrix::RotationZ(const float Angle)
{
	float s, c;

	CMaths::SinCos(Angle, s, c);

	return(CMatrix(	c, s, 0, 0, 
				   -s, c, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1));
}

MAT_INLINE CMatrix CMatrix::FPU_Multiply(const CMatrix &LH, const CMatrix &RH)
{
	CMatrix Temp;

#if 1
	// Unrolled FPU version
	Temp.e[0][0] = LH.e[0][0] * RH.e[0][0] + LH.e[0][1] * RH.e[1][0] + LH.e[0][2] * RH.e[2][0] + LH.e[0][3] * RH.e[3][0];
	Temp.e[0][1] = LH.e[0][0] * RH.e[0][1] + LH.e[0][1] * RH.e[1][1] + LH.e[0][2] * RH.e[2][1] + LH.e[0][3] * RH.e[3][1];
	Temp.e[0][2] = LH.e[0][0] * RH.e[0][2] + LH.e[0][1] * RH.e[1][2] + LH.e[0][2] * RH.e[2][2] + LH.e[0][3] * RH.e[3][2];
	Temp.e[0][3] = LH.e[0][0] * RH.e[0][3] + LH.e[0][1] * RH.e[1][3] + LH.e[0][2] * RH.e[2][3] + LH.e[0][3] * RH.e[3][3];

	Temp.e[1][0] = LH.e[1][0] * RH.e[0][0] + LH.e[1][1] * RH.e[1][0] + LH.e[1][2] * RH.e[2][0] + LH.e[1][3] * RH.e[3][0];
	Temp.e[1][1] = LH.e[1][0] * RH.e[0][1] + LH.e[1][1] * RH.e[1][1] + LH.e[1][2] * RH.e[2][1] + LH.e[1][3] * RH.e[3][1];
	Temp.e[1][2] = LH.e[1][0] * RH.e[0][2] + LH.e[1][1] * RH.e[1][2] + LH.e[1][2] * RH.e[2][2] + LH.e[1][3] * RH.e[3][2];
	Temp.e[1][3] = LH.e[1][0] * RH.e[0][3] + LH.e[1][1] * RH.e[1][3] + LH.e[1][2] * RH.e[2][3] + LH.e[1][3] * RH.e[3][3];

	Temp.e[2][0] = LH.e[2][0] * RH.e[0][0] + LH.e[2][1] * RH.e[1][0] + LH.e[2][2] * RH.e[2][0] + LH.e[2][3] * RH.e[3][0];
	Temp.e[2][1] = LH.e[2][0] * RH.e[0][1] + LH.e[2][1] * RH.e[1][1] + LH.e[2][2] * RH.e[2][1] + LH.e[2][3] * RH.e[3][1];
	Temp.e[2][2] = LH.e[2][0] * RH.e[0][2] + LH.e[2][1] * RH.e[1][2] + LH.e[2][2] * RH.e[2][2] + LH.e[2][3] * RH.e[3][2];
	Temp.e[2][3] = LH.e[2][0] * RH.e[0][3] + LH.e[2][1] * RH.e[1][3] + LH.e[2][2] * RH.e[2][3] + LH.e[2][3] * RH.e[3][3];

	Temp.e[3][0] = LH.e[3][0] * RH.e[0][0] + LH.e[3][1] * RH.e[1][0] + LH.e[3][2] * RH.e[2][0] + LH.e[3][3] * RH.e[3][0];
	Temp.e[3][1] = LH.e[3][0] * RH.e[0][1] + LH.e[3][1] * RH.e[1][1] + LH.e[3][2] * RH.e[2][1] + LH.e[3][3] * RH.e[3][1];
	Temp.e[3][2] = LH.e[3][0] * RH.e[0][2] + LH.e[3][1] * RH.e[1][2] + LH.e[3][2] * RH.e[2][2] + LH.e[3][3] * RH.e[3][2];
	Temp.e[3][3] = LH.e[3][0] * RH.e[0][3] + LH.e[3][1] * RH.e[1][3] + LH.e[3][2] * RH.e[2][3] + LH.e[3][3] * RH.e[3][3];
#else

	// Looped version
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Temp.e[i][j] =	LH.e[i][0] * RH.e[0][j] +
							LH.e[i][1] * RH.e[1][j] +
							LH.e[i][2] * RH.e[2][j] +
							LH.e[i][3] * RH.e[3][j];
#endif
		return Temp;
}


MAT_INLINE CMatrix CMatrix::SSE_Multiply(const CMatrix &LH, const CMatrix &RH)
{
	CMatrix Temp = Transpose(RH);

#if 1	
	// Fully expanded SSE asm loop, thank god for copy 'n' paste :p
	__asm
	{
		lea eax, Temp
		mov esi, LH
		mov edi, eax

		movaps xmm0, [esi]
		movaps xmm1, [edi]
		prefetchnta [esi+0x10]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax], xmm0

		movaps xmm0, [esi+0x10]
		prefetchnta [esi+0x20]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+4], xmm0

		movaps xmm0, [esi+0x20]
		prefetchnta [esi+0x30]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+8], xmm0

		movaps xmm0, [esi+0x30]
		prefetchnta [esi]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+12], xmm0

		movaps xmm0, [esi]
		movaps xmm1, [edi+0x10]
		prefetchnta [esi+0x10]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+16], xmm0

		movaps xmm0, [esi+0x10]
		prefetchnta [esi+0x20]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+20], xmm0

		movaps xmm0, [esi+0x20]
		prefetchnta [esi+0x30]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+24], xmm0

		movaps xmm0, [esi+0x30]
		prefetchnta [esi]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+28], xmm0

		movaps xmm0, [esi]
		movaps xmm1, [edi+0x20]
		prefetchnta [esi+0x10]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+32], xmm0

		movaps xmm0, [esi+0x10]
		prefetchnta [esi+0x20]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+36], xmm0

		movaps xmm0, [esi+0x20]
		prefetchnta [esi+0x30]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+40], xmm0

		movaps xmm0, [esi+0x30]
		prefetchnta [esi]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+44], xmm0

		movaps xmm0, [esi]
		movaps xmm1, [edi+0x30]
		prefetchnta [esi+0x10]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+48], xmm0

		movaps xmm0, [esi+0x10]
		prefetchnta [esi+0x20]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+52], xmm0

		movaps xmm0, [esi+0x20]
		prefetchnta [esi+0x30]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+56], xmm0

		movaps xmm0, [esi+0x30]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax+60], xmm0
	}	// PHEW!
#else
	__asm
	{
		// Looped version in case code size is an issue

		// MSVC classes ebx as sacrid, so store its value
		push ebx

		lea eax, Temp
		mov esi, LH
		lea edi, TRH

		// Initialise the offsets
		xor ecx, ecx
		xor edx, edx

		// 4 rows per matrix
l1:		mov ebx, 4
		movaps xmm1, [edi+edx]
l2:		movaps xmm0, [esi+ecx]
		prefetchnta [esi+ecx]

		// Dot product of the 2 rows
		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w

		// Store result
		movss [eax], xmm0
	
		// Point eax to next element of this
		add eax, 4

		// Point ecx to next row
		add ecx, 16

		dec ebx
		jnz l2

		xor ecx, ecx

		// Finished?
		add edx, 16
		cmp edx, 64
		jnz l1

		// Restore ebx
		pop ebx
	}
#endif	
	Temp = Transpose(Temp);

	//return Temp;	
}

MAT_INLINE CMatrix CMatrix::Inverse(const CMatrix &m)
{
	const float det = Determinant(m);

	return(Inverse(m, det));
}

MAT_INLINE CMatrix CMatrix::Inverse(const CMatrix &m, const float det)
{
	// Pre-calc 1/det
	const float rcpDet = CMaths::Rcp(det);

	// My god, I never want to type this out again :/
	return (CMatrix(rcpDet * (	m.e[1][1] * (m.e[2][2] * m.e[3][3] - m.e[2][3] * m.e[3][2]) +
								m.e[1][2] * (m.e[2][3] * m.e[3][1] - m.e[2][1] * m.e[3][3]) +
								m.e[1][3] * (m.e[2][1] * m.e[3][2] - m.e[2][2] * m.e[3][1])),

					rcpDet * (	m.e[2][1] * (m.e[0][2] * m.e[3][3] - m.e[0][3] * m.e[3][2]) +
								m.e[2][2] * (m.e[0][3] * m.e[3][1] - m.e[0][1] * m.e[3][3]) +
								m.e[2][3] * (m.e[0][1] * m.e[3][2] - m.e[0][2] * m.e[3][1])),

					rcpDet * (	m.e[3][1] * (m.e[0][2] * m.e[1][3] - m.e[0][3] * m.e[1][2]) +
								m.e[3][2] * (m.e[0][3] * m.e[1][1] - m.e[0][1] * m.e[1][3]) +
								m.e[3][3] * (m.e[0][1] * m.e[1][2] - m.e[0][2] * m.e[1][1])),

					rcpDet * (	m.e[0][1] * (m.e[1][3] * m.e[2][2] - m.e[1][2] * m.e[2][3]) +
								m.e[0][2] * (m.e[1][1] * m.e[2][3] - m.e[1][3] * m.e[2][1]) +
								m.e[0][3] * (m.e[1][2] * m.e[2][1] - m.e[1][1] * m.e[2][2])),

					rcpDet * (	m.e[1][2] * (m.e[2][0] * m.e[3][3] - m.e[2][3] * m.e[3][0]) +
								m.e[1][3] * (m.e[2][2] * m.e[3][0] - m.e[2][0] * m.e[3][2]) +
								m.e[1][0] * (m.e[2][3] * m.e[3][2] - m.e[2][2] * m.e[3][3])),

					rcpDet * (	m.e[2][2] * (m.e[0][0] * m.e[3][3] - m.e[0][3] * m.e[3][0]) +
								m.e[2][3] * (m.e[0][2] * m.e[3][0] - m.e[0][0] * m.e[3][2]) +
								m.e[2][0] * (m.e[0][3] * m.e[3][2] - m.e[0][2] * m.e[3][3])),

					rcpDet * (	m.e[3][2] * (m.e[0][0] * m.e[1][3] - m.e[0][3] * m.e[1][0]) +
								m.e[3][3] * (m.e[0][2] * m.e[1][0] - m.e[0][0] * m.e[1][2]) +
								m.e[3][0] * (m.e[0][3] * m.e[1][2] - m.e[0][2] * m.e[1][3])),

					rcpDet * (	m.e[0][2] * (m.e[1][3] * m.e[2][0] - m.e[1][0] * m.e[2][3]) +
								m.e[0][3] * (m.e[1][0] * m.e[2][2] - m.e[1][2] * m.e[2][0]) +
								m.e[0][0] * (m.e[1][2] * m.e[2][3] - m.e[1][3] * m.e[2][2])),

					rcpDet * (	m.e[1][3] * (m.e[2][0] * m.e[3][1] - m.e[2][1] * m.e[3][0]) +
								m.e[1][0] * (m.e[2][1] * m.e[3][3] - m.e[2][3] * m.e[3][1]) +
								m.e[1][1] * (m.e[2][3] * m.e[3][0] - m.e[2][0] * m.e[3][3])),

					rcpDet * (	m.e[2][3] * (m.e[0][0] * m.e[3][1] - m.e[0][1] * m.e[3][0]) +
								m.e[2][0] * (m.e[0][1] * m.e[3][3] - m.e[0][3] * m.e[3][1]) +
								m.e[2][1] * (m.e[0][3] * m.e[3][0] - m.e[0][0] * m.e[3][3])),

					rcpDet * (	m.e[3][3] * (m.e[0][0] * m.e[1][1] - m.e[0][1] * m.e[1][0]) +
								m.e[3][0] * (m.e[0][1] * m.e[1][3] - m.e[0][3] * m.e[1][1]) +
								m.e[3][1] * (m.e[0][3] * m.e[1][0] - m.e[0][0] * m.e[1][3])),

					rcpDet * (	m.e[0][3] * (m.e[1][1] * m.e[2][0] - m.e[1][0] * m.e[2][1]) +
								m.e[0][0] * (m.e[1][3] * m.e[2][1] - m.e[1][1] * m.e[2][3]) +
								m.e[0][1] * (m.e[1][0] * m.e[2][3] - m.e[1][3] * m.e[2][0])),

					rcpDet * (	m.e[1][0] * (m.e[2][2] * m.e[3][1] - m.e[2][1] * m.e[3][2]) +
								m.e[1][1] * (m.e[2][0] * m.e[3][2] - m.e[2][2] * m.e[3][0]) +
								m.e[1][2] * (m.e[2][1] * m.e[3][0] - m.e[2][0] * m.e[3][1])),

					rcpDet * (	m.e[2][0] * (m.e[0][2] * m.e[3][1] - m.e[0][1] * m.e[3][2]) +
								m.e[2][1] * (m.e[0][0] * m.e[3][2] - m.e[0][2] * m.e[3][0]) +
								m.e[2][2] * (m.e[0][1] * m.e[3][0] - m.e[0][0] * m.e[3][1])),

					rcpDet * (	m.e[3][0] * (m.e[0][2] * m.e[1][1] - m.e[0][1] * m.e[1][2]) +
								m.e[3][1] * (m.e[0][0] * m.e[1][2] - m.e[0][2] * m.e[1][0]) +
								m.e[3][2] * (m.e[0][1] * m.e[1][0] - m.e[0][0] * m.e[1][1])),

					rcpDet * (	m.e[0][0] * (m.e[1][1] * m.e[2][2] - m.e[1][2] * m.e[2][1]) +
								m.e[0][1] * (m.e[1][2] * m.e[2][0] - m.e[1][0] * m.e[2][2]) +
								m.e[0][2] * (m.e[1][0] * m.e[2][1] - m.e[1][1] * m.e[2][0]))));
}

MAT_INLINE CMatrix CMatrix::SSE_Transpose(const CMatrix &m)
{
	// Transposes matrix in m and returns transposed matrix Temp
	CMatrix Temp;

	__asm
	{
		lea eax, Temp
		mov esi, m

		// Load all the x and y elements
		movlps xmm0, [esi]			// xmm0 = xx | xx | 0y | 0x
		movhps xmm0, [esi+0x10]		// xmm0 = 1y | 1x | 0y | 0x
		movlps xmm1, [esi+0x20]		// xmm1 = xx | xx | 2y | 2x
		movhps xmm1, [esi+0x30]		// xmm1 = 3y | 3x | 2y | 2x
		movaps xmm4, xmm0			// Copy xmm0 into xmm4

		// Now all the z and w elements
		movlps xmm2, [esi+0x08]		// xmm2 = xx | xx | 0w | 0z
		movhps xmm2, [esi+0x18]		// xmm2 = 1w | 1z | 0w | 0z
		movlps xmm3, [esi+0x28]		// xmm3 = xx | xx | 2w | 2z
		movhps xmm3, [esi+0x38]		// xmm3 = 3w | 3z | 2w | 2z
		movaps xmm5, xmm2			// copy xmm2 into xmm5

		// Group em together
		shufps xmm0, xmm1, 0x88		// xmm0 = 3x | 2x | 1x | 0x		10 00 10 00
		shufps xmm4, xmm1, 0xDD		// xmm4 = 3y | 2y | 1y | 0y		11 01 11 01
		shufps xmm2, xmm3, 0x88		// xmm2 = 3z | 2z | 1z | 0z		10 00 10 00
		shufps xmm5, xmm3, 0xDD		// xmm5 = 3w | 2w | 1w | 0w		11 01 11 01

		// Now copy em all back
		movaps [eax], xmm0			// X
		movaps [eax+0x10], xmm4		// Y
		movaps [eax+0x20], xmm2		// Z
		movaps [eax+0x30], xmm5		// W
	}

	//return Temp;					
}

MAT_INLINE CMatrix CMatrix::FPU_Transpose(const CMatrix &m)
{
	return(CMatrix(	m.e[0][0], m.e[1][0], m.e[2][0], m.e[3][0],
					m.e[0][1], m.e[1][1], m.e[2][1], m.e[3][1],
					m.e[0][2], m.e[1][2], m.e[2][2], m.e[3][2],
					m.e[0][3], m.e[1][3], m.e[2][3], m.e[3][3]));
}

MAT_INLINE CMatrix CMatrix::SSE_ZeroMatrix()
{
	// Zero out all elements of the matrix, and return the zeroed matrix
	CMatrix Temp;

	__asm
	{
		lea eax, Temp
		xorps xmm0, xmm0

		movaps [eax], xmm0
		movaps [eax+0x10], xmm0
		movaps [eax+0x20], xmm0
		movaps [eax+0x30], xmm0
	}

	//return Temp;
}

MAT_INLINE CMatrix CMatrix::FPU_ZeroMatrix()
{
	return(CMatrix(0));
}

MAT_INLINE CMatrix CMatrix::Reflect(const CPlane &p)
{
	/*
	Formula:

	-2 * P.a * P.a + 1  -2 * P.b * P.a      -2 * P.c * P.a        0
	-2 * P.a * P.b      -2 * P.b * P.b + 1  -2 * P.c * P.b        0
	-2 * P.a * P.c      -2 * P.b * P.c      -2 * P.c * P.c + 1    0
	-2 * P.a * P.d      -2 * P.b * P.d      -2 * P.c * P.d        1
	*/

	// Pre-calc some values (takes it from 24 fmuls to 12, even less for SSE)
	CVector m2p = p.n * -2.0f;

	const float paa = m2p.x * p.n.x + 1.0f;
	const float pab = m2p.x * p.n.y;
	const float pac = m2p.x * p.n.z;
	const float pad = m2p.x * p.d;

	const float pbb = m2p.y * p.n.y + 1.0f;
	const float pbc = m2p.y * p.n.z;
	const float pbd = m2p.y * p.d;

	const float pcc = m2p.z * p.n.z + 1.0f;
	const float pcd = m2p.z * p.d;

	return(CMatrix(	paa,	pab,	pac,	0,
					pab,	pbb,	pbc,	0,
					pac,	pbc,	pcc,	0,
					pad,	pbd,	pcd,	1));
}

MAT_INLINE CMatrix CMatrix::Shadow(const CPlane &plane, const CVector &light)
{
	/*
	formula: ( according to the D3D9 docs anyway, they appear to have missed out that the light vector is negated :/ )

	P = normalize(Plane);
	L = Light;
	d = dot(P, L)

	P.a * L.x + d  P.a * L.y      P.a * L.z      P.a * L.w  
	P.b * L.x      P.b * L.y + d  P.b * L.z      P.b * L.w  
	P.c * L.x      P.c * L.y      P.c * L.z + d  P.c * L.w  
	P.d * L.x      P.d * L.y      P.d * L.z      P.d * L.w + d
	*/

	// Pre-calc some values
	float d = CVector::Dot(plane.n, light) + plane.d;
	CVector plx = -plane.n * light.x;	// Note the MINUS, where does it say that on the formula above! :/
	CVector ply = -plane.n * light.y;
	CVector plz = -plane.n * light.z;
	CVector pld = -light * plane.d;

	return(CMatrix(	plx.x + d,	ply.x,		plz.x,		-plane.n.x,
					plx.y,		ply.y + d,	plz.y,		-plane.n.y,
					plx.z,		ply.z,		plz.z + d,	-plane.n.z,
					pld.x,		pld.y,		pld.z,		-plane.d + d));
}

MAT_INLINE void CMatrix::Transform(CVector *vecs, const int nbCoords) const
{
	// Transform vector(s) with the current matrix

	// Extract the values for the vectors from the matrix (column major)
	CVector vX(e[0][0], e[1][0], e[2][0], e[3][0]);
	CVector vY(e[0][1], e[1][1], e[2][1], e[3][1]);
	CVector vZ(e[0][2], e[1][2], e[2][2], e[3][2]);

	for(int i = 0; i < nbCoords; i++)
	{
		CVector temp = vecs[i];

		// Make sure w == 1 (needed for DotW to function correctly)
		temp.w = 1.0f;

		// Use SSE optimised dot product
		vecs[i] = CVector((CVector::DotW(temp, vX)), (CVector::DotW(temp, vY)), (CVector::DotW(temp, vZ)));
	}
}

MAT_INLINE void CMatrix::TransformCoords(CVector *vecs, const int nbCoords) const
{
	// Transform vector(s) with the current matrix, projecting result back into w = 1

	// Extract the values for the vectors from the matrix (column major)
	CVector vX(e[0][0], e[1][0], e[2][0], e[3][0]);
	CVector vY(e[0][1], e[1][1], e[2][1], e[3][1]);
	CVector vZ(e[0][2], e[1][2], e[2][2], e[3][2]);

	for(int i = 0; i < nbCoords; i++)
	{
		CVector temp = vecs[i];

		// Make sure w == 1 (needed for DotW to function correctly)
		temp.w = 1.0f;	// Should compile to 1 mov dword ptr, so no great overhead anyway

		// Calculate 1/w
		float coef = CMaths::Rcp((e[0][3] * temp.x + e[1][3] * temp.y + e[2][3] * temp.z + e[3][3]));

		// Use SSE optimised dot product
		vecs[i] = CVector(coef*(CVector::DotW(temp, vX)), coef*(CVector::DotW(temp, vY)), coef*(CVector::DotW(temp, vZ)));
	}
}