#include "hashtag.hpp"

Hashtag::Hashtag(std::string word, int count) {
    this->word = word;
    this->count = count;
}

std::string Hashtag::get_word() const {
    return this->word;
}

int Hashtag::get_count() const {
    return this->count;
}

bool Hashtag::operator>(const Hashtag& other) const {
    
    if (this->count > other.count) {
        return true;
    }

    else if (this->count == other.count) {
        return this->word.compare(other.word) < 0; 
    }

    else {
        return false;
    }
}