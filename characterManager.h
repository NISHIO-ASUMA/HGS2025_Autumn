//=============================================================================
//
// �L�����N�^�[�}�l�[�W���[���� [characterManager.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _CHARACTERMANAGER_H_// ���̃}�N����`������Ă��Ȃ�������
#define _CHARACTERMANAGER_H_// 2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <vector>
#include "object.h"

//*****************************************************************************
// �L�����N�^�[�}�l�[�W���[�N���X
//*****************************************************************************
class CCharacterManager
{
public:
    // �C���X�^���X�̎擾
    static CCharacterManager& GetInstance(void)
    {
        static CCharacterManager instance;
        return instance;
    }

    // �L�����N�^�[�̒ǉ�����
    template <typename characterType, typename... Args>
    characterType* AddCharacter(Args&&... args)
    {
        characterType* character = new characterType(std::forward<Args>(args)...);
        m_Characters.push_back(character);
        return character;
    }

    // �L�����N�^�[�̎擾����
    template <class characterType>
    characterType* GetCharacter(void)
    {
        for (const auto& c : m_Characters)
        {
            if (auto casted = dynamic_cast<characterType*>(c))
            {
                return casted;
            }
        }
        return nullptr;
    }

    // �S�ẴL�����N�^�[�̏���������
    void InitAll(void)
    {
        for (auto& c : m_Characters)
        {
            // ����������
            c->Init();
        }
    }

    void Uninit(void)
    {
        // ���X�g�̃N���A
        m_Characters.clear();
    }

private:
    std::vector<CObject*> m_Characters;
};

#endif

