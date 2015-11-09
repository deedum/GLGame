// 3Dオブジェクト基本クラス
#pragma once

#include <windows.h>
#include "Matrix.h"
#include "define.h"
#include "Scene.h"

class C3DObj
{
protected:
	UINT			m_id;		// クラス種別
	MATRIX			m_world;	// ワールド変換行列
	VECTOR3			m_pos;		// ワールド座標
	VECTOR3			m_angle;	// 角度
	VECTOR3			m_speed;	// 速度
	VECTOR3			m_accel;	// 加速度
	bool			m_visible;	// 表示フラグ

//	static C3DObj*	m_pTop;		// オブジェクト リスト先頭
	CScene*			m_pScene;	// シーン
	C3DObj*			m_pPrev;	// 前のオブジェクト
	C3DObj*			m_pNext;	// 次のオブジェクト

public:
	C3DObj(CScene* pScene);		// コンストラクタ
	virtual ~C3DObj(void);		// デストラクタ

//	static void InitAll(void);	// 全オブジェクト初期化
//	static void UpdateAll(void);// 全オブジェクト更新
//	static void DrawAll(void);	// 全オブジェクト描画
//	static void FinAll(void);	// 全オブジェクト解放
//	static C3DObj* Find(UINT uID);	// 種別による検索
//	static C3DObj* Next(UINT uID, C3DObj* pObj);
	C3DObj* Find(UINT uID);			// 種別による検索
	C3DObj* Next(UINT uID, C3DObj* pObj);

	virtual void PreInit(void);		// 初期化前処理
	virtual void Init(void);			// 初期化
	virtual void PostInit(void);		// 初期化後処理
	virtual void PreUpdate(void);	// 更新前処理
	virtual void Update(void);		// 更新
	virtual void PostUpdate(void);	// 更新後処理
	virtual void PreDraw(void);		// 描画前処理
	virtual void Draw(void);			// 描画
	virtual void PostDraw(void);		// 描画後処理
	virtual void Fin(void);			// 解放

	UINT GetID(void) {return m_id;}	// クラス種別取得
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