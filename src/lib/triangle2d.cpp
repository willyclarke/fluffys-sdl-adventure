/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <iostream>

#include "fluffymath.hpp"
#include "triangle2d.hpp"

namespace
{
std::map<fluffy::render::edge_side, std::string> const edge_side_string_map = {
    {fluffy::render::edge_side::ON, "ON"},       //!<
    {fluffy::render::edge_side::LEFT, "LEFT"},   //!<
    {fluffy::render::edge_side::RIGHT, "RIGHT"}  //!<
};

}
namespace fluffy
{
namespace render
{

/**
 * Return a string of the enumerated value.
 */
std::string Stringify(edge_side const& Edge)
{
   if (edge_side_string_map.count(Edge))
   {
      auto Res = edge_side_string_map.find(Edge);
      return Res->second;
   }
   return {};
}

/**
 * Check to see if a point P is ON, LEFT or RIGHT of the vector between V0 and V1.
 */
auto Edge(fluffy::render::vertice_2d const& P,   //!<
          fluffy::render::vertice_2d const& V0,  //!<
          fluffy::render::vertice_2d const& V1   //!<
          )                                      //!<
    -> fluffy::render::edge_side
{
   fluffy::render::vertice_2d const Delta = {V1.X - V0.X, V1.Y - V0.Y};
   auto const E = (P.X - V0.X) * Delta.Y - (P.Y - V0.Y) * Delta.X;

   fluffy::render::edge_side Result{fluffy::render::edge_side::ON};

   if (E > 0)
      Result = fluffy::render::edge_side::RIGHT;
   else if (E < 0)
      Result = fluffy::render::edge_side::LEFT;

   return Result;
}

/**
 * Check to see if a point P is inside a render constructed by the vertices V0, V1, V2.
 */
auto Isinside(fluffy::render::vertice_2d const& P,   //!<
              fluffy::render::vertice_2d const& V0,  //!<
              fluffy::render::vertice_2d const& V1,  //!<
              fluffy::render::vertice_2d const& V2   //!<
              )                                      //!<
    -> bool
{
   auto E1 = Edge(P, V1, V0);
   auto E2 = Edge(P, V2, V1);
   auto E3 = Edge(P, V0, V2);

   bool const Inside = E1 != fluffy::render::edge_side::LEFT && E2 != fluffy::render::edge_side::LEFT &&
                       E3 != fluffy::render::edge_side::LEFT;

   return Inside;
}

/**
 * Compute the Cross product given three vertices that make up two vectors.
 * When the value returned is divided by 2 it will be the area of half
 * the parallelpiped that these two vectors make up.
 */
auto EdgeCross(vertice_2d const& A,  //!<
               vertice_2d const& B,  //!<
               vertice_2d const& P   //!<
               )                     //!<
    -> fluffy::math3d::FLOAT
{
   auto vAB = Vector(A, B);
   auto vAP = Vector(A, P);
   // auto Result = Cross(vAB, vAP);
   auto Result = vAB.X * vAP.Y - vAB.Y * vAP.X;
   return Result;
}

/**
 * Check if point P is inside a bounding box when given the vertices V0, V1 and V2.
 */
auto BoundingBox(fluffy::render::vertice_2d const& V0,  //!<
                 fluffy::render::vertice_2d const& V1,  //!<
                 fluffy::render::vertice_2d const& V2   //!<
                 )                                      //!<
    -> bounding_box
{
   auto MinX = std::min(V0.X, std::min(V1.X, V2.X));
   auto MinY = std::min(V0.Y, std::min(V1.Y, V2.Y));
   auto MaxX = std::max(V0.X, std::max(V1.X, V2.X));
   auto MaxY = std::max(V0.Y, std::max(V1.Y, V2.Y));
   bounding_box Result{{MinX, MinY}, {MaxX, MaxY}};
   return Result;
}

/**
 * Return the designated min of two vertices. I.e the one with the lowest Y value.
 */
auto MinY(vertice_2d const& V0,  //!<
          vertice_2d const& V1   //!<
          )                      //!<
    -> fluffy::render::vertice_2d
{
   return V0 <= V1 ? V0 : V1;
}

auto MaxY(vertice_2d const& V0,  //!<
          vertice_2d const& V1   //!<
          )                      //!<
    -> fluffy::render::vertice_2d
{
   return V0 <= V1 ? V1 : V0;
}

/**
 * Create a vector from two vertices.
 * The result is returned as a vertice.
 */
auto Vector(vertice_2d const& A,  //!<
            vertice_2d const& B   //!<
            )                     //!<
    -> vertice_2d
{
   return {B.X - A.X, B.Y - A.Y};
}

/**
 * Calculate the 'fake' cross product between two 2d vectors.
 * The result can be used to check for change of sign to determine
 * which way the resulting vector 'kind' of points in or out of the
 * screen.
 */
auto Cross(vertice_2d const& A,  //!<
           vertice_2d const& B   //!<
           )                     //!<
    -> fluffy::math3d::FLOAT
{
   return A.X * B.Y - A.Y * B.X;
}

/**
 * Rotate a vertice by an angle given in radians.
 * Implicitly this is a vector from {0,0} to the Vertice that is beeing rotated.
 */
auto Rotate(vertice_2d const& V0, fluffy::math3d::FLOAT Angle) -> vertice_2d
{
   vertice_2d Result{V0.X * std::cos(Angle) - V0.Y * std::sin(Angle), V0.X * std::sin(Angle) + V0.Y * std::cos(Angle)};
   return Result;
}

/**
 * Rotate a vertice around a reference vertice.
 * A positive angle will rotate Counter Clockwise.
 */
auto Rotate(vertice_2d const& Reference, vertice_2d const& V0, fluffy::math3d::FLOAT Angle) -> vertice_2d
{
   /**
    * Move the Vertice to produce local coordinates.
    */
   vertice_2d V = V0 - Reference;

   auto RotatedV = Rotate(V, Angle);

   /**
    * Move the Vertice back to World coordinates.
    */
   vertice_2d Result = Reference + RotatedV;

   return Result;
}

/**
 * Compute length of vector created by two vertices
 */
auto Length(vertice_2d const& V0, vertice_2d const& V1) -> fluffy::math3d::FLOAT
{
   auto V = V1 - V0;
   fluffy::math3d::FLOAT Result = fluffy::math3d::FLOAT(std::sqrt(V.X * V.X + V.Y * V.Y));
   return Result;
}

/**
 * Set up a configuration for a projection.
 */
auto Projection(fluffy::math3d::FLOAT Width,        //!<
                fluffy::math3d::FLOAT Height,       //!<
                fluffy::math3d::FLOAT FieldOfView,  //!<
                fluffy::math3d::FLOAT ZNear,        //!<
                fluffy::math3d::FLOAT ZFar          //!<
                ) -> projection
{
   return projection{Width, Height, FieldOfView, ZNear, ZFar};
}

/**
 * Create a projection matrix based on the projection configuration.
 * NOTE: The projection matrix will store the Z value as the W when
 *       a point is multiplied.
 */
auto Projection(fluffy::render::projection const& ProjIn) -> fluffy::math3d::matrix
{
   /**
    * NOTE: Taking tan(90) is not a good idea.
    */
   Assert(ProjIn.Theta < M_PI, __FUNCTION__, __LINE__);
   Assert(ProjIn.Theta > math3d::FLOAT(0), __FUNCTION__, __LINE__);
   Assert(ProjIn.ZFar > ProjIn.ZNear, __FUNCTION__, __LINE__);

   math3d::matrix Mp{};
   auto const OneOverThetaHalf = math3d::FLOAT(1) / std::tan(ProjIn.Theta / math3d::FLOAT(2));

   /**
    * NOTE: H / W is the aspect ratio.
    */
   Mp.R0.X = (ProjIn.H / ProjIn.W) * OneOverThetaHalf;
   Mp.R1.Y = OneOverThetaHalf;
   Mp.R2.Z = ProjIn.ZFar / (ProjIn.ZFar - ProjIn.ZNear);
   Mp.R2.W = -(ProjIn.ZFar * ProjIn.ZNear) / (ProjIn.ZFar - ProjIn.ZNear);
   Mp.R3.Z = math3d::FLOAT(1);

   return Mp;
}

/**
 * Return matrix that converts to screen coordinates.
 * The resulting matrix will convert the point 0,0 to be at the middle of the screen.
 */
auto ScreenCoord(fluffy::render::projection const& ProjIn) -> fluffy::math3d::matrix
{
   fluffy::math3d::matrix M = fluffy::math3d::I();

   auto XMin = math3d::FLOAT(0);
   auto XMax = ProjIn.W - 1;
   auto YMin = math3d::FLOAT(0);
   auto YMax = ProjIn.H - 1;

   M.R0.X = XMax / math3d::FLOAT(2);
   M.R0.W = XMax / math3d::FLOAT(2);
   M.R1.Y = YMax / math3d::FLOAT(2);
   M.R1.W = YMax / math3d::FLOAT(2);

   return M;
}

};  // end of namespace render
};  // end of namespace fluffy

