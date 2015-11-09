// 3D�I�u�W�F�N�g��{�N���X
#pragma once

#include <windows.h>
#include "Matrix.h"
#include "define.h"
#include "Scene.h"

class C3DObj
{
protected:
	UINT			m_id;		// �N���X���
	MATRIX			m_world;	// ���[���h�ϊ��s��
	VECTOR3			m_pos;		// ���[���h���W
	VECTOR3			m_angle;	// �p�x
	VECTOR3			m_speed;	// ���x
	VECTOR3			m_accel;	// �����x
	bool			m_visible;	// �\���t���O

//	static C3DObj*	m_pTop;		// �I�u�W�F�N�g ���X�g�擪
	CScene*			m_pScene;	// �V�[��
	C3DObj*			m_pPrev;	// �O�̃I�u�W�F�N�g
	C3DObj*			m_pNext;	// ���̃I�u�W�F�N�g

public:
	C3DObj(CScene* pScene);		// �R���X�g���N�^
	virtual ~C3DObj(void);		// �f�X�g���N�^

//	static void InitAll(void);	// �S�I�u�W�F�N�g������
//	static void UpdateAll(void);// �S�I�u�W�F�N�g�X�V
//	static void DrawAll(void);	// �S�I�u�W�F�N�g�`��
//	static void FinAll(void);	// �S�I�u�W�F�N�g���
//	static C3DObj* Find(UINT uID);	// ��ʂɂ�錟��
//	static C3DObj* Next(UINT uID, C3DObj* pObj);
	C3DObj* Find(UINT uID);			// ��ʂɂ�錟��
	C3DObj* Next(UINT uID, C3DObj* pObj);

	virtual void PreInit(void);		// �������O����
	virtual void Init(void);			// ������
	virtual void PostInit(void);		// �������㏈��
	virtual void PreUpdate(void);	// �X�V�O����
	virtual void Update(void);		// �X�V
	virtual void PostUpdate(void);	// �X�V�㏈��
	virtual void PreDraw(void);		// �`��O����
	virtual void Draw(void);			// �`��
	virtual void PostDraw(void);		// �`��㏈��
	virtual void Fin(void);			// ���

	UINT GetID(void) {return m_id;}	// �N���X��ʎ擾
	C3DObj* GetNext() {return m_pNext;}
	virtual void SetPos(VECTOR3* pPos);
	virtual void SetAngle(VECTOR3* pAngle);
	virtual void SetSpeed(VECTOR3* pSpeed);
	virtual void SetAccel(VECTOR3* pAccel);
	virtual VECTOR3& GetPos(void);
	virtual VECTOR3& GetSpeed(void) {return m_speed;}
	virtual MATRIX& GetWorld(void);
	virtual void SetVisible(bool bVisible = true) {m_visible = bVisible;}
	virtual bool IsVisible(void) {return (this) ? m_visible : false;}
};