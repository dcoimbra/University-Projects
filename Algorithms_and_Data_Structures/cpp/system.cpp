#include <stdexcept>
#include <iostream>
#include "system.hpp"

AirportGraph add_airport(std::string id, int capacity, AirportGraph graph) {

    if (id.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    if (capacity <= 0) {
        throw std::invalid_argument("Airport capacity must be positive");
    }

    try {
       BoostVertexDescriptor new_airport = graph.add_airport(id, capacity);
       
       std::cout << graph.print_id(new_airport) << " " << graph.print_capacity(new_airport) << " " << graph.print_state(new_airport) << std::endl;

       return graph;
    } 
    
    catch (std::invalid_argument &inv) { 
        throw inv;
    }
}

AirportGraph change_airport_capacity(std::string id, int capacity_delta, AirportGraph graph) {

    if (id.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    if (capacity_delta == 0) {
        throw std::invalid_argument("Capacity must change");
    }

    try {
        BoostVertexDescriptor airport = graph.change_airport_capacity(id, capacity_delta);
        std::cout << graph.print_id(airport) << " " << graph.print_capacity(airport) << " " << graph.print_state(airport) << std::endl;
        return graph;
    }

    catch (std::invalid_argument &inv) { 
        throw inv;
    }
}


AirportGraph add_flight(std::string id_1, std::string id_2, bool round, AirportGraph graph) {
   
    if (id_1.length() != IDLEN || id_2.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    try {
        BoostEdgeDescriptor new_flight_in = graph.add_flight(id_1, id_2);
        std::cout << graph.print_flight_src(new_flight_in) << " " << graph.print_flight_dst(new_flight_in) << std::endl;

        if (round) {
            BoostEdgeDescriptor new_flight_out = graph.add_flight(id_2, id_1);
            std::cout << graph.print_flight_src(new_flight_out) << " " << graph.print_flight_dst(new_flight_out) << std::endl;
        }
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }

    return graph;
}

AirportGraph remove_flight(std::string id_1, std::string id_2, bool round, AirportGraph graph) {

    if (id_1.length() != IDLEN || id_2.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    try {
        graph.remove_flight(id_1, id_2, round);
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }

    return graph;
}

std::pair<int, int> n_flights(std::string id_1, std::string id_2, AirportGraph graph) {

    if (id_1.length() != IDLEN || id_2.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    try {
        return graph.n_flights(id_1, id_2);
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }
}

std::string most_popular_airp(AirportGraph graph) {

    try {

        return graph.most_popular_airp();
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }
}

std::string most_connected(AirportGraph graph) {

    try {

        return graph.most_connected();
    }

    catch (std::invalid_argument &inv) {
        
        throw inv;
    }
}

std::pair<std::string, std::string> most_popular_flight(AirportGraph graph) {

    try {
        
        return graph.most_popular_flight();
    }

    catch (std::invalid_argument &inv) {
        
        throw inv;
    }
}

AirportGraph close_airport(std::string id, AirportGraph graph) {
    try {
        BoostVertexDescriptor airport = graph.close_airport(id);
        std::cout << graph.print_id(airport) << " " << graph.print_capacity(airport) << " " << graph.print_state(airport) << std::endl;

        return graph;   
    }

    catch (std::invalid_argument &inv) {
        
        throw inv;
    }
}

AirportGraph reopen_airport(std::string id, AirportGraph graph) {
    try {
        BoostVertexDescriptor airport = graph.reopen_airport(id);
        std::cout << graph.print_id(airport) << " " << graph.print_capacity(airport) << " " << graph.print_state(airport) << std::endl;

        return graph;   
    }

    catch (std::invalid_argument &inv) {
        
        throw inv;
    }
}

std::string list_airports(AirportGraph graph) {
    
    try {
        return graph.list_airports();
    }

    catch (std::invalid_argument &inv) {
        
        throw inv;
    }
}