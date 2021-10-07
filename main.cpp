// Basic test cases for PA3 2021S
//

#include <iostream>
#include <string>

#include "teree.h"

#define PRUNETOL 0.05
#define PRUNEHIGHTOL 0.20

using namespace std;
using namespace cs221util;

void TestBuild1x1();
void TestBuild3x1();
void TestBuild3x3();
void TestBuild2x5();
void TestPrune8x5();
void TestCopyBasic();
void TestCopyBigPrune();

bool PNGDataEqual(PNG& im1, PNG& im2);

int main() {
  // You can comment/uncomment or add other tests as needed
  TestBuild1x1();
  TestBuild3x1();
  TestBuild3x3();
  TestBuild2x5();

  TestPrune8x5();

  TestCopyBasic();

  TestCopyBigPrune();

  return 0;
}

void TestBuild1x1() {
  PNG img;
  img.readFromFile("./originals/green-1x1.png");

  cout << "Building tree from 1x1 image... ";
  Teree t(img);
  cout << "Done." << endl;

  cout << "Expecting 1 leaf node. NumLeaves reports: " << t.NumLeaves() << " leaf nodes." << endl;
  
  cout << "Rendering tree and writing to output PNG... ";
  PNG outimg = t.Render();
  outimg.convert();
  outimg.writeToFile("./outputs/out_green-1x1.png");
  cout << "Done.\n" << endl;
}

void TestBuild3x1() {
  PNG img;
  img.readFromFile("./originals/rgb-3x1.png");

  cout << "Building tree from 3x1 image... ";
  Teree t(img);
  cout << "Done." << endl;

  cout << "Expecting 3 leaf nodes. NumLeaves reports: " << t.NumLeaves() << " leaf nodes." << endl;

  cout << "Rendering tree and writing to output PNG... ";
  PNG outimg = t.Render();
  outimg.convert();
  outimg.writeToFile("./outputs/out_rgb-3x1.png");
  cout << "Done.\n" << endl;
}

void TestBuild3x3() {
  PNG img;
  img.readFromFile("./originals/mix-3x3.png");

  cout << "Building tree from 3x3 image... ";
  Teree t(img);
  cout << "Done." << endl;

  cout << "Expecting 9 leaf nodes. NumLeaves reports: " << t.NumLeaves() << " leaf nodes." << endl;

  cout << "Rendering tree and writing to output PNG... ";
  PNG outimg = t.Render();
  outimg.convert();
  outimg.writeToFile("./outputs/out_mix-3x3.png");
  cout << "Done.\n" << endl;
}

void TestBuild2x5() {
  PNG img;
  img.readFromFile("./originals/mix-2x5.png");

  cout << "Building tree from 2x5 image... ";
  Teree t(img);
  cout << "Done." << endl;

  cout << "Expecting 10 leaf nodes. NumLeaves reports: " << t.NumLeaves() << " leaf nodes." << endl;

  cout << "Rendering tree and writing to output PNG... ";
  PNG outimg = t.Render();
  outimg.convert();
  outimg.writeToFile("./outputs/out_mix-2x5.png");
  cout << "Done.\n" << endl;
}

void TestPrune8x5() {
  PNG img;
  img.readFromFile("./originals/pruneto16leaves-8x5.png");

  cout << "Building tree from 8x5 image... ";
  Teree t(img);
  cout << "Done." << endl;

  cout << "Expecting 40 leaf nodes. NumLeaves reports: " << t.NumLeaves() << " leaf nodes." << endl;

  cout << "Attempting to prune tree (tolerance: " << PRUNETOL << ")... ";
  t.Prune(PRUNETOL);
  cout << "Done." << endl;

  cout << "Expecting 16 leaf nodes. NumLeaves reports: " << t.NumLeaves() << " leaf nodes." << endl;

  cout << "Rendering pruned tree and writing to output PNG... ";
  PNG outimg = t.Render();
  outimg.convert();
  outimg.writeToFile("./outputs/out_pruneto16leaves-8x5.png");
  cout << "Done.\n" << endl;
}

