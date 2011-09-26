/*
** L-System による木の生成
*/
#include <cmath>
#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  undef GL_GLEXT_PROTOTYPES
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include "extrusion.h"
#include "Matrix.h"
#include "Tree.h"

/*
** 円周率 M_PI は Visual Studio の cmath では定義されていない
*/
#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

/*
** 基準の枝の根元の位置（原点）
*/
const double Tree::base[] = { 0.0, 0.0, 0.0, 1.0 };

/*
** タートルの処理
*/
void Tree::turtle(const char *p)
{
  for (; *p != '\0'; ++p) {
 
    switch (*p){

    case 'F': // 前進
      m.translate(top);
      m.projection(base, spine[nspine++]);
      break;
      
    case '+': // 軸右回転
      m.rotate( rotate, top);
      break;
    case '-': // 軸左回転
      m.rotate(-rotate, top);
      break;
      
    case '>': // 右分岐
      m.rotate( bend, 0.0, 0.0, 1.0);
      break;
    case '<': // 左分岐
      m.rotate(-bend, 0.0, 0.0, 1.0);
      break;
      
    case '[': // 現在位置保存
      m.push();
      break;
    case ']': // 保存位置復帰
      m.pop();
      m.projection(base, spine[branch[nbranch++] = nspine++]);
      break;
      
    default:
      break;
    }
  }
}

/*
** 節点数のカウント
*/
void Tree::count(const char *p)
{
  for (; *p != '\0'; ++p) {
    switch (*p){
    case ']': // 保存位置復帰
      ++nbranch;
    case 'F': // 前進
      ++nspine;
    default:
      break;
    }
  }
}

/*
** 生成文法の処理
*/
void Tree::production(const char *istr, const char * const *rstr, int iter)
{
  if (--iter >= 0) {
    for (const char *p = istr; *p; ++p) {   // 初期文字列から１文字取り出す
      const char * const *q;

      for (q = rstr; *q; ++q) {             // 規則をひとつ取り出す
        if (*p == **q) {                    // 初期文字列の文字と規則の１文字目を比較
          production(*q + 2, rstr, iter);   // 一致したら初期文字列をその規則で書き換える
          break;
        }
      }
      
      if (*q == 0) {                        // 一致する規則が見つからなかったら
        const char r[2] = { *p, 0 };        // 初期文字列の文字の処理を実行する

        if (flag)
          count(r);
        else
          turtle(r);
      }
    }
  }
  else {
    if (flag)
      count(istr);
    else
      turtle(istr);
  }
}

/*
** コンストラクタ（木の生成）
*/
Tree::Tree(
           const char *initial,       // 初期文字列
           const char * const *rule,  // 書き換え規則
           int level,                 // 再帰レベル
           const double *direction,   // 木が伸びる方向
           double rstep,              // 軸中心の回転の角度ステップ
           double bstep,              // 曲げ方向の角度ステップ
           double r,                  // 木の根元の半径
           int n                      // 木の側面数
           )
{
  // ポインタの初期化
  spine = 0;
  branch = 0;
  cs = 0;

  // 木が伸びる方向
  if (direction != 0) {
    top[0] = direction[0];
    top[1] = direction[1];
    top[2] = direction[2];
  }
  else {
    top[0] = 0.0;
    top[1] = 1.0;
    top[2] = 0.0;
  }
  top[3] = 1.0;

  // 軸中心の回転の角度ステップ
  rotate = rstep * M_PI / 180.0;

  // 曲げ方向の角度ステップ
  bend = bstep * M_PI / 180.0;

  // 根元の半径
  radius = r;

  // 断面形状を生成する
  ncs = (n <= EXTRUSION_CS_LIMIT) ? n : EXTRUSION_CS_LIMIT;
  cs = new double[ncs][2];
  for (int i = 0; i < ncs; ++i) {
    double t = 2.0 * M_PI * (double)i / (double)ncs;
    
    cs[i][0] = radius * cos(t);
    cs[i][1] = radius * sin(t);
  }
  
  // 必要なメモリを確保する
  flag = nspine = nbranch = 1;
  production(initial, rule, level);
  spine = new double[nspine][3];
  branch = new int[nbranch];

  // 最初の節点に木の根元の位置を設定する
  flag = nspine = nbranch = 0;
  spine[nspine][0] = base[0] / base[3];
  spine[nspine][1] = base[1] / base[3];
  spine[nspine][2] = base[2] / base[3];
  nspine++;

  // 木を生成する
  production(initial, rule, level);

  // 最後の分岐に最後の節点番号を登録する
  branch[nbranch++] = nspine;
}

/*
** デストラクタ（メモリ解放）
*/
Tree::~Tree()
{
  delete[] spine;
  spine = 0;
  
  delete[] branch;
  branch = 0;
  
  delete[] cs;
  cs = 0;
}

/*
** 木の描画
*/
void Tree::draw()
{
  for (int i = 0, j = 0; i < nbranch; ++i) {
#if 1
    extrusion(cs, ncs, spine + j, branch[i] - j);
    j = branch[i];
#else
    glBegin(GL_LINE_STRIP);
    while (j < branch[i]) glVertex4dv(spine[j++]);
    glEnd();
#endif
  }
}
