// X�t�@�C���Ǎ�/�\��(���}�e���A���Ή���)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Mesh.h"
#include "Texture.h"

//---------------------------------------------------------------------------------------
// ��d���p���Ɉ͂܂ꂽ������̐؂�o��
//---------------------------------------------------------------------------------------
char* trim(char* psz)
{
	if (psz == NULL)
		return NULL;
	for (; *psz; ++psz) {
		if (*psz == '"') {
			char* pszEnd;

			*psz++ = '\0';
			for (pszEnd = psz; *pszEnd; ++pszEnd);
			for (--pszEnd; pszEnd > psz; --pszEnd) {
				if (*pszEnd == '"') {
					*pszEnd = '\0';
					break;
				}
			}
			break;
		}
	}
	return psz;
}

//---------------------------------------------------------------------------------------
// ���b�V���Ǎ�
//---------------------------------------------------------------------------------------
int LoadMesh(MESH *mesh, const char *fname)
{
	FILE		*fp;					// �t�@�C���|�C���^
	char		line[256];				// �ǂݍ��݃o�b�t�@
	char		*token;					// �P��ւ̃|�C���^
	const char	sep[] = " ,;{}\t\n";	// ��؂蕶��
	int			phase;					// �������̏��
	int			i, j;					// �J�E���^
	char		szDir[_MAX_PATH];		// �h���C�u��
	char		szFolder[_MAX_DIR];		// �t�H���_��
	char		szCurrentDir[_MAX_PATH];// ���݂̃t�H���_
	int			n, t;					// ���[�N

	// �t�H���_���𒊏o
	_splitpath(fname, szDir, szFolder, NULL, NULL);
	strcat(szDir, szFolder);

	phase = 0;							// �f�[�^�擪�̌��o
	fp = fopen(fname, "r");				// X�t�@�C���I�[�v��
	if (fp == NULL) {
//		printf("X�t�@�C��\"%s\"��������܂���B\n", fname);
		return 0;						// �G���[�I��
	}
	while (fgets(line, 256, fp)) {
		token = strtok(line, sep);
		while (token) {
			switch (phase) {
			case 0:						// �f�[�^�擪�̒��o
				if (strcmp(token, "template") == 0) {
					while (strtok(NULL, sep));
					break;
				}
				if (strcmp(token, "Mesh") == 0) {
					phase = 1;
					break;
				}
				if (strcmp(token, "MeshNormals") == 0) {
					phase = 5;
					break;
				}
				if (strcmp(token, "MeshTextureCoords") == 0) {
					phase = 9;
					break;
				}
				if (strcmp(token, "MeshMaterialList") == 0) {
					phase = 11;
					break;
				}
				break;
			case 1:						// ���_���擾
				mesh->vnum = atoi(token);
				mesh->vertex = (VECTOR3*)malloc(mesh->vnum * sizeof(VECTOR3));
				phase = 2;
				i = j = 0;
				break;
			case 2:						// ���_���W�ǂݍ���
				mesh->vertex[i].f[j] = (float)atof(token);
				if (j == 2) mesh->vertex[i].f[j] *= -1.0f;
				j++;
				if (j >= 3) {
					j = 0;
					i++;
					if (i >= mesh->vnum)
						phase = 3;
				}
				break;
			case 3:						// �ʐ��擾
				mesh->fnum = atoi(token);
				mesh->face = (int *)malloc(mesh->fnum * 5 * sizeof(int));
				phase = 4;
				i = j = 0;
				break;
			case 4:						// �ʏ��ǂݍ���
				mesh->face[i * 5 + j] = atoi(token);	// ���_��/�C���f�b�N�X�𐔒l��
				j++;									// �Ǎ������Z
				if (j > mesh->face[i * 5]) {			// �Ǎ��������_���𒴂����H
					n = mesh->face[i * 5];
					t = mesh->face[i * 5 + 1];			// ���_�̕��т𔽓]
					mesh->face[i * 5 + 1] = mesh->face[i * 5 + n - 1];
					mesh->face[i * 5 + n - 1] = t;
					j = 0;								// �Ǎ������N���A
					i++;								// �Ǎ��ʐ����Z
					if (i >= mesh->fnum)				// �Ǎ��ʐ����ʐ��𒴂����H
						phase = 0;						// �f�[�^�擪�̒��o��
				}
				break;
			case 5:						// �@���x�N�g�����擾
				mesh->nnum = atoi(token);
				mesh->normal = (VECTOR3*)malloc(mesh->nnum * sizeof(VECTOR3));
				phase = 6;
				i = j = 0;
				break;
			case 6:						// �@���x�N�g���ǂݍ���
				mesh->normal[i].f[j] = (float)atof(token);
				if (j == 2) mesh->normal[i].f[j] *= -1.0f;
				j++;
				if (j >= 3) {
					j = 0;
					i++;
					if (i >= mesh->nnum)
						phase = 7;
				}
				break;
			case 7:						// �@���C���f�b�N�X���擾
				mesh->inum = atoi(token);
				mesh->index = (int *)malloc(mesh->inum * 5 * sizeof(int));
				phase = 8;
				i = j = 0;
				break;
			case 8:						// �@���C���f�b�N�X�Ǎ�
				mesh->index[i * 5 + j] = atoi(token);	// ���_��/�C���f�b�N�X�𐔒l��
				j++;									// �Ǎ������Z
				if (j > mesh->index[i * 5]) {			// �Ǎ��������_���𒴂����H
					n = mesh->index[i * 5];
					t = mesh->index[i * 5 + 1];			// �@���̕��т𔽓]
					mesh->index[i * 5 + 1] = mesh->index[i * 5 + n - 1];
					mesh->index[i * 5 + n - 1] = t;
					j = 0;								// �Ǎ������N���A
					i++;								// �Ǎ��C���f�b�N�X�����Z
					if (i >= mesh->inum)				// �Ǎ��C���f�b�N�X�����C���f�b�N�X���𒴂����H
						phase = 0;						// �f�[�^�擪�̒��o��
				}
				break;
			case 9:						// UV���W���擾(���_���Ɠ����͂�)
				i = atoi(token);
				mesh->uv = (VECTOR2*)malloc(mesh->vnum * sizeof(VECTOR2));
				phase = 10;
				i = j = 0;
				break;
			case 10:					// UV���W�ǂݍ���
				mesh->uv[i].f[j] = (float)atof(token);
				if (j == 1) mesh->uv[i].f[j] = 1.0f - mesh->uv[i].f[j];
				j++;
				if (j >= 2) {
					j = 0;
					i++;
					if (i >= mesh->vnum)
						phase = 0;
				}
				break;
			case 11:					// �ގ����擾
				mesh->mnum = atoi(token);
				mesh->material = (MATERIAL*)malloc(mesh->mnum * sizeof(MATERIAL));
				memset(mesh->material, 0, mesh->mnum * sizeof(MATERIAL));
				phase = 12;
				break;
			case 12:					// �ގ��C���f�b�N�X���擾(�ʐ��Ɠ����͂�)
				i = atoi(token);
				mesh->midx = (int *)malloc(mesh->fnum * sizeof(int));
				i = 0;
				phase = 13;
				break;
			case 13:					// �ގ��C���f�b�N�X�ǂݍ���
				mesh->midx[i] = (int)atoi(token);
				i++;
				if (i >= mesh->fnum) {
					i = 0;
					phase = 14;
				}
				break;
			case 14:					// �ގ����ǂݍ���
				if (strcmp(token, "Material") == 0) {
					j = 0;
					phase = 15;
				} else {
					phase = 0;
					continue;
				}
				break;
			case 15:					// �g�U��/����
				mesh->material[i].diffuse.f[j] = (float)atof(token);
				j++;
				if (j >= 4)
					phase = 16;
				break;
			case 16:					// ���ʌ��̎w��
				mesh->material[i].power = (float)atof(token);
				j = 0;
				phase = 17;
				break;
			case 17:					// ���ʌ�
				mesh->material[i].specular.f[j] = (float)atof(token);
				j++;
				if (j >= 3) {
					mesh->material[i].specular.a = 1.0f;
					j = 0;
					phase = 18;
				}
				break;
			case 18:					// ���ˋP�x
				mesh->material[i].emissive.f[j] = (float)atof(token);
				j++;
				if (j >= 3) {
					mesh->material[i].emissive.a = 1.0f;
					j = 0;
					phase = 19;
				}
				break;
			case 19:					// �e�N�X�`��
				if (strcmp(token, "TextureFilename") == 0) {
					phase = 20;
					break;
				}
				if (strcmp(token, "Material") == 0) {
					i++;
					j = 0;
					phase = 15;
					break;
				}
				if (*token) {
					phase = 0;
					continue;
				}
				break;
			case 20:					// �e�N�X�`��
				if (szDir[0]) {		// �J�����g�f�B���N�g����ύX
					GetCurrentDirectoryA(_MAX_PATH, szCurrentDir);
					SetCurrentDirectoryA(szDir);
				}
				mesh->material[i].textureID = LoadTexture(trim(token));
				if (szDir[0])		// �J�����g�f�B���N�g�������ɖ߂�
					SetCurrentDirectoryA(szCurrentDir);
				i++;
				if (i < mesh->mnum)
					phase = 14;
				else
					phase = 0;
				break;
			}
			token = strtok(NULL, sep);
		}
	}
	fclose(fp);							// X�t�@�C���N���[�Y

	return 1;							// ����I��
}

