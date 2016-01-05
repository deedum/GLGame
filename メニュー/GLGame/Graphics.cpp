//=======================================================================================
//
//	  OpenGL クラス
//
//=======================================================================================
#include <windows.h>
#include <tchar.h>
#include "define.h"
#include "Graphics.h"
#include "Texture.h"

// 定数
#define RAD					(float)(3.1415926/180.0)				// ラジアン変換用定数

#define FONT_WIDTH			10								// フォント幅
#define FONT_HEIGHT			16								// フォント高さ

// 頂点フォーマット定義
struct FVERTEX {
	// GL_T2F_V3F
	GLfloat		tu, tv;			// テクスチャ座標
	GLfloat		x, y, z;		// 頂点座標
};

// OpenGL 拡張関数
#ifndef WGL_EXT_swap_control
#define WGL_EXT_swap_control 1
#ifdef WGL_WGLEXT_PROTOTYPES
extern BOOL WINAPI wglSwapIntervalEXT (int interval);
extern int WINAPI wglGetSwapIntervalEXT (void);
#endif /* WGL_WGLEXT_PROTOTYPES */
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);
#endif
static BOOL WINAPI _fnwglSwapIntervalEXT(int interval)
{
	return FALSE;
}
PFNWGLSWAPINTERVALEXTPROC		wglSwapIntervalEXT = _fnwglSwapIntervalEXT;
static int WINAPI _fnwglGetSwapIntervalEXT(void)
{
	return 1;
}
PFNWGLGETSWAPINTERVALEXTPROC	wglGetSwapIntervalEXT = _fnwglGetSwapIntervalEXT;

