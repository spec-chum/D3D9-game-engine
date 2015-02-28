#include <math.h>
#include <assert.h>
#include <float.h>

MATHS_INLINE float CMaths::Sin(const float angle)
{
	float Sin;

	__asm
	{
		fld angle
		fsin
		fstp Sin
	}

	assert(!_isnan(Sin));
	return Sin;
}

MATHS_INLINE float CMaths::FPU_ASin(const float angle)
{
	float ASin;

	__asm
	{
		fld angle			// st0 = angle
		fld1				// st0 = 1; st1 = angle
		fadd st(0), st(1)	// st0 = angle + 1; st1 = angle;

		fld1				// st0 = 1; st1 = angle + 1; st2 = angle; 
		fsub st(0), st(2)	// st0 = 1 - angle; st1 = angle + 1; st2 = angle

        fmulp st(1), st(0)	// st0 = (angle + 1)*(1 - angle); st1 = angle;
		fsqrt				// st0 = sqrt((angle + 1)*(1 - angle)); st1 = angle
		fpatan				// st0 = atan(sqrt((angle + 1)*(1 - angle)) / angle)

		fstp ASin
	}

	assert(!_isnan(ASin));
	return ASin;
}


MATHS_INLINE float CMaths::SSE_ASin(const float angle)
{
	float ASin, A;
	const float one = 1.0f;

	__asm
	{	
		movss xmm0, angle	// xmm0 = angle
		movss xmm2, one		// xmm2 = one
		movss xmm1, xmm0	// xmm1 = angle
		addss xmm0, xmm2	// xmm1 = angle + 1
		subss xmm2, xmm1	// xmm0 = 1 - angle
		mulss xmm0, xmm2	// xmm0 = (angle + 1) * (1 - angle)
		
		movaps xmm2, f30
		movaps xmm3, f05

		rsqrtss xmm1, xmm0	// Get rsqrt (angle + 1) * (1 - angle)
		mulss xmm0, xmm1
		mulss xmm0, xmm1
		mulss xmm1, xmm3
		subss xmm2, xmm0
		mulss xmm1, xmm2

		rcpss xmm0, xmm1	// Get rcp of rsqrt
		mulss xmm1, xmm0
		mulss xmm1, xmm0
		addss xmm0, xmm0
		subss xmm0, xmm1
		movss A, xmm0

		fld angle
		fld A
		fpatan
		fstp ASin
	}

	assert(!_isnan(ASin));
	return ASin;
}

MATHS_INLINE float CMaths::Cos(const float angle)
{
	float Cos;

	__asm
	{
		fld angle
		fcos
		fstp Cos
	}
	
	assert(!_isnan(Cos));
	return Cos;
}

MATHS_INLINE float CMaths::FPU_ACos(const float angle)
{
	float ACos;

	__asm
	{
		fld angle			// st0 = angle
		fld1				// st0 = 1; st1 = angle
		fadd st(0), st(1)	// st0 = angle + 1; st1 = angle;

		fld1				// st0 = 1; st1 = angle + 1; st2 = angle; 
		fsub st(0), st(2)	// st0 = 1 - angle; st1 = angle + 1; st2 = angle

        fmulp st(1), st(0)	// st0 = (angle + 1)*(1 - angle); st1 = angle;
		fsqrt				// st0 = sqrt((angle + 1)*(1 - angle)); st1 = angle
		fxch				// st0 = angle; st1 = sqrt((angle + 1)*(1 - angle))
		fpatan				// st0 = atan(angle / sqrt((angle + 1)*(1 - angle)))

		fstp ACos
	}

	assert(!_isnan(ACos));
	return ACos;
}

MATHS_INLINE float CMaths::SSE_ACos(const float angle)
{
	float ACos, A;
	const float one = 1.0f;

	__asm
	{	
		movss xmm0, angle	// xmm0 = angle
		movss xmm2, one		// xmm2 = one
		movss xmm1, xmm0	// xmm1 = angle
		addss xmm0, xmm2	// xmm1 = angle + 1
		subss xmm2, xmm1	// xmm0 = 1 - angle
		mulss xmm0, xmm2	// xmm0 = (angle + 1) * (1 - angle)
		
		movaps xmm2, f30
		movaps xmm3, f05

		rsqrtss xmm1, xmm0	// Get sqrt
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
		movss A, xmm0

		fld A
		fld angle
		fpatan
		fstp ACos
	}

	assert(!_isnan(ACos));
	return ACos;
}

