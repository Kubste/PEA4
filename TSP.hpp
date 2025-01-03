#ifndef TSP_HPP
#define TSP_HPP
#include <vector>
#include <chrono>
#include <climits>
#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>
#include <limits>
#include <utility>
#include <unordered_map>
#include <chrono>
#include <random>
#include <map>

using namespace std;

class TSP {

public:
    void set_matrix(vector<vector<int>> matrix);
    pair<vector<int>, int> NN();
    void explore_paths(vector<int> path, int path_length, vector<int> Q, int current_node, int start_node, pair<vector<int>, int> &resultsNN);
    pair<vector<int>, int> ACO(int iterations, float a, float b, float p, int upper_bound);

private:
    vector<vector<int>> matrix;
    pair<vector<int>, int> results;
    vector<vector<double>> pheromones;
    int min_value = INT_MAX;
    float Q = 200.0;

    void set_min_value();
    void init_pheromones();
    int choose_city(int current_city, vector<bool> visited_cities, float a, float b);
    void refresh_pheromones(float p, vector<vector<int>> paths, vector<int> distances);
    pair<vector<int>, int> random();
    int calculate_path_length(vector<int> path);
};

#endif