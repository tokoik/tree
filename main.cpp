#include <cstdlib>
#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  undef GL_GLEXT_PROTOTYPES
#  include "glut.h"
#elif defined(X11)
#  include <GL/glut.h>
#elif defined(__APPLE__)
#  include <GLUT/glut.h>
#endif

/*
**トラックボール処理
*/
#include "Trackball.h"
static Trackball *tb = 0;               // トラックボールのオブジェクト
static int btn;                         // 押されているマウスボタン

/*
** L-System による木の生成
*/
#include "Tree.h"
static Tree *tree = 0;

/*
** 木の生成文法
*/
static const char initial[] = "X";      // 初期文字列
static const char * const rule[] = {    // 書き換え規則
  //"X:F[+<X]-<X",
  "X:F[+<X]F[++>X][+++<X]FX[++++<X]",
  //"X:F[>X]+[>X]+>X",
  //"X:F[>X]<X",
  //"X:F>X",
  //"X:F+X",
  //"X:F>X[[+>Y]->Y]<X",
  //"Y:F<Y[[-<X]-<X]>Y",
  0
};
static const int level = 6;             // 再帰レベル（深さ）

/*
** 木が伸びる方向
*/
static const double dir[] = { 0.0, 0.2, 0.0, 1.0 };

/*
** 軸回転 (+/-) の角度のステップ
*/
static const double rotate = 120.0;

/*
** 折り曲げ (>/<) 角度のステップ
*/
static const double bend = 30.0;

/*
** 幹の根元の半径
*/
static const double radius = 0.02;

/*
** 木の側面数
*/
static const int side = 8;

/*
** 木の色
*/
static const GLfloat wood[] = { 0.5f, 0.3f, 0.1f, 1.0f };

/*
** 光源の位置（ヘッドライト）
*/
static const GLfloat light[] = { 0.0f, 0.0f, 1.0f, 0.0f };

/*
** 画面表示
*/
static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // トラックボール処理
  glPushMatrix();
  glMultMatrixd(tb->rotation());
  
  // 材質の設定
  glMaterialfv(GL_FRONT, GL_DIFFUSE, wood);

  // 木の描画
  tree->draw();
  
  // モデルビュー変換行列の復帰
  glPopMatrix();
  
  // ダブルバッファリング
  glutSwapBuffers();
}


/*
** ウィンドウのリサイズ
*/
static void resize(int w, int h)
{
  // トラックボール処理の範囲
  tb->region(w, h);
  
  // ビューポートの設定
  glViewport(0, 0, w, h);
  
  // 透視変換行列の設定
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 1.0, 20.0);

  // モデルビュー変換行列の設定
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 5.0, 10.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);

  // 光源の位置の設定（視点位置基準）
  glLightfv(GL_LIGHT0, GL_POSITION, light);
}

/*
** 画面表示の更新
*/
static void idle(void)
{
  glutPostRedisplay();
}

/*
** マウスボタン操作
*/
static void mouse(int button, int state, int x, int y)
{
  // 押されたボタンを記憶しておく
  btn = button;
  
  switch (btn) {
      
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      // トラックボール開始
      tb->start(x, y);
      glutIdleFunc(idle);
    }
    else {
      // トラックボール停止
      tb->stop(x, y);
      glutIdleFunc(0);
    }
    break;
      
  case GLUT_MIDDLE_BUTTON:
    break;
      
  case GLUT_RIGHT_BUTTON:
    break;
      
  default:
    break;
  }
}

/*
** マウスドラッグ操作
*/
static void motion(int x, int y)
{
  switch (btn) {
      
  case GLUT_LEFT_BUTTON:
    // トラックボール移動
    tb->motion(x, y);
    break;
      
  case GLUT_MIDDLE_BUTTON:
    break;

  case GLUT_RIGHT_BUTTON:
    break;

  default:
    break;
  }
}

/*
** キーボード操作
*/
static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
      
  case 'q':
  case 'Q':
  case '\033':
    // 終了
    exit(0);
      
  default:
    break;
  }
}

/*
** 後始末
*/
static void cleanup(void)
{
  delete tb;
  delete tree;
}

/*
** 初期化
*/
static void init(void)
{
  // 背景色
  glClearColor(1.0, 1.0, 1.0, 1.0);
  
  // 隠面消去処理の設定
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  // 光源の設定
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

/*
** メイン
*/
int main(int argc, char *argv[])
{
  // オブジェクト生成
  tb = new Trackball;
  tree = new Tree(initial, rule, level, dir, rotate, bend, radius, side);
  atexit(cleanup);

  // 画面表示の設定
  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();

  return 0;
}