//---------------------------------------------------------------------------------------
// ���b�V�����
//---------------------------------------------------------------------------------------
void ReleaseMesh(MESH *mesh)
{
	if (mesh == NULL)
		return;
	if (mesh->vertex) {
		free(mesh->vertex);
		mesh->vertex = NULL;
	}
	if (mesh->face) {
		free(mesh->face);
		mesh->face = NULL;
	}
	if (mesh->normal) {
		free(mesh->normal);
		mesh->normal = NULL;
	}
	if (mesh->index) {
		free(mesh->index);
		mesh->index = NULL;
	}
	if (mesh->uv) {
		free(mesh->uv);
		mesh->uv = NULL;
	}
	if (mesh->material) {
		int i;

		for (i = 0; i < mesh->mnum; i++) {
			ReleaseTexture(mesh->material[i].textureID);
			mesh->material[i].textureID = 0;
		}
		free(mesh->material);
		mesh->material = NULL;
	}
	if (mesh->midx) {
		free(mesh->midx);
		mesh->midx = NULL;
	}
}

//---------------------------------------------------------------------------------------
// ���b�V���`��
//---------------------------------------------------------------------------------------
void DrawMesh(MATRIX *matWorld, MESH *mesh)
{
	int			i, j;
	int			nVtx, nMat;
	MATERIAL	*pMat;

	// ���[���h�}�g���b�N�X�ݒ�
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matWorld->f);

	// �`��
	nVtx = -1;
	nMat = -1;
	for (i = 0; i < mesh->fnum; i++) {
		if (nVtx != mesh->face[i * 5] || nMat != mesh->midx[i]) {
			if (nVtx > 0) glEnd();
			nVtx = mesh->face[i * 5];
			if (nMat != mesh->midx[i]) {
				nMat = mesh->midx[i];
				pMat = &mesh->material[mesh->midx[i]];
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pMat->diffuse.f);
				glMaterialf(GL_FRONT, GL_SHININESS, pMat->power);
				glMaterialfv(GL_FRONT, GL_SPECULAR, pMat->specular.f);
				glMaterialfv(GL_FRONT, GL_EMISSION, pMat->emissive.f);
				if (mesh->uv && pMat->textureID) {
					glBindTexture(GL_TEXTURE_2D, pMat->textureID);
					glEnable(GL_TEXTURE_2D);
				} else
					glDisable(GL_TEXTURE_2D);
			}
			glBegin((nVtx > 3) ? GL_QUADS : GL_TRIANGLES);
		}
		if (pMat->diffuse.a >= 1.0f) {
			for (j = 1; j <= nVtx; j++) {
				if (mesh->uv)
					glTexCoord2fv(mesh->uv[mesh->face[i * 5 + j]].f);
				if (mesh->normal && mesh->index)
					glNormal3fv(mesh->normal[mesh->index[i * 5 + j]].f);
				glVertex3fv(mesh->vertex[mesh->face[i * 5 + j]].f);
			}
		}
	}
	if (nVtx > 0) glEnd();

	// ���u�����h�L��
	glEnable(GL_BLEND);
	// �f�v�X�o�b�t�@�ւ̏������݂��֎~
	glDepthMask(GL_FALSE);
	// �u�����f�B���O�̂��߂̕�������ݒ�
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	nVtx = -1;
	nMat = -1;
	for (i = 0; i < mesh->fnum; i++) {
		if (nVtx != mesh->face[i * 5] || nMat != mesh->midx[i]) {
			if (nVtx > 0) glEnd();
			nVtx = mesh->face[i * 5];
			if (nMat != mesh->midx[i]) {
				nMat = mesh->midx[i];
				pMat = &mesh->material[mesh->midx[i]];
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pMat->diffuse.f);
				glMaterialf(GL_FRONT, GL_SHININESS, pMat->power);
				glMaterialfv(GL_FRONT, GL_SPECULAR, pMat->specular.f);
				glMaterialfv(GL_FRONT, GL_EMISSION, pMat->emissive.f);
				if (mesh->uv && pMat->textureID) {
					glBindTexture(GL_TEXTURE_2D, pMat->textureID);
					glEnable(GL_TEXTURE_2D);
				} else
					glDisable(GL_TEXTURE_2D);
			}
			glBegin((nVtx > 3) ? GL_QUADS : GL_TRIANGLES);
		}
		if (pMat->diffuse.a < 1.0f) {
			for (j = 1; j <= nVtx; j++) {
				if (mesh->uv)
					glTexCoord2fv(mesh->uv[mesh->face[i * 5 + j]].f);
				if (mesh->normal && mesh->index)
					glNormal3fv(mesh->normal[mesh->index[i * 5 + j]].f);
				glVertex3fv(mesh->vertex[mesh->face[i * 5 + j]].f);
			}
		}
	}
	if (nVtx > 0) glEnd();

	// �f�v�X�o�b�t�@�ւ̏������݂�����
	glDepthMask(GL_TRUE);
	// ���u�����h����
	glDisable(GL_BLEND);

	// �r���[�}�g���b�N�X���A
	glPopMatrix();
}

