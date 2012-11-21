#ifndef UTILS_H
#define UTILS_H

#include <string>

#include <d3dx10.h>
#include <dxerr.h>
#include <cassert>

using namespace std;

//*************************************************************************
// Simple d3d error checker for book demos.
//*************************************************************************

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif

// Release COM objects if not NULL and set them to NULL
#define SAFE_RELEASE(x)											\
	if( x )														\
	{															\
		x->Release();											\
		(x) = NULL; 											\
	}

//wstring wstrFromString(string from)
//{
//	wstring to(from.length(), L' ');
//	copy(from.begin(), from.end(), to.begin());
//	return to;
//}

#endif //UTILS_H