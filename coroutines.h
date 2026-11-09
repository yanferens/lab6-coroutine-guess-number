#pragma once
#include "guess_game.h"
#include <memory>

GuessGame keepSecret(std::shared_ptr<int> guess, unsigned seed);
