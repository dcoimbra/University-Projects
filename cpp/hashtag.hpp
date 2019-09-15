#pragma once

#include <string>
#include <set>

struct Hashtag {

private:
    std::string word{};
    int count{1};

public:
    Hashtag(std::string word, int count);
    std::string get_word() const;
    int get_count() const;

    bool operator>(const Hashtag& other) const;
};

using HashtagSet = std::set<Hashtag, std::greater<Hashtag>>;