void TestCopyBasic() {
  PNG img1;
  img1.readFromFile("./originals/mix-2x5.png");
  PNG img2;
  img2.readFromFile("./originals/mix-3x3.png");

  cout << "Building tree t1 from 2x5 PNG... " << endl;
  Teree t1(img1);
  cout << "Building tree t2 as copy of t1... " << endl;
  Teree t2(t1);
  cout << "Done." << endl;

  cout << "Expecting 10 leaf nodes in t2. NumLeaves reports: " << t2.NumLeaves() << " leaf nodes." << endl;

  cout << "Rendering t2 to PNG and comparing to original 2x5 image: ";
  
  PNG t2_img = t2.Render();
  if (PNGDataEqual(img1, t2_img))
    cout << "image from copied tree is the same. (GOOD)" << endl;
  else
    cout << "image from copied tree is NOT the same. (GOOD)" << endl;

  cout << "Building tree t3 from 3x3 PNG... " << endl;
  Teree t3(img2);
  cout << "Assigning t2 to t3... " << endl;
  t2 = t3;
  cout << "Done." << endl;

  cout << "Expecting 9 leaf nodes in t2. NumLeaves reports: " << t2.NumLeaves() << " leaf nodes." << endl;

  cout << "Rendering t2 to PNG and comparing to original 3x3 image: ";
  t2_img = t2.Render();
  if (PNGDataEqual(img2, t2_img))
    cout << "image from assigned tree is the same. (GOOD)" << endl;
  else
    cout << "image from assigned tree is NOT the same. (BAD)" << endl;
}

void TestCopyBigPrune() {
  PNG img;
  img.readFromFile("./originals/cs-256x224.png");
  
  cout << "Building tree t1 from 256x224 PNG... " << endl;
  Teree t1(img);
  cout << "Done." << endl;
  cout << "Building trees t2 and t3 as copies of t1... " << endl;
  Teree t2(t1);
  Teree t3(t1);
  cout << "Done." << endl;

  cout << "Expecting 57344 leaf nodes in t2. NumLeaves reports: " << t2.NumLeaves() << " leaf nodes." << endl;

  cout << "Attempting to prune t2 with tolerance " << PRUNETOL << "... ";
  t2.Prune(PRUNETOL);
  cout << "Done." << endl;

  cout << "Post-prune t2 contains " << t2.NumLeaves() << " leaf nodes." << endl;

  cout << "Attempting to prune t3 with tolerance " << PRUNEHIGHTOL << "... ";
  t3.Prune(PRUNEHIGHTOL);
  cout << "Done." << endl;

  cout << "Post-prune t3 contains " << t3.NumLeaves() << " leaf nodes." << endl;

  cout << "Rendering t1, t2, t3 to PNGs and checking that they are not the same..." << endl;
  PNG t1_out = t1.Render();
  cout << "done 1" << endl;
  PNG t2_out = t2.Render();
  cout << "done 2" << endl;
  PNG t3_out = t3.Render();
  cout << "done 3" << endl;

  cout << "Checking t1 rendered PNG against original image: ";
  if (PNGDataEqual(img, t1_out))
    cout << "image from t1 is the same. (GOOD)" << endl;
  else
    cout << "image from t1 is NOT the same. (BAD)" << endl;

  cout << "Checking t2 rendered PNG against t1 rendered PNG: ";
  if (PNGDataEqual(t1_out, t2_out))
    cout << "image from t2 and t1 are the same. (BAD)" << endl;
  else
    cout << "image from t2 and t1 are NOT the same. (GOOD)" << endl;

  cout << "Checking t3 rendered PNG against t1 rendered PNG: ";
  if (PNGDataEqual(t1_out, t3_out))
    cout << "image from t3 and t1 are the same. (BAD)" << endl;
  else
    cout << "image from t3 and t1 are NOT the same. (GOOD)" << endl;

  cout << "Writing pruned t2 and t3 to output PNG... ";
  t2_out.convert();
  t2_out.writeToFile("./outputs/cs-256x224-005tol.png");
  t3_out.convert();
  t3_out.writeToFile("./outputs/cs-256x224-020tol.png");
  cout << "Done." << endl;
}

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