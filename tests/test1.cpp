#include <catch2/catch_test_macros.hpp>

#include "../src/lib/triangle2d.hpp"

#include <iostream>

unsigned int Factorial(unsigned int number) { return number <= 1 ? number : Factorial(number - 1) * number; }

TEST_CASE("Factorials are computed", "[factorial]")
{
   REQUIRE(Factorial(1) == 1);
   REQUIRE(Factorial(2) == 2);
   REQUIRE(Factorial(3) == 6);
   REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("render2d", "[Side check]")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{1, 0};

   {
      fluffy::render::vertice_2d P{1, 1};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::LEFT);
   }

   {
      fluffy::render::vertice_2d P{1, -1};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::RIGHT);
   }

   {
      fluffy::render::vertice_2d P{1, 0};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::ON);
   }

   {
      fluffy::render::vertice_2d P{0, 0};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::ON);
   }

   {
      fluffy::render::vertice_2d P{-1, 0};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::ON);
   }

   {
      fluffy::render::vertice_2d P{2, 0};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::ON);
   }

   {
      fluffy::render::vertice_2d P{2, 2};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::LEFT);
   }

   {
      fluffy::render::vertice_2d P{2, -2};
      fluffy::render::edge_side Side = fluffy::render::Edge(P, V0, V1);
      REQUIRE(Side == fluffy::render::edge_side::RIGHT);
   }
}

TEST_CASE("render2d", "[Inside check]")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{1, 0};
   fluffy::render::vertice_2d V2{1, 1};
   {
      fluffy::render::vertice_2d P{0, 0};
      REQUIRE(fluffy::render::Isinside(P, V0, V1, V2) == true);
   }
   {
      fluffy::render::vertice_2d P{1, 0};
      REQUIRE(fluffy::render::Isinside(P, V0, V1, V2) == true);
   }
   {
      fluffy::render::vertice_2d P{1, 1};
      REQUIRE(fluffy::render::Isinside(P, V0, V1, V2) == true);
   }
}

TEST_CASE("render2d", "[Inside BB]")
{
   fluffy::render::vertice_2d V0{-10, -10};
   fluffy::render::vertice_2d V1{15, 0};
   fluffy::render::vertice_2d V2{10, 10};
   auto BB = fluffy::render::BoundingBox(V0, V1, V2);
   REQUIRE(BB.Min.X == -10);
   REQUIRE(BB.Min.Y == -10);
   REQUIRE(BB.Max.X == 15);
   REQUIRE(BB.Max.Y == 10);
}

TEST_CASE("render2d", "Check vertice_2d minimum")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{15, 0};
   fluffy::render::vertice_2d V2{10, 10};
   auto Min = fluffy::render::MinY(fluffy::render::MinY(V0, V1), V2);
   auto Max = fluffy::render::MaxY(fluffy::render::MaxY(V0, V1), V2);

   {
      auto FlagEQ = Min == V0;
      REQUIRE(FlagEQ == true);
   }

   {
      auto FlagLE = V0 <= V1;
      REQUIRE(FlagLE == true);
   }

   {
      auto FlagLE = V0 <= V2;
      REQUIRE(FlagLE == true);
   }

   {
      auto FlagLE = V2 <= V1;
      REQUIRE(FlagLE == false);
      auto FlagGT = V1 > V2;
      REQUIRE(FlagGT == false);
   }

   {
      auto VerticesAreEQ = V1 == Max;
      REQUIRE(VerticesAreEQ == false);
   }
}

TEST_CASE("render2d", "[Check Area 1]")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{1, 1};
   fluffy::render::vertice_2d V2{1, 0};

   auto Area = fluffy::render::EdgeCross(V0, V2, V1) / fluffy::math3d::FLOAT(2);
   REQUIRE(Area == fluffy::math3d::FLOAT(0.5));

   auto vV01 = fluffy::render::Vector(V0, V1);
   REQUIRE(vV01.X == fluffy::math3d::FLOAT(1));
   REQUIRE(vV01.Y == fluffy::math3d::FLOAT(1));

   auto vV12 = fluffy::render::Vector(V1, V2);
   REQUIRE(vV12.X == fluffy::math3d::FLOAT(0));
   REQUIRE(vV12.Y == fluffy::math3d::FLOAT(-1));

   auto Cross = fluffy::render::Cross(vV12, vV01);
   // std::cout << "Cross product between " << vV01 << " and " << vV12 << " = " << Cross << std::endl;
}

