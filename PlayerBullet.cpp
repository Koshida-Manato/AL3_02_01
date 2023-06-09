#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	assert(model);

	model_ = model;
	//テクスチャ読み込み
	PlayerBullet_ = TextureManager::Load("./resources/black.png");
	worldTransform_.Initialize();
	// X.Y.Z軸方向のスケーリングを設定
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, PlayerBullet_);
}
