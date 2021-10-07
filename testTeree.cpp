#define CATCH_CONFIG_MAIN

#include <iostream>
#include <string>

#include "cs221util/HSLAPixel.h"
#include "cs221util/PNG.h"
#include "cs221util/catch.hpp"

#include "teree.h"

using namespace std;
using namespace cs221util;

bool PNGDataEqual(PNG& im1, PNG& im2) {
  if (im1.width() == im2.width() && im1.height() == im2.height()) {
    // im1 and im2 have the same dimensions
    // check pixel data
    for (unsigned int x = 0; x < im1.width(); x++) {
      for (unsigned int y = 0; y < im1.height(); y++) {
        HSLAPixel* im1px = im1.getPixel(x, y);
        HSLAPixel* im2px = im2.getPixel(x, y);
        if (*im1px != *im2px)
          return false; // mismatched pixel, break immediately
      }
    }
    // all pixels matched
    return true;
  }
  return false;
}

TEST_CASE("Teree::Build,NumLeaves (basic 1x1)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/green-1x1.png");

  Teree t(img);

  int t_numleaves = t.NumLeaves();

  REQUIRE(t_numleaves == 1);

  cout << "1 x 1" << endl;
}

TEST_CASE("Teree::Build,NumLeaves (basic 3x1)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/rgb-3x1.png");

  Teree t(img);

  int t_numleaves = t.NumLeaves();

  REQUIRE(t_numleaves == 3);

  cout << "3 x 1" << endl;
}

TEST_CASE("Teree::Build,NumLeaves (basic 3x3)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/mix-3x3.png");

  Teree t(img);

  int t_numleaves = t.NumLeaves();

  REQUIRE(t_numleaves == 9);

  cout << "3 x 3" << endl;
}

TEST_CASE("Teree::Build,NumLeaves (basic 2x5)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/mix-2x5.png");

  Teree t(img);

  int t_numleaves = t.NumLeaves();

  REQUIRE(t_numleaves == 10);

  cout << "2 x 5" << endl;
}

TEST_CASE("Teree::Build,NumLeaves (large 256x224)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/cs-256x224.png");

  Teree t(img);

  int t_numleaves = t.NumLeaves();

  REQUIRE(t_numleaves == 57344);
  cout << "256 x 224" << endl;
} 
 
TEST_CASE("Teree::Copy (basic)", "[weight=1][part=Build]")
{
  PNG img1;
  img1.readFromFile("./originals/mix-2x5.png");
  PNG img2;
  img2.readFromFile("./originals/mix-3x3.png");

  Teree t1(img1);
  Teree t2(t1);

  int t2_numleaves = t2.NumLeaves();

  REQUIRE(t2_numleaves == 10);
  cout << "copy" << endl;
}

TEST_CASE("Teree::Build,Render (basic 1x1)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/green-1x1.png");

  Teree t(img);

  PNG outimg = t.Render();

  REQUIRE(PNGDataEqual(img, outimg) == true);
  cout << "1x1 render" << endl;
}

TEST_CASE("Teree::Build,Render (basic 3x1)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/rgb-3x1.png");

  Teree t(img);

  PNG outimg = t.Render();

  REQUIRE(PNGDataEqual(img, outimg) == true);
  cout << "3x1 render" << endl;
}

TEST_CASE("Teree::Build,Render (basic 3x3)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/mix-3x3.png");

  Teree t(img);

  PNG outimg = t.Render();

  REQUIRE(PNGDataEqual(img, outimg) == true);
  cout << "3x3 render" << endl;
}

TEST_CASE("Teree::Build,Render (basic 2x5)", "[weight=1][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/mix-2x5.png");

  Teree t(img);

  PNG outimg = t.Render();

  REQUIRE(PNGDataEqual(img, outimg) == true);
  cout << "2x5 render" << endl;
}


/* TEST_CASE("Teree::Build,Render (large 256x224)", "[weight=3][part=Build]")
{
  PNG img;
  img.readFromFile("./originals/cs-256x224.png");

  Teree t(img);

  cout << "built " << endl;

  PNG outimg = t.Render();

  REQUIRE(PNGDataEqual(img, outimg) == true);
  cout << "256x224 render" << endl;
}   */

TEST_CASE("Teree::Prune (small 8x5)", "[weight=2][part=Prune]")
{
  PNG img;
  img.readFromFile("./originals/pruneto16leaves-8x5.png");

  Teree t(img);

  t.Prune(0.05);

  int t_numleaves = t.NumLeaves();

  REQUIRE(t_numleaves == 16);

  PNG outimg = t.Render();

  REQUIRE(PNGDataEqual(img, outimg) == true);
}

TEST_CASE("Teree::Prune,Copy (large 256x224)", "[weight=3][part=Prune]")
{
  PNG img;
  img.readFromFile("./originals/cs-256x224.png");

  Teree t(img);

  Teree t005(t);
  Teree t020(t);

  t005.Prune(0.05);
  t020.Prune(0.20);

  int t005_numleaves = t005.NumLeaves();
  int t020_numleaves = t020.NumLeaves();

  REQUIRE(t005_numleaves == 8982);
  REQUIRE(t020_numleaves == 3935);

  PNG outimg = t.Render();
  PNG outimg005 = t005.Render();
  PNG outimg020 = t020.Render();

  PNG solnimg005;
  solnimg005.readFromFile("./soln_outputs/soln_cs-256x224-005tol.png");
  PNG solnimg020;
  solnimg020.readFromFile("./soln_outputs/soln_cs-256x224-020tol.png");


  REQUIRE(PNGDataEqual(img, outimg) == true);
  REQUIRE(PNGDataEqual(solnimg005, outimg005) == true);
  REQUIRE(PNGDataEqual(solnimg020, outimg020) == true);
  REQUIRE(PNGDataEqual(img, outimg005) == false);
  REQUIRE(PNGDataEqual(img, outimg020) == false);
}    