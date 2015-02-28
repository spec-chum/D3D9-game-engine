#ifndef _cvector_h
#define _cvector_h

#include <assert.h>
#include <d3dx9.h>
#include "maths.h"
#include "utils.h"

// w component not used in most calculations, mainly used to overwrite *this using movaps
// Although it can be used correctly as the homogeneous component in 4x4 matrix transforms

// No need to return a vector in SSE funcs, as long as eax is the dest reg

__declspec(align(16)) class CVector
{	
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};

		float e[4];
	};

	CVector() {};
	CVector(float value) { x = y = z = value; };
	CVector(float vx, float vy, float vz) { x = vx; y = vy; z = vz; };

	// 4D Vector constructor, only use for 4x4 matrix transforms
	CVector(float vx, float vy, float vz, float vw) { x = vx; y = vy; z = vz; w = vw; };

	operator float*();
	operator const float*() const;

	operator D3DXVECTOR3*();
	operator const D3DXVECTOR3*() const;

	float operator[](int index);

	CVector& operator+=(const CVector &v);
	CVector& operator-=(const CVector &v);
	CVector& operator*=(const float &scalar);
	CVector& operator/=(const float &scalar);
	
	CVector operator+(const CVector &v) const;
	CVector operator-(const CVector &v) const;
	CVector operator*(const float scalar) const;
	CVector operator/(const float scalar) const;

	// Cross product
	CVector operator^(const CVector &v) const;
	
	// Dot Product (calls Dot, not DotW)
	float operator*(const CVector &v) const;

	BOOL operator==(const CVector &v) const;
	BOOL operator!=(const CVector &v) const;

	CVector operator-() const;

	CVector& Normalise();

	float GetMagnitude() const;
	float GetSqrMagnitude() const;

	static CVector FPU_Negate(const CVector &v);
	static CVector SSE_Negate(const CVector &v);
	static CVector (*Negate)(const CVector &v);

	static CVector FPU_Multiply(const CVector &v, const float Scalar);
	static CVector SSE_Multiply(const CVector &v, const float Scalar);
	static CVector (*Multiply)(const CVector &v, const float Scalar);

	static CVector FPU_Divide(const CVector &v, const float Scalar);
	static CVector SSE_Divide(const CVector &v, const float Scalar);
	static CVector (*Divide)(const CVector &v, const float Scalar);
	
	static CVector FPU_Add(const CVector &v1, const CVector &v2);
	static CVector SSE_Add(const CVector &v1, const CVector &v2);
	static CVector (*Add)(const CVector &v1, const CVector &v2);

	static CVector FPU_Subtract(const CVector &v1, const CVector &v2);
	static CVector SSE_Subtract(const CVector &v1, const CVector &v2);
	static CVector (*Subtract)(const CVector &v1, const CVector &v2);

	static CVector FPU_Cross(const CVector &v1, const CVector &v2);
	static CVector SSE_Cross(const CVector &v1, const CVector &v2);
	static CVector (*Cross)(const CVector &v1, const CVector &v2);

	static float FPU_SqrMagnitude(const CVector &v);
	static float SSE_SqrMagnitude(const CVector &v);
	static float (*SqrMagnitude)(const CVector &v);

	static float FPU_Magnitude(const CVector &v);
	static float SSE_Magnitude(const CVector &v);
	static float (*Magnitude)(const CVector &v);

	// Default dot, ignores w component
	static float FPU_Dot(const CVector &v1, const CVector &v2);
	static float SSE_Dot(const CVector &v1, const CVector &v2);
	static float (*Dot)(const CVector &v1, const CVector &v2);

	// This dot uses w component
	static float FPU_DotW(const CVector &v1, const CVector &v2);
	static float SSE_DotW(const CVector &v1, const CVector &v2);
	static float (*DotW)(const CVector &v1, const CVector &v2);

	static BOOL IsNormalised(const CVector &v);

	static CVector Normalise(const CVector &v);

	static CVector Lerp(const CVector &v1, const CVector &v2, const float t);

	friend CVector operator*(const float scalar, const CVector &v);

	////////////////////////////////////////////////////////////
	// operators new and delete aligned to 16 byte boundaries //
	// Taken from MS D3DMATRIXA16							  //
	////////////////////////////////////////////////////////////
	void* operator new(size_t s)
    {
        LPBYTE p = ::new BYTE[s + 16];
        if (p)
        {
            BYTE offset = (BYTE)(16 - ((UINT_PTR)p & 15));
            p += offset;
            p[-1] = offset;
        }
        return p;
    };

    void* operator new[](size_t s)
    {
        LPBYTE p = ::new BYTE[s + 16];
        if (p)
        {
            BYTE offset = (BYTE)(16 - ((UINT_PTR)p & 15));
            p += offset;
            p[-1] = offset;
        }
        return p;
    };

    void operator delete(void* p)
    {
        if(p)
        {
            BYTE* pb = static_cast<BYTE*>(p);
            pb -= pb[-1];
            ::delete [] pb;
        }
    };

    void operator delete[](void* p)
    {
        if(p)
        {
            BYTE* pb = static_cast<BYTE*>(p);
            pb -= pb[-1];
            ::delete [] pb;
        }
    };
};

#ifndef _DEBUG
	#define VEC_INLINE __forceinline
	#include "vector3.inl"
#else
	#define VEC_INLINE 
#endif

// CVector.h included
#endif