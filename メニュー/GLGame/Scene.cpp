//=======================================================================================
//
//	  �V�[�� �N���X��`
//
//=======================================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "Scene.h"
#include "define.h"
#include "C3DObj.h"
#include "SplashScene.h"
#include "TitleScene.h"
#include "MainScene.h"

//---------------------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------------------
CScene::CScene()
{
	m_pTop = NULL;
	m_FPS = 0;
}

//---------------------------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------------------------
CScene::~CScene()
{
}

// �S�I�u�W�F�N�g������
void CScene::InitObj(void)
{
	// �������O����
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		pObj->PreInit();
	}
	// ������
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		pObj->Init();
	}
	// �������㏈��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		pObj->PostInit();
	}
}

// �S�I�u�W�F�N�g�X�V
void CScene::UpdateObj(void)
{
	C3DObj* pNext;
	// �X�V�O����
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->PreUpdate();
	}
	// �X�V
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->Update();
	}
	// �X�V�㏈��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->PostUpdate();
	}
}

// �S�I�u�W�F�N�g�`��
void CScene::DrawObj(void)
{
	// �`��O����
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		if (pObj->IsVisible())
			pObj->PreDraw();
	}
	// �`��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		if (pObj->IsVisible())
			pObj->Draw();
	}
	// �`��㏈��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		if (pObj->IsVisible())
			pObj->PostDraw();
	}
}

// �S�I�u�W�F�N�g�j��
void CScene::FinObj(void)
{
	C3DObj* pNext;
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->Fin();
	}
}

//---------------------------------------------------------------------------------------
// �V�[������
//---------------------------------------------------------------------------------------
CScene* CScene::Create(int nIndex)
{
	CScene* pScene = NULL;
	switch (nIndex) {
	case SCENE_SPLASH:
		pScene = new CSplashScene();
		break;
	case SCENE_TITLE:
		pScene = new CTitleScene();
		break;
	case SCENE_MAIN:
		pScene = new CMainScene();
		break;
	default:
		break;
	}
	if (pScene) {
		if (!pScene->Initialize()) {
			SAFE_DELETE(pScene);
		}
	}
	return pScene;
}

//---------------------------------------------------------------------------------------
// �f�o�C�X�A���\�[�X������
//---------------------------------------------------------------------------------------
bool CScene::Initialize()
{
	m_pGraph = CGameWindow::Get()->GetGraph();
	return true;
}

//---------------------------------------------------------------------------------------
// �f�o�C�X�A���\�[�X���
//---------------------------------------------------------------------------------------
void CScene::Finalize()
{
	m_pGraph = NULL;
}

//---------------------------------------------------------------------------------------
// �V�[�����
//---------------------------------------------------------------------------------------
void CScene::Release()
{
	Finalize();
	delete this;
}

//---------------------------------------------------------------------------------------
// �Q�[���`�揈���i���C�����[�v����R�[�������j
//---------------------------------------------------------------------------------------
void CScene::Render()
{
	if (m_pGraph->Begin()) {	// �`��J�n

		Draw();					// �`�揈��

		m_pGraph->End();		// �`��I��
	}
	m_pGraph->SwapBuffer();		// �o�b�N�o�b�t�@����
}

//---------------------------------------------------------------------------------------
// �Q�[�����C�������i���C�����[�v����R�[�������j
//---------------------------------------------------------------------------------------
void CScene::Update()
{
	//----- �����ɃQ�[���{�̏���
	CInput::Update();
}

//---------------------------------------------------------------------------------------
// �`�揈���iCGraphics::Render() ����R�[�������j
//---------------------------------------------------------------------------------------
void CScene::Draw()
{
	m_szDebug[0] = _T('\0');	// �f�o�b�O�����񏉊���

	// FPS ����ʂɕ`�悷�邽�߂̕�������쐬
	TCHAR	str[256];
	_stprintf(str, _T("FPS = %d\n"), m_FPS);
	lstrcat(m_szDebug, str);
}

//=======================================================================================
//	End of File
//=======================================================================================