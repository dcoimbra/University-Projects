#pragma once

#include "hashtag.hpp"
#include <vector>

void addHashtags(HashtagSet& set, std::string message);
std::pair<std::size_t, int> countHashtags(HashtagSet& set);
std::pair<std::string, int> mostPopularHashtag(HashtagSet& set);
std::vector<std::pair<std::string, int>> listHashtags(HashtagSet& set);

std::vector<std::string> processMessage(std::string message);
void lowercase(std::string& string);