#pragma once

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace desvu {

/**
 * @brief Collects event-based observations and computes statistics.
 *
 * Use this for measurements taken at specific events like waiting times,
 * service times, or any value that is observed at specific points in time.
 * Note: "Event-based" refers to when observations are recorded (at events),
 * not the nature of the data (which can be continuous values).
 */
class EventStats {
 private:
  std::string name_;
  std::vector<double> observations_;

 public:
  /**
   * @brief Constructs a new event-based statistics collector.
   * @param name Descriptive name for this statistic
   */
  explicit EventStats(const std::string& name) : name_(name), observations_() {}

  /**
   * @brief Adds an observation.
   * @param value The observed value
   */
  void Add(double value) { observations_.push_back(value); }

  /**
   * @brief Gets the number of observations.
   * @return Number of observations collected
   */
  size_t Count() const { return observations_.size(); }

  /**
   * @brief Computes the average of all observations.
   * @return Mean value, or 0.0 if no observations exist
   */
  double Average() const {
    if (observations_.empty()) return 0.0;
    return std::accumulate(observations_.begin(), observations_.end(), 0.0) /
           static_cast<double>(observations_.size());
  }

  /**
   * @brief Computes the standard deviation of all observations.
   * @return Standard deviation, or 0.0 if no observations exist
   */
  double StandardDeviation() const {
    if (observations_.empty()) return 0.0;
    double avg = Average();
    double variance = 0.0;
    for (double val : observations_) {
      variance += (val - avg) * (val - avg);
    }
    return std::sqrt(variance / static_cast<double>(observations_.size()));
  }

  /**
   * @brief Finds the minimum observation.
   * @return Minimum value, or 0.0 if no observations exist
   */
  double Min() const {
    if (observations_.empty()) return 0.0;
    return *std::min_element(observations_.begin(), observations_.end());
  }

  /**
   * @brief Finds the maximum observation.
   * @return Maximum value, or 0.0 if no observations exist
   */
  double Max() const {
    if (observations_.empty()) return 0.0;
    return *std::max_element(observations_.begin(), observations_.end());
  }

  /**
   * @brief Gets the name of this statistic.
   * @return The descriptive name
   */
  const std::string& Name() const { return name_; }

  /**
   * @brief Gets the underlying observations vector.
   * @return Const reference to the observations vector
   */
  const std::vector<double>& Observations() const { return observations_; }

  /**
   * @brief Generates a formatted report of statistical summaries.
   * @return String containing formatted statistics
   */
  std::string Report() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << name_ << " (Event-based)\n";
    oss << "  Count: " << Count() << "\n";
    oss << "  Average: " << Average() << "\n";
    oss << "  Std Dev: " << StandardDeviation() << "\n";
    oss << "  Min: " << Min() << "\n";
    oss << "  Max: " << Max();
    return oss.str();
  }
};

}  // namespace desvu