//---------------------------------------------------------------------------------------
// ���f���\��(�����̂�)
//---------------------------------------------------------------------------------------
void DrawMeshAlpha(MATRIX *matWorld, MESH *mesh)
{
	int			i, j;
	int			nVtx, nMat;
	MATERIAL	*pMat;

	// ���[���h�}�g���b�N�X�ݒ�
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matWorld->f);

	// ���u�����h�L��
	glEnable(GL_BLEND);
	// �f�v�X�o�b�t�@�ւ̏������݂��֎~
	glDepthMask(GL_FALSE);
	// �u�����f�B���O�̂��߂̕�������ݒ�
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	nVtx = -1;
	nMat = -1;
	for (i = 0; i < mesh->fnum; i++) {
		if (nVtx != mesh->face[i * 5] || nMat != mesh->midx[i]) {
			if (nVtx > 0) glEnd();
			nVtx = mesh->face[i * 5];
			if (nMat != mesh->midx[i]) {
				nMat = mesh->midx[i];
				pMat = &mesh->material[mesh->midx[i]];
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pMat->diffuse.f);
				glMaterialf(GL_FRONT, GL_SHININESS, pMat->power);
				glMaterialfv(GL_FRONT, GL_SPECULAR, pMat->specular.f);
				glMaterialfv(GL_FRONT, GL_EMISSION, pMat->emissive.f);
				if (mesh->uv && pMat->textureID) {
					glBindTexture(GL_TEXTURE_2D, pMat->textureID);
					glEnable(GL_TEXTURE_2D);
				} else
					glDisable(GL_TEXTURE_2D);
			}
			glBegin((nVtx > 3) ? GL_QUADS : GL_TRIANGLES);
		}
		if (pMat->diffuse.a < 1.0f) {
			for (j = 1; j <= nVtx; j++) {
				if (mesh->uv)
					glTexCoord2fv(mesh->uv[mesh->face[i * 5 + j]].f);
				if (mesh->normal && mesh->index)
					glNormal3fv(mesh->normal[mesh->index[i * 5 + j]].f);
				glVertex3fv(mesh->vertex[mesh->face[i * 5 + j]].f);
			}
		}
	}
	if (nVtx > 0) glEnd();

	// �f�v�X�o�b�t�@�ւ̏������݂�����
	glDepthMask(GL_TRUE);
	// ���u�����h����
	glDisable(GL_BLEND);

	// �r���[�}�g���b�N�X���A
	glPopMatrix();
}

