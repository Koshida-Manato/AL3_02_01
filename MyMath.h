#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

// プロトタイプ宣言 ( int num;

/// <summary>行列の掛け算 </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
/// <summary> X軸回転行列 </summary>
Matrix4x4 MakeRotateXMatrix(float radian);
/// <summary>Y軸回転行列 </summary>
Matrix4x4 MakeRotateYMatrix(float radian);
/// <summary>Z軸回転行列 </summary>
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, Vector3& rotate, const Vector3& translate);