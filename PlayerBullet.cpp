#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	assert(model);

	model_ = model;
	//テクスチャ読み込み
	PlayerBullet_ = TextureManager::Load("./Resources/Orb.png");

	worldTransform_.Initialize();
	// X.Y.Z軸方向のスケーリングを設定
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	//座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ =Add (worldTransform_.translation_,velocity_);

	worldTransform_.UpdateMatrix(); 
	//時間経過でデス
	if (--deathTimer_<=0) {
		isDead_ = true;
	}
}

void PlayerBullet::OnCollision() { 
	isDead_ = true; 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, PlayerBullet_);
}

Vector3 PlayerBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}