#ifndef WIN32
#include <unistd.h>
#endif

#include <random>

#include "png++/png.hpp"
#include <omp.h>

double doSomeWork(int x, int y) {
  std::random_device ranDev;
  std::mt19937 prng(ranDev());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  // this is where you normally do lots of work to determine the
  // pixel color, so we will approximate it by a random wait...
  //
  int rand_num = (int)floor(dist(prng) * 50000 + 25000);
  double avg = 0.0;
  for (int n = 0; n < rand_num; ++n) {
    avg += n;
  }
  avg = avg / (double)rand_num;
  return avg;
}

int main(int argc, char *argv[]) {
  int w = 1000, h = 1000;
  png::image<png::rgb_pixel> imData(w, h);

  const png::rgb_pixel palette[] = {
      png::rgb_pixel(255, 0, 0),   // Red   (Thread 0)
      png::rgb_pixel(0, 255, 0),   // Green (Thread 1)
      png::rgb_pixel(0, 0, 255),   // Blue  (Thread 2)
      png::rgb_pixel(255, 255, 0), // Yellow(Thread 3)
      png::rgb_pixel(255, 0, 255), // Magenta
      png::rgb_pixel(0, 255, 255), // Cyan
      png::rgb_pixel(255, 128, 0), // Orange
      png::rgb_pixel(128, 0, 255)  // Purple
  };
  const int num_colors = sizeof(palette) / sizeof(palette[0]);

#pragma omp parallel for
  for (auto y = 0; y < imData.get_height(); ++y) {
    int thread_id = omp_get_thread_num();
    png::rgb_pixel thread_color = palette[thread_id % num_colors];

    for (auto x = 0; x < imData.get_width(); ++x) {
      double v = doSomeWork(x, y);
      imData[y][x] = thread_color;
    }
  }

  imData.write("ompImage.png");
  exit(EXIT_SUCCESS);
}