MATHS_INLINE void CMaths::SinCos(const float angle, float &sin, float &cos)
{
	float Sin, Cos;

	__asm
	{
		// Get the sine and cosine of the angle
		fld angle
		fsincos
		fstp Cos	// Using the passed addresses oddly generates extra code
		fstp Sin	// it stores on the stack, then movs to sin and cos, weird
	}

	// This gets optimised out in release builds, and stores straight to &sin and &cos
	assert(!_isnan(Sin));
	assert(!_isnan(Cos));

	sin = Sin;
	cos = Cos;
}

MATHS_INLINE float CMaths::Tan(const float angle)
{
	float Tan;

	__asm
	{
		fld angle
		fptan
		fstp st(0)
		fstp Tan
	}

	assert(!_isnan(Tan));
	return Tan;
}

MATHS_INLINE float CMaths::CoTan(const float angle)
{
	float CoTan;

	// Compute 1/tan
	__asm
	{
		fld angle
		fptan
		// fptan puts 1.0f into st0, so just do an rdiv here to get 1.0f / tan
		fdivr
		fstp CoTan
	}

	assert(!_isnan(CoTan));
	return CoTan;
}

MATHS_INLINE float CMaths::ATan(const float tan)
{
	float ATan;

	__asm
	{
		fld tan
		fld1
		fpatan
		fstp ATan
	}

	assert(!_isnan(ATan));
	return ATan;
}

MATHS_INLINE float CMaths::ATan2(const float x, const float y)
{
	float ATan;

	__asm
	{
		fld x
		fld y
		fpatan
		fstp ATan
	}

	assert(!_isnan(ATan));
	return ATan;
}

MATHS_INLINE float CMaths::FPU_RSqrt(const float value)
{
	float Result;

	__asm
	{
		fld value
		fsqrt
		fld1
		fdivr
		fstp Result
	}

	assert(!_isnan(Result));
	return Result;
}

MATHS_INLINE float CMaths::SSE_RSqrt(const float value)
{
	float Result;

	__asm
	{
		movaps xmm2, f30
		movaps xmm3, f05
		movss xmm0, value

		rsqrtss xmm1, xmm0
		mulss xmm0, xmm1
		mulss xmm0, xmm1
		mulss xmm1, xmm3
		subss xmm2, xmm0
		mulss xmm1, xmm2

		movss Result, xmm1
	}

	assert(!_isnan(Result));
	return Result;
}

MATHS_INLINE float CMaths::FPU_Sqrt(const float value)
{
	float Result;

	__asm
	{
		fld value
		fsqrt
		fstp Result
	}

	assert(!_isnan(Result));
	return Result;
}

MATHS_INLINE float CMaths::SSE_Sqrt(const float value)
{
	float Result;

	__asm
	{
		movaps xmm2, f30
		movaps xmm3, f05
		movss xmm0, value

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
		movss Result, xmm0
	}

	assert(!_isnan(Result));
	return Result;
}

MATHS_INLINE int CMaths::FComp(const float value1, const float value2)
{
	//float Q = (FAbs(value1 - value2));

	assert(!_isnan(value1));
	assert(!_isnan(value2));

	__asm
	{
		fld value1
		fsub value2
		fabs
		fld EPSILON
		fucomip st(0), st(1)
		fstp st(0)

		ja T
	}

	return false;

T:
	return true;
}

MATHS_INLINE float CMaths::FAbs(const float value)
{
	float Result;

	__asm
	{
		//fld value
		//fabs
		//fstp Result

		// Integer is faster than FPU
		mov eax, [value]
		and eax, ABS_MASK
		mov [Result], eax
	}

	assert(!_isnan(Result));
	return Result;
}

MATHS_INLINE float CMaths::FPU_Rcp(const float value)
{
	float Result;

	__asm
	{
		fld1
		fdiv value
		fstp Result
	}

	assert(!_isnan(Result));
	return Result;
}

MATHS_INLINE float CMaths::SSE_Rcp(const float value)
{
	float Result;

	__asm
	{
		movss xmm1, value
		rcpss xmm0, xmm1
		mulss xmm1, xmm0
		mulss xmm1, xmm0
		addss xmm0, xmm0
		subss xmm0, xmm1
		movss Result, xmm0
	}

	assert(!_isnan(Result));
	return Result;
}