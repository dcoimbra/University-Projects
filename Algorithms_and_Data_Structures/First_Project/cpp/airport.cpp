#include <stdexcept>
#include <iostream>
#include "airport.hpp"

void Airport::set_id(std::string id) {
    
    if (id.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }
    
    this->id = id;
}

void Airport::set_capacity(int capacity) {
    
    if (capacity <= 0) {
        throw std::invalid_argument("Airport capacity must be positive");
    }
    
    this->capacity = capacity;
}

void Airport::flip_state() {
    this->open = !this->open;
}

std::string Airport::get_id() {
    return this->id;
}

int Airport::get_capacity() {
    return this->capacity;
}

int Airport::is_open() {
    return this->open;
}


void Flight::add_weight(int amount) {
    this->number += amount;
}

int Flight::get_weight() {
    return this->number;
}


BoostVertexDescriptor AirportGraph::add_airport(std::string id, int capacity) {
    
    BoostVertexDescriptor new_airport_vertex = boost::add_vertex(this->graph);

    try {
        
        this->graph[new_airport_vertex].set_id(id);
        this->graph[new_airport_vertex].set_capacity(capacity);
    } catch (std::invalid_argument &inv) {
        
        throw inv;
    }

    return new_airport_vertex;
}

BoostVertexDescriptor AirportGraph::change_airport_capacity(std::string id, int capacity_delta) {
     
    if (id.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    if (capacity_delta == 0) {
        throw std::invalid_argument("Capacity must change");
    }
    
    try { 
        BoostVertexDescriptor airport = find_airport_by_id(id);

        if (!this->graph[airport].is_open()) {
            throw std::invalid_argument("Airport " + id + " is closed, must be open");
        }

        int new_capacity = this->graph[airport].get_capacity() + capacity_delta;

        if (new_capacity < total_flights(airport)) {
            throw std::invalid_argument("New capacity is lower than the number of flights in airport " + id);
        }
    
        this->graph[airport].set_capacity(new_capacity);
        return airport;
    } catch (std::invalid_argument &inv) {
        throw inv;
    }
}

BoostEdgeDescriptor AirportGraph::add_flight(std::string id_1, std::string id_2) {

    if (id_1.length() != IDLEN || id_2.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }
    
    try {
        BoostVertexDescriptor airport_src = find_airport_by_id(id_1);
        BoostVertexDescriptor airport_dst = find_airport_by_id(id_2);

        if (!this->graph[airport_src].is_open()) {
           throw std::invalid_argument("Airport " + id_1 + " is closed, must be open");
        }

        if (!this->graph[airport_dst].is_open()) {
           throw std::invalid_argument("Airport " + id_2 + " is closed, must be open");
        }

        int current_flights_src = total_flights(airport_src);
        int current_flights_dst = total_flights(airport_dst);

        if (current_flights_src + 1 > this->graph[airport_src].get_capacity()) {
            throw std::invalid_argument("Airport " + id_1 + " is at max capacity");
        }

        if (current_flights_dst + 1 > this->graph[airport_dst].get_capacity()) {
            throw std::invalid_argument("Airport " + id_2 + " is at max capacity");
        }

        return add_edge(airport_src, airport_dst);
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }
}

void AirportGraph::remove_flight(std::string id_1, std::string id_2, bool round) {
    
    if (id_1.length() != IDLEN || id_2.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    try {
        BoostVertexDescriptor airport_src = find_airport_by_id(id_1);
        BoostVertexDescriptor airport_dst = find_airport_by_id(id_2);

        if (round) {
            remove_edge(airport_src, airport_dst);
            remove_edge(airport_dst, airport_src);

            return;
        }

        remove_edge(airport_src, airport_dst);
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }
}

std::pair<int, int> AirportGraph::n_flights(std::string id_1, std::string id_2) {
    
    if (id_1.length() != IDLEN || id_2.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    try {
        BoostVertexDescriptor airport_src = find_airport_by_id(id_1);
        BoostVertexDescriptor airport_dst = find_airport_by_id(id_2);
        
        int flights_outgoing = count_flights_between(airport_src, airport_dst);
        int flights_incoming = count_flights_between(airport_dst, airport_src);

        return std::pair(flights_outgoing, flights_incoming);
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }
} 

std::string AirportGraph::most_popular_airp() {

    int max_flights{};
    int current_total{};
    BoostVertexDescriptor most_popular_airport{};

    std::pair<BoostVertexIterator, BoostVertexIterator> vp = boost::vertices(this->graph);

    if (vp.first == vp.second) {
        throw std::invalid_argument("No airports.");
    }
    
    BoostVertexDescriptor airport{};

    for (; vp.first != vp.second; ++vp.first) {
        airport = *vp.first;

        current_total = total_flights(airport);

        if (current_total > max_flights) {
            max_flights = current_total;
            most_popular_airport = airport;
        }
    }

    return this->graph[most_popular_airport].get_id();
}

std::string AirportGraph::most_connected() {

    int max_connections{};
    int current_total_connections{};
    BoostVertexDescriptor most_connected_airport{};

    std::pair<BoostVertexIterator, BoostVertexIterator> vp = boost::vertices(this->graph);

    if (vp.first == vp.second) {
        throw std::invalid_argument("No airports.");
    }
    
    BoostVertexDescriptor airport{};
    for (; vp.first != vp.second; ++vp.first) {
        airport = *vp.first;
        
        current_total_connections = count_connections(airport);

        if (current_total_connections > max_connections) {
            max_connections = current_total_connections;
            most_connected_airport = airport;
        }
    }

    return this->graph[most_connected_airport].get_id();
}

std::pair<std::string, std::string> AirportGraph::most_popular_flight() {

    int max_flights{0};
    int flights{};
    BoostEdgeDescriptor most_popular_edge{};

    std::pair<BoostEdgeIterator, BoostEdgeIterator> edges = boost::edges(this->graph);

    if (edges.first == edges.second) {
        throw std::invalid_argument("No flights.");
    }

    BoostEdgeDescriptor edge{};

    for (; edges.first != edges.second; edges.first++) {
        edge = *edges.first;
        flights = this->graph[edge].get_weight();

        if (flights > max_flights) {
            max_flights = flights;
            most_popular_edge = edge;
        }
    }

    return std::pair(print_flight_src(most_popular_edge), print_flight_dst(most_popular_edge));
}

BoostVertexDescriptor AirportGraph::close_airport(std::string id) {

    if (id.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    try {

        BoostVertexDescriptor airport = find_airport_by_id(id);

        if (!this->graph[airport].is_open()) {
            throw std::invalid_argument("Airport " + id + " is already closed");
        }

        boost::clear_vertex(airport, this->graph);
        this->graph[airport].flip_state();

        return airport;
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }
}

BoostVertexDescriptor AirportGraph::reopen_airport(std::string id) {

    if (id.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    try {

        BoostVertexDescriptor airport = find_airport_by_id(id);

        if (this->graph[airport].is_open()) {
            throw std::invalid_argument("Airport " + id + " is already open");
        }

        this->graph[airport].flip_state();

        return airport;
    }

    catch (std::invalid_argument &inv) {
        throw inv;
    }
}

std::string AirportGraph::list_airports() {

    std::pair<BoostVertexIterator, BoostVertexIterator> vp = boost::vertices(this->graph);
    
    if (vp.first == vp.second) {
        throw std::invalid_argument("No airports.");        
    }

    std::string list{};
    std::string id{}, capacity{}, outgoing{}, incoming{};

    BoostVertexDescriptor airport{};
    for (; vp.first != vp.second; ++vp.first) {
        airport = *vp.first;

        id = this->graph[airport].get_id();
        capacity = std::to_string(this->graph[airport].get_capacity());
        outgoing = std::to_string(total_outgoing(airport));
        incoming = std::to_string(total_incoming(airport));

        list += (id + ':' + capacity + ':' + outgoing + ':' + incoming + '\n');
    }

    return list;
}

BoostVertexDescriptor AirportGraph::find_airport_by_id(std::string id) {
    
    if (id.length() != IDLEN) {
        throw std::invalid_argument("Airport id must have " + std::to_string(IDLEN) + " characters");
    }

    std::pair<BoostVertexIterator, BoostVertexIterator> vp = boost::vertices(this->graph);
    
    if (vp.first == vp.second) {
        throw std::invalid_argument("No airports.");        
    }

    BoostVertexDescriptor airport{};
    for (; vp.first != vp.second; ++vp.first) {
        airport = *vp.first;

        if (this->graph[airport].get_id() == id) {
            return airport;
        }
    }

    throw std::invalid_argument("Airport " + id + " does not exist");
}

int AirportGraph::total_flights(BoostVertexDescriptor airport) {

    return total_outgoing(airport) + total_incoming(airport);
}

int AirportGraph::total_outgoing(BoostVertexDescriptor airport) {

    int total{0};

    BoostEdgeDescriptor flight{};
    std::pair<BoostOutEdgeIterator, BoostOutEdgeIterator> edges_out{};

    for (edges_out = boost::out_edges(airport, this->graph); edges_out.first != edges_out.second; ++edges_out.first) {
        flight = *edges_out.first;
        total += this->graph[flight].get_weight();
    }

    return total;
}

int AirportGraph::total_incoming(BoostVertexDescriptor airport) {

    int total{0};

    BoostEdgeDescriptor flight{};
    std::pair<BoostInEdgeIterator, BoostInEdgeIterator> edges_in{};

    for (edges_in = boost::in_edges(airport, this->graph); edges_in.first != edges_in.second; ++edges_in.first) {
        flight = *edges_in.first;
        total += this->graph[flight].get_weight();
    }

    return total;
}

BoostEdgeDescriptor AirportGraph::add_edge(BoostVertexDescriptor source, BoostVertexDescriptor target) {

    std::pair<BoostEdgeDescriptor, bool> edge = boost::edge(source, target, this->graph);

    if (edge.second) {
            
        this->graph[edge.first].add_weight(1);
        return edge.first;
    }

    std::pair<BoostEdgeDescriptor, bool> new_edge = boost::add_edge(source, target, this->graph);
        
    return new_edge.first;
}

void AirportGraph::remove_edge(BoostVertexDescriptor source, BoostVertexDescriptor target) {

    std::pair<BoostEdgeDescriptor, bool> edge = boost::edge(source, target, this->graph);

    if (!edge.second) {
        throw std::invalid_argument("Flight <" + this->graph[source].get_id() + ", " + this->graph[target].get_id() + "> does not exist");
    }

    if (this->graph[edge.first].get_weight() > 1) {
        this->graph[edge.first].add_weight(-1);
        return;
    }  

    boost::remove_edge(edge.first, this->graph);
}

int AirportGraph::count_connections(BoostVertexDescriptor airport) {

    return boost::out_degree(airport, this->graph) + boost::in_degree(airport, this->graph);
}

int AirportGraph::count_flights_between(BoostVertexDescriptor airport_src, BoostVertexDescriptor airport_dst) {

    std::pair <BoostEdgeDescriptor, bool> flights_out = boost::edge(airport_src, airport_dst, this->graph);

    if (flights_out.second) {
        return this->graph[flights_out.first].get_weight();
    }

    return 0;
}

std::string AirportGraph::print_id(BoostVertexDescriptor v) {

    return this->graph[v].get_id();
}

int AirportGraph::print_capacity(BoostVertexDescriptor v) {

    return this->graph[v].get_capacity();
}

int AirportGraph::print_state(BoostVertexDescriptor v) {

    return this->graph[v].is_open();
}

std::string AirportGraph::print_flight_src(BoostEdgeDescriptor e) {

    return print_id(boost::source(e, this->graph));
}

std::string AirportGraph::print_flight_dst(BoostEdgeDescriptor e) {

    return print_id(boost::target(e, this->graph));
}
