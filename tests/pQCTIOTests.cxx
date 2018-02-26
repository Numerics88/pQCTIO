// Copyright (c) Numerics88
// See LICENSE for details.

#include "pQCTIO/pQCTIO.h"

#include <gtest/gtest.h>
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

char* test_dir = 0;

// Create a test fixture class.
class pQCTIOTests : public ::testing::Test
{};

// --------------------------------------------------------------------
// test implementations

// Useful macro for testing pixels
#define test_pixel(x, y, value) \
  ASSERT_EQ(value, image_data[reader.PicMatrixX*y + x]);

TEST_F (pQCTIOTests, AllTestFilesExist)
{
  std::vector<std::string> files;
  files.push_back("I0020002.M01");
  files.push_back("I0020027.M01");
  for (int i = 0; i < files.size(); ++i) {
    boost::filesystem::path fin = boost::filesystem::path(test_dir) / files.at(i);
    ASSERT_TRUE(boost::filesystem::exists( fin.string() ));
  }
}

TEST_F (pQCTIOTests, TestI0020002)
{
  // Read in the file
  boost::filesystem::path fin = boost::filesystem::path(test_dir) / "I0020002.M01";
  pQCTIO::pQCTFile reader;
  reader.filename = fin.string();
  ASSERT_NO_THROW(reader.ReadImageInfo());
  std::vector<short> image_data (reader.number_of_voxels);
  ASSERT_NO_THROW(reader.ReadImageData(image_data.data(),reader.number_of_voxels));

  // Test input parameters
  ASSERT_EQ(fin.string(), reader.filename);
  ASSERT_DOUBLE_EQ(0.5f, reader.VoxelSize);
  ASSERT_DOUBLE_EQ(370.0f, reader.ObjLen);
  ASSERT_EQ(294849, reader.number_of_voxels);
  ASSERT_EQ(543, reader.PicMatrixX);
  ASSERT_EQ(543, reader.PicMatrixY);
  ASSERT_EQ(39, reader.PicX0);
  ASSERT_EQ(39, reader.PicY0);
  ASSERT_EQ(20002, reader.PatNo);
  ASSERT_EQ(20007, reader.PatMeasNo);
  ASSERT_EQ(19840901, reader.PatBirth);
  ASSERT_EQ(20171102, reader.MeasDate);

  ASSERT_EQ("TEST LHANTOM TOM OB_OFFSET_2000_3000", reader.PatName);
  ASSERT_EQ("\"CT-X\"TRAININGA20002C01( -250657909 -958362185)", reader.MeasInfo);
  ASSERT_EQ("XCT3010.TYP", reader.Device);
  ASSERT_EQ("TEST_001", reader.PatID);

  // Test a few pixels. Values were read out through ITKSnap
  test_pixel(266, 236, 675);
  test_pixel(100, 100, -4);
  test_pixel(260, 260, 391);
  test_pixel(348, 294, 224);
}

TEST_F (pQCTIOTests, TestI0020027)
{
  // Read in the file
  boost::filesystem::path fin = boost::filesystem::path(test_dir) / "I0020027.M01";
  pQCTIO::pQCTFile reader;
  reader.filename = fin.string();
  ASSERT_NO_THROW(reader.ReadImageInfo());
  std::vector<short> image_data (reader.number_of_voxels);
  ASSERT_NO_THROW(reader.ReadImageData(image_data.data(),reader.number_of_voxels));

  // Test input parameters
  ASSERT_EQ(fin.string(), reader.filename);
  ASSERT_DOUBLE_EQ(0.4, reader.VoxelSize);
  ASSERT_DOUBLE_EQ(400.0f, reader.ObjLen);
  ASSERT_EQ(462400, reader.number_of_voxels);
  ASSERT_EQ(680, reader.PicMatrixX);
  ASSERT_EQ(680, reader.PicMatrixY);
  ASSERT_EQ(48, reader.PicX0);
  ASSERT_EQ(48, reader.PicY0);
  ASSERT_EQ(20021, reader.PatNo);
  ASSERT_EQ(20007, reader.PatMeasNo);
  ASSERT_EQ(19930101, reader.PatBirth);
  ASSERT_EQ(20180130, reader.MeasDate);

  ASSERT_EQ("TEST_002 PHANTOM  R_03FSET_2000_3000", reader.PatName);
  ASSERT_EQ("\"CT-X\"COMMENTXXY20027C01(  748015661  708055169)", reader.MeasInfo);
  ASSERT_EQ("XCT3010.TYP", reader.Device);
  ASSERT_EQ("TEST_0013_03", reader.PatID);

  // Test a few pixels. Values were read out through ITKSnap
  test_pixel(290, 338, 1027);
  test_pixel(338, 290, 6);
  test_pixel(679, 487, -21);
  test_pixel(345, 425, 430);
}

// --------------------------------------------------------------------
// main: custom in order to handle argument.

int main (int argc, char** argv)
{
  ::testing::InitGoogleTest (&argc, argv);
  if (argc != 2)
  {
    std::cerr << "Need to specify location of test data" << std::endl;
    exit(-1);
  }
  test_dir = argv[1];
  return RUN_ALL_TESTS();
}