//==============================
//
// 敵[enemy.h]
// Author:kaiti
//
//==============================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

#include "main.h"
#include "object.h"
#include "model.h"

#define MAX_DIS (300.0f)
#define GIMMICK_SPEED (8.0f)

//敵クラス
class CGimmick :public CObject
{
public:
	//種類
	typedef enum
	{
		TYPE_CAR = 0,
		TYPE_POLE,

		TYPE_MAX
	}TYPE;

	//メンバ関数
	CGimmick(int nPriority = 3);
	~CGimmick();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGimmick* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type);

	void SetType(TYPE type) { m_type = type; };
	TYPE GetType(void) { return m_type; }

	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	static TYPE SetType(const char* pFileName);
private:
	void Move(void);

	//メンバ変数
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_posOld;				//前回の位置
	D3DXVECTOR3 m_posOffSet;					//位置(オフセット)
	D3DXVECTOR3 m_rot;					//向き
	D3DXVECTOR3 m_move;					//移動量
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_rotDest;				//目標の向き
	D3DXVECTOR3 m_size;					//サイズ

	CModel* m_pModel;	//モデルへのポインタ

	TYPE m_type;			//種類
	bool m_bUse;			//使用しているか

	float m_fMoveDis;
	float m_fMaxDis;
};

#endif