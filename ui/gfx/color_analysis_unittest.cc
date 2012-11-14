// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/color_analysis.h"

#include <vector>

#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/skia/include/core/SkColor.h"

namespace {

const unsigned char k1x1White[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
  0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53,
  0xde, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47,
  0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00,
  0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00,
  0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00,
  0x9a, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x07, 0x74,
  0x49, 0x4d, 0x45, 0x07, 0xdb, 0x02, 0x11, 0x15,
  0x16, 0x1b, 0xaa, 0x58, 0x38, 0x76, 0x00, 0x00,
  0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x43, 0x6f,
  0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x43, 0x72,
  0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77, 0x69,
  0x74, 0x68, 0x20, 0x47, 0x49, 0x4d, 0x50, 0x57,
  0x81, 0x0e, 0x17, 0x00, 0x00, 0x00, 0x0c, 0x49,
  0x44, 0x41, 0x54, 0x08, 0xd7, 0x63, 0xf8, 0xff,
  0xff, 0x3f, 0x00, 0x05, 0xfe, 0x02, 0xfe, 0xdc,
  0xcc, 0x59, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x49,
  0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

const unsigned char k1x3BlueWhite[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03,
  0x08, 0x02, 0x00, 0x00, 0x00, 0xdd, 0xbf, 0xf2,
  0xd5, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47,
  0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00,
  0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00,
  0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00,
  0x9a, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x07, 0x74,
  0x49, 0x4d, 0x45, 0x07, 0xdb, 0x02, 0x12, 0x01,
  0x0a, 0x2c, 0xfd, 0x08, 0x64, 0x66, 0x00, 0x00,
  0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x43, 0x6f,
  0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x43, 0x72,
  0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77, 0x69,
  0x74, 0x68, 0x20, 0x47, 0x49, 0x4d, 0x50, 0x57,
  0x81, 0x0e, 0x17, 0x00, 0x00, 0x00, 0x14, 0x49,
  0x44, 0x41, 0x54, 0x08, 0xd7, 0x63, 0xf8, 0xff,
  0xff, 0x3f, 0x13, 0x03, 0x03, 0x03, 0x03, 0x03,
  0xc3, 0x7f, 0x00, 0x1e, 0xfd, 0x03, 0xff, 0xde,
  0x72, 0x58, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x49,
  0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

const unsigned char k1x3BlueRed[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03,
  0x08, 0x02, 0x00, 0x00, 0x00, 0xdd, 0xbf, 0xf2,
  0xd5, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47,
  0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00,
  0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00,
  0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00,
  0x9a, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x07, 0x74,
  0x49, 0x4d, 0x45, 0x07, 0xdb, 0x02, 0x12, 0x01,
  0x07, 0x09, 0x03, 0xa2, 0xce, 0x6c, 0x00, 0x00,
  0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x43, 0x6f,
  0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x43, 0x72,
  0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77, 0x69,
  0x74, 0x68, 0x20, 0x47, 0x49, 0x4d, 0x50, 0x57,
  0x81, 0x0e, 0x17, 0x00, 0x00, 0x00, 0x14, 0x49,
  0x44, 0x41, 0x54, 0x08, 0xd7, 0x63, 0xf8, 0xcf,
  0xc0, 0xc0, 0xc4, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
  0xf0, 0x1f, 0x00, 0x0c, 0x10, 0x02, 0x01, 0x2c,
  0x8f, 0x8b, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x49,
  0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

class MockKMeanImageSampler : public color_utils::KMeanImageSampler {
 public:
  MockKMeanImageSampler() : current_result_index_(0) {
  }

  explicit MockKMeanImageSampler(const std::vector<int>& samples)
      : prebaked_sample_results_(samples),
        current_result_index_(0) {
  }

  virtual ~MockKMeanImageSampler() {
  }

  void AddSample(int sample) {
    prebaked_sample_results_.push_back(sample);
  }

  void Reset() {
    prebaked_sample_results_.clear();
    ResetCounter();
  }

  void ResetCounter() {
    current_result_index_ = 0;
  }

  virtual int GetSample(int width, int height) OVERRIDE {
    if (current_result_index_ >= prebaked_sample_results_.size()) {
      current_result_index_ = 0;
    }

    if (prebaked_sample_results_.empty()) {
      return 0;
    }

    return prebaked_sample_results_[current_result_index_++];
  }

 protected:
  std::vector<int> prebaked_sample_results_;
  size_t current_result_index_;
};

} // namespace

class ColorAnalysisTest : public testing::Test {
};

TEST_F(ColorAnalysisTest, CalculatePNGKMeanAllWhite) {
  MockKMeanImageSampler test_sampler;
  test_sampler.AddSample(0);

  scoped_refptr<base::RefCountedBytes> png(
      new base::RefCountedBytes(
          std::vector<unsigned char>(
              k1x1White,
              k1x1White + sizeof(k1x1White) / sizeof(unsigned char))));

  SkColor color = CalculateKMeanColorOfPNG(png, 100, 600, test_sampler);

  EXPECT_EQ(color, SK_ColorWHITE);
}

TEST_F(ColorAnalysisTest, CalculatePNGKMeanIgnoreWhite) {
  MockKMeanImageSampler test_sampler;
  test_sampler.AddSample(0);
  test_sampler.AddSample(1);
  test_sampler.AddSample(2);

  scoped_refptr<base::RefCountedBytes> png(
     new base::RefCountedBytes(
         std::vector<unsigned char>(
             k1x3BlueWhite,
             k1x3BlueWhite + sizeof(k1x3BlueWhite) / sizeof(unsigned char))));

  SkColor color = CalculateKMeanColorOfPNG(png, 100, 600, test_sampler);

  EXPECT_EQ(color, SkColorSetARGB(0xFF, 0x00, 0x00, 0xFF));
}

TEST_F(ColorAnalysisTest, CalculatePNGKMeanPickMostCommon) {
  MockKMeanImageSampler test_sampler;
  test_sampler.AddSample(0);
  test_sampler.AddSample(1);
  test_sampler.AddSample(2);

  scoped_refptr<base::RefCountedBytes> png(
     new base::RefCountedBytes(
         std::vector<unsigned char>(
             k1x3BlueRed,
             k1x3BlueRed + sizeof(k1x3BlueRed) / sizeof(unsigned char))));

  SkColor color = CalculateKMeanColorOfPNG(png, 100, 600, test_sampler);

  EXPECT_EQ(color, SkColorSetARGB(0xFF, 0xFF, 0x00, 0x00));
}