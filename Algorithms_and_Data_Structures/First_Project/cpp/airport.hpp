#pragma once

#include <string>
#include <boost/graph/adjacency_list.hpp>

constexpr int IDLEN = 3;

struct Airport {

private: 
  std::string id{};
  int capacity{};
  bool open{true};

public: 
  void set_id(std::string id);
  void set_capacity(int capacity);
  void flip_state();
  std::string get_id();
  int get_capacity();
  int is_open();
};

struct Flight {

private: 
  int number{1};
  
public: 
  void add_weight(int amount);
  int get_weight();
};

using BoostAirportGraph = boost::adjacency_list < boost::vecS, boost::vecS, boost::bidirectionalS, Airport, Flight > ;
using BoostVertexDescriptor = BoostAirportGraph::vertex_descriptor;
using BoostEdgeDescriptor = BoostAirportGraph::edge_descriptor;
using BoostVertexIterator = BoostAirportGraph::vertex_iterator;
using BoostEdgeIterator = BoostAirportGraph::edge_iterator;
using BoostOutEdgeIterator = BoostAirportGraph::out_edge_iterator;
using BoostInEdgeIterator = BoostAirportGraph::in_edge_iterator;

class AirportGraph {

private:
  BoostAirportGraph graph;
  BoostVertexDescriptor find_airport_by_id(std::string id);
  BoostEdgeDescriptor add_edge(BoostVertexDescriptor source, BoostVertexDescriptor target);
  void remove_edge(BoostVertexDescriptor source, BoostVertexDescriptor target);
  int total_flights(BoostVertexDescriptor airport);
  int total_outgoing(BoostVertexDescriptor airport);
  int total_incoming(BoostVertexDescriptor airport);
  int count_connections(BoostVertexDescriptor airport);
  int count_flights_between(BoostVertexDescriptor airport_src, BoostVertexDescriptor airport_dst);

public:
  BoostVertexDescriptor add_airport(std::string id, int capacity);
  BoostVertexDescriptor change_airport_capacity(std::string id, int capacity_delta);
  BoostEdgeDescriptor add_flight(std::string id_1, std::string id_2);
  void remove_flight(std::string id_1, std::string id_2, bool round);
  std::pair < int, int > n_flights(std::string id_1, std::string id_2);
  std::string most_popular_airp();
  std::string most_connected();
  std::pair < std::string, std::string > most_popular_flight();
  BoostVertexDescriptor close_airport(std::string id);
  BoostVertexDescriptor reopen_airport(std::string id);
  std::string list_airports();
  std::string print_id(BoostVertexDescriptor v);
  int print_capacity(BoostVertexDescriptor v);
  int print_state(BoostVertexDescriptor v);
  std::string print_flight_src(BoostEdgeDescriptor e);
  std::string print_flight_dst(BoostEdgeDescriptor e);
};