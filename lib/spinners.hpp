#ifndef SPINNERS_HPP
#define SPINNERS_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <chrono>
#include <thread>

namespace spinners {

    std::map<const char *, const char *> spinnerType = {{"dots",           "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏"},
                                                        {"dots2",          "⣾⣽⣻⢿⡿⣟⣯⣷"},
                                                        {"dots3",          "⠋⠙⠚⠞⠖⠦⠴⠲⠳⠓"},
                                                        {"dots4",          "⠄⠆⠇⠋⠙⠸⠰⠠⠰⠸⠙⠋⠇⠆"},
                                                        {"dots5",          "⠋⠙⠚⠒⠂⠂⠒⠲⠴⠦⠖⠒⠐⠐⠒⠓⠋"},
                                                        {"dots6",          "⠁⠉⠙⠚⠒⠂⠂⠒⠲⠴⠤⠄⠄⠤⠴⠲⠒⠂⠂⠒⠚⠙⠉⠁"},
                                                        {"dots7",          "⠈⠉⠋⠓⠒⠐⠐⠒⠖⠦⠤⠠⠠⠤⠦⠖⠒⠐⠐⠒⠓⠋⠉⠈"},
                                                        {"dots8",          "⠁⠁⠉⠙⠚⠒⠂⠂⠒⠲⠴⠤⠄⠄⠤⠠⠠⠤⠦⠖⠒⠐⠐⠒⠓⠋⠉⠈⠈"},
                                                        {"dots9",          "⢹⢺⢼⣸⣇⡧⡗⡏"},
                                                        {"dots10",         "⢄⢂⢁⡁⡈⡐⡠"},
                                                        {"dots11",         "⠁⠂⠄⡀⢀⠠⠐⠈"},
                                                        {"pipe",           "┤┘┴└├┌┬┐"},
                                                        {"star",           "✶✸✹✺✹✷"},
                                                        {"star2",          "+x*"},
                                                        {"flip",           "___-``'´-___"},
                                                        {"hamburger",      "☱☲☴"},
                                                        {"growVertical",   "▁▃▄▅▆▇▆▅▄▃"},
                                                        {"growHorizontal", "▏▎▍▌▋▊▉▊▋▌▍▎"},
                                                        {"balloon",        " .oO@* "},
                                                        {"balloon2",       ".oO°Oo."},
                                                        {"noise",          "▓▒░"},
                                                        {"bounce",         "⠁⠂⠄⠂"},
                                                        {"boxBounce",      "▖▘▝▗"},
                                                        {"boxBounce2",     "▌▀▐▄"},
                                                        {"triangle",       "◢◣◤◥"},
                                                        {"arc",            "◜◠◝◞◡◟"},
                                                        {"circle",         "◡⊙◠"},
                                                        {"squareCorners",  "◰◳◲◱"},
                                                        {"circleQuarters", "◴◷◶◵"},
                                                        {"circleHalves",   "◐◓◑◒"},
                                                        {"squish",         "╫╪"},
                                                        {"toggle",         "⊶⊷"},
                                                        {"toggle2",        "▫▪"},
                                                        {"toggle3",        "□■"},
                                                        {"toggle4",        "■□▪▫"},
                                                        {"toggle5",        "▮▯"},
                                                        {"toggle6",        "ဝ၀"},
                                                        {"toggle7",        "⦾⦿"},
                                                        {"toggle8",        "◍◌"},
                                                        {"toggle9",        "◉◎"},
                                                        {"toggle10",       "㊂㊀㊁"},
                                                        {"toggle11",       "⧇⧆"},
                                                        {"toggle12",       "☗☖"},
                                                        {"toggle13",       "=*-"},
                                                        {"arrow",          "←↖↑↗→↘↓↙"}};

    const char *getSpinner(const char *key) {
        auto search = spinnerType.find(key);
        if (search != spinnerType.end()) {
            return search->second;
        } else {
            search = spinnerType.find("dots");
            return search->second;
        }
    }

    class Spinner {
    public:
        Spinner() : interval(80), text(""), stop_spinner(false), symbols(getSpinner("dots")) {};

        Spinner(int _interval, std::string _text, const char *_symbols) : interval(_interval), text(_text),
                                                                          stop_spinner(false),
                                                                          symbols(getSpinner(_symbols)) {};

        ~Spinner() { stop(); };

        void setInterval(int _interval) { interval = _interval; }

        void setText(std::string _text) { text = _text; }

        void setSymbols(const char *_symbols) { symbols = getSpinner(_symbols); }

        void startSpinner() {
            int len = strlen(symbols) / 3;
            int i = 0;
            char ch[4];

            hideCursor();
            while (!stop_spinner) {
                i = (i >= (len - 1)) ? 0 : i + 1;
                strncpy(ch, symbols + i * 3, 3);
                std::cout << ch << " " << text << " \r";
                std::cout.flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }

            showCursor();
        }

        void start() {
            t = std::thread(&Spinner::startSpinner, this);
        }

        void stop() {
            stop_spinner = true;
            if (t.joinable()) {
                t.join();
            }
        }

    private:
        int interval;
        std::string text;
        bool stop_spinner;
        const char *symbols;
        std::thread t;

        void hideCursor() {
            std::cout << "\u001b[?25l";
        }

        void showCursor() {
            std::cout << "\u001b[?25h";
        }
    };
}

#endif // SPINNERS_HPP
