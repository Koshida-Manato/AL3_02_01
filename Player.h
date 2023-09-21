#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include <list>

///<summary>
///自キャラ
///</summary>
class Player {
	public:
		///<summary>
		///初期化
		///</sumary>
		/// <param name="model">モデル</param>
		/// <param name="textureHandle">テクスチャハンドル</param>
	    void Initialize(Model* model, uint32_t Orb, Vector3 playerPosition);

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Player();

		///< summary>
		/// 更新
		///</sumary>
		void Update();

		///< summary>
		/// 描画
		///</sumary>
	    void Draw(ViewProjection& viewProjection);

		/// <summary>
		/// 攻撃
		/// </summary>
	    void Attack();

		//衝突を検出したら呼び出されるコールバック関数
	    void OnCollision();

		// ワールド座標を取得
	    Vector3 GetWorldPosition();

		//弾リストを取得
	    const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

		/// <summary>
		/// 親となるワールドトランスフォームをセット
		/// </summary>
	    void SetParent(const WorldTransform* parent);

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t Orb_ = 0u;
		//プレイヤーポジション
	    Vector3 playerPosition_;
	    // キーボード入力
	    Input* input_ = nullptr;
		//弾
	    std::list<PlayerBullet*> bullets_;
		
};
