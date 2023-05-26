#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Model.h"

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
	    void Initialize(Model* model, uint32_t Orb);

		///< summary>
		/// 更新
		///</sumary>
		void Update();

		///< summary>
		/// 描画
		///</sumary>
	    void Draw(ViewProjection& viewProjection);

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t Orb_ = 0u;
	    // キーボード入力
	    Input* input_ = nullptr;
};
