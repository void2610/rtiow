#include "rtmath/vec.hpp"
#include <string>
#include <vector>
namespace rtimage {
enum class format { png, ppm };

class image {
public:
  image(int width, int height);

  void set_pixel(int x, int y, const rtmath::color &c, int samples_per_pixel);
  bool save(const std::string &path, format fmt) const;

private:
  int width_, height_;
  std::vector<unsigned char> pixels_;
};
} // namespace rtimage
