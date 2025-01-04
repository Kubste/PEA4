#include "TSP.hpp"

void TSP::set_matrix(vector<vector<int>> matrix) {
    this -> matrix = std::move(matrix);
    set_min_value();
}

void TSP::set_min_value() {
    for(auto & i : matrix) for(int j : i) if(j < min_value && j >= 0) min_value = j;
}

pair<vector<int>, int> TSP::ACO(int iterations, float a, float b, float p, float Q, int alg_type, int upper_bound, int minutes, int optimal_value) {
    chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
    results.second = INT_MAX;
    if(upper_bound == 1) results = NN();

    init_pheromones();

    for(int i = 0; i < iterations; i++) {
        if(chrono::duration_cast<chrono::minutes>(chrono::steady_clock::now() - start).count() == minutes || results.second == optimal_value) return results;
        vector<vector<int>> paths(matrix.size(), vector<int>());
        vector<int> distances(matrix.size(), 0);

        for(int current_ant = 0; current_ant < matrix.size(); current_ant++) {
            vector<bool> visited_cities(matrix.size(), false);
            int current_city = current_ant;
            paths[current_ant].push_back(current_city);
            visited_cities[current_city] = true;

            for(int j = 0; j < matrix.size() - 1; j++) {
                int next_city = choose_city(current_city, visited_cities, a, b);
                if(next_city != -1) {
                    paths[current_ant].push_back(next_city);
                    //distances[current_ant] += matrix[current_city][next_city];
                    visited_cities[next_city] = true;
                    current_city = next_city;
                }
            }
            paths[current_ant].push_back(paths[current_ant].front());
            //distances[current_ant] += matrix[current_ant][paths[current_ant].front()];
            distances[current_ant] = calculate_path_length(paths[current_ant]);
            refresh_pheromones(Q, paths[current_ant], distances[current_ant], alg_type);
        }
        for(int j = 0; j < matrix.size(); j++) {
            if(distances[j] < results.second) {
                results.second = distances[j];
                results.first = paths[j];
            }
        }
        evaporate_pheromone(p);
        //refresh_pheromones(Q, paths, distances);
    }
    return results;
}

void TSP::evaporate_pheromone(float p) {
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix.size(); j++) {
            pheromones[i][j] = pheromones[i][j] * p;
            if(pheromones[i][j] == 0) pheromones[i][j] = numeric_limits<double>::min();
        }
    }
}

//void TSP::refresh_pheromones(float p, float Q, vector<vector<int>> paths, vector<int> distances) {
//    for(int i = 0; i < paths.size(); i++) {
//        for(int j = 0; j < paths[i].size() - 1; j++) {
//            pheromones[paths[i][j]][paths[i][j + 1]] += Q / distances[i];
//        }
//    }
//}

void TSP::refresh_pheromones(float Q, vector<int> path, int distance, int type) {
    if(type == 1) for(int i = 0; i < path.size() - 1; i++) pheromones[path[i]][path[i + 1]] += Q / distance;
    else if(type == 2) for(int i = 0; i < path.size() - 1; i++) pheromones[path[i]][path[i + 1]] += Q;
    else if(type == 3) for(int i = 0; i < path.size() - 1; i++) pheromones[path[i]][path[i + 1]] += Q / matrix[path[i]][path[i + 1]];
}

int TSP::choose_city(int current_city, vector<bool> visited_cities, float a, float b) {
    static mt19937 rng(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    uniform_real_distribution<> distribution(0.0, 1.0);

    vector<double> probabilities(matrix.size(), 0.0);
    double sum = 0.0;

    for(int i = 0; i < matrix.size(); i++) {
        if(!visited_cities[i]) {
            probabilities[i] = pow(pheromones[current_city][i], a) * pow(1.0 / matrix[current_city][i], b);
            if(probabilities[i] == 0) probabilities[i] = numeric_limits<double>::min();
            sum += probabilities[i];
        }
    }

    for(double & probability : probabilities) probability = probability / sum;

    double random_number = distribution(rng);
    double sum_probability = 0.0;

    for(int i = 0; i < matrix.size(); i++) {
        if(!visited_cities[i]) {
            sum_probability += probabilities[i];
            if(sum_probability >= random_number) return i;
        }
    }
    return -1;
}

void TSP::init_pheromones() {
    float start_pheromone = float(matrix.size()) / float(NN().second);
    for(int i = 0; i < matrix.size(); i++) {
        pheromones.emplace_back();
        for(int j = 0; j < matrix.size(); j++) pheromones[i].push_back(start_pheromone);
    }
}

pair<vector<int>, int> TSP::random() {
    pair<vector<int>, int> randomResults;
    vector<int> path;
    random_device random;
    mt19937 g(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
    randomResults.second = INT_MAX;

    for(int i = 0; i < matrix.size(); i++) path.push_back(i);

    while(randomResults.second == INT_MAX) {
        shuffle(path.begin(), path.end(), g);
        reverse(path.begin() + g() % matrix.size(), path.end());

        path.push_back(path.front());
        randomResults.first = path;
        randomResults.second = calculate_path_length(path);

        path.pop_back();
    }

    return randomResults;
}

int TSP::calculate_path_length(vector<int> path) {
    int path_length = 0;

    for(int i = 0; i < path.size() - 1; i++) {
        if(matrix[path[i]][path[i + 1]] == -1) return INT_MAX;
        else path_length = path_length + matrix[path[i]][path[i + 1]];
    }
    return path_length;
}

pair<vector<int>, int> TSP::NN() {
    pair<vector<int>, int> resultsNN;
    resultsNN.second = INT_MAX;
    vector<int> path;
    vector<int> Q;

    for(int j = 0; j < matrix.size(); j++) {

        path.push_back(j);

        for(int i = 0; i < matrix.size(); i++) if(i != j) Q.push_back(i);
        explore_paths(path, 0, Q, j, j, resultsNN);

        path.clear();
        Q.clear();
    }
    return resultsNN;
}

void TSP::explore_paths(vector<int> path, int path_length, vector<int> Q, int current_node, int start_node,
                        pair<vector<int>, int> &resultsNN) {
    vector<pair<int, int>> min_edges;
    int min_edge_value = INT_MAX;
    vector<int> new_path;
    vector<int> new_Q;
    int next_node;
    int edge_length;

    if(Q.empty()) {
        if(matrix[current_node][start_node] != -1) {
            path_length += matrix[current_node][start_node];
            path.push_back(start_node);

            if(path_length < resultsNN.second) {
                resultsNN.first = path;
                resultsNN.second = path_length;
            }
        }
        return;
    }

    for(int i = 0; i < matrix.size(); i++) {
        if(matrix[current_node][i] != -1 && find(Q.begin(), Q.end(), i) != Q.end()) {
            if(matrix[current_node][i] < min_edge_value) {
                min_edges.clear();
                min_edge_value = matrix[current_node][i];
                min_edges.emplace_back(i, matrix[current_node][i]);
            } else if(matrix[current_node][i] == min_edge_value) min_edges.emplace_back(i, matrix[current_node][i]);
        }
    }

    for(auto & min_edge : min_edges) {
        next_node = min_edge.first;
        edge_length = min_edge.second;

        new_path = path;
        new_path.push_back(next_node);
        new_Q = Q;
        new_Q.erase(remove(new_Q.begin(), new_Q.end(), next_node), new_Q.end());

        explore_paths(new_path, path_length + edge_length, new_Q, next_node, start_node, resultsNN);
    }
}