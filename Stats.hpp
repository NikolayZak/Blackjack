#ifndef STATISTICS
#define STATISTICS

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric> // for std::accumulate
#include <algorithm> // for std::sort
#include <iterator>  // for std::distance

double calculate_mean(const std::vector<double>& data);
double calculate_standard_deviation(const std::vector<double>& data, double mean);
double calculate_t_value(double confidence_level, int degrees_of_freedom);
std::pair<double, double> calculate_confidence_interval(const std::vector<double>& data, double confidence_level);
std::pair<int, int> findClosestRectangleSides(int area);

#endif