//---------------------------------------------------------------------------------------
// グラフィック環境の初期化
//---------------------------------------------------------------------------------------
bool CGraphics::Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	// 引数保存
	m_hWnd = hWnd;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_bWindow = bWindow;

	// ピクセル フォーマット
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,						// Version
		PFD_DRAW_TO_WINDOW |	// Supports GDI
		PFD_SUPPORT_OPENGL |	// Supports OpenGL
		PFD_DOUBLEBUFFER,		// Use double buffering (more efficient drawing)
		PFD_TYPE_RGBA,			// No pallettes
		24, 					// Number of color planes
		 						// in each color buffer
		8,	0,					// for Red-component
		8,	0,					// for Green-component
		8,	0,					// for Blue-component
		8,	0,					// for Alpha-component
		0,						// Number of planes
								// of Accumulation buffer
		16,						// for Red-component
		16,						// for Green-component
		16,						// for Blue-component
		16,						// for Alpha-component
		24, 					// Depth of Z-buffer
		0,						// Depth of Stencil-buffer
		0,						// Depth of Auxiliary-buffer
		0,						// Now is ignored
		0,						// Number of planes
		0,						// Now is ignored
		0,						// Color of transparent mask
		0						// Now is ignored
	};

	// ウィンドウ／フル スクリーン切り替え
	if (!bWindow) {
		DEVMODE	devmode;
		devmode.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		devmode.dmBitsPerPel	= 32;
		devmode.dmPelsWidth		= nWidth;
		devmode.dmPelsHeight	= nHeight;
		devmode.dmSize			= sizeof devmode;
		if (ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			MessageBox(hWnd, _T("フルスクリーン切り替えに失敗しました"),
				_T("error"), MB_OK | MB_ICONERROR);
			return false;
		}
		SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0,
			nWidth, nHeight, SWP_SHOWWINDOW);
	}

	// ウィンドウ コンテキストの取得
	m_hDC = GetDC(hWnd);

	// 合致する最も近いピクセル フォーマットの選択
	int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (!nPixelFormat) {
		MessageBox(hWnd, _T("ピクセルフォーマットの選択に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// ピクセル フォーマットの設定
	if (!SetPixelFormat(m_hDC, nPixelFormat, &pfd)) {
		MessageBox(hWnd, _T("ピクセルフォーマットの設定に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// OpenGL レンダリング コンテキストを作成
	m_hRC = wglCreateContext(m_hDC);
	if (!m_hRC) {
		MessageBox(hWnd, _T("レンダリングコンテキストの生成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// OpenGL レンダリング コンテキストをメイク
	if (!wglMakeCurrent(m_hDC, m_hRC)) {
		MessageBox(hWnd, _T("レンダリングコンテキストのメイクに失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// OpenGL 拡張関数の取得
	char *ext = (char *)glGetString(GL_EXTENSIONS);
	if (strstr(ext, "WGL_EXT_swap_control")) {
		wglSwapIntervalEXT    = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		if (!wglSwapIntervalEXT)
			wglSwapIntervalEXT = _fnwglSwapIntervalEXT;
		if (!wglGetSwapIntervalEXT)
			wglGetSwapIntervalEXT = _fnwglGetSwapIntervalEXT;
	}
	wglSwapIntervalEXT((bWindow) ? 0 : 1);	// ウィンドウ時、垂直同期無し

	// フォント セットアップ
	m_uFontTexID = LoadTexture("../data/texture/font.tga");
	m_fFontSize = float(FONT_HEIGHT);

	return true;
}

//---------------------------------------------------------------------------------------
// グラフィック環境のクリーンアップ
//---------------------------------------------------------------------------------------
void CGraphics::Finalize()
{
	ReleaseTexture(m_uFontTexID);		// フォント用テクスチャ解放
	m_uFontTexID = 0;

	wglMakeCurrent(NULL, NULL);
	if (m_hRC) {						// カレントコンテキストを解放
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}
	if (m_hDC) {						// デバイスコンテキストを解放
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}
	if (!m_bWindow)
		ChangeDisplaySettings(NULL, 0);
}

//---------------------------------------------------------------------------------------
// 描画クラス生成
//---------------------------------------------------------------------------------------
CGraphics* CGraphics::Create(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	CGraphics* pGraph = new CGraphics();
	if (pGraph) {
		if (!pGraph->Initialize(hWnd, nWidth, nHeight, bWindow)) {
			SAFE_DELETE(pGraph);
		}
	}
	return pGraph;
}

//---------------------------------------------------------------------------------------
// 描画クラス解放
//---------------------------------------------------------------------------------------
void CGraphics::Release()
{
	Finalize();
	delete this;
}

//---------------------------------------------------------------------------------------
// レンダリング開始
//---------------------------------------------------------------------------------------
bool CGraphics::Begin()
{
	// ビューポートをクリア
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Init3D();
	return true;
}

//---------------------------------------------------------------------------------------
// レンダリング終了
//---------------------------------------------------------------------------------------
void CGraphics::End()
{
	// 何もしない
}

//---------------------------------------------------------------------------------------
// バックバッファ入替
//---------------------------------------------------------------------------------------
void CGraphics::SwapBuffer()
{
	// バックバッファに描画した内容を表示
	SwapBuffers(m_hDC);
}

//---------------------------------------------------------------------------------------
// デバッグ文字列描画
//---------------------------------------------------------------------------------------
void CGraphics::DrawText(int nX, int nY, LPCTSTR psz)
{
	Init2D();
	// デバッグ ストリング描画
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	SetFontSize();
	DrawStr(0, 0, psz);
}

//---------------------------------------------------------------------------------------
// 2D 用レンダリング ステート初期化
//---------------------------------------------------------------------------------------
void CGraphics::Init2D()
{
	// ライト無効化
	glDisable(GL_LIGHTING);

	// テクスチャを有効にする
	glEnable(GL_TEXTURE_2D);

	// ポリゴンの片面だけ描画を無効化
	glDisable(GL_CULL_FACE);

	// 深度バッファを無効化
	glDisable(GL_DEPTH_TEST);

	// アルファ ブレンディング有効化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);

	// プロジェク ションマトリックスとして正射影を設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, m_nWidth, m_nHeight, 0.0, -1.0, 1.0);

	// ビュー マトリックス設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//---------------------------------------------------------------------------------------
// 3D 用レンダリング ステート初期化
//---------------------------------------------------------------------------------------
void CGraphics::Init3D()
{
	// テクスチャを無効にする
	glDisable(GL_TEXTURE_2D);

	// ポリゴンの片面だけ描画を有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// 深度バッファを有効化
	glEnable(GL_DEPTH_TEST);

	// アルファ ブレンディング無効化
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	// プロジェクション マトリックス設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((double)m_nWidth * -0.5,(double)m_nWidth * 0.5,(double)m_nHeight * -0.5,(double)m_nHeight * 0.5,NEAR_CLIP,FAR_CLIP);
//	gluPerspective(FOVY, (double)m_nWidth / m_nHeight, NEAR_CLIP, FAR_CLIP);
	glGetFloatv(GL_PROJECTION_MATRIX, m_MatProj.f);

	// ビュー マトリックス設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,											// 視点位置(X)
			  0.0f,											// 視点位置(Y)
			  m_nHeight * 0.5f / tanf(RAD * FOVY * 0.5f),	// 視点位置(Z)
			  0.0f, 0.0f, 0.0f,		// 注視点位置
			  0.0f, 1.0f, 0.0f);	// アップベクトル
	glGetFloatv(GL_MODELVIEW_MATRIX, m_MatView.f);

	// ライト有効化
	glEnable(GL_LIGHTING);
	VECTOR4 vLightPos = {1.0f, 1.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, vLightPos.f);
	float fDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffuse);
	float fAmbient[4] = {0.01f, 0.01f, 0.01f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbient);
	float fSpecular[4] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, fSpecular);
	glEnable(GL_LIGHT0);
}

//---------------------------------------------------------------------------------------
// 1 文字描画
//---------------------------------------------------------------------------------------
void CGraphics::DrawChar(int x, int y, int c)
{
	DrawChar(float(x), float(y), c);
}

void CGraphics::DrawChar(float x, float y, int c)
{
	FVERTEX	vtxQuad[4];

	vtxQuad[0].tu = (float)(c & 15) / FONT_HEIGHT;
	vtxQuad[0].tv = (float)(15 - (c >> 4)) / FONT_HEIGHT;
	vtxQuad[0].x  = x;
	vtxQuad[0].y  = y + m_fFontSize;
	vtxQuad[0].z  = 0.0f;
	vtxQuad[1].tu = (float)((c & 15) + 1.0f) / FONT_HEIGHT;
	vtxQuad[1].tv = (float)(15 - (c >> 4)) / FONT_HEIGHT;
	vtxQuad[1].x  = x + m_fFontSize;
	vtxQuad[1].y  = y + m_fFontSize;
	vtxQuad[1].z  = 0.0f;
	vtxQuad[2].tu = (float)((c & 15) + 1.0f) / FONT_HEIGHT;
	vtxQuad[2].tv = (float)(15 - (c >> 4) + 1.0f) / FONT_HEIGHT;
	vtxQuad[2].x  = x + m_fFontSize;
	vtxQuad[2].y  = y;
	vtxQuad[2].z  = 0.0f;
	vtxQuad[3].tu = (float)(c & 15) / FONT_HEIGHT;
	vtxQuad[3].tv = (float)(15 - (c >> 4) + 1.0f) / FONT_HEIGHT;
	vtxQuad[3].x  = x;
	vtxQuad[3].y  = y;
	vtxQuad[3].z  = 0.0f;

	glEnable(GL_TEXTURE_2D);	// テクスチャ有効化
	glBindTexture(GL_TEXTURE_2D, m_uFontTexID);
	glInterleavedArrays(GL_T2F_V3F, 0, vtxQuad);
	glDrawArrays(GL_QUADS, 0, 4);
}

//---------------------------------------------------------------------------------------
// 文字列描画
//---------------------------------------------------------------------------------------
void CGraphics::DrawStr(int x, int y, LPCTSTR psz)
{
	DrawStr(float(x), float(y), psz);
}

void CGraphics::DrawStr(float x, float y, LPCTSTR psz)
{
	for (; *psz; ++psz) {
		switch (*psz) {
		case _T('\n'):
			x = 0.0f;
			y += m_fFontSize;
			break;
		default:
			DrawChar(x, y, *psz);
			x += m_fFontSize * FONT_WIDTH / FONT_HEIGHT;
			break;
		}
	}
}

float CGraphics::GetCharWidth()
{
	return m_fFontSize * FONT_WIDTH / FONT_HEIGHT;
}

float CGraphics::GetCharHeight()
{
	return m_fFontSize;
}

//=======================================================================================
//	End of File
//=======================================================================================