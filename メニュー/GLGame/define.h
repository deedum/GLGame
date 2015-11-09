//=======================================================================================
//
//	  定数定義、マクロ定義
//
//=======================================================================================

#ifndef _H_DEFINE
#define _H_DEFINE

//-------- マクロ
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)			if(x){ delete x; x=NULL; }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if(x){ delete[] x; x=NULL; }
#endif

//-------- クラスID
enum EID {
	ID_3DOBJ = 0,	// C3DObjクラス
	ID_MESHOBJ,		// CMeshObjクラス
	ID_CAMERA,		// CCameraクラス
	ID_SKY,			// CSkyクラス
	ID_PLAYER,		// CPlayerクラス
	ID_ENEMY,		// CEnemyクラス
	ID_BULLET,		// CBulletクラス
	ID_MISSILE,		// CMissileクラス
	ID_LAND,		// CLandクラス
	ID_JINTAI,		// CJintaiクラス

	MAX_ID
};

#endif

//=======================================================================================
//	End of File
//=======================================================================================