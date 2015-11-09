//=======================================================================================
//
//	  �Q�[���p�E�B���h�E �N���X��`
//
//=======================================================================================
#pragma once

#include "Window.h"
#include "Graphics.h"
#include "Scene.h"

#define SCREEN_WIDTH	800			// �X�N���[���̕�
#define SCREEN_HEIGHT	600			// �X�N���[���̍���
#define FRAME_RATE		(1000/60)	// �t���[�����[�g

class CScene;
class CGameWindow : public CWindow
{
private:
	static CGameWindow*	m_pMainWnd;

	CGraphics*	m_pGraph;			// �`��f�o�C�X �N���X
	CScene*		m_pScene;			// �V�[��

	bool		m_bWindow;			// �E�B���h�E / �t���X�N���[��

	DWORD		m_dwExecLastTime;	// FPS�v���p
	DWORD		m_dwFPSLastTime;
	DWORD		m_dwFrameCount;

public:
	bool InitInstance();
	int ExitInstance();
	bool OnIdle(long lCount);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool OnEraseBkgnd(HDC hdc);
	bool OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg);
	void OnClose();
	bool OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	static CGameWindow* Get() {return m_pMainWnd;}
	void ChangeScene(int nIndex);
	CGraphics* GetGraph() {return m_pGraph;}
};

//=======================================================================================
//	End of File
//=======================================================================================