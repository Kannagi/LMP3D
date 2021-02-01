//#include <stdlib.h>
//#include <stdio.h>


#include "AURAE/AURAE.h"

Vector2i AURAE_Type_Vector2i(int x,int y)
{
	Vector2i vector;

	vector.x = x;
	vector.y = y;
	return vector;
}

Vector3i AURAE_Type_Vector3i(int x,int y,int z)
{
	Vector3i vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

Recti AURAE_Type_Recti(int x,int y,int w,int h)
{
	Recti rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}


Vector2s AURAE_Type_Vector2s(int x,int y)
{
	Vector2s vector;

	vector.x = x;
	vector.y = y;
	return vector;
}

Vector3s AURAE_Type_Vector3s(int x,int y,int z)
{
	Vector3s vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

Rects AURAE_Type_Rects(int x,int y,int w,int h)
{
	Rects rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

Vector2 AURAE_Type_Vector2(float x,float y)
{
	Vector2 vector;

	vector.x = x;
	vector.y = y;
	return vector;
}

Vector3 AURAE_Type_Vector3(float x,float y,float z)
{
	Vector3 vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

Rectf AURAE_Type_Rectf(float x,float y,float w,float h)
{
	Rectf rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

