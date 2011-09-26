/*
** L-System による木の生成
*/
#ifndef TREE_H
#define TREE_H

#include "Matrix.h"

class Tree {
  double rotate;                  // 軸回転 (+/-) の角度のステップ
  double bend;                    // 折り曲げ (>/<) 角度のステップ
  double radius;                  // 木の根元の半径
  double top[4];                  // 基準の枝の先端の位置（木が伸びる方向）
  static const double base[4];    // 基準の枝の根元の位置（原点）
  double (*spine)[3];             // 骨格の頂点位置
  int nspine;                     // 骨格の頂点数
  int *branch;                    // 分岐位置の頂点番号
  int nbranch;                    // 分岐の数
  int flag;                       // 実際に木を生成するなら 0
  double (*cs)[2];                // 断面の頂点位置
  int ncs;                        // 断面の頂点数
  Matrix m;                       // 作業用の変換行列
  void turtle(const char *p);
  void count(const char *p);
  void production(const char *istr, const char * const *rstr, int iter);

public:
  Tree(
    const char *initial,          // 初期文字列
    const char * const *rule,     // 書き換え規則
    int level,                    // 再帰レベル
    const double *direction = 0,  // 木が伸びる方向
    double rstep = 120.0,         // 軸中心の回転の角度ステップ
    double bstep = 30.0,          // 曲げ方向の角度ステップ
    double r = 0.02,              // 木の根元の半径
    int n = 8                     // 木の側面数
    );
  virtual ~Tree();
  void draw();
};

#endif
