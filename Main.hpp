#ifndef MAIN_HPP
#define MAIN_HPP
#include "File_manager.hpp"
#include "TSP.hpp"
#include <string>
#include <climits>
#include <iomanip>
#include <thread>

using namespace std;

class Main {

public:
    void run();

private:
    string config_path = R"(files/config.txt)";
    string data_path;
    string result_path;
    int minutes;
    float a;
    float b;
    float p;
    float Q;
    int iterations;
    int upper_bound;
    int alg_type;
    int optimal_value;
    int progress_indicator;
    int repetitions;
    File_manager file_manager;
    vector<vector<int>> matrix;
    TSP tsp;
    chrono::duration<double, micro> total_time{};
    float total_absolute_error;
    float total_relative_error;
    vector<chrono::duration<double, micro>> total_times;
    int time_measurements = 0;

    void assign_parameters(vector<string> parameters_string, vector<int> parameters_int, vector<float> parameters_float);
    void print_info();
    void print_partial_results(pair<vector<int>, int> results, int repetition, chrono::duration<double, micro> time);
    int calculate_path_length(vector<int> path);
    void print_total_results();

};

#endif