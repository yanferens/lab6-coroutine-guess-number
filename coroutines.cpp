#include "coroutines.h"
#include <random>

using namespace std;

GuessGame keepSecret(shared_ptr<int> guess, unsigned seed) {
    mt19937 engine{seed};
    int secret = uniform_int_distribution<int>{1, 100}(engine);
    while (true) {
        int answer = (*guess > secret) - (*guess < secret);
        if (answer == 0) {
            co_return answer;
        }
        co_yield answer;
    }
}
