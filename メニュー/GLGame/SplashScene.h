#pragma once
#include "Scene.h"
#include "Sound.h"	// CSound�N���X�錾
#include "Camera.h"	// CCamera�N���X�錾

class CSplashScene : public CScene
{
protected:
	CSound*		m_pSound;
	CCamera*	m_pCamera;
	DWORD		m_dwCount;
	GLuint		m_uTexID;

public:
	CSplashScene(void);
	virtual ~CSplashScene(void);

	virtual bool Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw();
};