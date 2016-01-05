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

TCHAR		szDebug[1024];	// デバッグ用文字列

bool  g_bHit;

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : C3DObj(pScene)
{
	m_id = ID_PLAYER;
	m_size.x = 46.0f;
	m_size.y = 80.0f;
	m_pStage = NULL;
}

// デストラクタ
CPlayer::~CPlayer(void)
{
}

// 初期化
void CPlayer::Init(void)
{
	m_bJump = false;
}

// 解放
void CPlayer::Fin(void)
{
	delete m_pOBB;
}

// 初期化後処理
void CPlayer::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.z = 0.0f;
	m_pos.y = 0.0f;
	m_accel.y = GRAVITY;
	//m_size.x = 80.0f;
	//m_size.y = 160.0f;
	m_tex = LoadTexture(PATH_PLAYERTEX);
	m_pBillboard = CBillboard::Create(m_tex, m_size, m_pos);	
	m_pStage = (CStage*)C3DObj::Find(ID_STAGE);
	m_pOBB = new COBB();

	VECTOR3 vCenter;
	vCenter.x = 0.0f;
	vCenter.y = 0.0f;
	vCenter.z = 0.0f;
	VECTOR3 vRadius;
	vRadius.x = m_size.x * 0.25f;
	vRadius.y = m_size.y * 0.5f;
	vRadius.z = m_size.x * 0.25f;
	C3DObj::Update();
	m_pOBB->SetRadius(vRadius);
	m_pOBB->SetCenter(vCenter);

	Update();
}

// 更新
void CPlayer::Update(void)
{
	if(m_pStage == NULL){
		m_pStage = (CStage*)C3DObj::Find(ID_STAGE);	
	}
	MATRIX mWorld = m_pBillboard->GetWorld();
	
	m_world = mWorld;
	VECTOR3 vX;
	VECTOR3 vY;
	VECTOR3 vZ;
	vX.x = mWorld._11;
	vX.y = mWorld._12;
	vX.z = mWorld._13;
	vY.x = mWorld._21;
	vY.y = mWorld._22;
	vY.z = mWorld._23;
	vZ.x = mWorld._31;
	vZ.y = mWorld._32;
	vZ.z = mWorld._33;

	if (CInput::GetKeyPress(VK_LEFT)) {
		m_speed.x += vX.x;
		m_speed.z += vX.z;
	}
	if (CInput::GetKeyPress(VK_RIGHT)) {
		m_speed.x -= vX.x;
		m_speed.z -= vX.z;
	}
	if(CInput::GetKeyPress(VK_UP)){
		m_speed.y += 1.0f;
	}if(CInput::GetKeyPress(VK_DOWN)){
		m_speed.y -= 1.0f;
	}
	if(CInput::GetKeyPress(VK_SPACE)){
		m_bJump = true;
	}
	if(CInput::GetKeyRelease(VK_SPACE)){
		m_bJump = false;
	}

	m_pos.x += m_speed.x;
	m_pos.y += m_speed.y;
	m_pos.z += m_speed.z;

//	m_pBillboard->SetPos(m_pos);
	VECTOR3 onPos;
	VECTOR3 Dir;
	Vec3Set(&Dir, 0.0f, -1.0f, 0.0f);
	int Camera = CCamera::GetCameraType() % 2; 
	g_bHit = m_pStage->IntersectAABB(*m_pOBB,mWorld, Camera);

	if(g_bHit)
	{
		m_pBillboard->SetPos(m_pos);
		//g_bHit = true;
	}else{
		m_pBillboard->SetPos(m_pos);
		//g_bHit = false;
	}
}

// 描画
void CPlayer::Draw(void)
{
	if (!m_pBillboard) {
		return;
	}
	// 後ろ向きモデルのため、Y軸180度回転
	m_pBillboard->PostUpdate();
	m_pBillboard->Draw();
	TCHAR	str[256];
	_stprintf(str, _T("Player Pos( %2.2f, %2.2f, %2.2f)\n"),m_pos.x,m_pos.y,m_pos.z);
	m_pScene->AddDebugStr(str);
	_stprintf(str, _T("PlayerSize( %2.2f, %2.2f)\n"),m_size.x,m_size.y);
	_stprintf(str, _T("( %2.2f, %2.2f, %2.2f, %2.2f)\n( %2.2f, %2.2f, %2.2f, %2.2f)\n( %2.2f, %2.2f, %2.2f, %2.2f)\n( %2.2f, %2.2f, %2.2f, %2.2f)\n"),m_world._11,m_world._12,m_world._13,m_world._14,m_world._21,m_world._22,m_world._23,m_world._24,m_world._31,m_world._32,m_world._33,m_world._34,m_world._41,m_world._42,m_world._43,m_world._44);
	m_pScene->AddDebugStr(str);
	if(g_bHit){
	_stprintf(str, _T("Hit\n"));
	m_pScene->AddDebugStr(str);
	}
	
	m_pOBB->Draw(&m_world);


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