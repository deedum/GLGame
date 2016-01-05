#pragma once
class CCamera;

#include "C3DObj.h"
#include "Player.h"
#include "Sky.h"

enum ECameraType {
	CT_NONE = 0,
	CT_FIRSTPERSON,
	CT_FIXED,
	CT_THIRDPERSON,
	CT_VIEWPLAYER,

	MAX_CAMERATYPE
};

class CCameraInfo
{
public:
	VECTOR3		m_eye;		// ���_
	VECTOR3		m_at;		// �����_
	VECTOR3		m_up;		// ����x�N�g��
};

class CCamera :	public C3DObj
{
protected:
	VECTOR3			m_newPos;
	VECTOR3			m_at;		// �����_
	VECTOR3			m_up;		// �����
static	int			m_camtype;	// 1:���@2:�k�@3:���@4:��
	CSky*			m_pSky;		// �X�J�C�h�[��
	CCameraInfo*	m_pCamInfo;	// �J�������
	bool			m_bMove;	// �ړ���

public:
	CCamera(CScene* pScene);
	virtual ~CCamera(void);
	virtual void PostInit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void PostUpdate2(void);
	void InitDraw(void);
	void LookAt(VECTOR3* pAt);
	void SetUp(VECTOR3* pUp);
	void SetSky(CSky* pSky) {m_pSky = pSky;}
	void SetType(int nType) {m_camtype = nType;}
	static int GetCameraType(){return m_camtype;}
};