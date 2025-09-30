//=============================================================================
//
// �J�������� [camera.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "player.h"

// ���O���std�̎g�p
using namespace std;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	// �l�̃N���A
	m_posV			= VECTOR3_NULL;	// ���_
	m_posVDest		= VECTOR3_NULL;	// �ړI�̎��_
	m_posR			= VECTOR3_NULL;	// �����_
	m_posRDest		= VECTOR3_NULL;	// �ړI�̒����_
	m_vecU			= VECTOR3_NULL;	// ������x�N�g��
	m_mtxProjection = {};			// �v���W�F�N�V�����}�g���b�N�X
	m_mtxView		= {};			// �r���[�}�g���b�N�X
	m_rot			= VECTOR3_NULL;	// ����
	m_Mode			= MODE_EDIT;	// �J�����̃��[�h
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
	// �Ȃ�
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 300.0f, -240.0f);
	m_posR = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);// �Œ�ł���
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	m_Mode = MODE_GAME;									// �J�����̃��[�h

	m_fDistance = sqrtf(
		((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x)) +
		((m_posV.y - m_posR.y) * (m_posV.y - m_posR.y)) +
		((m_posV.z - m_posR.z) * (m_posV.z - m_posR.z)));

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{

}
//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	switch (m_Mode)
	{
	case MODE_EDIT:

		// �G�f�B�^�[�J�����̏���
		EditCamera();
		break;
	case MODE_GAME:

		// �v���C���̃J��������
		PlayCamera();
		break;
	}
}
//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	CRenderer* renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(80.0f),						// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, // �A�X�y�N�g��
		1.0f,										// �߃N���b�v��
		2500.0f);									// ���N���b�v��

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}
//=============================================================================
// �G�f�B�^�[�J�����̏���
//=============================================================================
void CCamera::EditCamera(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// �}�E�X�̎擾
	CInputMouse* pInputMouse = CManager::GetMouse();

	// �}�E�X�J�[�\����\������
	pInputMouse->SetCursorVisibility(true);

	// ���݂̃J�[�\���ʒu���擾
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	// �O�t���[������̃}�E�X�ړ��ʂ��擾
	static POINT prevCursorPos = { cursorPos.x, cursorPos.y };
	float deltaX = (float)(cursorPos.x - prevCursorPos.x);
	float deltaY = (float)(cursorPos.y - prevCursorPos.y);

	// ���݂̃J�[�\���ʒu��ۑ��i���̃t���[���ł̔�r�p�j
	prevCursorPos = cursorPos;

	// �}�E�X���x
	const float mouseSensitivity = 0.004f;

	deltaX *= mouseSensitivity;
	deltaY *= mouseSensitivity;

	//====================================
	// �}�E�X�z�C�[���ŃY�[���C���E�A�E�g
	//====================================
	int wheel = pInputMouse->GetWheel();
	const float zoomSpeed = 15.0f;

	if (wheel != 0)
	{
		m_fDistance -= wheel * zoomSpeed;

		// �J������������
		if (m_fDistance < 100.0f)
		{
			m_fDistance = 100.0f;
		}
		if (m_fDistance > 800.0f)
		{
			m_fDistance = 800.0f;
		}
	}

	if (pInputKeyboard->GetPress(DIK_LALT) && pInputMouse->GetPress(0)) // ���N���b�N�����Ȃ���}�E�X�ړ� �� ���_��]
	{
		m_rot.y += deltaX; // ������]
		m_rot.x += deltaY; // ������]

		//�p�x�̐��K��
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		else if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		// ������]�̐���
		if (m_rot.x > 1.57f)
		{
			m_rot.x = 1.57f;
		}

		if (m_rot.x < -1.57f)
		{
			m_rot.x = -1.57f;
		}

		// ���_�̍X�V�i�J�����̕�����K�p�j
		m_posV.x = m_posR.x + sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;

	}
	else if (pInputMouse->GetPress(1)) // �E�N���b�N�����Ȃ���}�E�X�ړ� �� �����_��]
	{
		m_rot.y += deltaX; // ������]
		m_rot.x += deltaY; // ������]

		//�p�x�̐��K��
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		else if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		// ������]�̐���
		if (m_rot.x > 1.57f)
		{
			m_rot.x = 1.57f;
		}
		if (m_rot.x < -1.57f)
		{
			m_rot.x = -1.57f;
		}

		// �����_�̍X�V
		m_posR.x = m_posV.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
		m_posR.y = m_posV.y - sinf(m_rot.x) * m_fDistance;
		m_posR.z = m_posV.z - cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
	}
	else
	{
		// ���͂��Ȃ��ꍇ�ł��Y�[�����f�̂��߂Ɏ��_���X�V
		m_posV.x = m_posR.x + sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
		m_posV.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
	}

	// �����_�̍X�V
	m_posR.x = m_posV.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
	m_posR.y = m_posV.y - sinf(m_rot.x) * m_fDistance;
	m_posR.z = m_posV.z - cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
}
//=============================================================================
// �v���C���̃J�����̏���
//=============================================================================
void CCamera::PlayCamera(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// �}�E�X�̎擾
	CInputMouse* pInputMouse = CManager::GetMouse();

	// �}�E�X�J�[�\����\������
	pInputMouse->SetCursorVisibility(true);

	// ���݂̃J�[�\���ʒu���擾
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	// �O�t���[������̃}�E�X�ړ��ʂ��擾
	static POINT prevCursorPos = { cursorPos.x, cursorPos.y };
	float deltaX = (float)(cursorPos.x - prevCursorPos.x);
	float deltaY = (float)(cursorPos.y - prevCursorPos.y);

	// ���݂̃J�[�\���ʒu��ۑ��i���̃t���[���ł̔�r�p�j
	prevCursorPos = cursorPos;

	// �}�E�X���x
	const float mouseSensitivity = 0.004f;

	deltaX *= mouseSensitivity;
	deltaY *= mouseSensitivity;

	//====================================
	// �}�E�X�z�C�[���ŃY�[���C���E�A�E�g
	//====================================
	int wheel = pInputMouse->GetWheel();
	const float zoomSpeed = 15.0f;

	if (wheel != 0)
	{
		m_fDistance -= wheel * zoomSpeed;

		// �J������������
		if (m_fDistance < 100.0f)
		{
			m_fDistance = 100.0f;
		}
		if (m_fDistance > 800.0f)
		{
			m_fDistance = 800.0f;
		}
	}


	// �v���C���[���擾
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	if (pPlayer != nullptr)
	{// NULL�`�F�b�N
		D3DXVECTOR3 pos = pPlayer->GetPos();	// �v���C���[�̈ʒu�擾
		D3DXVECTOR3 rot = pPlayer->GetRot();	// �v���C���[�̌����擾

		//------------------------
		// �Ǐ]
		//------------------------
		// �ڕW
		// �����_
		m_posRDest.x = pos.x + sinf(rot.x) * (pos.x - m_posR.x);
		m_posRDest.z = pos.z + cosf(rot.z) * (pos.z - m_posR.z);
		// ���_
		m_posVDest.x = pos.x + sinf(m_rot.y - D3DX_PI) * m_fDistance;
		m_posVDest.z = pos.z + cosf(m_rot.y - D3DX_PI) * m_fDistance;

		// ���Z
		// �����_
		m_posR.x += (m_posRDest.x - m_posR.x) * 0.08f;
		m_posR.z += (m_posRDest.z - m_posR.z) * 0.08f;
		// ���_
		m_posV.x += (m_posVDest.x - m_posV.x) * 0.08f;
		m_posV.z += (m_posVDest.z - m_posV.z) * 0.08f;
	}

	//�p�x�̐��K��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// ���_���X�V
	m_posV.x = m_posR.x + sinf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.y) * cosf(m_rot.x) * m_fDistance;
}
