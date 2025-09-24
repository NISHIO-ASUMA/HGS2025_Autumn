//=====================================
//
// �e�N�X�`������ [ texture.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TEXTURE_H_ // ���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//*****************************
// �e�N�X�`���Ǘ��N���X���`
//*****************************
class CTexture
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CTexture();
	~CTexture();

	// �����o�֐�
	HRESULT Load(void);
	void UnLoad(void);
	int Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:

	static inline constexpr int NUM_TEXTURE = 128;	// �ő�e�N�X�`����

	// �e�N�X�`���ǂݍ��ݔz����쐬
	const char* TexName[NUM_TEXTURE] =
	{
		"data\\TEXTURE\\number.png",
		"data\\TEXTURE\\shadow000.jpg",
		"data\\TEXTURE\\effect000.jpg",
		"data\\TEXTURE\\pausemenu_continue.png",
		"data\\TEXTURE\\pausemenu_retry.png",
		"data\\TEXTURE\\pausemenu_quit.png",
		"data\\TEXTURE\\PauseBack.png",
		"data\\TEXTURE\\title.jpg",
		"data\\TEXTURE\\tutorial.jpg",
		"data\\TEXTURE\\game.jpg",
		"data\\TEXTURE\\result.jpg",
		"data\\TEXTURE\\ranking.jpg",
	};

	LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXTURE];	// �e�N�X�`���|�C���^
	static int m_nNumAll;							// �e�N�X�`���J�E���g�ϐ�
};

#endif