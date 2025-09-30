//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _PLAYER_H_// このマクロ定義がされていなかったら
#define _PLAYER_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "model.h"
#include "motion.h"
#include "block.h"
#include "shadowS.h"
#include "state.h"
#include "manager.h"
#include "gaugePlayer.h"
#include "bullet.h"

// 前方宣言
class CPlayer_StandState;
class CPlayer_MoveState;
class CPlayer_JumpState;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTS		(32)					// 最大パーツ数
#define PLAYER_SPEED	(5.0f)					// 移動スピード
#define MAX_JUMP_POWER	(330.3f)				// ジャンプ初速
#define CAPSULE_RADIUS (18.5f)					// カプセルコライダーの半径
#define CAPSULE_HEIGHT (55z.5f)					// カプセルコライダーの高さ
#define PLAYER_LIFE (10)

// 入力データ構造体
struct InputData
{
	D3DXVECTOR3 moveDir;      // 移動ベクトル
	bool jump;                // ジャンプ入力
};

//*****************************************************************************
// プレイヤークラス
//*****************************************************************************
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = 2);
	~CPlayer();

	// プレイヤーモーションの種類
	typedef enum
	{
		NEUTRAL = 0,		// 待機
		MOVE,				// 移動
		JUMP,				// ジャンプ
		MAX
	}PLAYER_MOTION;

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//*****************************************************************************
	// setter関数
	//*****************************************************************************
	void SetMotion(int type, int nBlendFrame);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetLife(int nLife) { m_nLife = nLife; }

	//*****************************************************************************
	// getter関数
	//*****************************************************************************
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetMove(void) const { return m_move; }
	CCapsuleCollider* GetCollider(void) { return m_pCollider; }
	InputData GatherInput(void);
	int GetLife(void) { return m_nLife; }

private:
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_colliderPos;			// カプセル中心
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_rotDest;				// 向き
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_targetMove;			// 目標速度
	D3DXVECTOR3 m_currentMove;			// 実際の移動速度
	D3DXVECTOR3 m_size;					// サイズ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	CModel* m_apModel[MAX_PARTS];		// モデル(パーツ)へのポインタ
	CShadowS* m_pShadowS;				// ステンシルシャドウへのポインタ
	CMotion* m_pMotion;					// モーションへのポインタ
	int m_currentMotion;				// 現在のモーション
	int m_nNumModel;					// モデル(パーツ)の総数
	int m_nLife;						// ライフ
	CCapsuleCollider* m_pCollider;		// カプセルコライダー
	CHpGauge* m_pHpGauge;				// ＨＰゲージへのポインタ

	// ステートを管理するクラスのインスタンス
	StateMachine<CPlayer> m_stateMachine;
};


//*****************************************************************************
// プレイヤーの待機状態
//*****************************************************************************
class CPlayer_StandState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* pPlayer)override
	{
		// 待機モーション
		pPlayer->SetMotion(CPlayer::NEUTRAL, 10);
	}

	void OnUpdate(CPlayer* pPlayer)override
	{
		// 入力を取得
		InputData input = pPlayer->GatherInput();

		CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
		CInputJoypad* pJoypad = CManager::GetJoyPad();				// ジョイパッドの取得

		// ---------------------------
		// 弾発射
		// ---------------------------
		if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTriggerR2())
		{
			CBullet::Create(pPlayer->GetPos(), pPlayer->GetRot(), CBullet::USER_PLAYER);
		}
		if (pKeyboard->GetTrigger(DIK_1) || pJoypad->GetTriggerR2())
		{
			int nLife = pPlayer->GetLife();
			nLife--;
			pPlayer->SetLife(nLife);
		}

		// 移動入力がある場合
		if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
		{
			// 移動ステートに移行
			m_pMachine->ChangeState<CPlayer_MoveState>();
		}
	}

	void OnExit(CPlayer* /*pPlayer*/)override
	{

	}

private:

};

//*****************************************************************************
// プレイヤーの移動状態
//*****************************************************************************
class CPlayer_MoveState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* pPlayer)override
	{
		// 移動モーション
		pPlayer->SetMotion(CPlayer::MOVE, 10);
	}

	void OnUpdate(CPlayer* pPlayer)override
	{
		// 入力を取得
		InputData input = pPlayer->GatherInput();

		CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
		CInputJoypad* pJoypad = CManager::GetJoyPad();				// ジョイパッドの取得

		// ---------------------------
		// 弾発射
		// ---------------------------
		if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTriggerR2())
		{
			CBullet::Create(pPlayer->GetPos(), pPlayer->GetRot(), CBullet::USER_PLAYER);
		}

#ifdef _DEBUG
		// HPを減らす
		if (pKeyboard->GetTrigger(DIK_1))
		{
			int nLife = pPlayer->GetLife();
			nLife--;
			pPlayer->SetLife(nLife);
		}
#endif

		pPlayer->SetMove(input.moveDir * PLAYER_SPEED);

		if (input.moveDir.x == 0.0f || input.moveDir.z == 0.0f)
		{
			// 移動入力が無ければ待機ステートに戻す
			m_pMachine->ChangeState<CPlayer_StandState>();
		}
	}

	void OnExit(CPlayer* /*pPlayer*/)override
	{

	}

private:

};

//*****************************************************************************
// プレイヤーのジャンプ状態
//*****************************************************************************
class CPlayer_JumpState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* pPlayer)override
	{
		// ジャンプモーション
		pPlayer->SetMotion(CPlayer::JUMP, 10);
	}

	void OnUpdate(CPlayer* pPlayer)override
	{
		// 入力を取得
		InputData input = pPlayer->GatherInput();


	}

	void OnExit(CPlayer* /*pPlayer*/)override
	{

	}

private:

};

#endif
