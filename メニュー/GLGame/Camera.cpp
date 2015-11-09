#include "Camera.h"
#include "Graphics.h"
#include "Input.h"

// �����l
static struct CAMPOS {
	VECTOR3	pos;
	VECTOR3	at;
	VECTOR3	up;
} g_campos[] = {
	{	// (0:�Œ�)
		{0.0f, 10.0f, -15.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 1:1�l��
		{0.0f, 0.654f, 3.4204f},
		{0.0f, 0.654f, 4.4204f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 2:�Œ�
		{0.0f, 100.0f, -500.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 3:3�l��
		{0.0f, 10.0f, -25.0f},
		{0.0f, 7.5f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 4:�v���C���[����
		{0.0f, 100.0f, -500.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
};

// �X�V�㏈��
void CCamera::PostUpdate2(void)
{
	if (CInput::GetKeyPress(VK_NUMPAD1) || CInput::GetKeyPress('1')) {
		m_camtype = 1;
	} else if (CInput::GetKeyPress(VK_NUMPAD2) || CInput::GetKeyPress('2')) {
		m_camtype = 2;
	} else if (CInput::GetKeyPress(VK_NUMPAD3) || CInput::GetKeyPress('3')) {
		m_camtype = 3;
	}
	switch (m_camtype) {
	case CT_FIRSTPERSON:	// 1�l��
	case CT_THIRDPERSON:	// 3�l��
	{
		MATRIX& mWorld = m_pPlayer->GetWorld();
		Vec3TransformCoord(&m_pos, &mWorld, &m_pCamInfo[m_camtype].m_eye);
		Vec3TransformCoord(&m_at, &mWorld, &m_pCamInfo[m_camtype].m_at);
		Vec3TransformNormal(&m_up, &mWorld, &m_pCamInfo[m_camtype].m_up);
		break;
	}
	case CT_FIXED:			// �Œ�
		m_pos = m_pCamInfo[2].m_eye;
		m_at = m_pCamInfo[2].m_at;
		m_up = m_pCamInfo[2].m_up;
		break;
	case CT_VIEWPLAYER:		// �v���C���[����
		if (m_pPlayer)
			m_at = m_pPlayer->GetPos();
		break;
	default:				// ���[�U�ݒ�
		break;
	}
	if (m_pSky) m_pSky->SetPos(&m_pos);
}

// �`�揉����
void CCamera::InitDraw(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_pos.x, m_pos.y, m_pos.z,
		m_at.x, m_at.y, m_at.z,
		m_up.x, m_up.y, m_up.z);

	if (m_pSky) m_pSky->RawDraw();	// �X�J�C�h�[���`��
}

// �R���X�g���N�^
CCamera::CCamera(CScene* pScene) : C3DObj(pScene)
{
	m_id = ID_CAMERA;

	m_pos.x = 0.0f, m_pos.y = 10.0f, m_pos.z = 15.0f;
	m_at.x = m_at.y = m_at.z = 0.0f;
	m_up.x = 0.0f, m_up.y = 1.0f, m_up.z = 0.0f;

	m_camtype = CT_FIXED;
	m_pCamInfo = new CCameraInfo[_countof(g_campos)];
	for (int i = 0; i < _countof(g_campos); i++) {
		m_pCamInfo[i].m_eye = g_campos[i].pos;
		m_pCamInfo[i].m_at = g_campos[i].at;
		m_pCamInfo[i].m_up = g_campos[i].up;
	}

	m_pPlayer = NULL;
	m_pSky = NULL;
}

// �f�X�g���N�^
CCamera::~CCamera(void)
{
	delete[] m_pCamInfo;
}

// �������㏈��
void CCamera::PostInit(void)
{
	m_pPlayer = (CPlayer*)C3DObj::Find(ID_PLAYER);
	m_pSky = (CSky*)C3DObj::Find(ID_SKY);
}

// �X�V
void CCamera::Update(void)
{
	// �������Ȃ�
}

// �`��
void CCamera::Draw(void)
{
	// �������Ȃ�
}

// �����_�ݒ�
void CCamera::LookAt(VECTOR3* pAt)
{
	if (pAt) {
		m_at = *pAt;
	} else {
		m_at.x = m_at.y = m_at.z = 0.0f;
	}
}

// ����x�N�g���ݒ�
void CCamera::SetUp(VECTOR3* pUp)
{
	if (pUp) {
		m_up = *pUp;
	} else {
		m_up.x = m_up.z = 0.0f;
		m_up.y =1.0f;
	}
}