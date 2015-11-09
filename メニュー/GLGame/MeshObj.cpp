#include "MeshObj.h"
#include "Graphics.h"

// メッシュ設定
void CMeshObj::SetMesh(MESH* pMesh)
{
	m_pMesh = pMesh;
	if (m_pMesh) {
		VECTOR3 vMin, vMax;
		ComputeBoundingBox(m_pMesh, &vMin, &vMax);
		m_vBBox.x = (vMax.x - vMin.x) * 0.5f;
		m_vBBox.y = (vMax.y - vMin.y) * 0.5f;
		m_vBBox.z = (vMax.z - vMin.z) * 0.5f;
		ComputeBoundingSphere(m_pMesh, &m_vCenter, &m_fRadius);
	}
}

// コンストラクタ
CMeshObj::CMeshObj(CScene* pScene) : C3DObj(pScene)
{
	m_id = ID_MESHOBJ;

	m_pMesh = NULL;
}

// デストラクタ
CMeshObj::~CMeshObj(void)
{
}

// 更新前処理
void CMeshObj::PreUpdate(void)
{
	// なし
}

// 更新後処理
void CMeshObj::PostUpdate(void)
{
	// なし
}

// 描画
void CMeshObj::Draw(void)
{
	if (m_pMesh) {
		DrawMeshNoAlpha(&m_world, m_pMesh);
	}
}

// 描画後処理
void CMeshObj::PostDraw(void)
{
	if (m_pMesh) {
		DrawMeshAlpha(&m_world, m_pMesh);
	}
}
