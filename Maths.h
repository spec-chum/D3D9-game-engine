#ifndef _cmaths_h
#define _cmaths_h

// Some common maths routines, the CRT does too much unnecessary checking for my liking
// NOT intended as a replacement however, just a supplement

__declspec(align(16)) static const float f30[4] = { 3.0f, 3.0f, 3.0f, 3.0f };
__declspec(align(16)) static const float f05[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

class CMaths
{
	CMaths() {};
	~CMaths() {};

public:
	static const float EPSILON;			// Has to be a little higher due to SSE
	static const float PI;
	static const unsigned int NEG_MASK;	// Used with xor to negate value
	static const unsigned int ABS_MASK;	// Used for FAbs()

	static void SinCos(const float angle, float &sin, float &cos);

	static int FComp(const float value1, const float value2);

	static float Sin(const float angle);
	static float FPU_ASin(const float angle);
	static float SSE_ASin(const float angle);
	static float (*ASin)(const float angle);

	static float Cos(const float angle);
	static float FPU_ACos(const float angle);
	static float SSE_ACos(const float angle);
	static float (*ACos)(const float angle);

	static float Tan(const float angle);
	static float CoTan(const float angle);
	static float ATan(const float tan);
	static float ATan2(const float x, const float y);

	static float FPU_RSqrt(const float value);
	static float SSE_RSqrt(const float value);
	static float (*RSqrt)(const float value);

	static float FPU_Sqrt(const float value);
	static float SSE_Sqrt(const float value);
	static float (*Sqrt)(const float value);

	static float FAbs(const float value);

	static float FPU_Rcp(const float value);
	static float SSE_Rcp(const float value);
	static float (*Rcp)(const float value);
};

#ifndef _DEBUG
	#define MATHS_INLINE __forceinline
	#include "maths.inl"
#else
	#define MATHS_INLINE 
#endif

#endif