TEST_CASE("render2d", "[Check Area 2]")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{1, 1};
   fluffy::render::vertice_2d V2{1, 0};

   auto Area = fluffy::render::EdgeCross(V0, V2, V1) / fluffy::math3d::FLOAT(2);
   REQUIRE(Area == fluffy::math3d::FLOAT(0.5));
}

TEST_CASE("render2d", "[Check Area 3]")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{1, 1};
   fluffy::render::vertice_2d V2{1, 0};

   auto Area = fluffy::render::EdgeCross(V0, V2, V1) / fluffy::math3d::FLOAT(2);
   REQUIRE(Area == fluffy::math3d::FLOAT(0.5));

   fluffy::render::vertice_2d P{fluffy::math3d::FLOAT(0.75), fluffy::math3d::FLOAT(0.5)};
   /**
    * Compute the Barycentric coordinates for point P.
    */
   if (Area > fluffy::math3d::EPSILON)
   {
      /**
       * Need to divide the EdgeCross by value of 2 to get render area.
       */
      auto const Alfa = fluffy::render::EdgeCross(V2, V1, P) / fluffy::math3d::FLOAT(2) / Area;
      auto const Beta = fluffy::render::EdgeCross(V0, V2, P) / fluffy::math3d::FLOAT(2) / Area;
      auto const Gamma = fluffy::render::EdgeCross(V1, V0, P) / fluffy::math3d::FLOAT(2) / Area;

      auto const CheckInside =
          Alfa >= fluffy::math3d::FLOAT(0) && Beta >= fluffy::math3d::FLOAT(0) && Gamma >= fluffy::math3d::FLOAT(0);
      auto const Isinside = fluffy::render::Isinside(P, V0, V2, V1);
      REQUIRE(CheckInside == Isinside);

      // std::cout << "\nAlfa:" << Alfa << " "
      //           << "\nBeta:" << Beta << " "
      //           << "\nGamma:" << Gamma << " "
      //           << "\nArea:" << Area << std::endl;

      REQUIRE((Alfa + Beta + Gamma) == fluffy::math3d::FLOAT(1));
   }
}

TEST_CASE("render2d", "[Rotate 1]")
{
   auto Conv = fluffy::math3d::Deg2Rad(fluffy::math3d::FLOAT(180));
   REQUIRE(Conv == M_PI);

   Conv = fluffy::math3d::Deg2Rad(fluffy::math3d::FLOAT(0));
   REQUIRE(Conv == fluffy::math3d::FLOAT(0));

   Conv = fluffy::math3d::Deg2Rad(fluffy::math3d::FLOAT(90));
   REQUIRE(Conv == fluffy::math3d::FLOAT(M_PI_2));

   Conv = fluffy::math3d::Rad2Deg(fluffy::math3d::FLOAT(0));
   REQUIRE(Conv == fluffy::math3d::FLOAT(0));

   Conv = fluffy::math3d::Rad2Deg(fluffy::math3d::FLOAT(M_PI));
   REQUIRE(Conv == fluffy::math3d::FLOAT(180));
}

TEST_CASE("render2d", "[Addition]")
{
   /**
    * Check if Addition is ok.
    */
   {
      fluffy::render::vertice_2d V0{0, 0};
      fluffy::render::vertice_2d V1{1, 0};
      auto AddV = V0 + V1;
      fluffy::render::vertice_2d ExpectedV{1, 0};
      auto Check = AddV == ExpectedV;
      REQUIRE(Check == true);
   }
   {
      fluffy::render::vertice_2d V0{1, 0};
      fluffy::render::vertice_2d V1{1, 0};
      auto AddV = V0 + V1;
      fluffy::render::vertice_2d ExpectedV{2, 0};
      auto Check = AddV == ExpectedV;
      REQUIRE(Check == true);
   }
   {
      fluffy::render::vertice_2d V0{1, 0};
      fluffy::render::vertice_2d V1{0, 1};
      auto AddV = V0 + V1;
      fluffy::render::vertice_2d ExpectedV{1, 1};
      auto Check = AddV == ExpectedV;
      REQUIRE(Check == true);
   }
}

