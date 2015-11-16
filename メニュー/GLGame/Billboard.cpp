#include "Billboard.h"

CCamera* CBillboard::m_pCamera = NULL;

CBillboard::CBillboard(GLuint texId, VECTOR2 size, VECTOR3 pos)
{
	m_texId = texId;
	m_size = size;
	m_pos = pos;

	m_vtxQuad[0].tu = 0.0f;
	m_vtxQuad[0].tv = 0.0f;
	m_vtxQuad[0].x  = pos.x - size.x * 0.5f;
	m_vtxQuad[0].y  = pos.y + size.y * 0.5f;
	m_vtxQuad[0].z  = pos.z;
	m_vtxQuad[1].tu = 1.0f;
	m_vtxQuad[1].tv = 0.0f;
	m_vtxQuad[1].x  = pos.x + size.x * 0.5f;
	m_vtxQuad[1].y  = pos.y + size.y * 0.5f;
	m_vtxQuad[1].z  = pos.z;
	m_vtxQuad[2].tu = 1.0f;
	m_vtxQuad[2].tv = 1.0f;
	m_vtxQuad[2].x  = pos.x + size.x * 0.5f;
	m_vtxQuad[2].y  = pos.y - size.y * 0.5f;
	m_vtxQuad[2].z  = pos.z;
	m_vtxQuad[3].tu = 0.0f;
	m_vtxQuad[3].tv = 1.0f;
	m_vtxQuad[3].x  = pos.x - size.x * 0.5f;
	m_vtxQuad[3].y  = pos.y - size.y * 0.5f;
	m_vtxQuad[3].z  = pos.z;
}

CBillboard::CBillboard(void)
{
}

CBillboard::~CBillboard(void)
{
	Release();
}

CBillboard* CBillboard::Create(GLuint texId, VECTOR2 size, VECTOR3 pos)
{
	CBillboard* billboard = new CBillboard();

	billboard->m_texId = texId;
	billboard->m_size = size;
	billboard->m_pos = pos;
	billboard->m_vtxQuad[0].tu = 0.0f;
	billboard->m_vtxQuad[0].tv = 0.0f;
	billboard->m_vtxQuad[0].x  = pos.x;// - size.x * 0.5f;
	billboard->m_vtxQuad[0].y  = pos.y;// - size.y * 0.5f;
	billboard->m_vtxQuad[0].z  = pos.z;
	billboard->m_vtxQuad[1].tu = 0.0f;
	billboard->m_vtxQuad[1].tv = 1.0f;
	billboard->m_vtxQuad[1].x  = pos.x;// - size.x * 0.5f;
	billboard->m_vtxQuad[1].y  = pos.y + size.y;// * 0.5f;
	billboard->m_vtxQuad[1].z  = pos.z;
	billboard->m_vtxQuad[2].tu = 1.0f;
	billboard->m_vtxQuad[2].tv = 1.0f;
	billboard->m_vtxQuad[2].x  = pos.x + size.x;// * 0.5f;
	billboard->m_vtxQuad[2].y  = pos.y + size.y;// * 0.5f;
	billboard->m_vtxQuad[2].z  = pos.z;
	billboard->m_vtxQuad[3].tu = 1.0f;
	billboard->m_vtxQuad[3].tv = 0.0f;
	billboard->m_vtxQuad[3].x  = pos.x + size.x;// * 0.5f;
	billboard->m_vtxQuad[3].y  = pos.y;// - size.y * 0.5f;
	billboard->m_vtxQuad[3].z  = pos.z;
	return billboard;
}

// カメラの後に呼び出すこと
void CBillboard::PostUpdate(void)
{
	if(m_pCamera == NULL)	return;

	MATRIX mat;
	MATRIX matView = m_pCamera->GetWorld();
	MATRIX mWorld = m_world;
	MatrixIdentity(&mWorld);
	// 現在位置更新
	mWorld.v[3].v3 = m_pos;

	// ビルボード用ワールド変換行列作成
	mat._11 = matView._11;
	mat._12 = matView._21;
	mat._13 = matView._31;
	mat._14 = 0.0f; 
	mat._21 = matView._12;
	mat._22 = matView._22;
	mat._23 = matView._32;
	mat._24 = 0.0f;
	mat._31 = matView._13;
	mat._32 = matView._23;
	mat._33 = matView._33;
	mat._34 = 0.0f; 
	mat._41 = m_pos.x;
	mat._42 = m_pos.y;
	mat._43 = m_pos.z;
	mat._44 = 1.0f;  

	MatrixMultiply(&mWorld, &mat, &mWorld);

	m_world = mWorld;
}

void CBillboard::Draw(void)
{
	VECTOR4 diffuse = {1.0f,1.0f,1.0f,1.0f};
	VECTOR4	emissive = {1.0f,1.0f,1.0f,1.0f};				// 放射輝度(RGBX)

	// ワールドマトリックス設定
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(m_world.f);
	
	// αブレンド有効
	glEnable(GL_BLEND);
	// デプスバッファへの書き込みを禁止
	glDepthMask(GL_FALSE);
	// ブレンディングのための方程式を設定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse.f);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive.f);
				

	glEnable(GL_TEXTURE_2D);	// テクスチャ有効化
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glInterleavedArrays(GL_T2F_V3F, 0, m_vtxQuad);
	glDrawArrays(GL_QUADS, 0, 4);

	// デプスバッファへの書き込みを許可
	glDepthMask(GL_TRUE);
	// αブレンド無効
	glDisable(GL_BLEND);

	// ビューマトリックス復帰
	glPopMatrix();
}

void CBillboard::Release(void)
{

}