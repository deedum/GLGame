// Xファイル読込/表示(αマテリアル対応版)
#ifndef _H_MESH
#define _H_MESH

#include <windows.h>
#include <GL/gl.h>

#include "Matrix.h"

// マテリアル
typedef struct _MATERIAL {
	VECTOR4		diffuse;				// 拡散光/環境光
	float		power;					// 鏡面光の指数
	VECTOR4		specular;				// 鏡面光(RGBX)
	VECTOR4		emissive;				// 放射輝度(RGBX)
	GLuint		textureID;				// テクスチャID
} MATERIAL;

// モデル
typedef struct _MESH {
	int			vnum;					// 頂点数
	VECTOR3		*vertex;				// 頂点座標
	int			fnum;					// 面数
	int			*face;					// 面情報
	int			nnum;					// 法線ベクトル数
	VECTOR3		*normal;				// 法線ベクトル
	int			inum;					// 法線インデックス数
	int			*index;					// 法線インデックス
	VECTOR2		*uv;					// UV座標
	int			mnum;					// マテリアル数
	MATERIAL	*material;				// マテリアル
	int			*midx;					// マテリアルインデックス
} MESH;

int LoadMesh(MESH *mesh, const char *fname);	// モデル読込
void ReleaseMesh(MESH *mesh);					// モデル解放
void DrawMesh(MATRIX *matWorld, MESH *mesh);		// モデル表示
void DrawMeshAlpha(MATRIX *matWorld, MESH *mesh);	// モデル表示(α部のみ)
void DrawMeshNoAlpha(MATRIX *matWorld, MESH *mesh);	// モデル表示(非α部のみ)
int ComputeBoundingBox(MESH *mesh, VECTOR3 *pvMin, VECTOR3 *pvMax);
int ComputeBoundingSphere(MESH *mesh, VECTOR3 *pvCenter, float *pfRadius);

#endif // _H_MESH