TEST_CASE("render2d", "[Subtraction]")
{
   /**
    * Check if Subtraction is ok.
    */
   {
      fluffy::render::vertice_2d V0{1, 0};
      fluffy::render::vertice_2d V1{1, 0};
      auto SubV = V1 - V0;
      fluffy::render::vertice_2d ExpectedV{0, 0};
      auto Check = SubV == ExpectedV;
      REQUIRE(Check == true);
   }
   {
      fluffy::render::vertice_2d V0{1, 0};
      fluffy::render::vertice_2d V1{1, 1};
      auto SubV = V1 - V0;
      fluffy::render::vertice_2d ExpectedV{0, 1};
      auto Check = SubV == ExpectedV;
      REQUIRE(Check == true);
   }
   {
      fluffy::render::vertice_2d V0{1, 0};
      fluffy::render::vertice_2d V1{0, 1};
      auto SubV = V1 - V0;
      fluffy::render::vertice_2d ExpectedV{-1, 1};
      auto Check = SubV == ExpectedV;
      REQUIRE(Check == true);
   }
}

TEST_CASE("render2d", "[Rotate]")
{
   {
      fluffy::render::vertice_2d V0{1, 0};
      auto RotV = fluffy::render::Rotate({}, V0, fluffy::math3d::Deg2Rad(90));
      fluffy::render::vertice_2d ExpectedV{0, 1};
      auto Check = RotV == ExpectedV;
      REQUIRE(Check == true);
   }
   {
      fluffy::render::vertice_2d V0{0, 1};
      auto RotV = fluffy::render::Rotate({}, V0, fluffy::math3d::Deg2Rad(90));
      fluffy::render::vertice_2d ExpectedV{-1, 0};
      auto Check = RotV == ExpectedV;
      REQUIRE(Check == true);
   }
   {
      fluffy::render::vertice_2d V0{0, 1};
      auto RotV = fluffy::render::Rotate({}, V0, fluffy::math3d::Deg2Rad(-90));
      fluffy::render::vertice_2d ExpectedV{1, 0};
      auto Check = RotV == ExpectedV;
      REQUIRE(Check == true);
   }
}

TEST_CASE("render2d", "[Magnitude1]")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{1, 0};
   auto V = V1 - V0;
   auto L = fluffy::render::Length(V0, V1);
   REQUIRE(L == fluffy::math3d::FLOAT(1));
}

TEST_CASE("render2d", "[Magnitude2]")
{
   fluffy::render::vertice_2d V0{0, 0};
   fluffy::render::vertice_2d V1{-1, -1};
   auto V = V1 - V0;
   auto L = fluffy::render::Length(V0, V1);
   REQUIRE(L == fluffy::math3d::FLOAT(M_SQRT2));
}

TEST_CASE("math3d", "[IdentityMatrix]")
{
   auto I = fluffy::math3d::I();
   auto P = fluffy::math3d::Point(1, 2, 3);
   auto M1 = fluffy::math3d::Translation(10, 20, 30);

   auto P2 = M1 * P;
   auto Check = P2 == fluffy::math3d::Point(11, 22, 33);
   REQUIRE(Check == true);
}

TEST_CASE("math3d", "[Inverse]")
{
   auto M1 = fluffy::math3d::Translation(10, 20, 30);
   auto IM1 = fluffy::math3d::Inverse(M1);
   auto CheckIsInv = fluffy::math3d::IsInvertible(M1);
   REQUIRE(CheckIsInv.IsInvertible == true);
   if (CheckIsInv.IsInvertible)
   {
      REQUIRE(IM1.R[0].W == -10);
      REQUIRE(IM1.R[1].W == -20);
      REQUIRE(IM1.R[2].W == -30);
   }
}

TEST_CASE("math3d", "[projectioninit]")
{
   auto Projection = fluffy::render::Projection(800, 600, fluffy::math3d::Deg2Rad(90), 0, 100);
   REQUIRE(Projection.W == 800);
   REQUIRE(Projection.H == 600);
   REQUIRE(Projection.Theta == M_PI_2);
   REQUIRE(Projection.ZNear == 0);
   REQUIRE(Projection.ZFar == 100);
}

