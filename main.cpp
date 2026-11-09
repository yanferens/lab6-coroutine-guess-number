#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <random>
#include <string>

#include "coroutines.h"

using namespace std;

using Client = function<optional<int>(int)>;

struct Options {
    bool manual = false;
    unsigned seed = 0;
};

Client binarySearchClient() {
    return [low = 1, high = 100, last = 0](int answer) mutable -> optional<int> {
        if (answer < 0) {
            low = last + 1;
        }
        if (answer > 0) {
            high = last - 1;
        }
        if (low > high) {
            return nullopt;
        }
        last = low + (high - low) / 2;
        return last;
    };
}

bool parseInt(const string &text, int &value) {
    try {
        size_t used = 0;
        value = stoi(text, &used);
        return used == text.size();
    } catch (const exception &) {
        return false;
    }
}

Client consoleClient() {
    return [](int) -> optional<int> {
        string line;
        int guess = 0;
        cout << "Your guess: ";
        while (getline(cin, line)) {
            if (parseInt(line, guess)) {
                return guess;
            }
            cout << "Invalid input '" << line << "', expected an integer" << endl;
            cout << "Your guess: ";
        }
        return nullopt;
    };
}

bool isNumber(const string &text) {
    if (text.empty() || text.size() > 10) {
        return false;
    }
    return all_of(text.begin(), text.end(), [](unsigned char c) { return isdigit(c) != 0; });
}

bool parseSeed(const string &text, unsigned &seed) {
    if (!isNumber(text)) {
        return false;
    }
    unsigned long long value = stoull(text);
    if (value > numeric_limits<unsigned>::max()) {
        return false;
    }
    seed = static_cast<unsigned>(value);
    return true;
}

bool parseOptions(int argc, char *argv[], Options &options) {
    options.seed = random_device{}();
    if (argc > 3) {
        return false;
    }
    string mode = argc > 1 ? argv[1] : "auto";
    if (mode != "auto" && mode != "manual") {
        return false;
    }
    options.manual = mode == "manual";
    if (argc == 3) {
        return parseSeed(argv[2], options.seed);
    }
    return true;
}

void printUsage() {
    cerr << "Usage: lab6 [auto|manual] [seed]\n"
         << "  auto    binary search client plays (default)\n"
         << "  manual  guesses are read from stdin, one integer per line\n"
         << "  seed    non-negative integer up to 4294967295 for the secret number\n";
}

void printAttempt(int attempt, int guess, int answer) {
    cout << "Attempt " << attempt << ": guess " << guess << " -> " << answer << endl;
}

void play(GuessGame &game, const Client &nextGuess) {
    int attempts = 0;
    int answer = 0;
    while (!game.done()) {
        optional<int> guess = nextGuess(answer);
        if (!guess) {
            cout << "Game abandoned after " << attempts << " attempts" << endl;
            return;
        }
        answer = game.ask(*guess);
        printAttempt(++attempts, *guess, answer);
    }
    cout << "Number guessed in " << attempts << " attempts" << endl;
}

void runGame(const Options &options) {
    cout << "Seed: " << options.seed << endl;
    auto game = keepSecret(make_shared<int>(0), options.seed);
    Client client = options.manual ? consoleClient() : binarySearchClient();
    play(game, client);
}

int main(int argc, char *argv[]) {
    Options options;
    if (parseOptions(argc, argv, options)) {
        runGame(options);
    } else {
        printUsage();
    }

}
