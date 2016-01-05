// 行列/ベクトル
#ifndef _H_MATRIX_
#define _H_MATRIX_

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD	(float)(M_PI/180.0)	// ラジアン変換用定数

typedef union _VECTOR2 {	// 2Dベクトル
	float		f[2];
	struct {
		float	x, y;
	};
	struct {
		float	u, v;
	};
	struct {
		float	s, t;
	};
} VECTOR2;

typedef union _VECTOR3 {	// 3Dベクトル
	float		f[3];
	VECTOR2		v2;
	struct {
		float	x, y, z;
	};
	struct {
		float	r, g, b;
	};
} VECTOR3;

typedef union _VECTOR4 {	// 4Dベクトル
	float		f[4];
	VECTOR3		v3;
	struct {
		float	x, y, z, w;
	};
	struct {
		float	r, g, b, a;
	};
} VECTOR4;

typedef union _MATRIX {	// 変換行列
	float		f[16];
	VECTOR4		v[4];
	struct {
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};
	float		m[4][4];
} MATRIX;
//
//VECTOR3 & operator+(VECTOR3 &a, VECTOR3 &b){
//	a.x += b.x;
//	a.y += b.y;
//	a.z += b.z;
//
//	return a;
//}
//VECTOR3 & operator-(VECTOR3 &a, VECTOR3 &b){
//	a.x -= b.x;
//	a.y -= b.y;
//	a.z -= b.z;
//
//	return a;
//}

void MatrixIdentity(MATRIX* pMatrix);
void MatrixTranslation(MATRIX* pMatrix, float x, float y, float z);
void MatrixRotationX(MATRIX* pMatrix, float fAngle);
void MatrixRotationY(MATRIX* pMatrix, float fAngle);
void MatrixRotationZ(MATRIX* pMatrix, float fAngle);
void MatrixMultiply(MATRIX* pOut, MATRIX* pL, MATRIX* pR);
void MatrixScaling(MATRIX* pMatrix, float x, float y, float z);
void MatrixRotationAxis(MATRIX* pOut, VECTOR3* pAxis, float fAngle);
void MatrixRotationYawPitchRoll(MATRIX* pOut, float fYaw, float fPitch, float fRoll);
void Vec4Transform(VECTOR4* pOut, MATRIX* pMatrix, VECTOR4* pV);
void Vec3TransformCoord(VECTOR3* pOut, MATRIX* pMatrix, VECTOR3* pV);
void Vec3TransformNormal(VECTOR3* pOut, MATRIX* pMatrix, VECTOR3* pV);
float Vec3LengthSq(VECTOR3* pIn);
float Vec3Length(VECTOR3* pIn);
void Vec3Normalize(VECTOR3* pOut, VECTOR3* pIn);
void Vec3Cross(VECTOR3* pOut, VECTOR3* pL, VECTOR3* pR);
float Vec3Dot(VECTOR3* pL, VECTOR3* pR);
void Vec3Set(VECTOR3* Vec3,float x, float y, float z);
VECTOR3 Vec3GetAxisX(MATRIX mat);
VECTOR3 Vec3GetAxisY(MATRIX mat);
VECTOR3 Vec3GetAxisZ(MATRIX mat);

MATRIX MtxLookAt(VECTOR3 eye, VECTOR3 center, VECTOR3 up);
#endif