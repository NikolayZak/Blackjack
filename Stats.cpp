#include "Stats.hpp"

double calculate_mean(const std::vector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double calculate_standard_deviation(const std::vector<double>& data, double mean) {
    double sum = 0.0;
    for(double value : data) {
        sum += std::pow(value - mean, 2);
    }
    return std::sqrt(sum / (data.size() - 1)); // Sample standard deviation
}

double calculate_t_value(double confidence_level, int degrees_of_freedom) {
    // For simplicity, we'll use approximate t-values for common confidence levels
    if (confidence_level == 0.90) return 1.645; // 90% confidence
    if (confidence_level == 0.95) return 1.96;  // 95% confidence
    if (confidence_level == 0.99) return 2.576; // 99% confidence
    // For other confidence levels, you would need a lookup table or a statistics library
    return 1.96; // Default to 95% confidence
}

std::pair<double, double> calculate_confidence_interval(const std::vector<double>& data, double confidence_level) {
    double mean = calculate_mean(data);
    double stddev = calculate_standard_deviation(data, mean);
    double t_value = calculate_t_value(confidence_level, data.size() - 1);
    double standard_error = stddev / std::sqrt(data.size());
    double margin_of_error = t_value * standard_error;
    return std::make_pair(mean - margin_of_error, mean + margin_of_error);
}