#pragma once
#include <windows.h>
#include <DirectXMath.h>
using namespace DirectX;

#if !defined(XMFINLINE)
#define XMFINLINE __forceinline
#endif

//------------------------------------------------------------------------------

// Using a control vector made up of 16 bytes from 0-31, remap V1 and V2's byte
// entries into a single 16 byte vector and return it. Index 0-15 = V1,
// 16-31 = V2
XMFINLINE XMVECTOR XMVectorPermute
(
	FXMVECTOR V1,
	FXMVECTOR V2,
	FXMVECTOR Control
)
{
#if defined(_XM_NO_INTRINSICS_)
	const BYTE* aByte[2];
	XMVECTOR Result;
	UINT i, uIndex, VectorIndex;
	const BYTE* pControl;
	BYTE* pWork;

	// Indices must be in range from 0 to 31
	XMASSERT((Control.vector4_u32[0] & 0xE0E0E0E0) == 0);
	XMASSERT((Control.vector4_u32[1] & 0xE0E0E0E0) == 0);
	XMASSERT((Control.vector4_u32[2] & 0xE0E0E0E0) == 0);
	XMASSERT((Control.vector4_u32[3] & 0xE0E0E0E0) == 0);

	// 0-15 = V1, 16-31 = V2
	aByte[0] = (const BYTE*)(&V1);
	aByte[1] = (const BYTE*)(&V2);
	i = 16;
	pControl = (const BYTE*)(&Control);
	pWork = (BYTE*)(&Result);
	do {
		// Get the byte to map from
		uIndex = pControl[0];
		++pControl;
		VectorIndex = (uIndex >> 4) & 1;
		uIndex &= 0x0F;
#if defined(_XM_LITTLEENDIAN_)
		uIndex ^= 3; // Swap byte ordering on little endian machines
#endif
		pWork[0] = aByte[VectorIndex][uIndex];
		++pWork;
	} while (--i);
	return Result;
#elif defined(_XM_SSE_INTRINSICS_)
#if defined(_PREFAST_) || defined(XMDEBUG)
	// Indices must be in range from 0 to 31
	static const XMVECTORI32 PremuteTest = { 0xE0E0E0E0,0xE0E0E0E0,0xE0E0E0E0,0xE0E0E0E0 };
	XMVECTOR vAssert = _mm_and_ps(Control, PremuteTest);
	__m128i vAsserti = _mm_cmpeq_epi32(reinterpret_cast<const __m128i*>(&vAssert)[0], g_XMZero);
	XMASSERT(_mm_movemask_ps(*reinterpret_cast<const __m128*>(&vAsserti)) == 0xf);
#endif
	// Store the vectors onto local memory on the stack
	XMVECTOR Array[2];
	Array[0] = V1;
	Array[1] = V2;
	// Output vector, on the stack
	XMVECTORU8 vResult;
	// Get pointer to the two vectors on the stack
	const BYTE* pInput = reinterpret_cast<const BYTE*>(Array);
	// Store the Control vector on the stack to access the bytes
	// don't use Control, it can cause a register variable to spill on the stack.
	XMVECTORU8 vControl;
	vControl.v = Control;   // Write to memory
	UINT i = 0;
	do {
		UINT ComponentIndex = vControl.u[i] & 0x1FU;
		ComponentIndex ^= 3; // Swap byte ordering
		vResult.u[i] = pInput[ComponentIndex];
	} while (++i < 16);
	return vResult;
#else // _XM_SSE_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}