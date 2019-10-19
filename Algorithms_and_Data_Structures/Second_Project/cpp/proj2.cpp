#include <iostream>
#include "system.hpp"

int main() {
    
    HashtagSet set{};

    char command{};

    do {
        std::cin.get(command);
        std::cin.get();

        if (command == 'a') {
            std::string message{};

            std::getline(std::cin, message);

            if (message.length() > 140) {
                continue;
            }

            addHashtags(set, message);
        }

        else if (command == 's') {
            auto count = countHashtags(set);
            std::cout << count.first << " " << count.second << std::endl;
        }

        else if (command == 'm') {
            auto most_popular = mostPopularHashtag(set);
            std::cout << most_popular.first << " " << most_popular.second << std::endl;
        }

        else if (command == 'l') {
            auto list = listHashtags(set);

            for (auto hashtag : list) {
                std::cout << hashtag.first << " " << hashtag.second << std::endl;
            }
        }
        
    } while (command != 'x');

    return 0;
}    