/**
 * Operator overloads.
 */

/**
 * Addition implicitly means that the vertice is actually a vector going from origin.
 */
fluffy::render::vertice_2d operator+(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs)
{
   return {lhs.X + rhs.X, lhs.Y + rhs.Y};
}

/**
 * Subtraction implicitly means that the vertice is actually a vector going from origin.
 */
fluffy::render::vertice_2d operator-(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs)
{
   return {lhs.X - rhs.X, lhs.Y - rhs.Y};
}

bool operator==(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs)
{
   return std::abs(lhs.X - rhs.X) <= fluffy::math3d::EPSILON && std::abs(lhs.Y - rhs.Y) <= fluffy::math3d::EPSILON;
}

/**
 * Return the vertice with the lowest Y value.
 */
bool operator<=(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs)
{
   return lhs.Y <= rhs.Y;
   // auto MagLhs = lhs.X * lhs.X + lhs.Y * lhs.Y;
   // auto MagRhs = rhs.X * rhs.X + rhs.Y * rhs.Y;
   // return MagLhs <= MagRhs;
}

bool operator>(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs) { return !(lhs <= rhs); }

std::ostream& operator<<(std::ostream& os, fluffy::render::vertice_2d const& v)
{
   os << "(" << v.X << ", " << v.Y << ")";
   return os;
}

fluffy::render::vertice_2d operator*(fluffy::render::vertice_2d const& lhs, fluffy::math3d::FLOAT rhs)
{
   return fluffy::render::vertice_2d{lhs.X * rhs, lhs.Y * rhs};
}

fluffy::render::vertice_2d operator*(fluffy::math3d::FLOAT lhs, fluffy::render::vertice_2d const& rhs)
{
   return fluffy::render::vertice_2d{rhs.X * lhs, rhs.Y * lhs};
}

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
