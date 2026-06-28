#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define GLFW_INCLUDE_NONE // GLFWによるGLヘッダ取り込みを止め、二重インクルードを避ける
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>
#include <memory>
#include <rtcore/camera.hpp>
#include <rtcore/hittable.hpp>
#include <rtcore/hittable_list.hpp>
#include <rtcore/material.hpp>
#include <rtcore/moving_sphere.hpp>
#include <rtcore/sphere.hpp>
#include <rtcore/texture.hpp>
#include <rtimage/writer.hpp>
#include <rtmath/ray.hpp>
#include <rtmath/utils.hpp>
#include <rtmath/vec.hpp>
#include <thread>

using namespace rtmath;
using namespace rtcore;
using std::make_shared;

struct render_config {
  int image_width;
  int image_height;
  float aspect_ratio;
  int samples_per_pixel;
  int max_depth;
};

// レイの動きを演算して色を返す。当たらなかったら白-水色のグラデーション(背景色)を返す
color ray_color(const ray &r, const hittable &world, int depth) {
  // 反射回数が一定よりも多くなったら、その時点で追跡をやめる
  if (depth <= 0)
    return color(0, 0, 0);

  if (auto rec = world.hit(r, 0.001, infinity)) {
    if (auto sr = rec->mat_ptr->scatter(r, *rec))
      return sr->attenuation * ray_color(sr->scattered, world, depth - 1);
    return color(0, 0, 0);
  }

  vec3 unit_direction = rtmath::unit_vector(r.direction());
  auto t = 0.5 * unit_direction.y() + 1.0;
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list earth() {
  auto tex = make_shared<image_texture>("earthmap.jpg");
  auto surface = make_shared<lambertian>(tex);
  auto globe = make_shared<sphere>(point3(0, 0, 0), 2, surface);
  return hittable_list(globe);
}

hittable_list random_scene() {
  hittable_list world;

  auto checker =
      make_shared<checker_texture>(make_shared<solid_color>(0.2, 0.3, 0.1),
                                   make_shared<solid_color>(0.9, 0.9, 0.9));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                make_shared<lambertian>(checker)));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<material> sphere_material;
        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material =
              make_shared<lambertian>(make_shared<solid_color>(albedo));
          auto center2 = center + vec3(0, random_double(0, 0.5), 0);
          world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2,
                                               sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
  auto material2 =
      make_shared<lambertian>(make_shared<solid_color>(color(0.4, 0.2, 0.1)));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  return world;
}

hittable_list two_spheres() {
  hittable_list objects;

  auto checker =
      make_shared<checker_texture>(make_shared<solid_color>(0.2, 0.3, 0.1),
                                   make_shared<solid_color>(0.9, 0.9, 0.9));

  objects.add(make_shared<sphere>(point3(0, -10, 0), 10,
                                  make_shared<lambertian>(checker)));
  objects.add(make_shared<sphere>(point3(0, 10, 0), 10,
                                  make_shared<lambertian>(checker)));

  return objects;
}

hittable_list two_perlin_spheres() {
  hittable_list objects;

  auto pertext = make_shared<noise_texture>(5);
  objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                  make_shared<lambertian>(pertext)));
  objects.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                  make_shared<lambertian>(pertext)));

  return objects;
}

// レンダリング済み画像をOpenGLテクスチャ化し、ImGuiウィンドウに表示する
void show_viewer(const rtimage::image &img) {
  glfwSetErrorCallback([](int code, const char *desc) {
    std::cerr << "GLFW error " << code << ": " << desc << '\n';
  });
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return;
  }

  // macOSはコアプロファイルのForward Compatが必須
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "raytracer viewer", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // 描画結果をRGBテクスチャとしてアップロード
  GLuint texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 行幅がwidth*3で4バイト境界に揃わないため
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width(), img.height(), 0, GL_RGB,
               GL_UNSIGNED_BYTE, img.data());

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Render");
    ImGui::Text("%d x %d", img.width(), img.height());
    ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(texture)),
                 ImVec2(static_cast<float>(img.width()),
                        static_cast<float>(img.height())));
    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  glDeleteTextures(1, &texture);
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void render_row(rtimage::image &img, int j, const camera &cam,
                const hittable &world, const render_config &cfg) {
  for (int i = 0; i < cfg.image_width; ++i) {
    color pixel_color(0, 0, 0);
    for (int s = 0; s < cfg.samples_per_pixel; ++s) {
      auto u = (i + random_double()) / (cfg.image_width - 1);
      auto v = (j + random_double()) / (cfg.image_height - 1);
      auto r = cam.get_ray(u, v);
      pixel_color += ray_color(r, world, cfg.max_depth);
    }
    img.set_pixel(i, cfg.image_height - 1 - j, pixel_color,
                  cfg.samples_per_pixel);
  }
}

int main() {
  const unsigned thread_count =
      std::max(1u, std::thread::hardware_concurrency());
  std::atomic<int> next_row{0};
  std::atomic<int> completed{0};

  const int image_width = 384;

  const render_config cfg{384, static_cast<int>(image_width / (16.0 / 9.0)),
                          (16.0 / 9.0), 50, 10};

  point3 lookfrom(13, 2, 3);
  point3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.0;
  camera cam(lookfrom, lookat, vup, 20, cfg.aspect_ratio, aperture,
             dist_to_focus, 0.0, 1.0);

  auto world = earth();

  rtimage::image img(image_width, cfg.image_height);

  auto worker = [&] {
    int j;
    while ((j = next_row.fetch_add(1)) < cfg.image_height) {
      render_row(img, j, cam, world, cfg);
      std::cerr << "\r" << completed.fetch_add(1) + 1 << " / "
                << cfg.image_height << std::flush;
    }
  };

  {
    std::vector<std::jthread> pool;
    for (unsigned t = 0; t < thread_count; ++t)
      pool.emplace_back(worker);
  } // このスコープを抜けるとjthreadが全部自動joinされる

  std::cerr << "\nDone.\n";

  if (!img.save("out.png", rtimage::format::png))
    std::cerr << "Failed to save out.png\n";

  show_viewer(img);
}
