#include <ctime>
#include <rtcore/camera.hpp>
#include <rtmath/ray.hpp>
#include <rtmath/utils.hpp>
#include <rtmath/vec.hpp>

namespace rtcore {
camera::camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov,
               double aspect_ratio, double aperture, double focus_dist,
               double t0, double t1) {
  auto theta = rtmath::degrees_to_radians(vfov);
  auto h = tan(theta / 2);
  auto viewport_height = 2.0 * h;
  auto viewport_width = aspect_ratio * viewport_height;

  w = unit_vector(lookfrom - lookat);
  u = unit_vector(cross(vup, w));
  v = cross(w, u);

  origin = lookfrom;
  horizontal = focus_dist * viewport_width * u;
  vertical = focus_dist * viewport_height * v;
  lower_left_corner =
      origin - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;

  lens_radius = aperture / 2;
  time0 = t0;
  time1 = t1;
}

rtmath::ray camera::get_ray(double s, double t) const {
  vec3 rd = lens_radius * rtmath::random_in_unit_disk<double, 3>();
  vec3 offset = u * rd.x() + v * rd.y();

  return rtmath::ray(origin + offset,
                     lower_left_corner + s * horizontal + t * vertical -
                         origin - offset,
                     rtmath::random_double(time0, time1));
}
} // namespace rtcore
