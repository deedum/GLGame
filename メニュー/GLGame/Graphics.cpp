//=======================================================================================
//
//	  OpenGL �N���X
//
//=======================================================================================
#include <windows.h>
#include <tchar.h>
#include "define.h"
#include "Graphics.h"
#include "Texture.h"

// �萔
#define RAD					(float)(3.1415926/180.0)				// ���W�A���ϊ��p�萔

#define FONT_WIDTH			10								// �t�H���g��
#define FONT_HEIGHT			16								// �t�H���g����

// ���_�t�H�[�}�b�g��`
struct FVERTEX {
	// GL_T2F_V3F
	GLfloat		tu, tv;			// �e�N�X�`�����W
	GLfloat		x, y, z;		// ���_���W
};

// OpenGL �g���֐�
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
// �O���t�B�b�N���̏�����
//---------------------------------------------------------------------------------------
bool CGraphics::Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	// �����ۑ�
	m_hWnd = hWnd;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_bWindow = bWindow;

	// �s�N�Z�� �t�H�[�}�b�g
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

	// �E�B���h�E�^�t�� �X�N���[���؂�ւ�
	if (!bWindow) {
		DEVMODE	devmode;
		devmode.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		devmode.dmBitsPerPel	= 32;
		devmode.dmPelsWidth		= nWidth;
		devmode.dmPelsHeight	= nHeight;
		devmode.dmSize			= sizeof devmode;
		if (ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			MessageBox(hWnd, _T("�t���X�N���[���؂�ւ��Ɏ��s���܂���"),
				_T("error"), MB_OK | MB_ICONERROR);
			return false;
		}
		SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0,
			nWidth, nHeight, SWP_SHOWWINDOW);
	}

	// �E�B���h�E �R���e�L�X�g�̎擾
	m_hDC = GetDC(hWnd);

	// ���v����ł��߂��s�N�Z�� �t�H�[�}�b�g�̑I��
	int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (!nPixelFormat) {
		MessageBox(hWnd, _T("�s�N�Z���t�H�[�}�b�g�̑I���Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// �s�N�Z�� �t�H�[�}�b�g�̐ݒ�
	if (!SetPixelFormat(m_hDC, nPixelFormat, &pfd)) {
		MessageBox(hWnd, _T("�s�N�Z���t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// OpenGL �����_�����O �R���e�L�X�g���쐬
	m_hRC = wglCreateContext(m_hDC);
	if (!m_hRC) {
		MessageBox(hWnd, _T("�����_�����O�R���e�L�X�g�̐����Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// OpenGL �����_�����O �R���e�L�X�g�����C�N
	if (!wglMakeCurrent(m_hDC, m_hRC)) {
		MessageBox(hWnd, _T("�����_�����O�R���e�L�X�g�̃��C�N�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// OpenGL �g���֐��̎擾
	char *ext = (char *)glGetString(GL_EXTENSIONS);
	if (strstr(ext, "WGL_EXT_swap_control")) {
		wglSwapIntervalEXT    = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		if (!wglSwapIntervalEXT)
			wglSwapIntervalEXT = _fnwglSwapIntervalEXT;
		if (!wglGetSwapIntervalEXT)
			wglGetSwapIntervalEXT = _fnwglGetSwapIntervalEXT;
	}
	wglSwapIntervalEXT((bWindow) ? 0 : 1);	// �E�B���h�E���A������������

	// �t�H���g �Z�b�g�A�b�v
	m_uFontTexID = LoadTexture("../data/texture/font.tga");
	m_fFontSize = float(FONT_HEIGHT);

	return true;
}

//---------------------------------------------------------------------------------------
// �O���t�B�b�N���̃N���[���A�b�v
//---------------------------------------------------------------------------------------
void CGraphics::Finalize()
{
	ReleaseTexture(m_uFontTexID);		// �t�H���g�p�e�N�X�`�����
	m_uFontTexID = 0;

	wglMakeCurrent(NULL, NULL);
	if (m_hRC) {						// �J�����g�R���e�L�X�g�����
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}
	if (m_hDC) {						// �f�o�C�X�R���e�L�X�g�����
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}
	if (!m_bWindow)
		ChangeDisplaySettings(NULL, 0);
}

//---------------------------------------------------------------------------------------
// �`��N���X����
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
// �`��N���X���
//---------------------------------------------------------------------------------------
void CGraphics::Release()
{
	Finalize();
	delete this;
}

//---------------------------------------------------------------------------------------
// �����_�����O�J�n
//---------------------------------------------------------------------------------------
bool CGraphics::Begin()
{
	// �r���[�|�[�g���N���A
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Init3D();
	return true;
}

//---------------------------------------------------------------------------------------
// �����_�����O�I��
//---------------------------------------------------------------------------------------
void CGraphics::End()
{
	// �������Ȃ�
}

//---------------------------------------------------------------------------------------
// �o�b�N�o�b�t�@����
//---------------------------------------------------------------------------------------
void CGraphics::SwapBuffer()
{
	// �o�b�N�o�b�t�@�ɕ`�悵�����e��\��
	SwapBuffers(m_hDC);
}

//---------------------------------------------------------------------------------------
// �f�o�b�O������`��
//---------------------------------------------------------------------------------------
void CGraphics::DrawText(int nX, int nY, LPCTSTR psz)
{
	Init2D();
	// �f�o�b�O �X�g�����O�`��
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	SetFontSize();
	DrawStr(0, 0, psz);
}

//---------------------------------------------------------------------------------------
// 2D �p�����_�����O �X�e�[�g������
//---------------------------------------------------------------------------------------
void CGraphics::Init2D()
{
	// ���C�g������
	glDisable(GL_LIGHTING);

	// �e�N�X�`����L���ɂ���
	glEnable(GL_TEXTURE_2D);

	// �|���S���̕Жʂ����`��𖳌���
	glDisable(GL_CULL_FACE);

	// �[�x�o�b�t�@�𖳌���
	glDisable(GL_DEPTH_TEST);

	// �A���t�@ �u�����f�B���O�L����
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);

	// �v���W�F�N �V�����}�g���b�N�X�Ƃ��Đ��ˉe��ݒ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, m_nWidth, m_nHeight, 0.0, -1.0, 1.0);

	// �r���[ �}�g���b�N�X�ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//---------------------------------------------------------------------------------------
// 3D �p�����_�����O �X�e�[�g������
//---------------------------------------------------------------------------------------
void CGraphics::Init3D()
{
	// �e�N�X�`���𖳌��ɂ���
	glDisable(GL_TEXTURE_2D);

	// �|���S���̕Жʂ����`���L����
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// �[�x�o�b�t�@��L����
	glEnable(GL_DEPTH_TEST);

	// �A���t�@ �u�����f�B���O������
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	// �v���W�F�N�V���� �}�g���b�N�X�ݒ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((double)m_nWidth * -0.5,(double)m_nWidth * 0.5,(double)m_nHeight * -0.5,(double)m_nHeight * 0.5,NEAR_CLIP,FAR_CLIP);
//	gluPerspective(FOVY, (double)m_nWidth / m_nHeight, NEAR_CLIP, FAR_CLIP);
	glGetFloatv(GL_PROJECTION_MATRIX, m_MatProj.f);

	// �r���[ �}�g���b�N�X�ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,											// ���_�ʒu(X)
			  0.0f,											// ���_�ʒu(Y)
			  m_nHeight * 0.5f / tanf(RAD * FOVY * 0.5f),	// ���_�ʒu(Z)
			  0.0f, 0.0f, 0.0f,		// �����_�ʒu
			  0.0f, 1.0f, 0.0f);	// �A�b�v�x�N�g��
	glGetFloatv(GL_MODELVIEW_MATRIX, m_MatView.f);

	// ���C�g�L����
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
// 1 �����`��
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

	glEnable(GL_TEXTURE_2D);	// �e�N�X�`���L����
	glBindTexture(GL_TEXTURE_2D, m_uFontTexID);
	glInterleavedArrays(GL_T2F_V3F, 0, vtxQuad);
	glDrawArrays(GL_QUADS, 0, 4);
}

//---------------------------------------------------------------------------------------
// ������`��
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