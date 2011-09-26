/*
** 変換行列の操作
*/
#include <cmath>
#include <stack>
#include "Matrix.h"

/*
** スタック
*/
static std::stack<Matrix> stack;

/*
** 単位行列
*/
const double Matrix::identity[] = {
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0,
};

/*
** 配列 src を配列 dst にコピーする
*/
void Matrix::copy(const double *src, double *dst)
{
  for (int i = 0; i < 16; ++i) dst[i] = src[i];
}

/*
** 行列に配列 array をかける
*/
Matrix &Matrix::multiply(const double *array)
{
  double t[16];
  
  for (int i = 0; i < 16; i += 4) transform(array + i, t + i);
  
  copy(t, m);
  
  return *this;
}

/*
** (x, y, z) に平行移動する
*/
Matrix &Matrix::translate(double x, double y, double z)
{
  double t[16];
  
  copy(identity, t);
  
  t[12] = x;
  t[13] = y;
  t[14] = z;

  return multiply(t);
}

/*
** 位置ベクトル v に平行移動する
*/
Matrix &Matrix::translate(const double *v)
{
  if (v[3] != 0.0)
    return translate(v[0] / v[3], v[1] / v[3], v[2] / v[3]);
  else
    return translate(v[0], v[1], v[2]);
}

/*
** 図形を原点中心に (x, y, z) 倍する
*/
Matrix &Matrix::scale(double x, double y, double z)
{
  double t[16];
  
  copy(identity, t);

  t[ 0] = x;
  t[ 5] = y;
  t[10] = z;
  
  return multiply(t);
}

/*
** 図形を原点中心にベクトル v 倍する
*/
Matrix &Matrix::scale(const double *v)
{
  if (v[3] != 0.0)
    return scale(v[0] / v[3], v[1] / v[3], v[2] / v[3]);
  else
    return scale(v[0], v[1], v[2]);
}

/*
** (x, y, z) を中心に a 度回転する（任意軸中心の回転）
*/
Matrix &Matrix::rotate(double a, double x, double y, double z)
{
  double d = x * x + y * y + z * z;

  if (d > 0.0) {
    d = sqrt(d);
    
    double l  = x / d, m  = y / d, n  = z / d;
    double l2 = l * l, m2 = m * m, n2 = n * n;
    double lm = l * m, mn = m * n, nl = n * l;
    
    double s  = sin(a);
    double c  = cos(a);
    double c1 = 1.0 - c;
    
    double t[] = {
      (1.0 - l2) * c + l2, lm * c1 + n * s, nl * c1 - m * s, 0.0,
      lm * c1 - n * s, (1.0 - m2) * c + m2, mn * c1 + l * s, 0.0,
      nl * c1 + m * s, mn * c1 - l * s, (1.0 - n2) * c + n2, 0.0,
      0.0, 0.0, 0.0, 1.0,
    };

    return multiply(t);
  }
  else
    return *this;
}

/*
** 座標変換（ベクトル v1 に行列をかけた結果をベクトル v2 に格納する）
*/
void Matrix::transform(const double *v1, double *v2) const
{
  v2[0] = v1[0] * m[ 0] + v1[1] * m[ 4] + v1[2] * m[ 8] + v1[3] * m[12];
  v2[1] = v1[0] * m[ 1] + v1[1] * m[ 5] + v1[2] * m[ 9] + v1[3] * m[13];
  v2[2] = v1[0] * m[ 2] + v1[1] * m[ 6] + v1[2] * m[10] + v1[3] * m[14];
  v2[3] = v1[0] * m[ 3] + v1[1] * m[ 7] + v1[2] * m[11] + v1[3] * m[15];
}

/*
** 投影（ベクトル v1 に行列をかけた結果を第四要素で割って v2 に格納する）
*/
void Matrix::projection(const double *v1, double *v2) const
{
  double t[4];
  
  transform(v1, t);
  
  if (t[3] != 0.0) {
    v2[0] = t[0] / t[3];
    v2[1] = t[1] / t[3];
    v2[2] = t[2] / t[3];
  }
  else {
    v2[0] = t[0];
    v2[1] = t[1];
    v2[2] = t[2];
  }
}

/*
** 行列を保存する
*/
const Matrix &Matrix::push() const
{
  stack.push(*this);

  return *this;
}

/*
** 保存した変換行列を取り出す
*/
Matrix &Matrix::pop()
{
  this->load(stack.top());
  stack.pop();

  return *this;
}
