#include "ball.h"
#include <algorithm>
#include "common_def.h"


Ball::Ball(int32_t speed, int32_t width, int32_t height) {
  ball_ = { kMax_width >> 1, kMax_height >> 1, width, height };
  speed_ = speed;
  direction_ = { 0.0f, 0.0f };
}

void Ball::collision(const SDL_Rect& player) {
  if (SDL_HasIntersection(&player, &ball_)) {
    float ball_center = (float)(ball_.y + (float)(ball_.h >> 1));
    float player_center = (float)(player.y + (float)(player.h >> 1));
    float offset = ball_center - player_center;
    float relative = offset / player.h;
    float max_angle = 1.047198f; //60º
    float angle = max_angle * relative;
    int32_t sgn = (0.0f < direction_.x) - (direction_.x < 0.0f);
    direction_.x = cosf(angle) * (-sgn);
    direction_.y = sinf(angle);
  }
}

SDL_Rect Ball::getRect()
{
  return ball_;
}

void Ball::update(std::vector<SDL_Rect> players) {
  ball_.x += int32_t(direction_.x * speed_);
  ball_.y += int32_t(direction_.y * speed_);

  for_each(players.begin(), players.end(), [this](auto input) { this->collision(input); });

  if (direction_.y > 0.0f && ball_.y >= (kMax_height - ball_.h)) {
    direction_.y = -direction_.y;
    return;
  }

  if (direction_.y < 0.0f && ball_.y <= 0)
    direction_.y = -direction_.y;
}

void Ball::reset()
{
  ball_.x = kMax_width >> 1;
  ball_.y = kMax_height >> 1;
  float rollx = -0.5f;
  float rolly = (rand() % 100 + 1) * 0.01f;
  direction_ = { rollx, rolly };
}

void Ball::draw(SDL_Renderer* render)
{
  SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
  SDL_RenderFillRect(render, &ball_);
}
