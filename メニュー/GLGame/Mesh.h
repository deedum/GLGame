// X�t�@�C���Ǎ�/�\��(���}�e���A���Ή���)
#ifndef _H_MESH
#define _H_MESH

#include <windows.h>
#include <GL/gl.h>

#include "Matrix.h"

// �}�e���A��
typedef struct _MATERIAL {
	VECTOR4		diffuse;				// �g�U��/����
	float		power;					// ���ʌ��̎w��
	VECTOR4		specular;				// ���ʌ�(RGBX)
	VECTOR4		emissive;				// ���ˋP�x(RGBX)
	GLuint		textureID;				// �e�N�X�`��ID
} MATERIAL;

// ���f��
typedef struct _MESH {
	int			vnum;					// ���_��
	VECTOR3		*vertex;				// ���_���W
	int			fnum;					// �ʐ�
	int			*face;					// �ʏ��
	int			nnum;					// �@���x�N�g����
	VECTOR3		*normal;				// �@���x�N�g��
	int			inum;					// �@���C���f�b�N�X��
	int			*index;					// �@���C���f�b�N�X
	VECTOR2		*uv;					// UV���W
	int			mnum;					// �}�e���A����
	MATERIAL	*material;				// �}�e���A��
	int			*midx;					// �}�e���A���C���f�b�N�X
} MESH;

int LoadMesh(MESH *mesh, const char *fname);	// ���f���Ǎ�
void ReleaseMesh(MESH *mesh);					// ���f�����
void DrawMesh(MATRIX *matWorld, MESH *mesh);		// ���f���\��
void DrawMeshAlpha(MATRIX *matWorld, MESH *mesh);	// ���f���\��(�����̂�)
void DrawMeshNoAlpha(MATRIX *matWorld, MESH *mesh);	// ���f���\��(�񃿕��̂�)
int ComputeBoundingBox(MESH *mesh, VECTOR3 *pvMin, VECTOR3 *pvMax);
int ComputeBoundingSphere(MESH *mesh, VECTOR3 *pvCenter, float *pfRadius);

#endif // _H_MESH