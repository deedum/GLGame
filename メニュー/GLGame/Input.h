//
// キー/ゲームパッド/マウス入力クラス定義
//
#pragma once

#include <windows.h>
#include <tchar.h>

//-------- キーボード関係
#define MAX_KEY			256			// キー数

//-------- ジョイスティック関係
#define JOY_X			0			// Ｘ軸
#define JOY_Y			1			// Ｙ軸

//-------- マウス関係
#define MOUSE_X			0			// Ｘ軸
#define MOUSE_Y			1			// Ｙ軸
#define MOUSE_Z			2			// Ｚ軸

//-------- ゲームパッド関係
#define MAX_JOY			4			// 最大認識接続数
#define JOY_MIN_X		-32768		// Ｘ軸最小値
#define JOY_MAX_X		32767		// Ｘ軸最大値
#define JOY_MIN_Y		-32768		// Ｙ軸最小値
#define JOY_MAX_Y		32767		// Ｙ軸最大値

typedef struct _MOUSESTATE {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    BYTE    rgbButtons[4];
} MOUSESTATE, *LPMOUSESTATE;

class CInput
{
private:
	static BYTE					m_diKeyState[MAX_KEY];		// キーボード情報取得バッファ
	static BYTE					m_diPrevKeyState[MAX_KEY];	// 直前のキーボード情報
	static MOUSESTATE			m_diMouseState;				// マウス情報取得バッファ
	static MOUSESTATE			m_diPrevMouseState;			// 直前のマウス情報
	static JOYINFOEX 			m_diJoyState[MAX_JOY];		// ジョイスティック情報取得バッファ
	static JOYINFOEX 			m_diPrevJoyState[MAX_JOY];	// 直前のジョイスティック情報
	static int					m_nJoy;						// ジョイスティック接続数
	static HWND					m_hWnd;

public:
	CInput(void);
	virtual ~CInput(void);

	static void Init(HWND hWnd);
	static void Fin(void);
	static void Update(void);

	static bool GetKeyPress(int nKey);
	static bool GetKeyTrigger(int nKey);
	static bool GetKeyRelease(int nKey);
	static int GetNumJoystick(void);
	static bool GetJoyPress(int nJoy, int nButton);
	static bool GetJoyTrigger(int nJoy, int nButton);
	static bool GetJoyRelease(int nJoy, int nButton);
	static float GetJoyAxis(int nJoy, int nAxis);
	static bool GetMousePress(int nButton);
	static bool GetMouseTrigger(int nButton);
	static bool GetMouseRelease(int nButton);
	static long GetMouseAxis(int nAxis);
	static void SetMouseAxis(int nAxis, long lValue);
};