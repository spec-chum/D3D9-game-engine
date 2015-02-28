VEC_INLINE CVector::operator float*()
{
	return (float*)e;
}

VEC_INLINE CVector::operator const float*() const
{
	return (const float*)e;
}

VEC_INLINE CVector::operator D3DXVECTOR3*()
{
	return (D3DXVECTOR3*)this;
}

VEC_INLINE CVector::operator const D3DXVECTOR3*() const
{
	return (const D3DXVECTOR3*)this;
}

VEC_INLINE float CVector::operator[](int index)
{
	assert(index >=0 && index < 4);
	return e[index];
}

VEC_INLINE CVector& CVector::operator+=(const CVector &v)
{	
	return *this = Add(*this, v);
}

VEC_INLINE CVector& CVector::operator-=(const CVector &v)
{
	return *this = Subtract(*this, v);	
}

VEC_INLINE CVector& CVector::operator*=(const float &scalar)
{
	return *this = Multiply(*this, scalar);
}

VEC_INLINE CVector& CVector::operator/=(const float &scalar)
{
	return *this = Divide(*this, scalar);
}

VEC_INLINE CVector CVector::operator+(const CVector &v) const
{
	return(Add(*this, v));
}

VEC_INLINE CVector CVector::operator-(const CVector &v) const
{
	return(Subtract(*this, v));
}

VEC_INLINE CVector CVector::operator*(const float scalar) const
{
	return(Multiply(*this, scalar));
}

VEC_INLINE CVector CVector::operator/(const float scalar) const
{
	return(Divide(*this, scalar));
}

// Cross product
VEC_INLINE CVector CVector::operator^(const CVector &v) const
{
	return(Cross(*this, v));
}

// Dot product
VEC_INLINE float CVector::operator*(const CVector &v) const
{
	return(Dot(*this, v));
}

VEC_INLINE BOOL CVector::operator==(const CVector &v) const
{
	if(!CMaths::FComp(x, v.x))
		return false;

	if(!CMaths::FComp(y, v.y))
		return false;

	if(!CMaths::FComp(z, v.z))
		return false;

	return true;
}

VEC_INLINE BOOL CVector::operator!=(const CVector &v) const
{	
	if(!CMaths::FComp(x, v.x))
		return true;

	if(!CMaths::FComp(y, v.y))
		return true;

	if(!CMaths::FComp(z, v.z))
		return true;
	
	return false;
}

VEC_INLINE CVector CVector::operator-() const
{
	return(Negate(*this));
}

VEC_INLINE CVector& CVector::Normalise()
{
	// No need to optimise this, as it uses the SSE optimised / operator :]

	float mag = Magnitude(*this);
	
	// Avoid 0 magnitude
	if(CMaths::FComp(mag, 0))
		return *this;

	return (*this = Divide(*this, mag));	
}

VEC_INLINE float CVector::GetSqrMagnitude() const
{
	return(SqrMagnitude(*this));
}

VEC_INLINE float CVector::GetMagnitude() const
{
	return(Magnitude(*this));
}

VEC_INLINE CVector CVector::FPU_Negate(const CVector &v)
{
	return(CVector(-v.x, -v.y, -v.z));
}

VEC_INLINE CVector CVector::SSE_Negate(const CVector &v)
{
	CVector vTemp;

	__asm
	{
		lea eax, vTemp
		mov esi, v

		movss xmm1, [CMaths::NEG_MASK]
		shufps xmm1, xmm1, 0
		movaps xmm0, [esi]
		xorps xmm0, xmm1
		movaps [eax], xmm0
	}

	//return vTemp;
}

