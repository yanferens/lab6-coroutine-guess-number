#pragma once
#include <coroutine>
#include <memory>

class GuessGame {
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        int answer = 0;
        std::shared_ptr<int> guess;

        promise_type(std::shared_ptr<int> question, unsigned seed);
        GuessGame get_return_object();
        std::suspend_always initial_suspend();
        std::suspend_always final_suspend() noexcept;
        std::suspend_always yield_value(int value);
        void return_value(int value);
        void unhandled_exception();
    };

    GuessGame(GuessGame const &) = delete;
    GuessGame &operator=(GuessGame const &) = delete;
    GuessGame(GuessGame &&other);
    ~GuessGame();

    int ask(int guess);
    bool done() const;

private:
    GuessGame(handle h);
    handle coro;
};
