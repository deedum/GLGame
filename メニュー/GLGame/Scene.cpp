//=======================================================================================
//
//	  シーン クラス定義
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
// コンストラクタ
//---------------------------------------------------------------------------------------
CScene::CScene()
{
	m_pTop = NULL;
	m_FPS = 0;
}

//---------------------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------------------
CScene::~CScene()
{
}

// 全オブジェクト初期化
void CScene::InitObj(void)
{
	// 初期化前処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		pObj->PreInit();
	}
	// 初期化
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		pObj->Init();
	}
	// 初期化後処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		pObj->PostInit();
	}
}

// 全オブジェクト更新
void CScene::UpdateObj(void)
{
	C3DObj* pNext;
	// 更新前処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->PreUpdate();
	}
	// 更新
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->Update();
	}
	// 更新後処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->PostUpdate();
	}
}

// 全オブジェクト描画
void CScene::DrawObj(void)
{
	// 描画前処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		if (pObj->IsVisible())
			pObj->PreDraw();
	}
	// 描画
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		if (pObj->IsVisible())
			pObj->Draw();
	}
	// 描画後処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->GetNext()) {
		if (pObj->IsVisible())
			pObj->PostDraw();
	}
}

// 全オブジェクト破棄
void CScene::FinObj(void)
{
	C3DObj* pNext;
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		pObj->Fin();
	}
}

//---------------------------------------------------------------------------------------
// シーン生成
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
// デバイス、リソース初期化
//---------------------------------------------------------------------------------------
bool CScene::Initialize()
{
	m_pGraph = CGameWindow::Get()->GetGraph();
	return true;
}

//---------------------------------------------------------------------------------------
// デバイス、リソース解放
//---------------------------------------------------------------------------------------
void CScene::Finalize()
{
	m_pGraph = NULL;
}

//---------------------------------------------------------------------------------------
// シーン解放
//---------------------------------------------------------------------------------------
void CScene::Release()
{
	Finalize();
	delete this;
}

//---------------------------------------------------------------------------------------
// ゲーム描画処理（メインループからコールされる）
//---------------------------------------------------------------------------------------
void CScene::Render()
{
	if (m_pGraph->Begin()) {	// 描画開始

		Draw();					// 描画処理

		m_pGraph->End();		// 描画終了
	}
	m_pGraph->SwapBuffer();		// バックバッファ入替
}

//---------------------------------------------------------------------------------------
// ゲームメイン処理（メインループからコールされる）
//---------------------------------------------------------------------------------------
void CScene::Update()
{
	//----- ここにゲーム本体処理
	CInput::Update();
}

//---------------------------------------------------------------------------------------
// 描画処理（CGraphics::Render() からコールされる）
//---------------------------------------------------------------------------------------
void CScene::Draw()
{
	m_szDebug[0] = _T('\0');	// デバッグ文字列初期化

	// FPS を画面に描画するための文字列を作成
	TCHAR	str[256];
	_stprintf(str, _T("FPS = %d\n"), m_FPS);
	lstrcat(m_szDebug, str);
}

//=======================================================================================
//	End of File
//=======================================================================================