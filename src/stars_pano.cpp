/**
 * Render points of starfield to equirectangular output images
 *  g++ -std=c++17 stars_pano.cpp -lopencv_highgui -lopencv_core
 */

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <random>
#include <vector>

struct StarPoint
{
  StarPoint(const double x = 0.0,
            const double y = 0.0,
            const double z = 0.0,
            const double brightness = 0.0) :
    x_(x),
    y_(y),
    z_(z),
    brightness_(brightness)
  {

  }

  double x_;
  double y_;
  double z_;
  double brightness_;
};

class StarField
{
public:
  StarField(
      double field_size,
      double image_width, double image_height, size_t num_stars) :
    field_size_(field_size),
    image_width_(image_width),
    image_height_(image_height),
    num_stars_(num_stars),
    random_distribution_(-0.5, 0.5)
  {

  }

  void generate()
  {
    stars_.resize(num_stars_);
    for (size_t i = 0; i < num_stars_; ++i) {
      const auto x = getRandom() * field_size_;
      const auto y = getRandom() * field_size_;
      const auto z = getRandom() * field_size_;
      const auto base_brightness = getRandom() + 1.3;
      const auto brightness = base_brightness * base_brightness;
      stars_[i] = StarPoint(x, y, z, base_brightness);
    }
  }

  cv::Mat render(double view_x = 0.0, double view_y = 0.0, double view_z = 0.0)
  {
    cv::Mat image(cv::Size(image_width_, image_height_), CV_8UC4, cv::Scalar::all(0));

    return image;
  }

private:
  const double field_size_ = 100.0;
  const double image_width_;
  const double image_height_;
  const size_t num_stars_;

  std::vector<StarPoint> stars_;

  // https://stackoverflow.com/questions/686353/random-float-number-generation
  std::default_random_engine random_engine_;
  std::uniform_real_distribution<> random_distribution_;
  double getRandom()
  {
    return random_distribution_(random_engine_);
  }
};

int main(int argn, char** argv)
{

  StarField star_field(100.0, 1000, 500, 1000);

  star_field.generate();
  cv::Mat image = star_field.render();
  cv::imshow("star field", image);
  cv::waitKey(0);

  return 0;
}
