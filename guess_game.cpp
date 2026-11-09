#include "guess_game.h"
#include <exception>
#include <stdexcept>

GuessGame::promise_type::promise_type(std::shared_ptr<int> question, unsigned)
    : guess(question) {}

GuessGame GuessGame::promise_type::get_return_object() {
    return GuessGame{handle::from_promise(*this)};
}

std::suspend_always GuessGame::promise_type::initial_suspend() {
    return {};
}

std::suspend_always GuessGame::promise_type::final_suspend() noexcept {
    return {};
}

std::suspend_always GuessGame::promise_type::yield_value(int value) {
    answer = value;
    return {};
}

void GuessGame::promise_type::return_value(int value) {
    answer = value;
}

void GuessGame::promise_type::unhandled_exception() {
    std::terminate();
}

GuessGame::GuessGame(handle h) : coro(h) {}

GuessGame::GuessGame(GuessGame &&other) : coro(other.coro) {
    other.coro = nullptr;
}

GuessGame::~GuessGame() {
    if (coro) {
        coro.destroy();
    }
}

bool GuessGame::done() const {
    return !coro || coro.done();
}

int GuessGame::ask(int guess) {
    if (done()) {
        throw std::logic_error("the game is already finished");
    }
    *coro.promise().guess = guess;
    coro.resume();
    return coro.promise().answer;
}
