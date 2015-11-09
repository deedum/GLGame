#pragma once
#include "Scene.h"
#include "Sound.h"
#include "Camera.h"
#include "Sky.h"

class CTitleScene : public CScene
{
protected:
	//-------- �Q�[���p�I�u�W�F�N�g
//	CSound*		m_pSound;
	CCamera*	m_pCamera;
	CSky*		m_pSky;

	int			m_nTick;	// �e��^�C�~���O�p
	int			m_nSelect;	// ���j���[�I������No.
	int			m_nCount;	// �I�[�g���s�[�g�p�J�E���^
	int			m_nBlink;	// �_�ŗp�J�E���^

	VECTOR3		m_campos;

public:
	CTitleScene(void);
	virtual ~CTitleScene(void);

	virtual bool Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw();
};

