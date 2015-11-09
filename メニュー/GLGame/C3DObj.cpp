// 3D�I�u�W�F�N�g��{�N���X
#include <windows.h>
#include "C3DObj.h"

//C3DObj*	C3DObj::m_pTop = NULL;

// �I�u�W�F�N�g����
// UINT uID�F�N���X���
// �߂�l�FNULL�ȊO�F���������I�u�W�F�N�g
//         NULL�F�Y���N���X����
C3DObj* C3DObj::Find(UINT uID)
{
	return Next(uID, NULL);
}

// ���̃I�u�W�F�N�g����
// UINT uID�F�N���X���
//      pObj�F�Ō�Ɍ��������I�u�W�F�N�g
// �߂�l�FNULL�ȊO�F���Ɍ��������I�u�W�F�N�g
//         NULL�F�Y���N���X����
C3DObj* C3DObj::Next(UINT uID, C3DObj* pObj)
{
	// �����ʒu�擪
	if (pObj) {
		pObj = pObj->m_pNext;
	} else {
		pObj = m_pScene->GetObj();
	}
	// ����
	if (uID) {
		while (pObj) {
			if (pObj->m_id == uID) {
				return pObj;
			}
			pObj = pObj->m_pNext;
		}
	}
	return pObj;
}

/*
// �S�I�u�W�F�N�g������
void C3DObj::InitAll(void)
{
	// �������O����
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		pObj->PreInit();
	}
	// ������
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		pObj->Init();
	}
	// �������㏈��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		pObj->PostInit();
	}
}

// �S�I�u�W�F�N�g�X�V
void C3DObj::UpdateAll(void)
{
	C3DObj* pNext;
	// �X�V�O����
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->PreUpdate();
	}
	// �X�V
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Update();
	}
	// �X�V�㏈��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->PostUpdate();
	}
}

// �S�I�u�W�F�N�g�`��
void C3DObj::DrawAll(void)
{
	// �`��O����
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->IsVisible())
			pObj->PreDraw();
	}
	// �`��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->IsVisible())
			pObj->Draw();
	}
	// �`��㏈��
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->IsVisible())
			pObj->PostDraw();
	}
}

// �S�I�u�W�F�N�g�j��
void C3DObj::FinAll(void)
{
	C3DObj* pNext;
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Fin();
	}
}
*/

// �R���X�g���N�^
C3DObj::C3DObj(CScene* pScene)
{
	m_id = ID_3DOBJ;
	m_pScene = pScene;

	MatrixIdentity(&m_world);
	m_pos.x = m_speed.x = m_accel.x = m_angle.x = 0.0f;
	m_pos.y = m_speed.y = m_accel.y = m_angle.y = 0.0f;
	m_pos.z = m_speed.z = m_accel.z = m_angle.z = 0.0f;

	// ���X�g�擪�ɒǉ�
	m_pPrev = NULL;
	m_pNext = m_pScene->GetObj();
	if (m_pNext) {
		m_pNext->m_pPrev = this;
	}
	m_pScene->SetObj(this);

	m_visible = true;
}

// �f�X�g���N�^
C3DObj::~C3DObj(void)
{
	// ���X�g����폜
	if (m_pPrev) {
		m_pPrev->m_pNext = m_pNext;
	} else {
		m_pScene->SetObj(m_pNext);
	}
	if (m_pNext) {
		m_pNext->m_pPrev = m_pPrev;
	}
}

// �������O����
void C3DObj::PreInit(void)
{
	// �Ȃ�
}

// ������
void C3DObj::Init(void)
{
	// �Ȃ�
}

// �������㏈��
void C3DObj::PostInit(void)
{
	// �Ȃ�
}

// �X�V�O����
void C3DObj::PreUpdate(void)
{
	// �Ȃ�
}

// �X�V
void C3DObj::Update(void)
{
	MATRIX mRot;
	MatrixRotationYawPitchRoll(&mRot,
		m_angle.y * RAD,
		m_angle.x * RAD,
		m_angle.z * RAD);
	m_speed.x += m_accel.x;
	m_speed.y += m_accel.y;
	m_speed.z += m_accel.z;
	m_pos.x += m_speed.x;
	m_pos.y += m_speed.y;
	m_pos.z += m_speed.z;
	MATRIX mMov;
	MatrixTranslation(&mMov, m_pos.x, m_pos.y, m_pos.z);
	MatrixMultiply(&m_world, &mMov, &mRot);
}

// �X�V�㏈��
void C3DObj::PostUpdate(void)
{
	// �Ȃ�
}

// �`��O����
void C3DObj::PreDraw(void)
{
	// �Ȃ�
}

// �`��
void C3DObj::Draw(void)
{
	// �Ȃ�
}

// �`��㏈��
void C3DObj::PostDraw(void)
{
	// �Ȃ�
}

// �j��
void C3DObj::Fin(void)
{
	// �Ȃ�
}

// �ʒu�ݒ�
void C3DObj::SetPos(VECTOR3* pPos)
{
	if (pPos) m_pos = *pPos;
}

// �p�x�ݒ�
void C3DObj::SetAngle(VECTOR3* pAngle)
{
	if (pAngle) m_angle = *pAngle;
}

// ���x�ݒ�
void C3DObj::SetSpeed(VECTOR3* pSpeed)
{
	if (pSpeed) m_speed = *pSpeed;
}

// �����x�ݒ�
void C3DObj::SetAccel(VECTOR3* pAccel)
{
	if (pAccel) m_accel = *pAccel;
}

// �ʒu�擾
VECTOR3& C3DObj::GetPos(void)
{
	return m_pos;
}

// ���[���h�}�g���b�N�X�擾
MATRIX& C3DObj::GetWorld(void)
{
	return m_world;
}