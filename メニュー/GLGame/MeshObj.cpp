#include "MeshObj.h"
#include "Graphics.h"

// ���b�V���ݒ�
void CMeshObj::SetMesh(MESH* pMesh)
{
	m_pMesh = pMesh;
	if (m_pMesh) {
		VECTOR3 vMin, vMax;
		ComputeBoundingBox(m_pMesh, &vMin, &vMax);
		VECTOR3 vRad, vCenter;
		vRad.x = (vMax.x - vMin.x) * 0.5f;
		vRad.y = (vMax.y - vMin.y) * 0.5f;
		vRad.z = (vMax.z - vMin.z) * 0.5f;
		m_OBB.SetRadius(vRad);

		ComputeBoundingSphere(m_pMesh, &vCenter, &m_fRadius);
		m_OBB.SetCenter(vCenter);
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


bool CMeshObj::IntersectOBB(COBB obb, MATRIX world)
{
	return(m_OBB.IntersectOBB(m_OBB, m_world, obb, world));
}

bool CMeshObj::IntersectAABB(COBB obb, MATRIX world, int type = 0)
{
	// ���S���W�����߂�
	VECTOR3 pos1 = obb.GetCenter();
	pos1.x += world._41;
	pos1.y += world._42;
	pos1.z += world._43;
	VECTOR3 pos2 = m_OBB.GetCenter();
	pos2.x += m_world._41;
	pos2.y += m_world._42;
	pos2.z += m_world._43;

	// ���E�{�b�N�X�T�C�Y�擾
	VECTOR3 r1 = obb.GetRadius();
	VECTOR3 r2 = m_OBB.GetRadius();

	// ����
	bool resultX,resultY,resultZ;
	resultX = resultY = resultZ = true;;
	if(type != 1){
		resultX = pos1.x  - r1.x  <= pos2.x + r2.x && pos2.x - r2.x <= pos1.x  + r1.x;
		if(!resultX) return false;
	}
	if(type != 2){
		resultZ = pos1.z  - r1.z  <= pos2.z + r2.z && pos2.z - r2.z <= pos1.z  + r1.z;
		if(!resultZ) return false;
	}
	
	resultY =  pos1.y  - r1.y  <= pos2.y + r2.y && pos2.y - r2.y <= pos1.y  + r1.y;

	return (resultX && resultY && resultZ);
}