#include "Skydome.h"
#include <cassert>


void Skydome::Initialize(Model* model, uint32_t Skydome) {
	//NULLポインタチェック
	assert(model);
	model_ = model;	
	Skydome_ = Skydome;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	//行列の転送
	worldTransform_.TransferMatrix();
}

void Skydome::Update(){

}

void Skydome::Draw(ViewProjection viewProjection_){
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
}
