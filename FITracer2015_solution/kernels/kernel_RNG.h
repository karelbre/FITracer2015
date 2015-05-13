/*
	Name: kernel_RNG.h
	Desc: Random number generator for OpenCL PT.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _KERNEL_RNG_H_
#define _KERNEL_RNG_H_

#include "kernel_types.h"
#include "kernel_functions.h"

uint MWC64X(uint2 *state)
{
	enum { A = 4294883355U };
	uint x = (*state).x, c = (*state).y;  // Unpack the state
	uint res = x^c;                     // Calculate the result
	uint hi = mul_hi(x, A);              // Step the RNG
	x = x*A + c;
	c = hi + (x<c);
	*state = (uint2)(x, c);              // Pack the state back up
	return res;                       // Return the next result
}

float randomFloat(uint2 *state, float size)
{
	const uint rndValue = MWC64X(state);
	return (((convert_float(rndValue & 65535)) / 65536)*size);
}

float eval(float x)
{
	return fabs(x - 5000);
}

float mutate(uint2 *state, float x)
{
	return x + randomFloat(state, 1) - 0.5f;
}

float2 getBestOf(uint2 *state, float2 pInput, uint cnt)
{
	float2 result = pInput;

	for (uint i = 0; i<cnt; i++)
	{
		float x = mutate(state, pInput.x);
		float y = eval(x);
		if (y < result.y)
		{
			result.x = x;
			result.y = y;
		}
	}

	return result;
}

uint xorshift_int(uint4* ctx) {
	uint t = (*ctx).x ^ ((*ctx).x << 11);
	*ctx = (*ctx).yzww;
	(*ctx).w = (*ctx).w ^ ((*ctx).w >> 19) ^ (t ^ (t >> 8));
	return (*ctx).w;
}

float xorshift_float(uint4* ctx) {
	return xorshift_int(ctx) * 2.3283064e-10;
}

float uniform_float(uint4* seed)
{
	float num = xorshift_int(seed) * 2.3283064e-3;
	return num;
}

#endif // _KERNEL_RNG_H_