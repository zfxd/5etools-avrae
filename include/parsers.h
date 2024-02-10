// parsers.h
// Converting between Avrae and 5etools formats

#pragma once
#include "main.h"

std::string fet_time(json fet_t);
std::string fet_range(json fet_r);
std::string fet_duration(json fet_d);