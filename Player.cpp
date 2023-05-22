#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t Orb) {
	assert(model);

	model_ = model;
	Orb_ = Orb;

	worldTransform_.Initialize();
}

void Player::Update() {

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection& viewProjection) {
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, Orb_);
}