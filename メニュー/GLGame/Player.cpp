#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Player.h"
#include "Input.h"
#include "Sound.h"
#include "Texture.h"

// 定数
#define PATH_PLAYERTEX	"../data/texture/whiteman.tga"
#define SPEED_ROLL		2.0f				// Y軸回転速度
#define SPEED_FWD		5.0f				// 前進速度
#define SPEED_BACK		(SPEED_FWD*-0.5f)	// 後進速度
#define SPEED_JUMP		20.0f				// ジャンプ速度
#define GRAVITY			-0.98f				// 重力

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : C3DObj(pScene)
{
	m_id = ID_PLAYER;
	m_size.x = 46.0f;
	m_size.y = 80.0f;
}

// デストラクタ
CPlayer::~CPlayer(void)
{
}

// 初期化
void CPlayer::Init(void)
{
}

// 解放
void CPlayer::Fin(void)
{
}

// 初期化後処理
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

// 更新
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

// 描画
void CPlayer::Draw(void)
{
	if (!m_billboard) {
		return;
	}
	// 後ろ向きモデルのため、Y軸180度回転
	m_billboard->PostUpdate();
	m_billboard->Draw();
}

// 描画後処理
void CPlayer::PostDraw(void)
{
	//if (!m_pMesh) {
	//	return;
	//}
	//// 後ろ向きモデルのため、Y軸180度回転
	//MATRIX world;
	//MatrixRotationY(&world, RAD * 180);
	//MatrixMultiply(&world, &m_world, &world);
	//DrawMeshAlpha(&world, m_pMesh);
}