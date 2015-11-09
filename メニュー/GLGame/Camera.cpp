#include "Camera.h"
#include "Graphics.h"
#include "Input.h"

// 初期値
static struct CAMPOS {
	VECTOR3	pos;
	VECTOR3	at;
	VECTOR3	up;
} g_campos[] = {
	{	// (0:固定)
		{0.0f, 10.0f, -15.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 1:1人称
		{0.0f, 0.654f, 3.4204f},
		{0.0f, 0.654f, 4.4204f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 2:固定
		{0.0f, 100.0f, -500.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 3:3人称
		{0.0f, 10.0f, -25.0f},
		{0.0f, 7.5f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 4:プレイヤー注視
		{0.0f, 100.0f, -500.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
};

// 更新後処理
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
	case CT_FIRSTPERSON:	// 1人称
	case CT_THIRDPERSON:	// 3人称
	{
		MATRIX& mWorld = m_pPlayer->GetWorld();
		Vec3TransformCoord(&m_pos, &mWorld, &m_pCamInfo[m_camtype].m_eye);
		Vec3TransformCoord(&m_at, &mWorld, &m_pCamInfo[m_camtype].m_at);
		Vec3TransformNormal(&m_up, &mWorld, &m_pCamInfo[m_camtype].m_up);
		break;
	}
	case CT_FIXED:			// 固定
		m_pos = m_pCamInfo[2].m_eye;
		m_at = m_pCamInfo[2].m_at;
		m_up = m_pCamInfo[2].m_up;
		break;
	case CT_VIEWPLAYER:		// プレイヤー注視
		if (m_pPlayer)
			m_at = m_pPlayer->GetPos();
		break;
	default:				// ユーザ設定
		break;
	}
	if (m_pSky) m_pSky->SetPos(&m_pos);
}

// 描画初期化
void CCamera::InitDraw(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_pos.x, m_pos.y, m_pos.z,
		m_at.x, m_at.y, m_at.z,
		m_up.x, m_up.y, m_up.z);

	if (m_pSky) m_pSky->RawDraw();	// スカイドーム描画
}

// コンストラクタ
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

// デストラクタ
CCamera::~CCamera(void)
{
	delete[] m_pCamInfo;
}

// 初期化後処理
void CCamera::PostInit(void)
{
	m_pPlayer = (CPlayer*)C3DObj::Find(ID_PLAYER);
	m_pSky = (CSky*)C3DObj::Find(ID_SKY);
}

// 更新
void CCamera::Update(void)
{
	// 何もしない
}

// 描画
void CCamera::Draw(void)
{
	// 何もしない
}

// 注視点設定
void CCamera::LookAt(VECTOR3* pAt)
{
	if (pAt) {
		m_at = *pAt;
	} else {
		m_at.x = m_at.y = m_at.z = 0.0f;
	}
}

// 上方ベクトル設定
void CCamera::SetUp(VECTOR3* pUp)
{
	if (pUp) {
		m_up = *pUp;
	} else {
		m_up.x = m_up.z = 0.0f;
		m_up.y =1.0f;
	}
}