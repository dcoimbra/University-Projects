#include "system.hpp"
#include <algorithm>

void addHashtags(HashtagSet& set, std::string message) {
    auto hashtags = processMessage(message);

    for (auto hashtag : hashtags) {
        
        auto search = std::find_if(set.begin(), set.end(), 
                                    [hashtag](Hashtag h) -> bool { return h.get_word() == hashtag; });

        if (search != set.end()) {

            auto count = search->get_count() + 1;       
            set.erase(search);
            set.insert(Hashtag(hashtag, count));
        }

        else {
            set.insert(Hashtag(hashtag, 1));
        }
    }    
}

std::pair<std::size_t, int> countHashtags(HashtagSet& set) {
    
    auto ntotal{0};

    for (auto hashtag = set.begin(); hashtag != set.end(); ++hashtag) {
        ntotal += hashtag->get_count();
    }
    
    return std::make_pair(set.size(), ntotal);
}

std::pair<std::string, int> mostPopularHashtag(HashtagSet& set) {
    
    auto hashtag = set.begin();

    return std::make_pair(hashtag->get_word(), hashtag->get_count());
}

std::vector<std::pair<std::string, int>> listHashtags(HashtagSet& set) {
    
    std::vector<std::pair<std::string, int>> list{};

    for (auto hashtag = set.begin(); hashtag != set.end(); ++hashtag) {
        list.push_back(std::make_pair(hashtag->get_word(), hashtag->get_count()));
    }

    return list;
}


std::vector<std::string> processMessage(std::string message) {

    std::string const delims{' ','\t',',',';','.','?','!','"','\n',':','\0'};
    std::vector<std::string> hashtags{};

    size_t beg, pos = 0;
    std::string token{};
    while ((beg = message.find_first_not_of(delims, pos)) != std::string::npos) { 
        pos = message.find_first_of(delims, beg + 1);
        token = message.substr(beg, pos - beg);
        
        if (token.at(0) == '#' && token.length() > 1) {
            lowercase(token);
            hashtags.push_back(token);
        }
    }

    return hashtags;
}

void lowercase(std::string& string) {
    
    int length = string.length();
    for (auto i = 0; i < length; ++i) {
        string[i] = std::tolower(string[i]);
    }
}