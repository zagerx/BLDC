/*
 * Copyright (c) 2021 Teslabs Engineering S.L.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>

#undef CLAMP
#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(val, low, high) (((val) <= (low)) ? (low) : MIN(val, high))

/*******************************************************************************
 * Private
 ******************************************************************************/

/** Value sqrt(3). */
#define SQRT_3 1.7320508075688773f

/*******************************************************************************
 * Public
 ******************************************************************************/

void svm_set(float va, float vb, float *dabc) {
  int8_t sector;
  sector = 0;
  if (vb * (1 << 15) > 0) {
    sector = 1;
  }
  if (((SQRT_3 * va - vb) / 2.0F * (1 << 15)) > 0) {
    sector += 2;
  }
  if (((-SQRT_3 * va - vb) / 2.0F) * (1 << 15) > 0) {
    sector += 4;
  }
  float X, Y, Z;
  X = (SQRT_3 * vb);
  Y = (1.5F * va + SQRT_3 / 2.0f * vb);
  Z = (-1.5F * va + SQRT_3 / 2.0f * vb);

  float s_vector = 0.0f, m_vector = 0.0f;
  switch (sector) {
  case 1:
    m_vector = Z;
    s_vector = Y;
    break;

  case 2:
    m_vector = Y;
    s_vector = -X;
    break;

  case 3:
    m_vector = -Z;
    s_vector = X;
    break;

  case 4:
    m_vector = -X;
    s_vector = Z;
    break;

  case 5:
    m_vector = X;
    s_vector = -Y;
    break;

  default:
    m_vector = -Y;
    s_vector = -Z;
    break;
  }
  if (m_vector + s_vector > 1.0f) {
    float sum;
    sum = m_vector + s_vector;
    m_vector = (m_vector / (sum));
    s_vector = (s_vector / (sum));
  }
  float ta, tb, tc;
  ta = (1.0f - (m_vector + s_vector)) / 4.0f;
  tb = ta + m_vector / 2.0f;
  tc = tb + s_vector / 2.0f;
  float t_cmp1 = 0.0f;
  float t_cmp2 = 0.0f;
  float t_cmp3 = 0.0f;
  switch (sector) {
  case 1:
    t_cmp1 = tb;
    t_cmp2 = ta;
    t_cmp3 = tc;
    break;
  case 2:
    t_cmp1 = ta;
    t_cmp2 = tc;
    t_cmp3 = tb;
    break;
  case 3:
    t_cmp1 = ta;
    t_cmp2 = tb;
    t_cmp3 = tc;
    break;
  case 4:
    t_cmp1 = tc;
    t_cmp2 = tb;
    t_cmp3 = ta;
    break;
  case 5:
    t_cmp1 = tc;
    t_cmp2 = ta;
    t_cmp3 = tb;
    break;
  case 6:
    t_cmp1 = tb;
    t_cmp2 = tc;
    t_cmp3 = ta;
    break;
  }
  /*-------------------------占空比---------------------------*/
  float da, db, dc;
  da = (1.0f - t_cmp1 * 2.0f);
  db = (1.0f - t_cmp2 * 2.0f);
  dc = (1.0f - t_cmp3 * 2.0f);
  dabc[0] = CLAMP(da, 0.01f, 0.99f);
  dabc[1] = CLAMP(db, 0.01f, 0.99f);
  dabc[2] = CLAMP(dc, 0.01f, 0.99f);
}
