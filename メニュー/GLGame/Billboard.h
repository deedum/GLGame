#pragma once

class CBillboard;

#include "Matrix.h"
#include "Camera.h"

// 頂点フォーマット定義
typedef struct _FVERTEX {
	// GL_T2F_V3F
	GLfloat		tu, tv;			// テクスチャ座標
	GLfloat		x, y, z;		// 頂点座標
} FVERTEX;

class CBillboard
{
private:
	FVERTEX		m_vtxQuad[4];
	VECTOR2		m_size;
	VECTOR3		m_pos;
	MATRIX		m_world;
	GLuint		m_texId;
	static	CCamera*	m_pCamera;

public:
	CBillboard(void);
	CBillboard(GLuint texId, VECTOR2 size, VECTOR3 pos);
	~CBillboard();
	void SetPos(VECTOR3 pos){m_pos = pos;}
	void PostUpdate(void);
	void Draw(void);
	void Release();
	static CBillboard* Create(GLuint texTd, VECTOR2 size, VECTOR3 pos);
	static void SetCamera(CCamera* pCamera){m_pCamera = pCamera;}
};
