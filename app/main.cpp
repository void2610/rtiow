#include <iostream>
#include <rtmath/color.hpp>
#include <rtmath/vec.hpp>

int main() {
  const int image_width = 256;
  const int image_height = 256;

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remainning: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto r = double(i) / (image_width - 1);
      auto g = double(j) / (image_height - 1);
      auto b = 0.25;

      rtmath::color c(double(i) / (image_width - 1),
                      double(j) / (image_width - 1), 0.25);
      write_color(std::cout, c);
    }
  }
  std::cerr << "\nDone.\n";
}
