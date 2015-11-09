#include "Sky.h"

#define PATH_MESHSKY	"../data/model/sky.x"

// �R���X�g���N�^
CSky::CSky(CScene* pScene) : CMeshObj(pScene)
{
	m_id = ID_SKY;
}

// �f�X�g���N�^
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
	glDisable(GL_DEPTH_TEST);	// Z�o�b�t�@�Q��&�X�V����
	glDisable(GL_LIGHTING);		// ��������
	DrawMesh(&m_world, m_pMesh);
	glEnable(GL_LIGHTING);		// �����L��
	glEnable(GL_DEPTH_TEST);		// Z�o�b�t�@�Q��&�X�V�L��
}