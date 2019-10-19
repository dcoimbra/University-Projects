#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "airport.hpp"

AirportGraph add_airport(std::string id, int capacity, AirportGraph graph);
AirportGraph change_airport_capacity(std::string id, int capacity_delta, AirportGraph graph);
AirportGraph add_flight(std::string id_1, std::string id_2, bool round, AirportGraph graph);
AirportGraph remove_flight(std::string id_1, std::string id_2, bool round, AirportGraph graph);
std::pair<int, int> n_flights(std::string id_1, std::string id_2, AirportGraph graph);
std::string most_popular_airp(AirportGraph graph);
std::string most_connected(AirportGraph graph);
std::pair<std::string, std::string> most_popular_flight(AirportGraph graph);
AirportGraph close_airport(std::string id, AirportGraph graph);
AirportGraph reopen_airport(std::string id, AirportGraph graph);
std::string list_airports(AirportGraph graph);

#endif