#include <cmath>
#include <iomanip>  // for std::setprecision
#include <iostream>

/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include "fluffymath.hpp"
namespace fluffy
{
namespace math3d
{
bool ApproxEq(FLOAT A, FLOAT B, FLOAT Tolerance = EPSILON) { return std::abs(A - B) < Tolerance; }

/**
 * Convert to degrees from radians.
 */
auto Rad2Deg(FLOAT Angle) -> FLOAT { return FLOAT(180) * Angle / FLOAT(M_PI); }
auto Deg2Rad(FLOAT Angle) -> FLOAT { return M_PI * Angle / FLOAT(180); }

//------------------------------------------------------------------------------
tup Negate(tup const &Tup)
{
   tup const Result{-Tup.X, -Tup.Y, -Tup.Z, -Tup.W};
   return (Result);
}

//------------------------------------------------------------------------------
tup Mul(FLOAT const S, tup const &Tup)
{
   tup const Result{S * Tup.X, S * Tup.Y, S * Tup.Z, Tup.W};
   return (Result);
}

//------------------------------------------------------------------------------
tup Mul(tup const A, tup const B)
{
   tup const Result{A.R * B.R, A.G * B.G, A.B * B.B, A.W * B.W};
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT MagSquared(tup const &Tup)
{
   FLOAT const Result = Tup.X * Tup.X +  //<!
                        Tup.Y * Tup.Y +  //<!
                        Tup.Z * Tup.Z +  //<!
                        Tup.W * Tup.W;   //<!
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Mag(tup const &Tup) { return (std::sqrt(MagSquared(Tup))); }

//------------------------------------------------------------------------------
tup Normalize(tup const &Tup)
{
   tup const Result = Tup / Mag(Tup);
   return (Result);
}

//------------------------------------------------------------------------------
tup Point(FLOAT A, FLOAT B, FLOAT C)
{
   tup Result{A, B, C, FLOAT(1)};
   return (Result);
}

//------------------------------------------------------------------------------
tup Point(tup P)
{
   P.W = FLOAT(1);
   return P;
}

//------------------------------------------------------------------------------
tup Vector(FLOAT A, FLOAT B, FLOAT C)
{
   tup Result{A, B, C, FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
tup Vector(tup A)
{
   // ---
   // NOTE: Use the variable on the stack and convert the incoming tuple to a vector.
   // --
   A.W = FLOAT(0);
   return A;
}

//------------------------------------------------------------------------------
tup VectorXZY(FLOAT X, FLOAT Y, FLOAT Z)
{
   tup Result{X, Z, Y, FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
tup VectorXY(FLOAT X, FLOAT Y)
{
   tup Result{X, Y, FLOAT(0), FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
tup VectorXYZ(tup const &A)
{
   tup Result{A.X, A.Y, A.Z, FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
tup VectorYZX(tup const &A)
{
   tup Result{A.Y, A.Z, A.X, FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
tup VectorXZY(tup const &A)
{
   tup Result{A.X, A.Z, A.Y, FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
tup VectorZXY(tup const &A)
{
   tup Result{A.Z, A.X, A.Y, FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
/**
 * @return: Vector with Z=0.
 */
tup VectorXY(tup const &A)
{
   tup Result{A.X, A.Y, FLOAT(0), FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
/**
 * @return: Vector where Z->Y and the resulting Z=0.
 */
tup VectorXZ(tup const &A)
{
   tup Result{A.X, FLOAT(0), A.Z, FLOAT(0)};
   return (Result);
}

//------------------------------------------------------------------------------
/**
 * @return: Vector where X<->Z and the resulting Y=0.
 */
tup VectorZX(tup const &A)
{
   tup Result{A.Z, FLOAT(0), A.X, FLOAT(0)};
   return (Result);
}
// ---
// tuple related functions.
// ---

//------------------------------------------------------------------------------
tup Add(tup const &A, tup const &B)
{
   tup const Result{A.X + B.X, A.Y + B.Y, A.Z + B.Z, A.W + B.W};
   return (Result);
}

//------------------------------------------------------------------------------
tup Sub(tup const &A, tup const &B)
{
   /**
    * NOTE: Cater for the use of W in a point beeing used for storage.
    *       So when two points are subtracted a vector with W should be the result.
    */
   if (A.W != FLOAT(0) && B.W != FLOAT(0))
   {
      tup const Result = {A.X - B.X, A.Y - B.Y, A.Z - B.Z, FLOAT(0)};
      return Result;
   }

   tup const Result = {A.X - B.X, A.Y - B.Y, A.Z - B.Z, A.W - B.W};
   return (Result);
}

/**
 * @return: std::sin to the elements X, Y, Z individually. W remains unchanged.
 */
tup Sin(tup const &Input)
{
   return tup{
       std::sin(Input.X),  //!<
       std::sin(Input.Y),  //!<
       std::sin(Input.Z),  //!<
       Input.W             //!<
   };
}
//------------------------------------------------------------------------------
tup Color(FLOAT const R, FLOAT const G, FLOAT const B)
{
   tup Result{R, G, B, 0.f};
   return (Result);
}

//------------------------------------------------------------------------------
tup Cross(tup const &A, tup const &B)
{
   tup const Result = Vector(A.Y * B.Z - A.Z * B.Y, A.Z * B.X - A.X * B.Z, A.X * B.Y - A.Y * B.X);
   return (Result);
}

//------------------------------------------------------------------------------
tup Div(tup const &A, tup const &B)
{
   tup Result{A.X / B.X, A.Y / B.Y, A.Z / B.Z, A.W};
   return Result;
}

//------------------------------------------------------------------------------
FLOAT Dot(tup const &A, tup const &B)
{
   FLOAT const Result = A.X * B.X +  //!<
                        A.Y * B.Y +  //<!
                        A.Z * B.Z +  //<!
                        A.W * B.W;   //<!
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT NDot(tup const &A, tup const &B) { return A.X * B.X - A.Y * B.Y; }

//------------------------------------------------------------------------------
/**
 * @return: The dot product of the vector itself.
 */
FLOAT Dot(tup const &A) { return Dot(A, A); }

//------------------------------------------------------------------------------
bool Equal(FLOAT const A, FLOAT const B)
{
   if (std::abs(A - B) < 1.f * EPSILON)
   {
      return true;
   }
   return false;
}

//------------------------------------------------------------------------------
bool Equal(tup const &A, tup const &B)
{
   bool const Result = Equal(A.X, B.X) &&  //<!
                       Equal(A.Y, B.Y) &&  //<!
                       Equal(A.Z, B.Z) &&  //<!
                       Equal(A.W, B.W);
   return (Result);
}

//------------------------------------------------------------------------------
bool IsPoint(tup const &Tup)
{
   bool Result{};
   Result = (Tup.W != 0);
   return (Result);
}

//------------------------------------------------------------------------------
bool IsVector(tup const &Tup)
{
   bool Result{};
   Result = !(Tup.W != 0);
   return (Result);
}

// ---
// NOTE: Matrix functions.
// ---
matrix Matrix44()
{
   matrix Result{};
   return (Result);
}

//------------------------------------------------------------------------------
matrix Matrix44(tup const &R0, tup const &R1, tup const &R2, tup const &R3)
{
   matrix Result{R0, R1, R2, R3};
   return (Result);
}

//------------------------------------------------------------------------------
// NOTE: Use the data structure for 4x4 matrix for all types of matrixes.
matrix Matrix33(tup const &R0, tup const &R1, tup const &R2)
{
   matrix M{R0, R1, R2, tup{}};
   M.Dimension = 3;
   return (M);
}

//------------------------------------------------------------------------------
// NOTE: Use the data structure for 4x4 matrix for all types of matrixes.
matrix Matrix22(tup const &R0, tup const &R1)
{
   matrix M{R0, R1, tup{}, tup{}};
   M.Dimension = 2;
   return (M);
}

//------------------------------------------------------------------------------
FLOAT Get(matrix const &M, int Row, int Col)
{
   Assert(Row < M.Dimension, __FUNCTION__, __LINE__);
   Assert(Col < 4, __FUNCTION__, __LINE__);
   return M.R[Row].C[Col];
};

void Set(matrix &M, int Row, int Col, FLOAT Value) { M.R[Row].C[Col] = Value; }

//------------------------------------------------------------------------------
matrix SubMatrix(matrix const &M, int RemoveRow, int RemoveCol)
{
   matrix R{};
   int ShiftR{};

   // NOTE: For each row copy source until we get to the removerow
   //       When we get to the remove row the source shifts by one,
   //       so that we skip the RemoveRow.
   for (int Row = 0;            ///<!
        Row < M.Dimension - 1;  ///<!
        ++Row)
   {
      if (Row == RemoveRow) ShiftR++;
      R.R[Row] = M.R[Row + ShiftR];
   }

   // NOTE: Dimension reduces by 1 when we remove one row/column.
   R.Dimension = M.Dimension - 1;

   // NOTE: Copy the columns.
   for (int Row = 0;        ///<!
        Row < R.Dimension;  ///<! The rows are already in order, so we only need to
        ++Row)              ///<! iterate over the result rows.
   {
      for (int Col = 0;        ///<!
           Col < M.Dimension;  ///<!
           ++Col)
      {
         if (Col < RemoveCol) continue;  // No point in copy of data that is already correctly placed.

         if (Col < M.Dimension - 1)
            R.R[Row].C[Col] = R.R[Row].C[Col + 1];
         else
            R.R[Row].C[Col] = 0.f;
      }
   }

   return (R);
}

//------------------------------------------------------------------------------
FLOAT Determinant22(matrix const &M)
{
   // NOTE: The determinant is D = a*d - b*c
   FLOAT const Result = M.R[0].C[0] * M.R[1].C[1] - M.R[1].C[0] * M.R[0].C[1];
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Minor(matrix const &M, int RemoveRow, int RemoveCol)
{
   matrix const SM = SubMatrix(M, RemoveRow, RemoveCol);
   FLOAT Result = Determinant22(SM);

   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Cofactor33(matrix const &M, int RemoveRow, int RemoveCol)
{
   Assert(M.Dimension == 3, __FUNCTION__, __LINE__);
   // NOTE: Change sign for the Cofactor when the sum of Row and Col is an odd number.
   //       So; move to -2 for sign and then add 1.
   int const Sign = -((RemoveRow + RemoveCol) % 2) * 2 + 1;
   FLOAT const Result = Sign * Minor(M, RemoveRow, RemoveCol);
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Determinant33(matrix const &M)
{
   FLOAT const CF0 = Cofactor33(M, 0, 0);
   FLOAT const CF1 = Cofactor33(M, 0, 1);
   FLOAT const CF2 = Cofactor33(M, 0, 2);

   FLOAT const Result = M.R0.C[0] * CF0 + M.R0.C[1] * CF1 + M.R0.C[2] * CF2;
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Cofactor44(matrix const &M, int RemoveRow, int RemoveCol)
{
   Assert(M.Dimension == 4, __FUNCTION__, __LINE__);
   matrix const A = SubMatrix(M, RemoveRow, RemoveCol);

   int const Sign = -((RemoveRow + RemoveCol) % 2) * 2 + 1;
   FLOAT const Result = Sign * Determinant33(A);
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Cofactor(matrix const &M, int RemoveRow, int RemoveCol)
{
   FLOAT Result{};

   if (M.Dimension == 4)
      Result = Cofactor44(M, RemoveRow, RemoveCol);
   else if (M.Dimension == 3)
      Result = Cofactor33(M, RemoveRow, RemoveCol);

   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Determinant44(matrix const &M)
{
   matrix const M0 = SubMatrix(M, 0, 0);
   matrix const M1 = SubMatrix(M, 0, 1);
   matrix const M2 = SubMatrix(M, 0, 2);
   matrix const M3 = SubMatrix(M, 0, 3);
   FLOAT const DetM0 = Determinant33(M0);
   FLOAT const DetM1 = Determinant33(M1);
   FLOAT const DetM2 = Determinant33(M2);
   FLOAT const DetM3 = Determinant33(M3);
   FLOAT const Result = M.R0.C[0] * DetM0 - M.R0.C[1] * DetM1 + M.R0.C[2] * DetM2 - M.R0.C[3] * DetM3;
   return (Result);
}

//------------------------------------------------------------------------------
FLOAT Determinant(matrix const &M)
{
   FLOAT Result{};
   if (M.Dimension == 4)
      Result = Determinant44(M);
   else if (M.Dimension == 3)
      Result = Determinant33(M);
   else if (M.Dimension == 2)
      Result = Determinant22(M);
   return (Result);
}

//------------------------------------------------------------------------------
is_invertible_return IsInvertible(matrix const &M)
{
   // NOTE: When the determinant has already been calculated we just return that result.
   //       Otherwise the Determinant is calculated and a tuple is returned..
   is_invertible_return Result{M.ID};

   if (!Result.IsInvertible)
   {
      Result.Determinant = Determinant(M);
      Result.IsInvertible = Result.Determinant != 0;
   }
   return (Result);
}

//------------------------------------------------------------------------------
matrix Transpose(matrix const &M)
{
   matrix R{};
   for (size_t Row = 0;  ///<!
        Row < 4;         ///<!
        ++Row)
   {
      for (size_t Col = 0;  ///<!
           Col < 4;         ///<!
           ++Col)
      {
         R.R[Row].C[Col] = M.R[Col].C[Row];
      }
   }
   return (R);
}

//------------------------------------------------------------------------------
matrix Inverse(matrix const &M)
{
   // NOTE: Inverse of matrix is done by
   // 1. Calculate the determinant. If different than zero ok
   // 2. Calculate the cofactor of the input matrix.
   // 3. Transpose the Resulting matrix.
   // 4. Divide each element by the determinant.

   matrix Result{};
   FLOAT &DetM = Result.ID.Determinant;
   DetM = Determinant(M);

   if (Equal(DetM, 0.f)) return (Result);

   // NOTE: Since we did not return above the matrix must be invertible
   Result.ID.IsInvertible = true;

   for (int Row = 0;        ///<!
        Row < M.Dimension;  ///<!
        ++Row)
   {
      for (int Col = 0;        ///<!
           Col < M.Dimension;  ///<!
           ++Col)
      {
         Result.R[Row].C[Col] = Cofactor(M, Row, Col) / DetM;
      }
   }

   // NOTE: The transposed matrix is not updated with the Determinant and the IsInvertible flag.
   Result = Transpose(Result);
   Result.ID.IsInvertible = true;
   Result.ID.Determinant = DetM;
   Result.ID.IsComputed = true;

   return (Result);
}
//------------------------------------------------------------------------------
matrix Mul(matrix const &A, matrix const &B)
{
   matrix M{};
   for (size_t Row = 0;                            ///<!
        Row < std::min(A.Dimension, B.Dimension);  ///<!
        ++Row)
   {
      for (size_t Col = 0;                            ///<!
           Col < std::min(A.Dimension, B.Dimension);  ///<!
           ++Col)
      {
         FLOAT const Mrc = Get(A, Row, 0) * Get(B, 0, Col) +  //
                           Get(A, Row, 1) * Get(B, 1, Col) +  //
                           Get(A, Row, 2) * Get(B, 2, Col) +  //
                           Get(A, Row, 3) * Get(B, 3, Col);   //
         Set(M, Row, Col, Mrc);
      }
   }
   return (M);
}

//------------------------------------------------------------------------------
tup Mul(matrix const &M, tup const &T)
{
   tup Result{};
   for (size_t Row = 0;     ///<!
        Row < M.Dimension;  ///<!
        ++Row)
   {
      Result.C[Row] = Get(M, Row, 0) * T.C[0] +  //
                      Get(M, Row, 1) * T.C[1] +  //
                      Get(M, Row, 2) * T.C[2] +  //
                      Get(M, Row, 3) * T.C[3];
   }

   /**
    * Normalize the resulting point.
    * Also called the perspective divide in pikumas youtube video: https://youtu.be/EqNcqBdrNyI?t=1628
    */
   if (Result.W != 0 && Result.W != FLOAT(1))
   {
      Result.X /= Result.W;
      Result.Y /= Result.W;
      Result.Z /= Result.W;
   }

   return (Result);
}

//------------------------------------------------------------------------------
tup Mul(tup const &T, matrix const &M)
{
   tup Result{};
   for (size_t Col = 0;     ///<!
        Col < M.Dimension;  ///<!
        ++Col)
   {
      Result.C[Col] = Get(M, 0, Col) * T.C[0] +  //
                      Get(M, 1, Col) * T.C[1] +  //
                      Get(M, 2, Col) * T.C[2] +  //
                      Get(M, 3, Col) * T.C[3];
   }

   /**
    * Normalize the resulting point.
    * Also called the perspective divide in pikumas youtube video: https://youtu.be/EqNcqBdrNyI?t=1628
    */
   if (Result.W != 0 && Result.W != FLOAT(1))
   {
      Result.X /= Result.W;
      Result.Y /= Result.W;
      Result.Z /= Result.W;
   }

   return (Result);
}

//------------------------------------------------------------------------------
matrix I()
{
   matrix Identity{};
   Set(Identity, 0, 0, 1.f);
   Set(Identity, 1, 1, 1.f);
   Set(Identity, 2, 2, 1.f);
   Set(Identity, 3, 3, 1.f);
   return (Identity);
}

//------------------------------------------------------------------------------
bool Equal(matrix const &A, matrix const &B)
{
   return Equal(A.R[0], B.R[0]) &&  //
          Equal(A.R[1], B.R[1]) &&  //
          Equal(A.R[2], B.R[2]) &&  //
          Equal(A.R[3], B.R[3]);
}

matrix Translation(FLOAT X, FLOAT Y, FLOAT Z)
{
   matrix M{I()};
   Set(M, 0, 3, X);
   Set(M, 1, 3, Y);
   Set(M, 2, 3, Z);

   return (M);
}

//------------------------------------------------------------------------------
matrix Scaling(FLOAT X, FLOAT Y, FLOAT Z)
{
   matrix M{I()};

   Set(M, 0, 0, X);
   Set(M, 1, 1, Y);
   Set(M, 2, 2, Z);

   return (M);
}

//------------------------------------------------------------------------------
matrix RotateX(FLOAT Alfa)
{
   matrix M{I()};
   Set(M, 1, 1, std::cos(Alfa));
   Set(M, 1, 2, std::sin(Alfa));
   Set(M, 2, 1, -std::sin(Alfa));
   Set(M, 2, 2, std::cos(Alfa));

   return (M);
}

//------------------------------------------------------------------------------
matrix RotateY(FLOAT Alfa)
{
   matrix M{I()};
   Set(M, 0, 0, std::cos(Alfa));
   Set(M, 0, 2, -std::sin(Alfa));
   Set(M, 2, 0, std::sin(Alfa));
   Set(M, 2, 2, std::cos(Alfa));

   return (M);
}

//------------------------------------------------------------------------------
matrix RotateZ(FLOAT Alfa)
{
   matrix M{I()};
   Set(M, 0, 0, std::cos(Alfa));
   Set(M, 0, 1, -std::sin(Alfa));
   Set(M, 1, 0, std::sin(Alfa));
   Set(M, 1, 1, std::cos(Alfa));

   return (M);
}

//------------------------------------------------------------------------------
tup RotateX(tup const &Reference, tup const &Vertice, FLOAT Alfa)
{
   /**
    * Move the vertice to the base (the zero in the basis).
    * The effect of this is creating a vector between the starting point at
    * the Reference going to the Vertice.
    * And only after that apply the rotation.
    */
   auto LocalVertice = Vertice - Reference;
   auto RotatedVertice = RotateX(Alfa) * LocalVertice;

   /**
    * Compute the result by adding back the Reference point.
    */
   auto Result = RotatedVertice + Reference;
   return Result;
}

//------------------------------------------------------------------------------
tup RotateY(tup const &Reference, tup const &Vertice, FLOAT Alfa)
{
   /**
    * Move the vertice to the base (the zero in the basis).
    * The effect of this is creating a vector between the starting point at
    * the Reference going to the Vertice.
    * And only after that apply the rotation.
    */
   auto LocalVertice = Vertice - Reference;
   auto RotatedVertice = RotateY(Alfa) * LocalVertice;

   /**
    * Compute the result by adding back the Reference point.
    */
   auto Result = RotatedVertice + Reference;
   return Result;
}

//------------------------------------------------------------------------------
tup RotateZ(tup const &Reference, tup const &Vertice, FLOAT Alfa)
{
   /**
    * Move the vertice to the base (the zero in the basis).
    * The effect of this is creating a vector between the starting point at
    * the Reference going to the Vertice.
    * And only after that apply the rotation.
    */
   auto LocalVertice = Vertice - Reference;
   auto RotatedVertice = RotateZ(Alfa) * LocalVertice;

   /**
    * Compute the result by adding back the Reference point.
    */
   auto Result = RotatedVertice + Reference;
   return Result;
}

//------------------------------------------------------------------------------
matrix Shearing(FLOAT Xy, FLOAT Xz, FLOAT Yx, FLOAT Yz, FLOAT Zx, FLOAT Zy)
{
   matrix M{I()};
   Set(M, 0, 1, Xy);
   Set(M, 0, 2, Xz);
   Set(M, 1, 0, Yx);
   Set(M, 1, 2, Yz);
   Set(M, 2, 0, Zx);
   Set(M, 2, 1, Zy);
   return (M);
}

//------------------------------------------------------------------------------
matrix TranslateScaleRotate(                   //!<
    FLOAT TransX, FLOAT TransY, FLOAT TransZ,  //!< Translation is in m(?)
    FLOAT ScaleX, FLOAT ScaleY, FLOAT ScaleZ,  //!< Scale input is unitless.
    FLOAT AlfaX, FLOAT AlfaY, FLOAT AlfaZ      //!< Input rotation in radians.
)
{
   matrix const M = Translation(TransX, TransY, TransZ) *             //!<
                    Scaling(ScaleX, ScaleY, ScaleZ) *                 //!<
                    RotateX(AlfaX) * RotateY(AlfaY) * RotateZ(AlfaZ)  //!<
       ;

   return (M);
}

/**
 * Initialize the Catmull Rom matrix.
 */
auto SplineInitCatmullRom() -> matrix
{
   matrix M = I();

   Set(M, 0, 0, FLOAT(0));
   Set(M, 0, 1, FLOAT(1));

   Set(M, 1, 0, -FLOAT(0.5));
   Set(M, 1, 1, FLOAT(0));
   Set(M, 1, 2, FLOAT(0.5));

   Set(M, 2, 0, FLOAT(1));
   Set(M, 2, 1, -FLOAT(5) / FLOAT(2));
   Set(M, 2, 2, FLOAT(2));
   Set(M, 2, 3, -FLOAT(1) / FLOAT(2));

   Set(M, 3, 0, -FLOAT(1) / FLOAT(2));
   Set(M, 3, 1, FLOAT(3) / FLOAT(2));
   Set(M, 3, 2, -FLOAT(3) / FLOAT(2));
   Set(M, 3, 3, FLOAT(1) / FLOAT(2));

   return M;
}

/**
 * Multiply a spline matrix at with a point p.
 * The resulting matrix is the result of M * [P0,P1,P2,P3]T.
 * The resulting matrix can be cached until any one of the points change.
 */
auto MultSpline(matrix const &M, tup const &P0, tup const &P1, tup const &P2, tup const &P3) -> matrix
{
   auto Row0 = M.R0.X * P0 + +M.R0.Y * P1 + M.R0.Z * P2 + M.R0.W * P3;
   auto Row1 = M.R1.X * P0 + +M.R1.Y * P1 + M.R1.Z * P2 + M.R1.W * P3;
   auto Row2 = M.R2.X * P0 + +M.R2.Y * P1 + M.R2.Z * P2 + M.R2.W * P3;
   auto Row3 = M.R3.X * P0 + +M.R3.Y * P1 + M.R3.Z * P2 + M.R3.W * P3;

   matrix Mc{};
   Set(Mc, 0, 0, Row0.X);
   Set(Mc, 0, 1, Row0.Y);
   Set(Mc, 0, 2, Row0.Z);
   Set(Mc, 0, 3, Row0.W);

   Set(Mc, 1, 0, Row1.X);
   Set(Mc, 1, 1, Row1.Y);
   Set(Mc, 1, 2, Row1.Z);
   Set(Mc, 1, 3, Row1.W);

   Set(Mc, 2, 0, Row2.X);
   Set(Mc, 2, 1, Row2.Y);
   Set(Mc, 2, 2, Row2.Z);
   Set(Mc, 2, 3, Row2.W);

   Set(Mc, 3, 0, Row3.X);
   Set(Mc, 3, 1, Row3.Y);
   Set(Mc, 3, 2, Row3.Z);
   Set(Mc, 3, 3, Row3.W);

   return Mc;
}

/**
 * Multiply the combined Mf and the 4 points P0,P1,P2,P3 with the vector v=[1 u u^2 u^3].
 * u is a floating point value between 0 and 1.
 */
auto MultSpline(fluffy::math3d::FLOAT u, matrix const &M) -> tup
{
   auto uSquared = u * u;
   auto uCubic = uSquared * u;
   auto V = tup{fluffy::math3d::FLOAT(1), u, uSquared, uCubic};
   auto P = V * M;
   return P;
}
};  // end of namespace math3d
};  // end of namespace fluffy

// ---
// NOTE: Stream operator
// ---
std::ostream &operator<<(std::ostream &stream, const fluffy::math3d::tup &T)
{
   // ---
   // NOTE: The width need to be big enough to hold a negative sign.
   // ---
   size_t const P{5};
   size_t const W{P + 5};
   stream << ((T.W != 0) ? "Point :" : "Vector:");
   stream << " " << std::fixed << std::setprecision(P) << std::setw(W) << T.X  //<!
          << " " << std::fixed << std::setprecision(P) << std::setw(W) << T.Y  //<!
          << " " << std::fixed << std::setprecision(P) << std::setw(W) << T.Z  //<!
          << " " << std::fixed << std::setprecision(P) << std::setw(W) << T.W;
   return stream;
}

std::ostream &operator<<(std::ostream &stream, const fluffy::math3d::matrix &M)
{
   size_t const P{5};
   size_t const W{P + 5};
   switch (M.Dimension)
   {
      case 2:
         stream << "\n |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].X         //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].Y << " |";  //<!
         stream << "\n |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].X         //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].Y << " |";  //<!
         break;
      case 3:
         stream << "\n |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].X         //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].Y           //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].Z << " |";  //<!
         stream << "\n |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].X         //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].Y           //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].Z << " |";  //<!
         stream << "\n |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[2].X         //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[2].Y           //<!
                << " |" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[2].Z << " |";  //<!
         break;
      default:
         stream << "\n {" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].X         //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].Y           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].Z           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[0].W << " }";  //<!
         stream << "\n {" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].X         //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].Y           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].Z           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[1].W << " }";  //<!
         stream << "\n {" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[2].X         //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[2].Y           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[2].Z           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[2].W << " }";  //<!
         stream << "\n {" << std::fixed << std::setprecision(P) << std::setw(W) << M.R[3].X         //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[3].Y           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[3].Z           //<!
                << " ," << std::fixed << std::setprecision(P) << std::setw(W) << M.R[3].W << " }";  //<!
         break;
   };
   stream << "\n";
   return stream;
}

fluffy::math3d::matrix operator*(fluffy::math3d::matrix const &A, fluffy::math3d::matrix const &B)
{
   return (fluffy::math3d::Mul(A, B));
}

fluffy::math3d::tup operator*(fluffy::math3d::matrix const &M, fluffy::math3d::tup const &T)
{
   return (fluffy::math3d::Mul(M, T));
}

fluffy::math3d::tup operator*(fluffy::math3d::tup const &T, fluffy::math3d::matrix const &M)
{
   return (fluffy::math3d::Mul(T, M));
}

fluffy::math3d::tup operator/(fluffy::math3d::tup const &Tup, fluffy::math3d::FLOAT const S)
{
   return (fluffy::math3d::Mul(1.f / S, Tup));
}

fluffy::math3d::tup operator+(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B)
{
   return fluffy::math3d::Add(A, B);
}

fluffy::math3d::tup operator-(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B)
{
   return fluffy::math3d::Sub(A, B);
}

fluffy::math3d::tup operator*(fluffy::math3d::FLOAT const S, fluffy::math3d::tup const &B)
{
   return fluffy::math3d::tup{S * B.X, S * B.Y, S * B.Z, S * B.W};
}

fluffy::math3d::tup operator*(fluffy::math3d::tup const &B, fluffy::math3d::FLOAT const S) { return S * B; }

bool operator==(fluffy::math3d::tup const &A, fluffy::math3d::tup const &B) { return fluffy::math3d::Equal(A, B); }

/**
* The MIT License (MIT)
Copyright © 2023 <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Ref: https://mit-license.org
*/
