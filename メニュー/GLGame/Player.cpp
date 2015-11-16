#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Player.h"
#include "Input.h"
#include "Sound.h"
#include "Texture.h"

// �萔
#define PATH_PLAYERTEX	"../data/texture/whiteman.tga"
#define SPEED_ROLL		2.0f				// Y����]���x
#define SPEED_FWD		5.0f				// �O�i���x
#define SPEED_BACK		(SPEED_FWD*-0.5f)	// ��i���x
#define SPEED_JUMP		20.0f				// �W�����v���x
#define GRAVITY			-0.98f				// �d��

// �R���X�g���N�^
CPlayer::CPlayer(CScene* pScene) : C3DObj(pScene)
{
	m_id = ID_PLAYER;
	m_size.x = 46.0f;
	m_size.y = 80.0f;
}

// �f�X�g���N�^
CPlayer::~CPlayer(void)
{
}

// ������
void CPlayer::Init(void)
{
}

// ���
void CPlayer::Fin(void)
{
}

// �������㏈��
void CPlayer::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	m_accel.y = GRAVITY;
	//m_size.x = 80.0f;
	//m_size.y = 160.0f;
	m_tex = LoadTexture(PATH_PLAYERTEX);
	m_billboard = CBillboard::Create(m_tex, m_size, m_pos);	
	C3DObj::Update();

	Update();
}

// �X�V
void CPlayer::Update(void)
{
	if (CInput::GetKeyPress(VK_LEFT)) {
		m_pos.x += 1.0f;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_pos.x -= 1.0f;
	}

	m_billboard->SetPos(m_pos);

}

// �`��
void CPlayer::Draw(void)
{
	if (!m_billboard) {
		return;
	}
	// ���������f���̂��߁AY��180�x��]
	m_billboard->PostUpdate();
	m_billboard->Draw();
}

// �`��㏈��
void CPlayer::PostDraw(void)
{
	//if (!m_pMesh) {
	//	return;
	//}
	//// ���������f���̂��߁AY��180�x��]
	//MATRIX world;
	//MatrixRotationY(&world, RAD * 180);
	//MatrixMultiply(&world, &m_world, &world);
	//DrawMeshAlpha(&world, m_pMesh);
}