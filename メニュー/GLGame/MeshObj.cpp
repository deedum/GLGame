#include "MeshObj.h"
#include "Graphics.h"

// ���b�V���ݒ�
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

// �R���X�g���N�^
CMeshObj::CMeshObj(CScene* pScene) : C3DObj(pScene)
{
	m_id = ID_MESHOBJ;

	m_pMesh = NULL;
}

// �f�X�g���N�^
CMeshObj::~CMeshObj(void)
{
}

// �X�V�O����
void CMeshObj::PreUpdate(void)
{
	// �Ȃ�
}

// �X�V�㏈��
void CMeshObj::PostUpdate(void)
{
	// �Ȃ�
}

// �`��
void CMeshObj::Draw(void)
{
	if (m_pMesh) {
		DrawMeshNoAlpha(&m_world, m_pMesh);
	}
}

// �`��㏈��
void CMeshObj::PostDraw(void)
{
	if (m_pMesh) {
		DrawMeshAlpha(&m_world, m_pMesh);
	}
}
