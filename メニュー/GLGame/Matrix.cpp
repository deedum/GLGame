#include "Matrix.h"

// 単位行列設定
void MatrixIdentity(MATRIX* pMatrix)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			pMatrix->m[i][j] = 0.0f;
		}
	}
	pMatrix->_11 = pMatrix->_22 = pMatrix->_33 = pMatrix->_44 = 1.0f;
}

// 平行移動行列設定
void MatrixTranslation(MATRIX* pMatrix, float x, float y, float z)
{
	MatrixIdentity(pMatrix);
	pMatrix->_41 = x;
	pMatrix->_42 = y;
	pMatrix->_43 = z;
}

// X軸回転行列設定
void MatrixRotationX(MATRIX* pMatrix, float fAngle)
{
//                               |1  0      0    0|
//glRotate(θ,1,0,0) : MrotX(θ)=|0 cosθ -sinθ 0|
//                               |0 sinθ  cosθ 0|
//                               |0  0      0    1|
	MatrixIdentity(pMatrix);
	pMatrix->_22 = pMatrix->_33 = cosf(fAngle);
	pMatrix->_23 = sinf(fAngle);
	pMatrix->_32 = -pMatrix->_23;
}

// Y軸回転行列設定
void MatrixRotationY(MATRIX* pMatrix, float fAngle)
{
//                               | cosθ 0 sinθ 0|
//glRotate(θ,0,1,0) : MrotY(θ)=|  0    1  0    0|
//                               |-sinθ 0 cosθ 0|
//                               |  0    0  0    1|
	MatrixIdentity(pMatrix);
	pMatrix->_11 = pMatrix->_33 = cosf(fAngle);
	pMatrix->_31 = sinf(fAngle);
	pMatrix->_13 = -pMatrix->_31;
}

// Z軸回転行列設定
void MatrixRotationZ(MATRIX* pMatrix, float fAngle)
{
//                               |cosθ -sinθ 0 0|
//glRotate(θ,0,0,1) : MrotZ(θ)=|sinθ  cosθ 0 0|
//                               | 0      0    1 0|
//                               | 0      0    0 1|
	MatrixIdentity(pMatrix);
	pMatrix->_11 = pMatrix->_22 = cosf(fAngle);
	pMatrix->_12 = sinf(fAngle);
	pMatrix->_21 = -pMatrix->_12;
}

// 行列同士の積
void MatrixMultiply(MATRIX* pOut, MATRIX* pL, MATRIX* pR)
{
	MATRIX m;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m.m[i][j] = 0.0f;
			for (int k = 0; k < 4; k++) {
				m.m[i][j] += pL->m[k][j] * pR->m[i][k];
			}
		}
	}
	*pOut = m;
}

// 拡大縮小行列設定
void MatrixScaling(MATRIX* pMatrix, float x, float y, float z)
{
	MatrixIdentity(pMatrix);
	pMatrix->_11 = x;
	pMatrix->_22 = y;
	pMatrix->_33 = z;
}

// 任意軸周りの回転
void MatrixRotationAxis(MATRIX* pOut, VECTOR3* pAxis, float fAngle)
{
//|c+(1-c)xx  (1-c)xy+sz (1-c)xz-sy 0|
//|(1-c)xy-sz c+(1-c)yy  (1-c)yz+sx 0|
//|(1-c)xz+sy (1-c)yz-sx c+(1-c)zz  0|
//|0          0          0          1|
	float s = sinf(fAngle);
	float c = cosf(fAngle);
	pOut->_11 = c + (1 - c) * pAxis->x * pAxis->x;
	pOut->_12 = (1 - c) * pAxis->x * pAxis->y - s * pAxis->z;
	pOut->_13 = (1 - c) * pAxis->x * pAxis->z + s * pAxis->y;
	pOut->_14 = 0.0f;
	pOut->_21 = (1 - c) * pAxis->x * pAxis->y + s * pAxis->z;
	pOut->_22 = c + (1 - c) * pAxis->y * pAxis->y;
	pOut->_23 = (1 - c) * pAxis->y * pAxis->z - s * pAxis->x;
	pOut->_24 = 0.0f;
	pOut->_31 = (1 - c) * pAxis->x * pAxis->z - s * pAxis->y;
	pOut->_32 = (1 - c) * pAxis->y * pAxis->z + s * pAxis->x;
	pOut->_33 = c + (1 - c) * pAxis->z * pAxis->z;
	pOut->_34 = 0.0f;
	pOut->_41 = 0.0f;
	pOut->_42 = 0.0f;
	pOut->_43 = 0.0f;
	pOut->_44 = 1.0f;
}

