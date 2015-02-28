#ifndef _cmatrix_h
#define _cmatrix_h

#include "base.h"
#include "plane.h"
#include "vector3.h"

class CPlane;

// Left Handed, Column-Major matrix class (each column contains a vector)
// No need to return a Matrix on SSE funcs, as long as eax is the dest reg

__declspec(align(16)) class CMatrix
{
public:
	float e[4][4];

	// Default constructor, should be optimised out in release builds
	CMatrix() {};

	// This constructor sets all the elements to the specifed value
	CMatrix(const float Value);

	// This constructor sets all the elements from the explicit values
	CMatrix(const float e00, const float e01, const float e02, const float e03,
			const float e10, const float e11, const float e12, const float e13,
			const float e20, const float e21, const float e22, const float e23, 
			const float e30, const float e31, const float e32, const float e33); 

	// Casting operators, used to cast to a D3D specific matrix
	operator D3DXMATRIX*() const;
	operator const D3DXMATRIX*() const;

	// Matrix Multiply *this * Matrix, leaves *this intact
	// Returns: new CMatrix containing the resulting matrix
	CMatrix operator*(const CMatrix &m) const;

	// Multiply *this by Matrix, storing result in *this
	// Returns: *this
	CMatrix& operator*=(const CMatrix &m);

	// Determinant: Calculates the determinant of a matrix
	// Returns: float Determinant
	static float Determinant(const CMatrix &m);

    // Translation: Builds a translation matrix in New matrix from the specified values
	// Returns: New matrix
	static CMatrix Translation(const float x, const float y, const float z);

	// LookAtLH: Builds a Left Handed Look at matrix in New matrix
	// Returns: New matrix
	static CMatrix LookAtLH(const CVector &Eye, const CVector &At, const CVector &Up);

	// RotationAxis:  Builds a rotation matrix in New matrix that rotates Angle radians around the axis specified by the Vector Axis
	// Returns: New matrix
	static CMatrix RotationAxis(const CVector &Axis, const float Angle);

	// Identity: Builds an identity matrix in New matrix
	// Returns: New matrix
	static CMatrix Identity();

	// Perspective: Builds a left handed perspective matrix based on a FOV in New matrix
	// Returns: New matrix
	static CMatrix PerspectiveFovLH(const float FOV, const float Aspect, const float zNear, const float zFar);

	// YawPitchRoll: Builds a matrix in New matrix that rotates around all 3 axis by the specified amounts (in radians)
	// Returns: New matrix
	static CMatrix YawPitchRoll(const float Yaw, const float Pitch, const float Roll);

	// Scaling:  Builds a scaling matrix in New matrix
	// Returns: New matrix
	static CMatrix Scaling(const float sx, const float sy, const float sz);

	// RotationX: Builds a matrix that rotates around the X Axis in New matrix
	// Returns: New matrix
	static CMatrix RotationX(const float Angle);

	// RotationY:  As above, but around the Y Axis
	static CMatrix RotationY(const float Angle);

	// RotationZ: As above, but around the Z Axis
	static CMatrix RotationZ(const float Angle);

	// Multiply: Multiplies LH by RH, storing result in New matrix
	// Returns: New matrix 
	static CMatrix FPU_Multiply(const CMatrix &LH, const CMatrix &RH);
	static CMatrix SSE_Multiply(const CMatrix &LH, const CMatrix &RH);
	static CMatrix (*Multiply)(const CMatrix &LH, const CMatrix &RH);

	// Inverse: Makes New matrix =  inverse of New matrix
	// Returns: New matrix
	static CMatrix Inverse(const CMatrix &m);
	static CMatrix Inverse(const CMatrix &m, const float det);

	// Transpose: Makes New matrix = transpsoe of New matrix
	// Returns: New matrix
	static CMatrix FPU_Transpose(const CMatrix &m);
	static CMatrix SSE_Transpose(const CMatrix &m);
	static CMatrix (*Transpose)(const CMatrix &m);

	// ZeroMatrix: Zeros out all elements of the matrix
	// Returns: New matrix full of zeros
	static CMatrix FPU_ZeroMatrix();
	static CMatrix SSE_ZeroMatrix();
	static CMatrix (*ZeroMatrix)();

	// Reflect: Builds a matrix to reflect about the given plane, assumes plane is normalised
	// Returns: New reflection matrix
	static CMatrix Reflect(const CPlane &p);

	// Shadow: Builds a matrix that flattens geometry onto a plane, assumses a normalised plane
	static CMatrix Shadow(const CPlane &plane, const CVector &light);

	// Transform: Transform nbCoords number of vectors pointed at by *vec with the matrix in New matrix
	// Resturns: Nothing
	void Transform(CVector *vec, const int nbCoords) const;

	// TransformCoords: Transform nbCoords number of vectors pointed at by *vec with the matrix in New matrix
	// projecting result back into w = 1
	// Returns: Nothing
	void TransformCoords(CVector *vec, const nbCoords) const;

	// operators new and delete aligned to 16 byte boundaries
	// Taken from MS D3DMATRIXA16
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

// Inline the functions if not in debug build
#ifndef _DEBUG
	#define MAT_INLINE __forceinline
	#include "matrix.inl"
#else
	#define MAT_INLINE 
#endif

// matrix.h included
#endif
