#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Player.h"
#include "Input.h"
#include "Sound.h"

// 定数
#define PATH_PLAYERMESH	"../data/model/gunmo.x"
#define SPEED_ROLL		2.0f				// Y軸回転速度
#define SPEED_FWD		5.0f				// 前進速度
#define SPEED_BACK		(SPEED_FWD*-0.5f)	// 後進速度
#define SPEED_JUMP		20.0f				// ジャンプ速度
#define GRAVITY			-0.98f				// 重力

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_PLAYER;
}

// デストラクタ
CPlayer::~CPlayer(void)
{
}

// 初期化
void CPlayer::Init(void)
{
	LoadMesh(&m_mesh, PATH_PLAYERMESH);
	SetMesh(&m_mesh);
}

// 解放
void CPlayer::Fin(void)
{
	SetMesh(NULL);
	ReleaseMesh(&m_mesh);
}

// 初期化後処理
void CPlayer::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	m_accel.y = GRAVITY;
	C3DObj::Update();

	Update();
}

// 更新
void CPlayer::Update(void)
{
	// ワールドマトリックスをコピー
	MATRIX mWorld = m_world;

	// 現在位置取得
	m_pos = mWorld.v[3].v3;
	mWorld._41 = mWorld._42 = mWorld._43 = 0.0f;

	// Y軸回転角取得
	m_angle.y = 0.0f;
	if (CInput::GetKeyPress(VK_LEFT)) {
		m_angle.y = -SPEED_ROLL;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_angle.y = SPEED_ROLL;
	}

	// ローカル座標軸取得
	VECTOR3 vX;
	VECTOR3 vY = mWorld.v[1].v3;
	VECTOR3 vZ;

	// Y軸回転
	MATRIX mRoll;
	MatrixRotationAxis(&mRoll, &vY, RAD * m_angle.y);
	MatrixMultiply(&mWorld, &mRoll, &mWorld);

	// ローカル座標軸再取得
	vX = mWorld.v[0].v3;
	vY = mWorld.v[1].v3;
	vZ = mWorld.v[2].v3;

	// 速度更新
	m_speed.x += m_accel.x;
	m_speed.y += m_accel.y;
	m_speed.z += m_accel.z;

	// ジャンプ
	if (m_pos.y <= 0.0f && CInput::GetKeyTrigger(VK_SPACE)) {
		CSound::Play(SE_JUMP);
		m_speed.y += SPEED_JUMP;
	}
	if (CInput::GetKeyTrigger('C')) {
		CSound::Play(SE_COIN);
	}

	// 前後進
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

	// 現在位置を更新
	mWorld.v[3].v3 = m_pos;

	// ワールドマトリックス更新
	m_world = mWorld;
}

// 描画
void CPlayer::Draw(void)
{
	if (!m_pMesh) {
		return;
	}
	// 後ろ向きモデルのため、Y軸180度回転
	MATRIX world;
	MatrixRotationY(&world, RAD * 180);
	MatrixMultiply(&world, &m_world, &world);
	DrawMeshNoAlpha(&world, m_pMesh);
}

// 描画後処理
void CPlayer::PostDraw(void)
{
	if (!m_pMesh) {
		return;
	}
	// 後ろ向きモデルのため、Y軸180度回転
	MATRIX world;
	MatrixRotationY(&world, RAD * 180);
	MatrixMultiply(&world, &m_world, &world);
	DrawMeshAlpha(&world, m_pMesh);
}