//---------------------------------------------------------------------------------------
// ���f���\��(�񃿕��̂�)
//---------------------------------------------------------------------------------------
void DrawMeshNoAlpha(MATRIX *matWorld, MESH *mesh)
{
	int			i, j;
	int			nVtx, nMat;
	MATERIAL	*pMat;

	// ���[���h�}�g���b�N�X�ݒ�
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matWorld->f);

	// �`��
	nVtx = -1;
	nMat = -1;
	for (i = 0; i < mesh->fnum; i++) {
		if (nVtx != mesh->face[i * 5] || nMat != mesh->midx[i]) {
			if (nVtx > 0) glEnd();
			nVtx = mesh->face[i * 5];
			if (nMat != mesh->midx[i]) {
				nMat = mesh->midx[i];
				pMat = &mesh->material[mesh->midx[i]];
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pMat->diffuse.f);
				glMaterialf(GL_FRONT, GL_SHININESS, pMat->power);
				glMaterialfv(GL_FRONT, GL_SPECULAR, pMat->specular.f);
				glMaterialfv(GL_FRONT, GL_EMISSION, pMat->emissive.f);
				if (mesh->uv && pMat->textureID) {
					glBindTexture(GL_TEXTURE_2D, pMat->textureID);
					glEnable(GL_TEXTURE_2D);
				} else
					glDisable(GL_TEXTURE_2D);
			}
			glBegin((nVtx > 3) ? GL_QUADS : GL_TRIANGLES);
		}
		if (pMat->diffuse.a >= 1.0f) {
			for (j = 1; j <= nVtx; j++) {
				if (mesh->uv)
					glTexCoord2fv(mesh->uv[mesh->face[i * 5 + j]].f);
				if (mesh->normal && mesh->index)
					glNormal3fv(mesh->normal[mesh->index[i * 5 + j]].f);
				glVertex3fv(mesh->vertex[mesh->face[i * 5 + j]].f);
			}
		}
	}
	if (nVtx > 0) glEnd();

	// �r���[�}�g���b�N�X���A
	glPopMatrix();
}

