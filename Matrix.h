/*
** ïœä∑çsóÒÇÃëÄçÏ
*/
#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
  double m[16];
  static const double identity[];
  void copy(const double *src, double *dst);

public:
  Matrix() { loadIdentity(); };
  Matrix(const double *array) { load(array); };
  Matrix(const Matrix &matrix) { load(matrix); };
  virtual ~Matrix() {};

  Matrix &operator=(const double *array) { return load(array); };
  Matrix &operator=(const Matrix &matrix) { return load(matrix); };

  Matrix &operator*=(const double *array) { return multiply(array); };
  Matrix &operator*=(const Matrix &matrix) { return multiply(matrix); };
  
  Matrix &load(const double *array) { copy(array, m); return *this; };
  Matrix &load(const Matrix &matrix) { return load(matrix.m); };
  Matrix &loadIdentity() { return load(identity); };
  Matrix &multiply(const double *array);
  Matrix &multiply(const Matrix &matrix) { return multiply(matrix.m); };
  Matrix &translate(double x, double y, double z);
  Matrix &translate(const double *v);
  Matrix &scale(double x, double y, double z);
  Matrix &scale(const double *v);
  Matrix &rotate(double a, double x, double y, double z);
  Matrix &rotate(double a, const double *v) { return rotate(a, v[0], v[1], v[2]); };

  void transform(const double *v1, double *v2) const;
  void projection(const double *v1, double *v2) const;

  const Matrix &push() const;
  Matrix &pop();
  
  const double *get() const { return m; };
};

#endif