// ヨー・ピッチ・ロールによる回転マトリックス生成
void MatrixRotationYawPitchRoll(MATRIX* pOut, float fYaw, float fPitch, float fRoll)
{
	MATRIX m;
	MatrixRotationY(pOut, fYaw);
	MatrixRotationX(&m, fPitch);
	MatrixMultiply(pOut, pOut, &m);
	MatrixRotationZ(&m, fRoll);
	MatrixMultiply(pOut, pOut, &m);
}

// 行列×4Dベクトル
void Vec4Transform(VECTOR4* pOut, MATRIX* pMatrix, VECTOR4* pV)
{
	VECTOR4 v;
	for (int i = 0; i < 4; i++) {
		v.f[i] = 0.0f;
		for (int j = 0; j < 4; j++) {
			v.f[i] += pMatrix->m[j][i] * pV->f[j];
		}
	}
	*pOut = v;
}

// 行列×3Dベクトル(w=1)
void Vec3TransformCoord(VECTOR3* pOut, MATRIX* pMatrix, VECTOR3* pV)
{
	VECTOR4 v;
	v.v3 = *pV;
	v.w = 1.0f;
	Vec4Transform(&v, pMatrix, &v);
	*pOut = v.v3;
}

// 行列×3Dベクトル(w=0)
void Vec3TransformNormal(VECTOR3* pOut, MATRIX* pMatrix, VECTOR3* pV)
{
	VECTOR4 v;
	v.v3 = *pV;
	v.w = 0.0f;
	Vec4Transform(&v, pMatrix, &v);
	*pOut = v.v3;
}

// ベクトルの長さの二乗を求める
float Vec3LengthSq(VECTOR3* pIn)
{
	return pIn->x * pIn->x + pIn->y * pIn->y + pIn->z * pIn->z;
}

// ベクトルの長さを求める
float Vec3Length(VECTOR3* pIn)
{
	return sqrtf(Vec3LengthSq(pIn));
}

// 単位ベクトル化 (正規化)
void Vec3Normalize(VECTOR3* pOut, VECTOR3* pIn)
{
	float fLen = Vec3Length(pIn);
	if (fLen > 0.0f) {
		pOut->x = pIn->x / fLen;
		pOut->y = pIn->y / fLen;
		pOut->z = pIn->z / fLen;
	} else {
		pOut->x = 1.0f;
		pOut->y = 0.0f;
		pOut->z = 0.0f;
	}
}

// 外積
void Vec3Cross(VECTOR3* pOut, VECTOR3* pL, VECTOR3* pR)
{
	VECTOR3 v;
	v.x = pL->y * pR->z - pL->z * pR->y;
	v.y = pL->z * pR->x - pL->x * pR->z;
	v.z = pL->x * pR->y - pL->y * pR->x;
	*pOut = v;
}

// 内積
float Vec3Dot(VECTOR3* pL, VECTOR3* pR)
{
	return pL->x * pR->x + pL->y * pR->y + pL->z * pR->z;
}

MATRIX MtxLookAt(VECTOR3 eye, VECTOR3 center, VECTOR3 up){
	VECTOR3 forward;
	
	forward.x = center.x - eye.x;
	forward.y = center.y - eye.y;
	forward.z = center.z - eye.z;

	Vec3Normalize(&forward, &forward);

	VECTOR3 side;
	Vec3Cross(&side, &forward, &up);

	Vec3Normalize(&side,&side);

	MATRIX Out;
	Out._11 = side.x;
	Out._12 = up.x;
	Out._13 = forward.x * -1.0f;
	Out._14 = 0;
	Out._21 = side.y;
	Out._22 = up.y;
	Out._23 = forward.y * -1.0f;
	Out._24 = 0;
	Out._31 = side.z;
	Out._32 = up.z;
	Out._33 = forward.z * -1.0f;
	Out._34 = 0;
	Out._41 = center.x;
	Out._42 = center.y;
	Out._43 = center.z;
	Out._44 = 1;

	MATRIX m;
	MatrixIdentity(&m);
	MatrixMultiply(&Out, &m, &Out);

	return Out;
}

void Vec3Set(VECTOR3* Vec3,float x, float y, float z)
{
	Vec3->x = x;
	Vec3->y = y;
	Vec3->z = z;

}
VECTOR3 Vec3GetAxisX(MATRIX mat)
{
	VECTOR3 Vec3;
	Vec3.x = mat._11;
	Vec3.y = mat._12;
	Vec3.z = mat._13;

	return Vec3;
}
VECTOR3 Vec3GetAxisY(MATRIX mat)
{
	VECTOR3 Vec3;
	Vec3.x = mat._21;
	Vec3.y = mat._22;
	Vec3.z = mat._23;

	return Vec3;
}
VECTOR3 Vec3GetAxisZ(MATRIX mat)
{
	VECTOR3 Vec3;
	Vec3.x = mat._31;
	Vec3.y = mat._32;
	Vec3.z = mat._33;

	return Vec3;
}