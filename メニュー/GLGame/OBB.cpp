
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "OBB.h"

COBB::COBB(void)
{
	Vec3Set(&m_vCenter,0.0f,0.0f,0.0f);
	Vec3Set(&m_vRadius,0.0f,0.0f,0.0f);
}


COBB::~COBB(void)
{
}
// AABB���m�̓����蔻��

bool COBB::IntersectOBB(COBB obb1, MATRIX world1,COBB obb2, MATRIX world2)
{
	MATRIX mWorld1 = world1;
	MATRIX mWorld2 = world2;
	VECTOR3 vCenter1 = obb1.GetCenter();
	VECTOR3 vCenter2 = obb2.GetCenter();
	VECTOR3 vBBox1 = obb1.GetRadius();
	VECTOR3 vBBox2 = obb2.GetRadius();

	// ���f�����W�����o
	VECTOR3	vN[6];
	vN[0] = Vec3GetAxisX(mWorld1);	// X
	vN[1] = Vec3GetAxisY(mWorld1);	// Y
	vN[2] = Vec3GetAxisZ(mWorld1);	// Z
	vN[3] = Vec3GetAxisX(mWorld2);	// X
	vN[4] = Vec3GetAxisY(mWorld2);	// Y
	vN[5] = Vec3GetAxisZ(mWorld2);	// Z

	// OBB�T�C�Y�擾
	VECTOR3	vR[6];
	Vec3Set(&vR[0],vN[0].x * vBBox1.x, vN[0].y * vBBox1.y, vN[0].z * vBBox1.z);
	Vec3Set(&vR[1],vN[1].x * vBBox1.x, vN[1].y * vBBox1.y, vN[1].z * vBBox1.z);
	Vec3Set(&vR[2],vN[2].x * vBBox1.x, vN[2].y * vBBox1.y, vN[2].z * vBBox1.z);
	Vec3Set(&vR[3],vN[3].x * vBBox2.x, vN[3].y * vBBox2.y, vN[3].z * vBBox2.z);
	Vec3Set(&vR[4],vN[4].x * vBBox2.x, vN[4].y * vBBox2.y, vN[4].z * vBBox2.z);
	Vec3Set(&vR[5],vN[5].x * vBBox2.x, vN[5].y * vBBox2.y, vN[5].z * vBBox2.z);
	
	// ���S�Ԃ̃x�N�g��
	VECTOR3	vPos1, vPos2 , vDistance;
	Vec3TransformCoord(&vPos1, &mWorld1, &vCenter1);
	Vec3TransformCoord(&vPos2, &mWorld2, &vCenter2);
	Vec3Set(&vDistance, vPos1.x- vPos2.x, vPos1.y- vPos2.y, vPos1.z- vPos2.z);
	
	VECTOR3* pSep;			// ������(���)

	float fRad;
	for(int i = 0; i < 6; i++){
		pSep = &vN[i];
		fRad = 0.0f;
		for(int j = 0; j < 6; j ++){				// ��������ōł��������_�܂ł̋���
			fRad += fabsf(Vec3Dot(pSep, &vR[j]));
		}
		if(fabsf(Vec3Dot(pSep, &vDistance)) > fRad){	// ����������̋����Ɣ�r
			return false;			// �������Ă��Ȃ�
		}
	}
	VECTOR3 vSep;			// ������(���)
	for (int i = 0; i < 3; i++) {				// OBB1
		for (int k = 3; k < 6; k++) {			// OBB2
			Vec3Cross(&vSep, &vN[i], &vN[k]);
			Vec3Normalize(&vSep, &vSep);
			pSep = &vSep;						// ���������m�̊O��

			fRad = 0.0f;
			for (int j = 0; j < 6; j++) {
				fRad += fabsf(Vec3Dot(pSep, &vR[j]));
			}
			if (fabsf(Vec3Dot(pSep, &vDistance)) > fRad) {
				return false;	// �������Ă��Ȃ�
			}
		}
	}
	return true;				// �������Ă���
}


void COBB::Draw(MATRIX *matWorld)
{
	GLdouble vertex[][3] = {
	{ m_vRadius.x * -1.0, m_vRadius.y * -1.0, m_vRadius.z * -1.0 },
	{ m_vRadius.x		, m_vRadius.y * -1.0, m_vRadius.z * -1.0 },
	{ m_vRadius.x		, m_vRadius.y		, m_vRadius.z * -1.0 },
	{ m_vRadius.x * -1.0, m_vRadius.y		, m_vRadius.z * -1.0 },
	{ m_vRadius.x * -1.0, m_vRadius.y * -1.0, m_vRadius.z },
	{ m_vRadius.x		, m_vRadius.y * -1.0, m_vRadius.z },
	{ m_vRadius.x		, m_vRadius.y		, m_vRadius.z },
	{ m_vRadius.x * -1.0, m_vRadius.y		, m_vRadius.z }
	};
	
	int face[][4] = {//�ʂ̒�`
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	MATRIX mWorld;
	MatrixIdentity(&mWorld);
	
	MatrixTranslation(&mWorld, m_vCenter.x, m_vCenter.y, m_vCenter.z);
	MatrixMultiply(&mWorld, matWorld, &mWorld);

	// ���[���h�}�g���b�N�X�ݒ�
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(mWorld.f);



	
	// ���u�����h�L��
	glEnable(GL_BLEND);
	//// �f�v�X�o�b�t�@�ւ̏������݂��֎~
	//glDepthMask(GL_FALSE);
	//// �u�����f�B���O�̂��߂̕�������ݒ�
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(0.0, 0.0, 1.0, 0.5);//�F�̐ݒ�

	glBegin(GL_QUADS);
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}
	glEnd();

	//// �f�v�X�o�b�t�@�ւ̏������݂�����
	//glDepthMask(GL_TRUE);
	// ���u�����h����
	glDisable(GL_BLEND);

	// �r���[�}�g���b�N�X���A
	glPopMatrix();
	
	
	
}