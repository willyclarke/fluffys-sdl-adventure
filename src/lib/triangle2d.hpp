#ifndef FLUFFY_RENDER_TRIANGLE2D_HPP_D0C87E4D_89EA_435E_9D9F_4D43C7A96C88
#define FLUFFY_RENDER_TRIANGLE2D_HPP_D0C87E4D_89EA_435E_9D9F_4D43C7A96C88

#include <cmath>
#include <map>
#include <string>

#include "fluffymath.hpp"

/**
 * https://www.cs.drexel.edu/~david/Classes/Papers/comp175-06-pineda.pdf
 * The functions in this file has been derived from the paper mentioned above
 * and as per tutorial from Pikuma's excellent Youtube video https://youtu.be/k5wtuKWmV48 .
 *
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

namespace fluffy
{
namespace render
{

enum class edge_side
{
   ON = 0,
   LEFT = 1,
   RIGHT = 2,
};

std::string Stringify(edge_side const& Edge);

struct vertice_2d
{
   math3d::FLOAT X{};
   math3d::FLOAT Y{};
   bool Print{};
};

struct bounding_box
{
   vertice_2d Min{};
   vertice_2d Max{};
};

struct projection
{
   fluffy::math3d::FLOAT W{};      //!< Width
   fluffy::math3d::FLOAT H{};      //!< Height
   fluffy::math3d::FLOAT Theta{};  //!< Field of view
   fluffy::math3d::FLOAT ZNear{};  //!< Near plane on screen
   fluffy::math3d::FLOAT ZFar{};   //!< Far plane on screen
};

//------------------------------------------------------------------------------
// NOTE: Declarations
//------------------------------------------------------------------------------

/**
 * Edge - function according to the description in https://www.cs.drexel.edu/~david/Classes/Papers/comp175-06-pineda.pdf
 */
auto Edge(render::vertice_2d const& P,   //!<
          render::vertice_2d const& V0,  //!<
          render::vertice_2d const& V1   //!<
          )                              //!<
    -> render::edge_side;

auto Isinside(render::vertice_2d const& P,   //!<
              render::vertice_2d const& V0,  //!<
              render::vertice_2d const& V1,  //!<
              render::vertice_2d const& V2   //!<
              )                              //!<
    -> bool;

auto BoundingBox(render::vertice_2d const& V0,  //!<
                 render::vertice_2d const& V1,  //!<
                 render::vertice_2d const& V2   //!<
                 )                              //!<
    -> bounding_box;

auto MinY(render::vertice_2d const& V0,  //!<
          render::vertice_2d const& V1   //!<
          )                              //!<
    -> render::vertice_2d;

auto MaxY(render::vertice_2d const& V0,  //!<
          render::vertice_2d const& V1   //!<
          )                              //!<
    -> render::vertice_2d;

auto Vector(vertice_2d const& A,  //!<
            vertice_2d const& B   //!<
            )                     //!<
    -> vertice_2d;

auto Cross(vertice_2d const& A,  //!<
           vertice_2d const& B   //!<
           )                     //!<
    -> math3d::FLOAT;

auto EdgeCross(vertice_2d const& A,  //!<
               vertice_2d const& B,  //!<
               vertice_2d const& P   //!<
               )                     //!<
    -> math3d::FLOAT;

auto Rotate(vertice_2d const& Reference, vertice_2d const& V0, math3d::FLOAT Angle) -> vertice_2d;

auto Length(vertice_2d const& V0, vertice_2d const& V1) -> math3d::FLOAT;

auto Projection(fluffy::math3d::FLOAT Width,        //!<
                fluffy::math3d::FLOAT Height,       //!<
                fluffy::math3d::FLOAT FieldOfView,  //!<
                fluffy::math3d::FLOAT ZNear,        //!<
                fluffy::math3d::FLOAT ZFar          //!<
                ) -> projection;

auto Projection(projection const& ProjIn) -> fluffy::math3d::matrix;

auto ScreenCoord(fluffy::render::projection const& ProjIn) -> fluffy::math3d::matrix;

};  // end namespace render
};  // end namespace fluffy

fluffy::render::vertice_2d operator+(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs);
fluffy::render::vertice_2d operator-(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs);
bool operator==(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs);
bool operator<=(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs);
bool operator>(fluffy::render::vertice_2d const& lhs, fluffy::render::vertice_2d const& rhs);
fluffy::render::vertice_2d operator*(fluffy::render::vertice_2d const& lhs, fluffy::math3d::FLOAT rhs);
fluffy::render::vertice_2d operator*(fluffy::math3d::FLOAT lhs, fluffy::render::vertice_2d const& rhs);
std::ostream& operator<<(std::ostream& os, fluffy::render::vertice_2d const& v);

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
