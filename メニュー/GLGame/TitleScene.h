#pragma once
#include "Scene.h"
#include "Sound.h"
#include "Camera.h"
#include "Sky.h"

class CTitleScene : public CScene
{
protected:
	//-------- ゲーム用オブジェクト
//	CSound*		m_pSound;
	CCamera*	m_pCamera;
	CSky*		m_pSky;

	int			m_nTick;	// 各種タイミング用
	int			m_nSelect;	// メニュー選択項目No.
	int			m_nCount;	// オートリピート用カウンタ
	int			m_nBlink;	// 点滅用カウンタ

	VECTOR3		m_campos;

public:
	CTitleScene(void);
	virtual ~CTitleScene(void);

	virtual bool Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw();
};

