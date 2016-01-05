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
		{0.0f, 500.0f, -15.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 1:1人称
		{-100.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 2:固定
		{0.0f, 0.0f, -100.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 3:3人称
		{100.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
	{	// 4:プレイヤー注視
		{0.0f, 0.0f, 100.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
	},
};

int CCamera::m_camtype;

// 更新後処理
void CCamera::PostUpdate2(void)
{
	if (CInput::GetKeyPress(VK_NUMPAD1) || CInput::GetKeyPress('0')) {
		m_camtype = 0;
	}
	if (CInput::GetKeyPress(VK_NUMPAD1) || CInput::GetKeyPress('1')) {
		m_camtype = 1;
	} else if (CInput::GetKeyPress(VK_NUMPAD2) || CInput::GetKeyPress('2')) {
		m_camtype = 2;
	} else if (CInput::GetKeyPress(VK_NUMPAD3) || CInput::GetKeyPress('3')) {
		m_camtype = 3;

	} else if (CInput::GetKeyPress(VK_NUMPAD4) || CInput::GetKeyPress('4')) {
		m_camtype = 4;
	}
	if (CInput::GetKeyPress('A') && !m_bMove) {
		m_camtype--;
		if(m_camtype <= 0) m_camtype = 4;
		m_bMove = true;
	}
	if (CInput::GetKeyPress('D') && !m_bMove) {
		m_camtype++;
		if(m_camtype > 4) m_camtype = 1;
		m_bMove = true;
	}
	
	// 現在位置取得
	m_pos.x = m_world._41;
	m_pos.y = m_world._42;
	m_pos.z = m_world._43;

	// カメラ目的位置取得
	m_newPos = m_pCamInfo[m_camtype].m_eye;
	m_at  = m_pCamInfo[m_camtype].m_at;		// 基本変わらない
	m_up  = m_pCamInfo[m_camtype].m_up;
	
	VECTOR3 Dist;
	Dist.x = m_newPos.x - m_pos.x;
	Dist.y = m_newPos.y - m_pos.y;
	Dist.z = m_newPos.z - m_pos.z;
	m_pos.x += (m_newPos.x - m_pos.x) * 0.1f;
	m_pos.y += (m_newPos.y - m_pos.y) * 0.1f;
	m_pos.z += (m_newPos.z - m_pos.z) * 0.1f;
	if(Vec3Length(&Dist)< 5.0f)
	{
		m_pos = m_newPos;	
		m_bMove = false; 
	}
	
	m_world = MtxLookAt(m_at,m_pos, m_up);
//	MatrixRotationY(&m_world,1.0f);

	if (m_pSky) m_pSky->SetPos(&m_pos);
}

// 描画初期化
void CCamera::InitDraw(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
		(m_pos.x, m_pos.y, m_pos.z,
		m_at.x, m_at.y, m_at.z,
		m_up.x, m_up.y, m_up.z);

	if (m_pSky) m_pSky->RawDraw();	// スカイドーム描画
}

// コンストラクタ
CCamera::CCamera(CScene* pScene) : C3DObj(pScene)
{
	m_id = ID_CAMERA;

	m_newPos.x = m_pos.x = 0.0f, m_newPos.y = m_pos.y = 100.0f, m_newPos.z = m_pos.z = 15.0f;
	m_at.x = m_at.z = 0.0f;
	m_at.y = 100.0f;
	m_up.x = 0.0f, m_up.y = 1.0f, m_up.z = 0.0f;

	m_camtype = CT_FIXED;
	m_pCamInfo = new CCameraInfo[_countof(g_campos)];
	for (int i = 0; i < _countof(g_campos); i++) {
		m_pCamInfo[i].m_eye = g_campos[i].pos;
		m_pCamInfo[i].m_at = g_campos[i].at;
		m_pCamInfo[i].m_up = g_campos[i].up;
	}

	m_bMove = true;

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
	TCHAR	str[256];
	_stprintf(str, _T("C( %2.2f, %2.2f, %2.2f, %2.2f)\n( %2.2f, %2.2f, %2.2f, %2.2f)\n( %2.2f, %2.2f, %2.2f, %2.2f)\n( %2.2f, %2.2f, %2.2f, %2.2f)\n"),m_world._11,m_world._12,m_world._13,m_world._14,m_world._21,m_world._22,m_world._23,m_world._24,m_world._31,m_world._32,m_world._33,m_world._34,m_world._41,m_world._42,m_world._43,m_world._44);
	m_pScene->AddDebugStr(str);
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