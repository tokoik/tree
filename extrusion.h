/*
** 押し出し形状の作成
*/

#define EXTRUSION_CS_LIMIT 100  /* 断面の頂点数の最大値（＝側面数） */

extern void extrusion(const double cs[][2], int nc, const double sp[][3], int ns);
