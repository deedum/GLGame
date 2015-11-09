//
// キー/ゲームパッド/マウス入力クラス
//
#include "Input.h"
#include "define.h"

#pragma comment(lib, "winmm")

// 静的メンバ変数
BYTE					CInput::m_diKeyState[MAX_KEY] = {0};// キーボード情報取得バッファ
BYTE					CInput::m_diPrevKeyState[MAX_KEY] = {0};// 直前のキーボード情報
MOUSESTATE				CInput::m_diMouseState = {0};			// マウス情報取得バッファ
MOUSESTATE				CInput::m_diPrevMouseState = {0};		// 直前のマウス情報
JOYINFOEX 				CInput::m_diJoyState[MAX_JOY] = {0};	// ジョイスティック情報取得バッファ
JOYINFOEX 				CInput::m_diPrevJoyState[MAX_JOY] = {0};// 直前のジョイスティック情報
int						CInput::m_nJoy = 0;						// ジョイスティック接続数
HWND					CInput::m_hWnd = NULL;
TCHAR					g_szTitle[] = _T("CInput");

// コンストラクタ
CInput::CInput(void)
{
}

// デストラクタ
CInput::~CInput(void)
{
}

// 初期化
void CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	// ジョイスティック接続数を調べる
	m_nJoy = 0;
	UINT uID = JOYSTICKID1;
	JOYINFO ji;
	while (joyGetPos(uID, &ji) == JOYERR_NOERROR) {
		uID++;
		m_nJoy++;
		if (m_nJoy >= MAX_JOY) {
			break;
		}
	}
}

// 解放
void CInput::Fin(void)
{
	// 無し
}

// 更新
void CInput::Update(void)
{
	int		i;
	USHORT	u;

	CopyMemory(m_diPrevKeyState, m_diKeyState, MAX_KEY);
	// 現在のキーボードの状態を取得
	for (i = 0; i < MAX_KEY; i++) {
		u = (USHORT)GetAsyncKeyState(i);
		m_diKeyState[i] = (u & 0x8000) >> 8;
	}

	m_diPrevMouseState = m_diMouseState;
	// 現在のマウスの状態を取得
	u = (USHORT)GetAsyncKeyState(VK_LBUTTON);
	m_diMouseState.rgbButtons[0] = (u & 0x8000) >> 8;
	u = (USHORT)GetAsyncKeyState(VK_RBUTTON);
	m_diMouseState.rgbButtons[1] = (u & 0x8000) >> 8;
	u = (USHORT)GetAsyncKeyState(VK_MBUTTON);
	m_diMouseState.rgbButtons[2] = (u & 0x8000) >> 8;
	u = (USHORT)GetAsyncKeyState(VK_XBUTTON1);
	m_diMouseState.rgbButtons[3] = (u & 0x8000) >> 8;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	m_diMouseState.lX = pt.x;
	m_diMouseState.lY = pt.y;

	for (i = 0; i < MAX_JOY; i++) {
		m_diPrevJoyState[i] = m_diJoyState[i];
	}
	// 現在のジョイスティックの状態を取得
	for (i = 0; i < MAX_JOY; i++) {
		if (joyGetPosEx(i, &m_diJoyState[i]) == JOYERR_NOERROR) {
			m_nJoy = i;
			break;
		}
	}
}

// キーの押下状態を取得
bool CInput::GetKeyPress(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diKeyState[nKey] & 0x80) != 0;
}

// キーのトリガ状態を取得
bool CInput::GetKeyTrigger(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diPrevKeyState[nKey] & 0x80) == 0 &&
		(m_diKeyState[nKey] & 0x80) != 0;
}

// キーのリリ－ス状態を取得
bool CInput::GetKeyRelease(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diPrevKeyState[nKey] & 0x80) != 0 &&
		(m_diKeyState[nKey] & 0x80) == 0;
}

// ジョイスティック数取得
int CInput::GetNumJoystick(void)
{
	return m_nJoy;
}

// ジョイスティックのボタンの押下状態取得
bool CInput::GetJoyPress(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diJoyState[nJoy].dwButtons & (1 << nButton)) != 0;
}

// ジョイスティックのボタンのトリガ状態取得
bool CInput::GetJoyTrigger(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diPrevJoyState[nJoy].dwButtons & (1 << nButton)) == 0 &&
		(m_diJoyState[nJoy].dwButtons & (1 << nButton)) != 0;
}

// ジョイスティックのボタンのリリース状態取得
bool CInput::GetJoyRelease(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diPrevJoyState[nJoy].dwButtons & (1 << nButton)) != 0 &&
		(m_diJoyState[nJoy].dwButtons & (1 << nButton)) == 0;
}

// ジョイスティックの軸状態取得
float CInput::GetJoyAxis(int nJoy, int nAxis)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return 0.0f;
	switch (nAxis) {
	case JOY_X:
		if (m_diJoyState[nJoy].dwXpos < 32768) {
			return ((int)m_diJoyState[nJoy].dwXpos - 32768) / (float)JOY_MIN_X;
		}
		return (m_diJoyState[nJoy].dwXpos - 32768) / (float)JOY_MAX_X;
	case JOY_Y:
		if (m_diJoyState[nJoy].dwYpos < 32768) {
			return ((int)m_diJoyState[nJoy].dwYpos - 32768) / (float)JOY_MIN_X;
		}
		return (m_diJoyState[nJoy].dwYpos - 32768) / (float)JOY_MAX_X;
	default:
		break;
	}
	return 0.0f;
}

// マウスのボタンの押下状態取得
bool CInput::GetMousePress(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diMouseState.rgbButtons[nButton] & 0x80) != 0;
}

// マウスのボタンのトリガ状態取得
bool CInput::GetMouseTrigger(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diPrevMouseState.rgbButtons[nButton] & 0x80) == 0 &&
		(m_diMouseState.rgbButtons[nButton] & 0x80) != 0;
}

// マウスのボタンのリリース状態取得
bool CInput::GetMouseRelease(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diPrevMouseState.rgbButtons[nButton] & 0x80) != 0 &&
		(m_diMouseState.rgbButtons[nButton] & 0x80) == 0;
}

// マウスの変化量取得
long CInput::GetMouseAxis(int nAxis)
{
	switch (nAxis) {
	case MOUSE_X:
		return m_diMouseState.lX;
	case MOUSE_Y:
		return m_diMouseState.lY;
	case MOUSE_Z:
		return m_diMouseState.lZ;
	default:
		break;
	}
	return 0L;
}

// マウスの変化量更新
void CInput::SetMouseAxis(int nAxis, long lValue)
{
	switch (nAxis) {
	case MOUSE_X:
		m_diMouseState.lX = lValue;
		break;
	case MOUSE_Y:
		m_diMouseState.lY = lValue;
		break;
	case MOUSE_Z:
		m_diMouseState.lZ = lValue;
		break;
	default:
		break;
	}
}
