#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Field.h"

// �萔
#define PATH_FIELDMESH	"../data/model/Stage/StageData1.x"

// �R���X�g���N�^
CField::CField(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_STAGE;
}

// �f�X�g���N�^
CField::~CField(void)
{
}

// ������
void CField::Init(void)
{
	m_mesh = new MESH;
	LoadMesh(m_mesh, PATH_FIELDMESH);
	SetMesh(m_mesh);
}

// ���
void CField::Fin(void)
{
	SetMesh(NULL);
	ReleaseMesh(m_mesh);
	delete m_mesh;
}

// �������㏈��
void CField::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	C3DObj::Update();

	Update();
}

// �X�V
void CField::Update(void)
{

}

// �`��
void CField::Draw(void)
{
	if (!m_pMesh) {
		return;
	}
	// ���������f���̂��߁AY��180�x��]
	MATRIX world;
	MatrixRotationY(&world, RAD * 180);
	MatrixMultiply(&world, &m_world, &world);
	DrawMeshNoAlpha(&world, m_pMesh);
}

// �`��㏈��
void CField::PostDraw(void)
{
	if (!m_pMesh) {
		return;
	}
	// ���������f���̂��߁AY��180�x��]
	MATRIX world;
	MatrixRotationY(&world, RAD * 180);
	MatrixMultiply(&world, &m_world, &world);
	DrawMeshAlpha(&world, m_pMesh);
}
