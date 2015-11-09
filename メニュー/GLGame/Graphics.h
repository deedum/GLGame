//=======================================================================================
//
//	  OpenGL �N���X��`
//
//=======================================================================================
#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Matrix.h"

#define FOVY			45.0f								// ����p
#define NEAR_CLIP		0.3f								// �j�A�N���b�v���s������
#define FAR_CLIP		10000.0f							// �t�@�[�N���b�v���s������

class CGraphics
{
private:
	HDC			m_hDC;			// �f�o�C�X�R���e�L�X�g
	HGLRC		m_hRC;			// �J�����g�R���e�L�X�g
	bool		m_bWindow;		// �E�B���h�E���[�h
	GLuint		m_uFontTexID;	// �t�H���g�e�N�X�`�� ID
	float		m_fFontSize;	// �t�H���g�T�C�Y

	MATRIX		m_MatView;		// �r���[ �}�g���b�N�X
	MATRIX		m_MatProj;		// �ˉe�}�g���b�N�X

	HWND		m_hWnd;			// �E�B���h�E �n���h��
	int			m_nWidth;		// �\���̈敝
	int			m_nHeight;		// �\���̈捂��

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