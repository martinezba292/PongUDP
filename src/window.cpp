#include <window.h>

Window* Window::internal_data_ = nullptr;

Window& Window::getInternalData() {
  if (internal_data_ == NULL) {
    internal_data_ = new Window();
  }
  return *internal_data_;
}

Window::Window() {}

Window::~Window() {
  SDL_DestroyRenderer(render_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void Window::init(int window_width, int window_height, Uint8 r, Uint8 g, Uint8 b) {
  Window::initVideo();
  Window::setWindowColor(r, g, b);
  Window::windowCreate(window_width, window_height);
  Window::createRenderer();
}

void Window::init() {
  Window::init(800, 600);
}

void Window::setWindowColor(Uint8 r, Uint8 g, Uint8 b) {
  window_color_.r = r;
  window_color_.g = g;
  window_color_.b = b;
  window_color_.a = SDL_ALPHA_OPAQUE;
}

int Window::initVideo() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }
  return 0;
}

int Window::windowCreate(int width, int height) {

  window_ = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window_) {
    printf("error creating window %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  return 0;
}

int Window::createRenderer() {
  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  render_ = SDL_CreateRenderer(window_, -1, render_flags);
  if (!render_) {
    printf("error creating renderer %s\n", SDL_GetError());
    SDL_DestroyWindow(window_);
    SDL_Quit();
    return 1;
  }
  return 0;
}

void Window::clearWindow() {
  SDL_SetRenderDrawColor(render_, window_color_.r, window_color_.g,
    window_color_.b, window_color_.a);
  SDL_RenderClear(render_);
}

void Window::renderDraw() {
  SDL_RenderPresent(render_);
}

void Window::destroy()
{
  if (internal_data_) {
    delete(internal_data_);
    internal_data_ = nullptr;
  }
}
