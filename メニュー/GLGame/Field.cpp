#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include "Field.h"

// 定数
#define PATH_FIELDMESH	"../data/model/Stage/StageData1.x"

// コンストラクタ
CField::CField(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_STAGE;
}

// デストラクタ
CField::~CField(void)
{
}

// 初期化
void CField::Init(void)
{
	m_mesh = new MESH;
	LoadMesh(m_mesh, PATH_FIELDMESH);
	SetMesh(m_mesh);
}

// 解放
void CField::Fin(void)
{
	SetMesh(NULL);
	ReleaseMesh(m_mesh);
	delete m_mesh;
}

// 初期化後処理
void CField::PostInit(void)
{
	m_angle.y = 0.0f;
	m_pos.x = m_pos.y = m_pos.z = 0.0f;
	C3DObj::Update();

	Update();
}

// 更新
void CField::Update(void)
{

}

// 描画
void CField::Draw(void)
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
void CField::PostDraw(void)
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
