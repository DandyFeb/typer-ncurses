#include <string>
#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>
#include <ncurses.h>

char *argv0;
#include "chars.h"
#include "arg.h"

int scr_y, scr_x;

std::random_device rd;
std::mt19937 gen(rd());
std::chrono::steady_clock::time_point start_interval = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point end_interval = std::chrono::steady_clock::now();

std::string shown_str;
std::string charset = "";
int str_len = 4;
int passes = 0;
