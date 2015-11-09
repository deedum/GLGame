//=======================================================================================
//
//	  OpenGL クラス定義
//
//=======================================================================================
#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Matrix.h"

#define FOVY			45.0f								// 視野角
#define NEAR_CLIP		0.3f								// ニアクリップを行う距離
#define FAR_CLIP		10000.0f							// ファークリップを行う距離

class CGraphics
{
private:
	HDC			m_hDC;			// デバイスコンテキスト
	HGLRC		m_hRC;			// カレントコンテキスト
	bool		m_bWindow;		// ウィンドウモード
	GLuint		m_uFontTexID;	// フォントテクスチャ ID
	float		m_fFontSize;	// フォントサイズ

	MATRIX		m_MatView;		// ビュー マトリックス
	MATRIX		m_MatProj;		// 射影マトリックス

	HWND		m_hWnd;			// ウィンドウ ハンドル
	int			m_nWidth;		// 表示領域幅
	int			m_nHeight;		// 表示領域高さ

public:
	static CGraphics* Create(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Release();
	bool Begin();
	void End();
	void SwapBuffer();
	void DrawText(int nX, int nY, LPCTSTR psz);
	void DrawText(float x, float y, LPCTSTR psz);
	void SetFontSize(float f = 16.0f) {m_fFontSize = f;}
	void DrawChar(int x, int y, int c);
	void DrawChar(float x, float y, int c);
	void DrawStr(float x, float y, LPCTSTR psz);
	void DrawStr(int x, int y, LPCTSTR psz);
	float GetCharWidth();
	float GetCharHeight();

private:
	bool Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Finalize();
	void Init2D();
	void Init3D();
};

//=======================================================================================
//	End of File
//=======================================================================================