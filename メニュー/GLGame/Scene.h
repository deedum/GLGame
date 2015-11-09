//=======================================================================================
//
//	  シーン クラス定義
//
//=======================================================================================
#pragma once

#include "GameWnd.h"

#include "Mesh.h"	// CMeshクラス宣言
#include "Input.h"	// CInputクラス宣言

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
	CGraphics*	m_pGraph;			// グラフィックデバイス
	C3DObj*		m_pTop;				// 3Dオブジェクト先頭

	TCHAR		m_szDebug[1024];	// デバッグ用文字列
	int			m_FPS;				// フレーム数カウント用

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
	void InitObj(void);		// 全オブジェクト初期化
	void UpdateObj(void);	// 全オブジェクト更新
	void DrawObj(void);		// 全オブジェクト描画
	void FinObj(void);		// 全オブジェクト解放
};

//=======================================================================================
//	End of File
//=======================================================================================