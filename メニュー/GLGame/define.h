//=======================================================================================
//
//	  �萔��`�A�}�N����`
//
//=======================================================================================

#ifndef _H_DEFINE
#define _H_DEFINE

//-------- �}�N��
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)			if(x){ delete x; x=NULL; }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if(x){ delete[] x; x=NULL; }
#endif

//-------- �N���XID
enum EID {
	ID_3DOBJ = 0,	// C3DObj�N���X
	ID_MESHOBJ,		// CMeshObj�N���X
	ID_CAMERA,		// CCamera�N���X
	ID_SKY,			// CSky�N���X
	ID_PLAYER,		// CPlayer�N���X
	ID_ENEMY,		// CEnemy�N���X
	ID_BULLET,		// CBullet�N���X
	ID_MISSILE,		// CMissile�N���X
	ID_LAND,		// CLand�N���X
	ID_JINTAI,		// CJintai�N���X

	MAX_ID
};

#endif

//=======================================================================================
//	End of File
//=======================================================================================