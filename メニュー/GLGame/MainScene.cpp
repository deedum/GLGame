#include "MainScene.h"


// �R���X�g���N�^
CMainScene::CMainScene(void)
{
	m_pPlayer = NULL;
	m_pCamera = NULL;
	m_pSky = NULL;
	//m_pSound = NULL;
}

// �f�X�g���N�^
CMainScene::~CMainScene(void)
{
}

//---------------------------------------------------------------------------------------
// �f�o�C�X�A���\�[�X������
//---------------------------------------------------------------------------------------
bool CMainScene::Initialize()
{
	if (!CScene::Initialize()) return false;

	//-------- �Q�[���p�I�u�W�F�N�g�̏�����
	m_pPlayer = new CPlayer(this);
	m_pCamera = new CCamera(this);
	m_pSky = new CSky(this);
	//m_pSound = new CSound(this);
	InitObj();				// �S�I�u�W�F�N�g������

	//m_pSound->Play(BGM_MARIO);

	CBillboard::SetCamera(m_pCamera);
	return true;
}

//---------------------------------------------------------------------------------------
// �f�o�C�X�A���\�[�X���
//---------------------------------------------------------------------------------------
void CMainScene::Finalize()
{
	//-------- �Q�[���p�I�u�W�F�N�g�̌�n��
	FinObj();				// �S�I�u�W�F�N�g�j��
	//SAFE_DELETE(m_pSound);
	SAFE_DELETE(m_pSky);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pPlayer);

	CScene::Finalize();
}

//---------------------------------------------------------------------------------------
// �Q�[�����C�������i���C�����[�v����R�[�������j
//---------------------------------------------------------------------------------------
void CMainScene::Update()
{
	CScene::Update();

	//----- �����ɃQ�[���{�̏���
	UpdateObj();				// �S�I�u�W�F�N�g�X�V

	m_pCamera->PostUpdate2();	// �J�����X�V
}

//---------------------------------------------------------------------------------------
// �`�揈���iCGraphics::Render() ����R�[�������j
//---------------------------------------------------------------------------------------
void CMainScene::Draw()
{
	CScene::Draw();

	//----- �����ɕ`�揈��
	m_pCamera->InitDraw();		// �J�����ݒ�
	DrawObj();					// �S�I�u�W�F�N�g�`��

	// �f�o�b�O������`��
	m_pGraph->DrawText(0, 0, m_szDebug);
}