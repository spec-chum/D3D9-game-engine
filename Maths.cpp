#include "maths.h"

#ifdef _DEBUG
	#include "maths.inl"
#endif

const float CMaths::EPSILON = 1e-5f;					// Has to be a little higher due to SSE
const float CMaths::PI = 3.141592654f;
const unsigned int CMaths::NEG_MASK = 0x80000000;	// Used with xor to negate value
const unsigned int CMaths::ABS_MASK = 0x7FFFFFFF;	// Used for ssefabs()

float (*CMaths::RSqrt)(const float value) = 0;
float (*CMaths::Sqrt)(const float value) = 0;

float (*CMaths::ASin)(const float angle) = 0;
float (*CMaths::ACos)(const float angle) = 0;

float (*CMaths::Rcp)(const float value) = 0;