VEC_INLINE CVector CVector::FPU_Add(const CVector &v1, const CVector &v2)
{
	return(CVector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

VEC_INLINE CVector CVector::SSE_Add(const CVector &v1, const CVector &v2)
{
	CVector vTemp;

	__asm
	{
		// Copy v1 into xmm0 and v2 into xmm1
		lea eax, vTemp
		mov ecx, v1
		mov esi, v2
		movaps xmm0, [ecx]
		movaps xmm1, [esi]

		// Add em
		addps xmm0, xmm1
		
		// Store result in temp
		movaps [eax], xmm0
	}
	
	//return(CVector(vTemp.x, vTemp.y, vTemp.z));
}

VEC_INLINE CVector CVector::FPU_Subtract(const CVector &v1, const CVector &v2)
{
	return(CVector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

VEC_INLINE CVector CVector::SSE_Subtract(const CVector &v1, const CVector &v2)
{
	CVector vTemp;

	__asm
	{
		// Copy v1 into xmm0 and v2 into xmm1
		lea eax, vTemp
		mov ecx, v1
		mov esi, v2
		movaps xmm0, [ecx]
		movaps xmm1, [esi]

		// Subtract em
		subps xmm0, xmm1
		
		// Store result in temp
		movaps [eax], xmm0
	}
	
	//return vTemp;
}

VEC_INLINE CVector CVector::FPU_Multiply(const CVector &v, const float scalar)
{
	return(CVector(v.x*scalar, v.y*scalar, v.z*scalar));
}

VEC_INLINE CVector CVector::SSE_Multiply(const CVector &v, float scalar)
{
	 CVector vTemp;

	__asm
	{
		// Copy *this into xmm0
		lea eax, vTemp
		mov ecx, v
			
		// Multiply by scalar
		movss xmm1, scalar		// xmm1 = scalar
		shufps xmm1, xmm1, 0	// Broadcast scalar
		movaps xmm0, [ecx]		// xmm0 = v
		mulps xmm0, xmm1		// xmm0 = temp * scalar

		// Store result in temp
		movaps [eax], xmm0		// temp = xmm0
	}

	//return vTemp;
}

VEC_INLINE CVector CVector::FPU_Divide(const CVector &v, const float scalar)
{
	// Get 1/a
	float rcpscalar = CMaths::Rcp(scalar);

	return(CVector(v.x * rcpscalar, v.y * rcpscalar, v.z * rcpscalar));
}

VEC_INLINE CVector CVector::SSE_Divide(const CVector &v, const float scalar)
{
	CVector vTemp;

	__asm
	{
		lea eax, vTemp
		mov ecx, v
		
		// Get 1/scalar and broadcast it across xmm1
		movss xmm1, scalar
		rcpss xmm2, xmm1	// get rcp of scalar (uses Newton-Raphson for improved precision)
		mulps xmm1, xmm2
		mulps xmm1, xmm2
		addps xmm2, xmm2
		subps xmm2, xmm1
		shufps xmm2, xmm2, 0

		// multiply by this, and copy into temp
		movaps xmm0, [ecx]	// xmm0 = v
		mulps xmm0, xmm2	// xmm0 = v * (1/scalar)
		movaps [eax], xmm0
	}

	//return vTemp;
}

VEC_INLINE float CVector::FPU_SqrMagnitude(const CVector &v)
{
	// Avoid a zero vector
	if(CMaths::FComp(v.x, 0) && CMaths::FComp(v.y, 0) && CMaths::FComp(v.z, 0))
		return 0;

	// Compute the magnitude, ignores w component
	return(v.x * v.x + v.y * v.y + v.z * v.z);
}

VEC_INLINE float CVector::SSE_SqrMagnitude(const CVector &v)
{
	float result;

	// Avoid a zero vector
	if(CMaths::FComp(v.x, 0) && CMaths::FComp(v.y, 0) && CMaths::FComp(v.z, 0))
		return 0;
	
	// Compute the magnitude, ignores w component
	__asm
	{
		mov edx, v

		// Put *this into xmm0
		movaps xmm0, [edx]
			
		// Multiply it by itself
		mulps xmm0, xmm0		// xmm0 = w*w | z*z | y*y | x*x
			
		// Move z*z into lower half of xmm1
		movhlps xmm1, xmm0
			
		// Add x*x + z*z
		addss xmm1, xmm0

		// Now add (x*x+z+z) to y*y
		shufps xmm0, xmm1, 0x01
		addss xmm0, xmm1
			
		// Store result
		movss result, xmm0
	}
	
	return result;	
}

VEC_INLINE float CVector::FPU_Magnitude(const CVector &v)
{
	float result = SqrMagnitude(v);

	// Avoid 0 length
	if(CMaths::FComp(result, 0))
		return 0;

	return CMaths::Sqrt(result);
}

VEC_INLINE float CVector::SSE_Magnitude(const CVector &v)
{
	float result = SqrMagnitude(v);
	
	// Avoid 0 length
	if(CMaths::FComp(result, 0))
		return 0;

	__asm
	{
		movaps xmm2, f30
		movaps xmm3, f05
		movss xmm0, result

		rsqrtss xmm1, xmm0
		mulss xmm0, xmm1
		mulss xmm0, xmm1
		mulss xmm1, xmm3
		subss xmm2, xmm0
		mulss xmm1, xmm2

		rcpss xmm0, xmm1
		mulss xmm1, xmm0
		mulss xmm1, xmm0
		addss xmm0, xmm0
		subss xmm0, xmm1
		movss result, xmm0
	}

	return result;
}

VEC_INLINE float CVector::FPU_Dot(const CVector &v1, const CVector &v2)
{
	return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

VEC_INLINE float CVector::SSE_Dot(const CVector &v1, const CVector &v2)
{
	// Return the dot product of v1 and v2

	float result;

	// Compute dot product, ignores w component
	__asm
	{
		mov ecx, v1
		mov esi, v2		
			
		// Put v2 into xmm0 and v2 into xmm1
		movaps xmm0, [ecx]
		movaps xmm1, [esi]

		mulps xmm0, xmm1			// xmm0 = xxx | z*z | y*y | x*x
		movhlps xmm1, xmm0			// xmm1 = xxx | xxx | xxx | z*z
		addss xmm1, xmm0			// xmm1 = xxx | xxx | xxx | x*x+z*z
		shufps xmm0, xmm1, 0x01		// xmm0 = xxx | xxx | xxx | y*y					00 00 00 01
		addss xmm0, xmm1			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y

		// Store result
		movss result, xmm0
	}

	return result;
}

VEC_INLINE float CVector::FPU_DotW(const CVector &v1, const CVector &v2)
{
	// Compute dot product, uses w component
	return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

VEC_INLINE float CVector::SSE_DotW(const CVector &v1, const CVector &v2)
{
	// Return the dot product of v1 and v2

	float result;

	// Compute dot product, uses w component
	__asm
	{
		mov ecx, v1
		mov esi, v2		
			
		// Put *this into xmm0
		movaps xmm0, [ecx]
		movaps xmm1, [esi]

		mulps xmm0, xmm1			// xmm0 = w*w | z*z | y*y | x*x
		movhlps xmm2, xmm0			// xmm2 = xxx | xxx | w*w | z*z
		addps xmm2, xmm0			// xmm2 = xxx | xxx | y+w | x+z
		shufps xmm2, xmm2, 0x10		// xmm2 = xxx |	y*y+w*w | xxx | x*x+z*z	00 01 00 00
		movhlps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | y*y+w*w
		addps xmm0, xmm2			// xmm0 = xxx | xxx | xxx | x*x+z*z+y*y+w*w
			
		// Store result
		movss result, xmm0
	}

	return result;
}

VEC_INLINE CVector CVector::FPU_Cross(const CVector &v1, const CVector &v2)
{	
	return(CVector(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x));
}

VEC_INLINE CVector CVector::SSE_Cross(const CVector &v1, const CVector &v2)
{
	CVector vTemp;

	__asm
	{
		lea eax, vTemp
		mov esi, v1
		mov edi, v2

		movaps xmm0, [esi]		// xmm0 = w	| z | y | x						*v1
		movaps xmm1, xmm0		// xmm1 = w	| z | y | x
		movaps xmm2, [edi]		// xmm2 = w	| z | y | x						*v2
		shufps xmm1, xmm1, 0xC9	// xmm1 = w	| x	| z	| y						11 00 10 01
		shufps xmm0, xmm0, 0xD2	// xmm0 = w	| y | x | z						11 01 00 10
		mulps xmm0, xmm2		// xmm0 = y*vz | x*vy | z*vx
		shufps xmm0, xmm0, 0xC9	// xmm0 = z*vx | y*vz | x*vy				11 00 10 01
		mulps xmm1, xmm2		// xmm1 = x*vz | z*vy | y*vx
		subps xmm0, xmm1		// xmm0 = z*vx-x*vz | y*vz-z*vy | x*vy-y*vx
		shufps xmm0, xmm0, 0xC9	// xmm0 = x*vy-y*vx | z*vx-x*vz | y*vz-z*vy	11 00 10 01

		movaps [eax], xmm0
	}
	
	//return vTemp;	
}

VEC_INLINE CVector CVector::Normalise(const CVector &v)
{
	// Get magnitude of v
	float mag = Magnitude(v);
	
	// Avoid 0 magnitude
	if(CMaths::FComp(mag, 0))
		return(CVector(v));
    
	return(Divide(v, mag));	
}

VEC_INLINE BOOL CVector::IsNormalised(const CVector &v)
{
	// No need to get sqrt, sqrt(1) is 1 anyway
	float mag = SqrMagnitude(v);
	
	// use FComp to compensate for precision errors
	return(CMaths::FComp(mag, 1.0f));
}

VEC_INLINE CVector CVector::Lerp(const CVector &v1, const CVector &v2, const float t)
{
	// Interpolate between the 2 vectors using time constant t
	return(CVector(v1 + (v2 - v1) * t));
}

VEC_INLINE CVector operator*(const float scalar, const CVector &v)
{
	return(CVector::Multiply(v, scalar));
}