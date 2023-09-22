#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <assert.h>
#include "ViewProjection.h"
#include "MyMath.h"
#include "Input.h"
class Skydome {
public:
	
	void Initialize(Model* model, uint32_t Skydome);

	void Update();

	void Draw(ViewProjection viewProjection_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t Skydome_ = 0u;
};
