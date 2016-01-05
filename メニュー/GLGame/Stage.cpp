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
	m_id = ID_STAGE;
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

bool CStage::Intersect(VECTOR3* pCross, VECTOR3* pPos, VECTOR3* pDir)
{
	VECTOR3 P[3],N;
	VECTOR3 W = *pPos;
	VECTOR3 H = *pDir;
	H.x = H.x + W.x;
	H.y = H.y + W.y;
	H.z = H.z + W.z;

	VECTOR3 work;

	for(int i = 0; i < m_mesh.inum;){
		// 三角形の頂点を抽出
		P[0] = m_mesh.vertex[m_mesh.index[i++]];
		P[1] = m_mesh.vertex[m_mesh.index[i++]];
		P[2] = m_mesh.vertex[m_mesh.index[i++]];
		int j;
		for(j = 3; j >= 0; --j){
			work.x = P[(j + 1) % 3].x + P[j].x;
			work.y = P[(j + 1) % 3].y + P[j].y;
			work.z = P[(j + 1) % 3].z + P[j].z;
			// 三角形を囲む平面の法線ベクトルを求める。
			Vec3Cross(&N,&H,&work);
			
			if(Vec3Dot(&N, &W) * Vec3Dot(&N, &H) > 0)
			{
				break;
			}
		}
		if(j < 3) continue;

		// 三角形の法線ベクトルを求める
		VECTOR3 P1,P2;
		P1.x = P[1].x - P[0].x;
		P1.y = P[1].y - P[0].y;
		P1.z = P[1].z - P[0].z;
		P2.x = P[2].x - P[1].x;
		P2.y = P[2].y - P[1].y;
		P2.z = P[2].z - P[1].z;
		Vec3Cross(&N, &P2, &P1);
		Vec3Normalize(&N,&N);

		// 媒介変数tの分母を求める
		float base = Vec3Dot(&N, &H);
		if(base == 0.0f){
			continue;		// 平面と平行　(あたっていない)
		}

		// tを求める
		work.x = P[0].x - W.x;
		work.y = P[0].y - W.y;
		work.z = P[0].z - W.z;
		float t = Vec3Dot(&N, &work) / base;

		// 交点を求める
		VECTOR3 X;
		X.x = W.x + t * H.x;
		X.y = W.y + t * H.y;
		X.z = W.z + t * H.z;
		if(pCross)
			*pCross = X;

		
		return true;
	}
	return false;
}
//bool CStage::Intersect(VECTOR3* pCross, VECTOR3* pPos, VECTOR3* pDir)
//{
//	VECTOR3 P[3],N;
//	VECTOR3 W = *pPos;
//	VECTOR3 H = *pDir;
//	VECTOR3 work;
//
//	for(int i = 0; i < m_mesh.inum;){
//		// 三角形の頂点を抽出
//		P[0] = m_mesh.vertex[m_mesh.index[i++]];
//		P[1] = m_mesh.vertex[m_mesh.index[i++]];
//		P[2] = m_mesh.vertex[m_mesh.index[i++]];
//		int j;
//		for(j = 0; j < 3; j++){
//			// 三角形を囲む平面の法線ベクトルを求める。
//			work.x = P[(j + 1) % 3].x - P[j].x;
//			work.y = P[(j + 1) % 3].y - P[j].y;
//			work.z = P[(j + 1) % 3].z - P[j].z;
//			Vec3Cross(&N,&H,&work);
//			// 始点が平面の表ならあたっていない
//			work.x = W.x - P[j].x;
//			work.y = W.y - P[j].y;
//			work.z = W.z - P[j].z;
//			if(Vec3Dot(&N, &work) > 0.0f){
//				break;
//			}
//		}
//		if(j < 3) continue;
//
//		// 三角形の法線ベクトルを求める
//		VECTOR3 P1,P2;
//		P1.x = P[1].x - P[0].x;
//		P1.y = P[1].y - P[0].y;
//		P1.z = P[1].z - P[0].z;
//		P2.x = P[2].x - P[1].x;
//		P2.y = P[2].y - P[1].y;
//		P2.z = P[2].z - P[1].z;
//		Vec3Cross(&N, &P1, &P2);
//		Vec3Normalize(&N,&N);
//
//		// 媒介変数tの分母を求める
//		float base = Vec3Dot(&N, &H);
//		if(base == 0.0f){
//			continue;		// 平面と平行　(あたっていない)
//		}
//
//		// tを求める
//		work.x = P[0].x - W.x;
//		work.y = P[0].y - W.y;
//		work.z = P[0].z - W.z;
//		float t = Vec3Dot(&N, &work) / base;
//
//		// 交点を求める
//		VECTOR3 X;
//		X.x = W.x + t * H.x;
//		X.y = W.y + t * H.y;
//		X.z = W.z + t * H.z;
//		if(pCross) *pCross = X;
//		return true;
//	}
//	return false;
//}