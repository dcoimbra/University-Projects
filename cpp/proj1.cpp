#include <iostream>
#include <limits>  
#include <ios>
#include "system.hpp"

int main() {
   
    AirportGraph graph{};
    
    char command{};
    
    do {
        std::cin.get(command);
        
        try {
            if (command == 'A') {
                std::string id;
                int capacity;

                std::cin >> id >> capacity;
                
                graph = add_airport(id, capacity, graph);
            }

            else if (command == 'I') {
                std::string id;
                int capacity;

                std::cin >> id >> capacity;

                graph = change_airport_capacity(id, capacity, graph);
            }

            else if (command == 'F') {
                std::string id_1;
                std::string id_2;

                std::cin >> id_1 >> id_2;

                graph = add_flight(id_1, id_2, true, graph);
            }

            else if (command == 'G') {
                std::string id_1;
                std::string id_2;

                std::cin >> id_1 >> id_2;

                graph = add_flight(id_1, id_2, false, graph);
            }

            else if (command == 'R') {
                std::string id_1;
                std::string id_2;

                std::cin >> id_1 >> id_2;

                graph = remove_flight(id_1, id_2, false, graph);
            }

            else if (command == 'S') {
                std::string id_1;
                std::string id_2;

                std::cin >> id_1 >> id_2;

                graph = remove_flight(id_1, id_2, true, graph);
            }

            else if (command == 'N') {
                std::string id_1;
                std::string id_2;

                std::pair<int, int> flights;
                
                std::cin >> id_1 >> id_2;

                flights = n_flights(id_1, id_2, graph);

                std::cout << id_1 << ':' << id_2 << ':' << std::to_string(flights.first) << ':' << std::to_string(flights.second) << std::endl;
            }

            else if (command == 'P') {
                std::cout << most_popular_airp(graph) << std::endl;
            }

            else if (command == 'Q') {
                std::cout << most_connected(graph) << std::endl;
            }

            else if (command == 'V') {
                std::pair<std::string, std::string> flight = most_popular_flight(graph);
                
                std::cout << "<" << flight.first << ", " << flight.second << ">" << std::endl;
            }

            else if (command == 'C') {
                std::string id;
                
                std::cin >> id;

                graph = close_airport(id, graph);
            }

            else if (command == 'O') {
                std::string id;

                std::cin >> id;

                graph = reopen_airport(id, graph);
            }

            else if (command == 'L') {
                
                std::cout << list_airports(graph);
            }

            else {
                if (command != 'X' && command != '\n') {
                    std::cout << "Unrecognized command" << std::endl;
                }
            }
        } catch (std::invalid_argument &inv) {
            std::cout << inv.what() << std::endl;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    } while (command != 'X');
    
    return 0;
}
