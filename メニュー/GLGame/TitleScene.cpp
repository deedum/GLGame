#include "TitleScene.h"

// コンストラクタ
CTitleScene::CTitleScene(void)
{
	m_pCamera = NULL;
	m_pSky = NULL;
	m_nSelect = 0;
	m_nCount = 0;
	m_nBlink = 0;
	m_nTick = 0;
}

// デストラクタ
CTitleScene::~CTitleScene(void)
{
}

//---------------------------------------------------------------------------------------
// デバイス、リソース初期化
//---------------------------------------------------------------------------------------
bool CTitleScene::Initialize()
{
	if (!CScene::Initialize()) return false;

	//-------- ゲーム用オブジェクトの初期化
	m_pCamera = new CCamera(this);
	m_pSky = new CSky(this);
	InitObj();		// 全オブジェクト初期化
	m_campos = m_pCamera->GetPos();
	m_campos.y = 0.0f;
	m_pCamera->SetType(CT_NONE);

	//CSound::Play(BGM_MARIO);

	return true;
}

//---------------------------------------------------------------------------------------
// デバイス、リソース解放
//---------------------------------------------------------------------------------------
void CTitleScene::Finalize()
{
	//-------- ゲーム用オブジェクトの後始末
	FinObj();		// 全オブジェクト破棄
	SAFE_DELETE(m_pSky);
	SAFE_DELETE(m_pCamera);
	
	CScene::Finalize();
}

//---------------------------------------------------------------------------------------
// ゲームメイン処理（メインループからコールされる）
//---------------------------------------------------------------------------------------
void CTitleScene::Update()
{
	CScene::Update();

	//----- ここにゲーム本体処理
	UpdateObj();				// 全オブジェクト更新

	m_nTick++;
	if (m_nBlink > 0) {
		--m_nBlink;
		if (m_nBlink == 0) {
			switch (m_nSelect) {
			case 0:
				CGameWindow::Get()->ChangeScene(SCENE_MAIN);
				break;
			case 1:
				CGameWindow::Get()->ChangeScene(SCENE_TESTMODE);
				break;
			case 2:
				CGameWindow::Get()->OnClose();
				break;
			}
			return;
		}
	} else {
		int nPress = CInput::GetKeyPress(VK_UP) ? 1 : 0;
		nPress |= CInput::GetKeyPress(VK_DOWN) ? 2 : 0;

		if (nPress & 1) {
			if (m_nCount <= 0) {
				if (m_nSelect > 0)
					--m_nSelect;
				else
					m_nSelect = 2;
				if (CInput::GetKeyTrigger(VK_UP))
					m_nCount = 40;
				else
					m_nCount = 15;
			}
		}
		if (nPress & 2) {
			if (m_nCount <= 0) {
				if (m_nSelect < 2)
					++m_nSelect;
				else
					m_nSelect = 0;
				if (CInput::GetKeyTrigger(VK_DOWN))
					m_nCount = 40;
				else
					m_nCount = 15;
			}
		}
		if (nPress == 0) {
			m_nCount = 0;
		}
		if (m_nCount > 0) {
			--m_nCount;
		}
	}

	if (m_nBlink <= 0 && CInput::GetKeyPress(VK_SPACE)) {
		m_nBlink = 60;
	}

	VECTOR3 pos;
	MATRIX mtx;
	MatrixRotationY(&mtx, RAD * m_nTick * -0.1f);
	Vec3TransformCoord(&pos, &mtx, &m_campos);
	m_pCamera->SetPos(&pos);
	m_pCamera->PostUpdate2();	// カメラ更新
}

//---------------------------------------------------------------------------------------
// 描画処理（CGraphics::Render() からコールされる）
//---------------------------------------------------------------------------------------
void CTitleScene::Draw()
{
	CScene::Draw();

	//----- ここに描画処理
	m_pCamera->InitDraw();	// カメラ設定
	DrawObj();				// 全オブジェクト描画

	// デバッグ文字列描画
	m_pGraph->DrawText(0, 0, m_szDebug);

	// メニュー描画
	m_pGraph->SetFontSize(50.0f);
	LPCTSTR pszMenu[] = {
		_T("START"),
		_T("TESTMODE"),
		_T("END"),
	};
	float fCharWidth = m_pGraph->GetCharWidth();
	float fLeft = (SCREEN_WIDTH - fCharWidth * 11) / 2.0f;
	float fTop = SCREEN_HEIGHT / 2.0f + m_pGraph->GetCharHeight();
	for (int i = 0; i < _countof(pszMenu); ++i) {
		if (m_nBlink == 0 && m_nSelect == i) {
			glColor3f(1.0f, 1.0f, 1.0f);
			m_pGraph->DrawStr(fLeft + sinf(RAD * (m_nTick * 12)) * fCharWidth * 0.5f,
				fTop + i * 64.0f, _T("\x60"));
		}
		if (m_nSelect == i) {
			glColor3f(1.0f, 1.0f, 1.0f);
			if (m_nBlink && (m_nBlink & 4) == 0) {
				continue;
			}
		} else {
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		m_pGraph->DrawStr(fLeft + fCharWidth * 2.0f,
			fTop + i * 64.0f, pszMenu[i]);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
}