TEST_CASE("math3d", "[projectionmatrix]")
{
   auto Projection = fluffy::render::Projection(800, 600, fluffy::math3d::Deg2Rad(90), 10, 100);
   fluffy::math3d::matrix Mp = fluffy::render::Projection(Projection);

   auto OneOverThetaHalf = (fluffy::math3d::FLOAT(1) / std::tan(Projection.Theta / 2));
   auto R00 = (Projection.H / Projection.W) * OneOverThetaHalf;
   auto R11 = OneOverThetaHalf;
   auto R22 = Projection.ZFar / (Projection.ZFar - Projection.ZNear);
   auto R23 = -Projection.ZFar * Projection.ZNear / (Projection.ZFar - Projection.ZNear);
   auto R32 = fluffy::math3d::FLOAT(1);

   REQUIRE(Mp.R0.X == R00);
   REQUIRE(Mp.R1.Y == R11);
   REQUIRE(Mp.R2.Z == R22);
   REQUIRE(Mp.R2.W == R23);
   REQUIRE(Mp.R3.Z == R32);
   REQUIRE(Mp.R2.W == -fluffy::math3d::FLOAT(1000) / fluffy::math3d::FLOAT(90));
   REQUIRE(Mp.R3.Z == 1);

   // auto Point1 = fluffy::math3d::Point(1, 1, Projection.ZNear);
   // auto Point2 = fluffy::math3d::Point(1, 1, fluffy::math3d::FLOAT(2) * Projection.ZNear);

   // auto ProjPoint1 = Mp * Point1;
   // auto ProjPoint2 = Mp * Point2;

   {
      auto PInput = fluffy::math3d::Point(-1, -1, 0);
      auto MatrixScreen = fluffy::render::ScreenCoord(Projection);
      auto Screenpoint = MatrixScreen * PInput;
      REQUIRE(Screenpoint.X == 0);
      REQUIRE(Screenpoint.Y == 0);
   }

   {
      auto PInput = fluffy::math3d::Point(-1, 1, 0);
      auto MatrixScreen = fluffy::render::ScreenCoord(Projection);
      auto Screenpoint = MatrixScreen * PInput;
      REQUIRE(Screenpoint.X == 0);
      REQUIRE(Screenpoint.Y == Projection.H - 1);
   }

   {
      auto PInput = fluffy::math3d::Point(1, 1, 0);
      auto MatrixScreen = fluffy::render::ScreenCoord(Projection);
      auto Screenpoint = MatrixScreen * PInput;
      REQUIRE(Screenpoint.X == Projection.W - 1);
      REQUIRE(Screenpoint.Y == Projection.H - 1);
   }

   {
      auto PInput = fluffy::math3d::Point(1, -1, 0);
      auto MatrixScreen = fluffy::render::ScreenCoord(Projection);
      auto Screenpoint = MatrixScreen * PInput;
      REQUIRE(Screenpoint.X == Projection.W - 1);
      REQUIRE(Screenpoint.Y == 0);
   }
}

