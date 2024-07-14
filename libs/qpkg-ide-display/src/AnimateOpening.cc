#include <Internal.hh>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <qpkg-ide-display/Layer.hh>
#include <random>
#include <thread>

#define PARTICLE_COUNT 400
#define ITERATIONS 200

int width = 1, height = 1;

struct Color {
  uint8_t r, g, b;

  Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) {}
};

struct Particle {
  float x, y;
  float xv, yv;
  Color c;

  Particle(size_t i = 0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 2.5);

    float velocity = dis(gen);

    x = 0.5 * width;
    y = 0.3 * height;

    xv = velocity * cos(i * ((2.0 * M_PI) / PARTICLE_COUNT));
    yv = velocity * sin(i * ((2.0 * M_PI) / PARTICLE_COUNT));
    c = Color(rand() % 255, rand() % 255, rand() % 255);
  }

  void update() {
    x += xv;
    y += yv;
    yv += 0.05;

    if (x < 0 || x >= width) {
      xv = -xv;
    }
    if (y < 0 || y >= height) {
      yv = -yv;
    }
  }
};

static std::array<Particle, PARTICLE_COUNT> particles;

static void create_particles() {
  for (size_t i = 0; i < PARTICLE_COUNT; i++) {
    particles[i] = Particle(i);
  }
}

bool qpkg::ide::display::IDisplayLayer::do_animate_opening() {
  static bool has_created_particles = false;
  static size_t counter = 0;

  width = m_impl->width;
  height = m_impl->height;

  if (!has_created_particles) {
    create_particles();
    has_created_particles = true;
  }

  SDL_SetRenderDrawColor(m_impl->renderer, 2, 6, 23, 255);
  SDL_RenderClear(m_impl->renderer);

  for (size_t j = 0; j < PARTICLE_COUNT; j++) {
    particles[j].update();

    SDL_SetRenderDrawColor(m_impl->renderer, particles[j].c.r, particles[j].c.g, particles[j].c.b,
                           255);
    filledCircleRGBA(m_impl->renderer, particles[j].x, particles[j].y, 4, particles[j].c.r,
                     particles[j].c.g, particles[j].c.b, 255);
  }

  counter++;

  return counter < ITERATIONS;
}