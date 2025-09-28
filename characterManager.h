//=============================================================================
//
// キャラクターマネージャー処理 [characterManager.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _CHARACTERMANAGER_H_// このマクロ定義がされていなかったら
#define _CHARACTERMANAGER_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <vector>
#include "object.h"

//*****************************************************************************
// キャラクターマネージャークラス
//*****************************************************************************
class CCharacterManager
{
public:
    // インスタンスの取得
    static CCharacterManager& GetInstance(void)
    {
        static CCharacterManager instance;
        return instance;
    }

    // キャラクターの追加処理
    template <typename characterType, typename... Args>
    characterType* AddCharacter(Args&&... args)
    {
        characterType* character = new characterType(std::forward<Args>(args)...);
        m_Characters.push_back(character);
        return character;
    }

    // キャラクターの取得処理
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

    // 全てのキャラクターの初期化処理
    void InitAll(void)
    {
        for (auto& c : m_Characters)
        {
            // 初期化処理
            c->Init();
        }
    }

    void Uninit(void)
    {
        // リストのクリア
        m_Characters.clear();
    }

private:
    std::vector<CObject*> m_Characters;
};

#endif

