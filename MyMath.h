#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include <assert.h>


// プロトタイプ宣言 ( int num;
Matrix4x4 Initialize();
//足し算
Vector3 Add(Vector3 vector1, Vector3 vector2);
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
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Vector3 Subtract(Vector3 vector1, Vector3 vector2);
Vector3 VectorMultiply(Vector3 vector1, Vector3 vector2);
Vector3 Normalize(const Vector3& v);
Vector3 Velocity(Vector3 vector1, float kBulletSpeed);
Vector3 Multiply(const float& v1, const Vector3& v2);
Vector3 Multiply(Vector3 vector, Matrix4x4 matrix);
Matrix4x4 MakeViewPortMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Matrix4x4 Inverse(const Matrix4x4& m);
float Dot(Vector3 vector1, Vector3 vector2);
float Length(Vector3 v);
//void affin(WorldTransform& affin);