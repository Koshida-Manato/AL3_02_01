#pragma once
#include "Model.h"
/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {
public:
	///<summary>
	///初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	///<summary>
	///更新
	///</summary>
	void Update();

	///<summary>
	///描画
	///</summary>
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t PlayerBullet_ = 0u;
};
