#include <rtimage/writer.hpp>

namespace rtimage {

void image::set_pixel(int x, int y, const rtmath::color &c,
                      int samples_per_pixel) {}
bool image::save(const std::string &path, format fmt) const { return false; }
} // namespace rtimage
