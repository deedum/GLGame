// Xファイル読込/表示(αマテリアル対応版)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Mesh.h"
#include "Texture.h"

//---------------------------------------------------------------------------------------
// 二重引用符に囲まれた文字列の切り出し
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
// メッシュ読込
//---------------------------------------------------------------------------------------
int LoadMesh(MESH *mesh, const char *fname)
{
	FILE		*fp;					// ファイルポインタ
	char		line[256];				// 読み込みバッファ
	char		*token;					// 単語へのポインタ
	const char	sep[] = " ,;{}\t\n";	// 区切り文字
	int			phase;					// 処理中の状態
	int			i, j;					// カウンタ
	char		szDir[_MAX_PATH];		// ドライブ名
	char		szFolder[_MAX_DIR];		// フォルダ名
	char		szCurrentDir[_MAX_PATH];// 現在のフォルダ
	int			n, t;					// ワーク

	// フォルダ名を抽出
	_splitpath(fname, szDir, szFolder, NULL, NULL);
	strcat(szDir, szFolder);

	phase = 0;							// データ先頭の検出
	fp = fopen(fname, "r");				// Xファイルオープン
	if (fp == NULL) {
//		printf("Xファイル\"%s\"が見つかりません。\n", fname);
		return 0;						// エラー終了
	}
	while (fgets(line, 256, fp)) {
		token = strtok(line, sep);
		while (token) {
			switch (phase) {
			case 0:						// データ先頭の抽出
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
			case 1:						// 頂点数取得
				mesh->vnum = atoi(token);
				mesh->vertex = (VECTOR3*)malloc(mesh->vnum * sizeof(VECTOR3));
				phase = 2;
				i = j = 0;
				break;
			case 2:						// 頂点座標読み込み
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
			case 3:						// 面数取得
				mesh->fnum = atoi(token);
				mesh->face = (int *)malloc(mesh->fnum * 5 * sizeof(int));
				phase = 4;
				i = j = 0;
				break;
			case 4:						// 面情報読み込み
				mesh->face[i * 5 + j] = atoi(token);	// 頂点数/インデックスを数値化
				j++;									// 読込数加算
				if (j > mesh->face[i * 5]) {			// 読込数が頂点数を超えた？
					n = mesh->face[i * 5];
					t = mesh->face[i * 5 + 1];			// 頂点の並びを反転
					mesh->face[i * 5 + 1] = mesh->face[i * 5 + n - 1];
					mesh->face[i * 5 + n - 1] = t;
					j = 0;								// 読込数をクリア
					i++;								// 読込面数加算
					if (i >= mesh->fnum)				// 読込面数が面数を超えた？
						phase = 0;						// データ先頭の抽出へ
				}
				break;
			case 5:						// 法線ベクトル数取得
				mesh->nnum = atoi(token);
				mesh->normal = (VECTOR3*)malloc(mesh->nnum * sizeof(VECTOR3));
				phase = 6;
				i = j = 0;
				break;
			case 6:						// 法線ベクトル読み込み
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
			case 7:						// 法線インデックス数取得
				mesh->inum = atoi(token);
				mesh->index = (int *)malloc(mesh->inum * 5 * sizeof(int));
				phase = 8;
				i = j = 0;
				break;
			case 8:						// 法線インデックス読込
				mesh->index[i * 5 + j] = atoi(token);	// 頂点数/インデックスを数値化
				j++;									// 読込数加算
				if (j > mesh->index[i * 5]) {			// 読込数が頂点数を超えた？
					n = mesh->index[i * 5];
					t = mesh->index[i * 5 + 1];			// 法線の並びを反転
					mesh->index[i * 5 + 1] = mesh->index[i * 5 + n - 1];
					mesh->index[i * 5 + n - 1] = t;
					j = 0;								// 読込数をクリア
					i++;								// 読込インデックス数加算
					if (i >= mesh->inum)				// 読込インデックス数がインデックス数を超えた？
						phase = 0;						// データ先頭の抽出へ
				}
				break;
			case 9:						// UV座標数取得(頂点数と同じはず)
				i = atoi(token);
				mesh->uv = (VECTOR2*)malloc(mesh->vnum * sizeof(VECTOR2));
				phase = 10;
				i = j = 0;
				break;
			case 10:					// UV座標読み込み
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
			case 11:					// 材質数取得
				mesh->mnum = atoi(token);
				mesh->material = (MATERIAL*)malloc(mesh->mnum * sizeof(MATERIAL));
				memset(mesh->material, 0, mesh->mnum * sizeof(MATERIAL));
				phase = 12;
				break;
			case 12:					// 材質インデックス数取得(面数と同じはず)
				i = atoi(token);
				mesh->midx = (int *)malloc(mesh->fnum * sizeof(int));
				i = 0;
				phase = 13;
				break;
			case 13:					// 材質インデックス読み込み
				mesh->midx[i] = (int)atoi(token);
				i++;
				if (i >= mesh->fnum) {
					i = 0;
					phase = 14;
				}
				break;
			case 14:					// 材質情報読み込み
				if (strcmp(token, "Material") == 0) {
					j = 0;
					phase = 15;
				} else {
					phase = 0;
					continue;
				}
				break;
			case 15:					// 拡散光/環境光
				mesh->material[i].diffuse.f[j] = (float)atof(token);
				j++;
				if (j >= 4)
					phase = 16;
				break;
			case 16:					// 鏡面光の指数
				mesh->material[i].power = (float)atof(token);
				j = 0;
				phase = 17;
				break;
			case 17:					// 鏡面光
				mesh->material[i].specular.f[j] = (float)atof(token);
				j++;
				if (j >= 3) {
					mesh->material[i].specular.a = 1.0f;
					j = 0;
					phase = 18;
				}
				break;
			case 18:					// 放射輝度
				mesh->material[i].emissive.f[j] = (float)atof(token);
				j++;
				if (j >= 3) {
					mesh->material[i].emissive.a = 1.0f;
					j = 0;
					phase = 19;
				}
				break;
			case 19:					// テクスチャ
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
			case 20:					// テクスチャ
				if (szDir[0]) {		// カレントディレクトリを変更
					GetCurrentDirectoryA(_MAX_PATH, szCurrentDir);
					SetCurrentDirectoryA(szDir);
				}
				mesh->material[i].textureID = LoadTexture(trim(token));
				if (szDir[0])		// カレントディレクトリを元に戻す
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
	fclose(fp);							// Xファイルクローズ

	return 1;							// 正常終了
}

//---------------------------------------------------------------------------------------
// メッシュ解放
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
// メッシュ描画
//---------------------------------------------------------------------------------------
void DrawMesh(MATRIX *matWorld, MESH *mesh)
{
	int			i, j;
	int			nVtx, nMat;
	MATERIAL	*pMat;

	// ワールドマトリックス設定
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matWorld->f);

	// 描画
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

	// αブレンド有効
	glEnable(GL_BLEND);
	// デプスバッファへの書き込みを禁止
	glDepthMask(GL_FALSE);
	// ブレンディングのための方程式を設定
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

	// デプスバッファへの書き込みを許可
	glDepthMask(GL_TRUE);
	// αブレンド無効
	glDisable(GL_BLEND);

	// ビューマトリックス復帰
	glPopMatrix();
}

//---------------------------------------------------------------------------------------
// モデル表示(α部のみ)
//---------------------------------------------------------------------------------------
void DrawMeshAlpha(MATRIX *matWorld, MESH *mesh)
{
	int			i, j;
	int			nVtx, nMat;
	MATERIAL	*pMat;

	// ワールドマトリックス設定
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matWorld->f);

	// αブレンド有効
	glEnable(GL_BLEND);
	// デプスバッファへの書き込みを禁止
	glDepthMask(GL_FALSE);
	// ブレンディングのための方程式を設定
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

	// デプスバッファへの書き込みを許可
	glDepthMask(GL_TRUE);
	// αブレンド無効
	glDisable(GL_BLEND);

	// ビューマトリックス復帰
	glPopMatrix();
}

//---------------------------------------------------------------------------------------
// モデル表示(非α部のみ)
//---------------------------------------------------------------------------------------
void DrawMeshNoAlpha(MATRIX *matWorld, MESH *mesh)
{
	int			i, j;
	int			nVtx, nMat;
	MATERIAL	*pMat;

	// ワールドマトリックス設定
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matWorld->f);

	// 描画
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

	// ビューマトリックス復帰
	glPopMatrix();
}

//-------------------------------------------------------------------------------------------------
// 境界ボックス取得
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
// 境界球取得
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