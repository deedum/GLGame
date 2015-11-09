// 3Dオブジェクト基本クラス
#include <windows.h>
#include "C3DObj.h"

//C3DObj*	C3DObj::m_pTop = NULL;

// オブジェクト検索
// UINT uID：クラス種別
// 戻り値：NULL以外：見つかったオブジェクト
//         NULL：該当クラス無し
C3DObj* C3DObj::Find(UINT uID)
{
	return Next(uID, NULL);
}

// 次のオブジェクト検索
// UINT uID：クラス種別
//      pObj：最後に見つかったオブジェクト
// 戻り値：NULL以外：次に見つかったオブジェクト
//         NULL：該当クラス無し
C3DObj* C3DObj::Next(UINT uID, C3DObj* pObj)
{
	// 検索位置先頭
	if (pObj) {
		pObj = pObj->m_pNext;
	} else {
		pObj = m_pScene->GetObj();
	}
	// 検索
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
// 全オブジェクト初期化
void C3DObj::InitAll(void)
{
	// 初期化前処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		pObj->PreInit();
	}
	// 初期化
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		pObj->Init();
	}
	// 初期化後処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		pObj->PostInit();
	}
}

// 全オブジェクト更新
void C3DObj::UpdateAll(void)
{
	C3DObj* pNext;
	// 更新前処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->PreUpdate();
	}
	// 更新
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Update();
	}
	// 更新後処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->PostUpdate();
	}
}

// 全オブジェクト描画
void C3DObj::DrawAll(void)
{
	// 描画前処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->IsVisible())
			pObj->PreDraw();
	}
	// 描画
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->IsVisible())
			pObj->Draw();
	}
	// 描画後処理
	for (C3DObj* pObj = m_pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->IsVisible())
			pObj->PostDraw();
	}
}

// 全オブジェクト破棄
void C3DObj::FinAll(void)
{
	C3DObj* pNext;
	for (C3DObj* pObj = m_pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Fin();
	}
}
*/

// コンストラクタ
C3DObj::C3DObj(CScene* pScene)
{
	m_id = ID_3DOBJ;
	m_pScene = pScene;

	MatrixIdentity(&m_world);
	m_pos.x = m_speed.x = m_accel.x = m_angle.x = 0.0f;
	m_pos.y = m_speed.y = m_accel.y = m_angle.y = 0.0f;
	m_pos.z = m_speed.z = m_accel.z = m_angle.z = 0.0f;

	// リスト先頭に追加
	m_pPrev = NULL;
	m_pNext = m_pScene->GetObj();
	if (m_pNext) {
		m_pNext->m_pPrev = this;
	}
	m_pScene->SetObj(this);

	m_visible = true;
}

// デストラクタ
C3DObj::~C3DObj(void)
{
	// リストから削除
	if (m_pPrev) {
		m_pPrev->m_pNext = m_pNext;
	} else {
		m_pScene->SetObj(m_pNext);
	}
	if (m_pNext) {
		m_pNext->m_pPrev = m_pPrev;
	}
}

// 初期化前処理
void C3DObj::PreInit(void)
{
	// なし
}

// 初期化
void C3DObj::Init(void)
{
	// なし
}

// 初期化後処理
void C3DObj::PostInit(void)
{
	// なし
}

// 更新前処理
void C3DObj::PreUpdate(void)
{
	// なし
}

// 更新
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

// 更新後処理
void C3DObj::PostUpdate(void)
{
	// なし
}

// 描画前処理
void C3DObj::PreDraw(void)
{
	// なし
}

// 描画
void C3DObj::Draw(void)
{
	// なし
}

// 描画後処理
void C3DObj::PostDraw(void)
{
	// なし
}

// 破棄
void C3DObj::Fin(void)
{
	// なし
}

// 位置設定
void C3DObj::SetPos(VECTOR3* pPos)
{
	if (pPos) m_pos = *pPos;
}

// 角度設定
void C3DObj::SetAngle(VECTOR3* pAngle)
{
	if (pAngle) m_angle = *pAngle;
}

// 速度設定
void C3DObj::SetSpeed(VECTOR3* pSpeed)
{
	if (pSpeed) m_speed = *pSpeed;
}

// 加速度設定
void C3DObj::SetAccel(VECTOR3* pAccel)
{
	if (pAccel) m_accel = *pAccel;
}

// 位置取得
VECTOR3& C3DObj::GetPos(void)
{
	return m_pos;
}

// ワールドマトリックス取得
MATRIX& C3DObj::GetWorld(void)
{
	return m_world;
}