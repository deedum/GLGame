#include "Sky.h"

#define PATH_MESHSKY	"../data/model/sky.x"

// コンストラクタ
CSky::CSky(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_SKY;
}

// デストラクタ
CSky::~CSky(void)
{
}

void CSky::Init(void)
{
	CMeshObj::Init();
	if (LoadMesh(&m_mesh, PATH_MESHSKY)) {
		SetMesh(&m_mesh);
	} else {
		ZeroMemory(&m_mesh, sizeof(m_mesh));
	}
}

void CSky::Fin(void)
{
	if (m_pMesh) {
		m_pMesh = NULL;
		ReleaseMesh(&m_mesh);
	}
	CMeshObj::Fin();
}

void CSky::RawDraw(void)
{
	if (!m_pMesh) {
		return;
	}
	glDisable(GL_DEPTH_TEST);	// Zバッファ参照&更新無効
	glDisable(GL_LIGHTING);		// 光源無効
	DrawMesh(&m_world, m_pMesh);
	glEnable(GL_LIGHTING);		// 光源有効
	glEnable(GL_DEPTH_TEST);		// Zバッファ参照&更新有効
}