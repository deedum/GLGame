#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Player.h"
#include "Input.h"
#include "Sound.h"

// �萔
#define PATH_PLAYERMESH	"../data/model/gunmo.x"
#define SPEED_ROLL		2.0f				// Y����]���x
#define SPEED_FWD		5.0f				// �O�i���x
#define SPEED_BACK		(SPEED_FWD*-0.5f)	// ��i���x
#define SPEED_JUMP		20.0f				// �W�����v���x
#define GRAVITY			-0.98f				// �d��

// �R���X�g���N�^
CPlayer::CPlayer(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_PLAYER;
}

// �f�X�g���N�^
CPlayer::~CPlayer(void)
{
}

// ������
void CPlayer::Init(void)
{
	LoadMesh(&m_mesh, PATH_PLAYERMESH);
	SetMesh(&m_mesh);
}

// ���
void CPlayer::Fin(void)
{
	SetMesh(NULL);
	ReleaseMesh(&m_mesh);
}

// �������㏈��
void CPlayer::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	m_accel.y = GRAVITY;
	C3DObj::Update();

	Update();
}

// �X�V
void CPlayer::Update(void)
{
	// ���[���h�}�g���b�N�X���R�s�[
	MATRIX mWorld = m_world;

	// ���݈ʒu�擾
	m_pos = mWorld.v[3].v3;
	mWorld._41 = mWorld._42 = mWorld._43 = 0.0f;

	// Y����]�p�擾
	m_angle.y = 0.0f;
	if (CInput::GetKeyPress(VK_LEFT)) {
		m_angle.y = -SPEED_ROLL;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_angle.y = SPEED_ROLL;
	}

	// ���[�J�����W���擾
	VECTOR3 vX;
	VECTOR3 vY = mWorld.v[1].v3;
	VECTOR3 vZ;

	// Y����]
	MATRIX mRoll;
	MatrixRotationAxis(&mRoll, &vY, RAD * m_angle.y);
	MatrixMultiply(&mWorld, &mRoll, &mWorld);

	// ���[�J�����W���Ď擾
	vX = mWorld.v[0].v3;
	vY = mWorld.v[1].v3;
	vZ = mWorld.v[2].v3;

	// ���x�X�V
	m_speed.x += m_accel.x;
	m_speed.y += m_accel.y;
	m_speed.z += m_accel.z;

	// �W�����v
	if (m_pos.y <= 0.0f && CInput::GetKeyTrigger(VK_SPACE)) {
		CSound::Play(SE_JUMP);
		m_speed.y += SPEED_JUMP;
	}
	if (CInput::GetKeyTrigger('C')) {
		CSound::Play(SE_COIN);
	}

	// �O��i
	if (CInput::GetKeyPress(VK_UP)) {
		m_speed.x = vZ.x * SPEED_FWD;
		m_speed.z = vZ.z * SPEED_FWD;
	} else if (CInput::GetKeyPress(VK_DOWN)) {
		m_speed.x = vZ.x * SPEED_BACK;
		m_speed.z = vZ.z * SPEED_BACK;
	} else {
		m_speed.x = m_speed.z = 0.0f;
	}
	m_pos.x += m_speed.x;
	m_pos.y += m_speed.y;
	m_pos.z += m_speed.z;

	if (m_pos.y <= 0.0f) {
		m_pos.y = 0.0f;
		m_speed.y = 0.0f;
	}

	// ���݈ʒu���X�V
	mWorld.v[3].v3 = m_pos;

	// ���[���h�}�g���b�N�X�X�V
	m_world = mWorld;
}

// �`��
void CPlayer::Draw(void)
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
void CPlayer::PostDraw(void)
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