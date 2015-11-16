#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Stage.h"
#include "Input.h"
#include "Sound.h"

// 定数
#define PATH_PLAYERMESH	"../data/model/Stage/StageData.x"
#define SPEED_ROLL		2.0f				// Y軸回転速度
#define SPEED_FWD		5.0f				// 前進速度
#define SPEED_BACK		(SPEED_FWD*-0.5f)	// 後進速度
#define SPEED_JUMP		20.0f				// ジャンプ速度
#define GRAVITY			-0.98f				// 重力

// コンストラクタ
CStage::CStage(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_PLAYER;
}

// デストラクタ
CStage::~CStage(void)
{
}

// 初期化
void CStage::Init(void)
{
	LoadMesh(&m_mesh, PATH_PLAYERMESH);
	SetMesh(&m_mesh);
}

// 解放
void CStage::Fin(void)
{
	SetMesh(NULL);
	ReleaseMesh(&m_mesh);
}

// 初期化後処理
void CStage::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	m_accel.y = GRAVITY;
	C3DObj::Update();

	Update();
}

// 更新
void CStage::Update(void)
{

}

// 描画
void CStage::Draw(void)
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
void CStage::PostDraw(void)
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