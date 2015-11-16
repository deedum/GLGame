#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Stage.h"
#include "Input.h"
#include "Sound.h"

// �萔
#define PATH_PLAYERMESH	"../data/model/Stage/StageData.x"
#define SPEED_ROLL		2.0f				// Y����]���x
#define SPEED_FWD		5.0f				// �O�i���x
#define SPEED_BACK		(SPEED_FWD*-0.5f)	// ��i���x
#define SPEED_JUMP		20.0f				// �W�����v���x
#define GRAVITY			-0.98f				// �d��

// �R���X�g���N�^
CStage::CStage(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_PLAYER;
}

// �f�X�g���N�^
CStage::~CStage(void)
{
}

// ������
void CStage::Init(void)
{
	LoadMesh(&m_mesh, PATH_PLAYERMESH);
	SetMesh(&m_mesh);
}

// ���
void CStage::Fin(void)
{
	SetMesh(NULL);
	ReleaseMesh(&m_mesh);
}

// �������㏈��
void CStage::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	m_accel.y = GRAVITY;
	C3DObj::Update();

	Update();
}

// �X�V
void CStage::Update(void)
{

}

// �`��
void CStage::Draw(void)
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
void CStage::PostDraw(void)
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