TEST_CASE("math3d", "[combinedprojectionandscreen]")
{
   auto Projection = fluffy::render::Projection(800, 600, fluffy::math3d::Deg2Rad(90), 10, 100);
   fluffy::math3d::matrix MatrixProjection = fluffy::render::Projection(Projection);

   auto MatrixScreen = fluffy::render::ScreenCoord(Projection);
   auto MatrixConversion = MatrixScreen * MatrixProjection;

   /**
    * Test that upper screen left is associated with -1, -1 which is the
    * normalized position for upper left.
    */
   {
      auto PInput = fluffy::math3d::Point(-1, -1, 0);
      auto Screenpoint = MatrixScreen * PInput;
      REQUIRE(Screenpoint.X == 0);
      REQUIRE(Screenpoint.Y == 0);

      PInput = fluffy::math3d::Point(0, 0, 0);
      Screenpoint = MatrixScreen * PInput;
      REQUIRE(Screenpoint.X == (Projection.W - 1) / 2);
      REQUIRE(Screenpoint.Y == (Projection.H - 1) / 2);

      PInput = fluffy::math3d::Point(1, 1, 0);
      Screenpoint = MatrixScreen * PInput;
      REQUIRE(Screenpoint.X == (Projection.W - 1));
      REQUIRE(Screenpoint.Y == (Projection.H - 1));
   }

   /**
    * Test the combination of the combined matrix for projection and screen coordinates
    * gives the expected result.
    */
   {
      auto Point1 = fluffy::math3d::Point(0, 0, 10);
      auto Point1Projected = MatrixProjection * Point1;
      auto Point1Screen = MatrixScreen * Point1Projected;

      REQUIRE(Point1Screen.X == (Projection.W - 1) / fluffy::math3d::FLOAT(2));
      REQUIRE(Point1Screen.Y == (Projection.H - 1) / fluffy::math3d::FLOAT(2));
      REQUIRE(Point1Screen.Z == fluffy::math3d::FLOAT(0));
      REQUIRE(Point1Screen.W == Projection.ZNear);
   }

   {
      auto Point1 = fluffy::math3d::Point(10 * Projection.W / Projection.H, 0, 10);
      auto Point1Screen = MatrixConversion * Point1;

      REQUIRE(int(Point1Screen.X) == int(Projection.W - 1));
      REQUIRE(int(Point1Screen.Y) == int((Projection.H - 1) / fluffy::math3d::FLOAT(2)));
      REQUIRE(Point1Screen.Z == fluffy::math3d::FLOAT(0));
      REQUIRE(Point1Screen.W == Projection.ZNear);
   }
}

TEST_CASE("math3d", "[rotateverticearoundx]")
{
   {
      auto Reference = fluffy::math3d::Point(0, 0, 0);
      auto Point = fluffy::math3d::Point(2, 0, 0);
      auto PRotated = fluffy::math3d::RotateX(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(2, 0, 0);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }

   {
      auto Reference = fluffy::math3d::Point(0, 0, 0);
      auto Point = fluffy::math3d::Point(0, 2, 0);
      auto PRotated = fluffy::math3d::RotateX(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(0, 0, -2);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }

   {
      auto Reference = fluffy::math3d::Point(2, 1, 0);
      auto Point = fluffy::math3d::Point(2, 2, 0);
      auto PRotated = fluffy::math3d::RotateX(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(2, 1, -1);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }
}

TEST_CASE("math3d", "[rotateverticearoundy]")
{
   {
      auto Reference = fluffy::math3d::Point(0, 0, 0);
      auto Point = fluffy::math3d::Point(0, 1, 0);
      auto PRotated = fluffy::math3d::RotateY(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(0, 1, 0);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }

   {
      auto Reference = fluffy::math3d::Point(0, 0, 0);
      auto Point = fluffy::math3d::Point(1, 0, 0);
      auto PRotated = fluffy::math3d::RotateY(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(0, 0, 1);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }

   {
      auto Reference = fluffy::math3d::Point(0, 1, 1);
      auto Point = fluffy::math3d::Point(0, 1, 2);
      auto PRotated = fluffy::math3d::RotateY(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(-1, 1, 1);
      auto AllGood = PRotated == PExpected;
      // std::cout << "P in  :"<< Point << std::endl;
      // std::cout << "Refer :" << Reference << std::endl;
      // std::cout << "Result:"<< PRotated << std::endl;
      REQUIRE(AllGood == true);
   }
}

TEST_CASE("math3d", "[rotateverticearoundz]")
{
   {
      auto Reference = fluffy::math3d::Point(0, 0, 0);
      auto Point = fluffy::math3d::Point(1, 0, 0);
      auto PRotated = fluffy::math3d::RotateZ(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(0, 1, 0);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }

   {
      auto Reference = fluffy::math3d::Point(1, 1, 0);
      auto Point = fluffy::math3d::Point(2, 2, 0);
      auto PRotated = fluffy::math3d::RotateZ(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(0, 2, 0);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }

   {
      auto Reference = fluffy::math3d::Point(0, 0, 0);
      auto Point = fluffy::math3d::Point(2, 2, 0);
      auto PRotated = fluffy::math3d::RotateZ(Reference, Point, fluffy::math3d::Deg2Rad(90));
      auto PExpected = fluffy::math3d::Point(-2, 2, 0);
      auto AllGood = PRotated == PExpected;
      REQUIRE(AllGood == true);
   }
}
