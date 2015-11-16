#include "MainScene.h"


// コンストラクタ
CMainScene::CMainScene(void)
{
	m_pPlayer = NULL;
	m_pCamera = NULL;
	m_pSky = NULL;
	//m_pSound = NULL;
}

// デストラクタ
CMainScene::~CMainScene(void)
{
}

//---------------------------------------------------------------------------------------
// デバイス、リソース初期化
//---------------------------------------------------------------------------------------
bool CMainScene::Initialize()
{
	if (!CScene::Initialize()) return false;

	//-------- ゲーム用オブジェクトの初期化
	m_pPlayer = new CPlayer(this);
	m_pCamera = new CCamera(this);
	m_pSky = new CSky(this);
	//m_pSound = new CSound(this);
	InitObj();				// 全オブジェクト初期化

	//m_pSound->Play(BGM_MARIO);

	CBillboard::SetCamera(m_pCamera);
	return true;
}

//---------------------------------------------------------------------------------------
// デバイス、リソース解放
//---------------------------------------------------------------------------------------
void CMainScene::Finalize()
{
	//-------- ゲーム用オブジェクトの後始末
	FinObj();				// 全オブジェクト破棄
	//SAFE_DELETE(m_pSound);
	SAFE_DELETE(m_pSky);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pPlayer);

	CScene::Finalize();
}

//---------------------------------------------------------------------------------------
// ゲームメイン処理（メインループからコールされる）
//---------------------------------------------------------------------------------------
void CMainScene::Update()
{
	CScene::Update();

	//----- ここにゲーム本体処理
	UpdateObj();				// 全オブジェクト更新

	m_pCamera->PostUpdate2();	// カメラ更新
}

//---------------------------------------------------------------------------------------
// 描画処理（CGraphics::Render() からコールされる）
//---------------------------------------------------------------------------------------
void CMainScene::Draw()
{
	CScene::Draw();

	//----- ここに描画処理
	m_pCamera->InitDraw();		// カメラ設定
	DrawObj();					// 全オブジェクト描画

	// デバッグ文字列描画
	m_pGraph->DrawText(0, 0, m_szDebug);
}