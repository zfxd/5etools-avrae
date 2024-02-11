// parsers.h
// Converting between Avrae and 5etools formats

#pragma once
#include "main.h"

std::string fet_time(json fet_t);
std::string fet_range(json fet_r);
std::string fet_duration(json fet_d);
std::string fet_classes(json fet_c);
std::string fet_entries(std::vector < std::string > entries);
json fet_components(json fet_c, int level);

std::string fet_sanitizer(std::string in);