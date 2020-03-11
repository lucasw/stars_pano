/**
 * Render points of starfield to equirectangular output images
 *  g++ -std=c++17 stars_pano.cpp -lopencv_highgui -lopencv_core
 */

#include <iostream>
#include <cmath>
// #include <numbers>  // C++ 20
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
      size_t image_width, size_t image_height, size_t num_stars) :
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

    for (const auto& star : stars_) {
      const double dx = star.x_ - view_x;
      const double dy = star.y_ - view_y;
      const double dz = star.z_ - view_z;

      // TODO(lucasw) optionally make a looping effect where every point repeats
      // into infinity, but the same point won't be visible twice - just render
      // the closest instance.

      // need to get latitude and longitude to plot a point
      // -> altitude, azimuth
      // TODO(lucasw) make an atan2 lookup table accurate to 1.0/image_width_ tau,
      // make it also use square of distance rather than distance
      const double dist2 = dx * dx + dy * dy + dz * dz;
      if (dist2 == 0.0) {
        continue;
      }
      const double dist = std::sqrt(dist2);
      const double dxn = dx / dist;
      const double dyn = dy / dist;
      const double dzn = dz / dist;
      const double xyn_dist = std::sqrt(dxn * dxn + dyn * dyn);
      const double altitude = std::atan2(dzn, xyn_dist) / (M_PI) + 0.5;
      const double azimuth = std::atan2(dyn, dxn) / (M_PI * 2.0) + 0.5;

      // const double image_x = std::fmod(azimuth * image_width_, image_width_);
      // const double image_y = std::fmod(altitude * image_height_, image_height_);
      const double image_x = azimuth * image_width_;
      const double image_y = altitude * image_height_;
      const size_t pix_x = static_cast<int>(image_x) % image_width_;
      const size_t pix_y = static_cast<int>(image_y) % image_height_;

      // TODO(lucasw) dim with distance
      const double sc = field_size_ * 0.2;
      const double fr = (sc * sc) / dist2;
      double intensity = 1.0 * fr;
      if (intensity > 1.0) {
        // TODO(lucasw) maybe draw a bigger circle instead
        intensity = 1.0;
      }
      const unsigned char br = intensity * 255;
      if (br > 0) {
        // TODO(lucasw) don't draw a darker pixel on top of a lighter
        image.at<cv::Vec4b>(pix_y, pix_x) = cv::Vec4b(br, br, br, 255);
      }
    }

    return image;
  }

  void animate()
  {
    double x = 0.0;
    do {
      cv::Mat image = render(x);
      cv::imshow("star field", image);
      x += 0.05;
    } while ('q' != cv::waitKey(10));
  }

private:
  const double field_size_ = 100.0;
  const size_t image_width_;
  const size_t image_height_;
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
  StarField star_field(100.0, 1600, 800, 2000);

  star_field.generate();
  star_field.animate();

  return 0;
}
