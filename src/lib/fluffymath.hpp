#ifndef FLUFFY_FLUFFYMATH_HPP_D3C5C0B4_20C1_4CBF_9C68_961E59FF5A48
#define FLUFFY_FLUFFYMATH_HPP_D3C5C0B4_20C1_4CBF_9C68_961E59FF5A48

/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <tuple>
namespace fluffy
{

#define HANDMADE_SLOW 1
//------------------------------------------------------------------------------
// NOTE: The assert will write to a null pointer.
#if HANDMADE_SLOW
#include <cstdio>
#define debug(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#define Assert(Condition, ...)                                          \
   if (!(Condition))                                                    \
   {                                                                    \
      fprintf(stderr, "ASSERT. Function %s. Line %d\n", ##__VA_ARGS__); \
      *(volatile int *)0 = 0;                                           \
   }
#else
#define Assert(Condition, ...)
#endif

namespace math3d
{
/**
 * The type to use in the project is defined in order to play with the resolution.
 */
#define USE_render_DOUBLE
#ifdef USE_render_DOUBLE
typedef double FLOAT;
constexpr fluffy::math3d::FLOAT EPSILON = 1e-3;
#else
typedef int fluffy::math3d::FLOAT;
constexpr fluffy::math3d::FLOAT EPSILON = 0;
#endif

auto Rad2Deg(fluffy::math3d::FLOAT Angle) -> fluffy::math3d::FLOAT;
auto Deg2Rad(fluffy::math3d::FLOAT Angle) -> fluffy::math3d::FLOAT;

/**
  union tup   Contains four elements
*
*  Can represent
*
*     1: A 3D point
*
*     2: An RGB value or four fluffy::math3d::FLOATs
*
*     3: X Y Z W with W at 0 when tuple is a vector and 1 when tuple is point
*
*     4: R G B I with intensity at 1 at max and 0 at pitch black
*
*     5: Array C four of fluffy::math3d::FLOATs
*/
union tup
{
   struct  //!< A tuple is initially a vector with four elements or a 3D point.
   {
      fluffy::math3d::FLOAT X;
      fluffy::math3d::FLOAT Y;
      fluffy::math3d::FLOAT Z;
      fluffy::math3d::FLOAT W;  //!< is 1.0 when tuple is point and 0.0 when tuple is a vector.
   };
   struct  //!< A tuple can also represent colors with Intensity
   {
      fluffy::math3d::FLOAT R;
      fluffy::math3d::FLOAT G;
      fluffy::math3d::FLOAT B;
      fluffy::math3d::FLOAT I;  //!< Intensity is 1.0 at max and 0.0 at pitch black.
   };
   struct  //!< A tuple is a vector with four columns.
   {
      fluffy::math3d::FLOAT C[4];
   };
};  // end of union tup.

// NOTE: Use a struct to return multiple values.
struct is_invertible_return
{
   bool IsInvertible;                  // FIXME: (Willy Clarke) Not implemented.
   bool IsComputed;                    // FIXME: (Willy Clarke) Not implemented.
   fluffy::math3d::FLOAT Determinant;  // FIXME: (Willy Clarke) Not implemented.
};

//------------------------------------------------------------------------------
union matrix
{
   matrix() : R0{}, R1{}, R2{}, R3{}, Dimension{4} {}
   matrix(tup const &Cr0, tup const &Cr1, tup const &Cr2, tup const &Cr3)
       : R0{Cr0}, R1{Cr1}, R2{Cr2}, R3{Cr3}, Dimension{4}
   {
   }
   ~matrix() {}
   matrix(matrix const &Other)
   {
      R0 = Other.R0;
      R1 = Other.R1;
      R2 = Other.R2;
      R3 = Other.R3;
      Dimension = Other.Dimension;
      ID = Other.ID;
   }
   struct  //!< A matrix can be four tuple rows
   {
      tup R0{};
      tup R1{};
      tup R2{};
      tup R3{};
      int Dimension{4};
      // NOTE: Storeage of invertible and determinant
      is_invertible_return ID{};
   };
   struct  //!< or it can be an array of 4 tuple rows.
   {
      tup R[4];
   };
};

bool ApproxEq(FLOAT A, FLOAT B, FLOAT Tolerance);

/**
 * Tuple functions.
 */
tup Mul(tup const A, tup const B);
tup Mul(fluffy::math3d::FLOAT const S, tup const &Tup);
tup Negate(tup const &Tup);
tup Normalize(tup const &Tup);
tup Point(fluffy::math3d::FLOAT A, fluffy::math3d::FLOAT B, fluffy::math3d::FLOAT C);
tup Point(tup P);
tup Add(tup const &A, tup const &B);
tup Sub(tup const &A, tup const &B);
tup Sin(tup const &Input);
tup Vector(fluffy::math3d::FLOAT A, fluffy::math3d::FLOAT B, fluffy::math3d::FLOAT C);
tup Vector(tup A);
tup VectorXZY(fluffy::math3d::FLOAT X, fluffy::math3d::FLOAT Y, fluffy::math3d::FLOAT Z);
tup VectorXY(fluffy::math3d::FLOAT X, fluffy::math3d::FLOAT Y);
tup VectorXYZ(tup const &A);
tup VectorYZX(tup const &A);
tup VectorXZY(tup const &A);
tup VectorZXY(tup const &A);
tup VectorXY(tup const &A);
tup VectorXZ(tup const &A);
tup VectorZX(tup const &A);
tup RotateX(tup const &Reference, tup const &Vertice, FLOAT Alfa);
tup RotateY(tup const &Reference, tup const &Vertice, FLOAT Alfa);
tup RotateZ(tup const &Reference, tup const &Vertice, FLOAT Alfa);

/**
 * Matrix functions.
 */
fluffy::math3d::FLOAT Determinant(matrix const &M);
bool Equal(matrix const &A, matrix const &B);
fluffy::math3d::FLOAT Get(matrix const &M, int Row, int Col);

is_invertible_return IsInvertible(matrix const &M);

/// \fn Inverse Calculate the inverse of the matrix M.
///
/// \brief The inverse is not always possible to calculate. When inversion is
///        not possible the Zero matrix will be returned.
/// \return Inverse when possible, Zero matrix otherwise.
matrix Inverse(matrix const &M);

/// ---
/// \fn Identity matrix
/// \return Returs a 4x4 identity matrix.
/// ---
matrix I();
matrix Mul(matrix const &A, matrix const &B);
tup Mul(matrix const &A, tup const &T);
void Set(matrix &M, int Row, int Col, fluffy::math3d::FLOAT Value);
matrix Transpose(matrix const &M);
matrix RotateX(fluffy::math3d::FLOAT Alfa);
matrix RotateY(fluffy::math3d::FLOAT Alfa);
matrix RotateZ(fluffy::math3d::FLOAT Alfa);
matrix Scaling(fluffy::math3d::FLOAT X, fluffy::math3d::FLOAT Y, fluffy::math3d::FLOAT Z);
matrix Shearing(fluffy::math3d::FLOAT Xy, fluffy::math3d::FLOAT Xz, fluffy::math3d::FLOAT Yx, fluffy::math3d::FLOAT Yz,
                fluffy::math3d::FLOAT Zx, fluffy::math3d::FLOAT Zy);
matrix Translation(fluffy::math3d::FLOAT X, fluffy::math3d::FLOAT Y, fluffy::math3d::FLOAT Z);
// Combine translation, rotate and scale in one single function.
matrix TranslateScaleRotate(  //!<
    fluffy::math3d::FLOAT TransX, fluffy::math3d::FLOAT TransY,
    fluffy::math3d::FLOAT TransZ,  //!< Translation is in m(?)
    fluffy::math3d::FLOAT ScaleX, fluffy::math3d::FLOAT ScaleY,
    fluffy::math3d::FLOAT ScaleZ,  //!< Scale input is unitless.
    fluffy::math3d::FLOAT AlfaX, fluffy::math3d::FLOAT AlfaY,
    fluffy::math3d::FLOAT AlfaZ  //!< Input rotation in radians.
);

auto SplineInitCatmullRom() -> matrix;
auto MultSpline(fluffy::math3d::FLOAT u, matrix const &M) -> tup;
auto MultSpline(matrix const &M, tup const &P0, tup const &P1, tup const &P2, tup const &P3) -> matrix;

};  // end of namespace math3d
};  // end of namespace fluffy

/**
 * Operator overloads
 */
std::ostream &operator<<(std::ostream &stream, const fluffy::math3d::tup &T);
std::ostream &operator<<(std::ostream &stream, const fluffy::math3d::matrix &M);
fluffy::math3d::matrix operator*(fluffy::math3d::matrix const &A, fluffy::math3d::matrix const &B);
fluffy::math3d::tup operator/(fluffy::math3d::tup const &Tup, fluffy::math3d::FLOAT const S);
fluffy::math3d::tup operator+(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B);
fluffy::math3d::tup operator-(fluffy::math3d::tup const &Tup);
fluffy::math3d::tup operator-(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B);
fluffy::math3d::tup operator*(fluffy::math3d::FLOAT const S, fluffy::math3d::tup const &Tup);
fluffy::math3d::tup operator*(fluffy::math3d::tup const &Tup, fluffy::math3d::FLOAT const S);
fluffy::math3d::tup operator*(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B);
fluffy::math3d::tup operator/(fluffy::math3d::tup const &Tup, fluffy::math3d::FLOAT const S);
fluffy::math3d::tup operator/(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B);
fluffy::math3d::tup operator/(fluffy::math3d::FLOAT const A, fluffy::math3d::tup const &B);
fluffy::math3d::matrix operator*(fluffy::math3d::matrix const &A, fluffy::math3d::matrix const &B);
fluffy::math3d::tup operator*(fluffy::math3d::matrix const &M, fluffy::math3d::tup const &T);
fluffy::math3d::tup operator*(fluffy::math3d::tup const &T, fluffy::math3d::matrix const &M);
bool operator==(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B);
#endif

/**
* The MIT License (MIT)
Copyright © 2023 <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Ref: https://mit-license.org
*/
