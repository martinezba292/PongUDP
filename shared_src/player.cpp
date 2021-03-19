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

