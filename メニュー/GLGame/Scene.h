//=======================================================================================
//
//	  �V�[�� �N���X��`
//
//=======================================================================================
#pragma once

#include "GameWnd.h"

#include "Mesh.h"	// CMesh�N���X�錾
#include "Input.h"	// CInput�N���X�錾

enum EScene
{
	SCENE_SPLASH = 0,
	SCENE_TITLE,
	SCENE_MAIN,
	SCENE_GAMEOVER,
	SCENE_TESTMODE,

	MAX_SCENE
};

class C3DObj;
class CScene
{
protected:
	CGraphics*	m_pGraph;			// �O���t�B�b�N�f�o�C�X
	C3DObj*		m_pTop;				// 3D�I�u�W�F�N�g�擪

	TCHAR		m_szDebug[1024];	// �f�o�b�O�p������
	int			m_FPS;				// �t���[�����J�E���g�p

public:
	CScene();
	virtual ~CScene();

	static CScene* Create(int nIndex);
	virtual void Release();
	virtual void Render();
	virtual void Update();
	virtual void Draw();
	void SetFPS(int nFPS) {m_FPS = nFPS;}
	void AddDebugStr(LPCTSTR psz) {lstrcat(m_szDebug, psz);}
	C3DObj* GetObj() {return m_pTop;}
	void SetObj(C3DObj* pObj) {m_pTop = pObj;}

protected:
	virtual bool Initialize();
	virtual void Finalize();
	void InitObj(void);		// �S�I�u�W�F�N�g������
	void UpdateObj(void);	// �S�I�u�W�F�N�g�X�V
	void DrawObj(void);		// �S�I�u�W�F�N�g�`��
	void FinObj(void);		// �S�I�u�W�F�N�g���
};

//=======================================================================================
//	End of File
//=======================================================================================