//-------------------------------------------------------------------------------------------------
// ���E�{�b�N�X�擾
//-------------------------------------------------------------------------------------------------
int ComputeBoundingBox(MESH* mesh, VECTOR3 *pvMin, VECTOR3 *pvMax)
{
	int		i, j;

	if (mesh == NULL || mesh->vnum < 1) {
		pvMin->x = pvMax->x = 0.0f;
		pvMin->y = pvMax->y = 0.0f;
		pvMin->z = pvMax->z = 0.0f;
		return 0;
	}
	*pvMin = *pvMax = mesh->vertex[0];
	for (i = 1; i < mesh->vnum; i++) {
		for (j = 0; j < 3; j++) {
			if (pvMin->f[j] > mesh->vertex[i].f[j])
				pvMin->f[j] = mesh->vertex[i].f[j];
			if (pvMax->f[j] < mesh->vertex[i].f[j])
				pvMax->f[j] = mesh->vertex[i].f[j];
		}
	}
	return 1;
}

//-------------------------------------------------------------------------------------------------
// ���E���擾
//-------------------------------------------------------------------------------------------------
int ComputeBoundingSphere(MESH* mesh, VECTOR3 *pvCenter, float *pfRadius)
{
	VECTOR3		vecMin, vecMax ,vecLen, vecCenter;
	float		fMax, fRadius;
	int			i;

	if (mesh == NULL) return 0;
	if (pvCenter == NULL) pvCenter = &vecCenter;
	if (!ComputeBoundingBox(mesh, &vecMin, &vecMax)) return 0;
	pvCenter->x = (vecMin.x + vecMax.x) / 2.0f;
	pvCenter->y = (vecMin.y + vecMax.y) / 2.0f;
	pvCenter->z = (vecMin.z + vecMax.z) / 2.0f;
	fMax = 0.0f;
	for (i = 0; i < mesh->vnum; i++) {
		vecLen.x = mesh->vertex[i].x - pvCenter->x;
		vecLen.y = mesh->vertex[i].y - pvCenter->y;
		vecLen.z = mesh->vertex[i].z - pvCenter->z;
		fRadius = Vec3Length(&vecLen);
		if (fMax < fRadius) fMax = fRadius;
	}
	if (pfRadius) *pfRadius = fMax;
	return 1;
}

//=======================================================================================
//	End of File
//=======================================================================================