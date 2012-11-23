#ifndef MINMAX_H
#define MINMAX_H

#include "stdafx.h"

struct MinMax 
{
	D3DXVECTOR3 max,min;

	MinMax(D3DXVECTOR3 p_min, D3DXVECTOR3 p_max)
	{
		min = p_min;
		max = p_max;
	}
	MinMax()
	{
		min = max = D3DXVECTOR3(0,0,0);
	}
	D3DXVECTOR3* getVCorners()
	{
		D3DXVECTOR3* vertexPoints = new D3DXVECTOR3[8];
		D3DXVECTOR3 zero(	min.x,	min.y,	max.z),
			one(	min.x,	max.y,	max.z),
			two(	max.x,	max.y,	max.z),
			three(	max.x,	min.y,	max.z),
			four(	min.x,	min.y,	min.z),
			five(	min.x,	max.y,	min.z),
			six(	max.x,	max.y,	min.z),
			seven(	max.x,	min.y,	min.z);

		vertexPoints[0] = zero;
		vertexPoints[1] = one;
		vertexPoints[2] = two;
		vertexPoints[3] = three;
		vertexPoints[4] = four;
		vertexPoints[5] = five;
		vertexPoints[6] = six;
		vertexPoints[7] = seven;

		return vertexPoints;
	}
	DWORD* getICorners()
	{
		DWORD* indicies = new DWORD[24];
		indicies[0] = 0; indicies[1] = 1; 
		indicies[2] = 1; indicies[3] = 2; 
		indicies[4] = 2; indicies[5] = 3;
		indicies[6] = 3; indicies[7] = 0;

		indicies[8] = 0; indicies[9] = 4;
		indicies[10] = 4; indicies[11] = 5;
		indicies[12] = 5; indicies[13] = 1; 
		indicies[14] = 5; indicies[15] = 6; 

		indicies[16] = 6; indicies[17] = 7;
		indicies[18] = 7; indicies[19] = 4; 
		indicies[20] = 2; indicies[21] = 6; 
		indicies[22] = 3; indicies[23] = 7;

		return indicies;
	}
};

#endif