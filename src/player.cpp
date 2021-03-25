#include "player.h"

Player::Player()
{
  tag_ = -1;
  direction_ = 0;
  speed_ = 0;
  score_ = 0;
  playerRect_ = { 0,220,20,80 };
}

Player::Player(const Player& other)
{
  tag_ = other.tag_;
  direction_ = other.direction_;
  speed_ = other.speed_;
  playerRect_ = other.playerRect_;
  score_ = other.score_;
}

void Player::init(uint32_t player_tag)
{
  tag_ = player_tag;
  speed_ = 5;
}

void Player::setSpeed(float speed)
{
  speed_ = speed;
}

int32_t Player::getTag()
{
  return tag_;
}

void Player::playerController(SDL_Event event, SDL_Keycode up, SDL_Keycode down) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == up) {
      direction_ = -1;
    }
    else if (event.key.keysym.sym == down) {
      direction_ = 1;
    }
  }
  else if (event.type == SDL_KEYUP) direction_ = 0;
}

void Player::setLimits(int height) {
  if (playerRect_.y <= -playerRect_.h)
    playerRect_.y += height + playerRect_.h;
  else if (playerRect_.y >= height)
    playerRect_.y -= (height + playerRect_.h);
}

void Player::setPosition(int x, int y) {
  playerRect_.x = x;
  playerRect_.y = y;
}

void Player::setPosition(Player other)
{
  playerRect_.x = other.playerRect_.x;
  playerRect_.y = other.playerRect_.y;
}

void Player::movePosition() {
  playerRect_.y += direction_ * speed_;
}

void Player::draw(SDL_Renderer* rend) {
  SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
  SDL_RenderFillRect(rend, &playerRect_);
}

int32_t Player::getHeight()
{
  return playerRect_.h;
}

int32_t Player::getWidth()
{
  return playerRect_.w;
}

