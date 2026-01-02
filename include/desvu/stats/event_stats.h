#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>
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
   * @brief Computes the sample standard deviation of all observations.
   * @return Standard deviation, or 0.0 if less than 2 observations exist
   */
  double StandardDeviation() const {
    if (observations_.size() < 2) return 0.0;
    double avg = Average();
    double variance = 0.0;
    for (double val : observations_) {
      variance += (val - avg) * (val - avg);
    }
    double scale = static_cast<double>(observations_.size() - 1);
    return std::sqrt(variance / scale);
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
   * @brief Computes 95% confidence interval for the mean.
   *
   * For n > 30, uses normal approximation (z = 1.96).
   * For n ≤ 30, uses t-distribution critical values.
   *
   * @return Pair of (lower_bound, upper_bound) for the confidence interval
   * @throws std::invalid_argument if count < 2
   */
  std::pair<double, double> ConfidenceInterval95() const {
    if (observations_.size() < 2) {
      throw std::invalid_argument(
          "Need at least 2 observations to compute confidence interval");
    }

    double mean = Average();
    double std_dev = StandardDeviation();
    size_t n = observations_.size();

    // Compute standard error
    double std_error = std_dev / std::sqrt(static_cast<double>(n));

    double critical_value;

    if (n > 30) {
      // Use normal approximation for large samples
      critical_value = 1.96;
    } else {
      // Use t-distribution critical values for small samples
      // Approximate values for common sample sizes (95% CI, two-tailed)
      static const double t_values[] = {
          12.706, 4.303, 3.182, 2.776, 2.571,  // df = 1-5
          2.447,  2.365, 2.306, 2.262, 2.228,  // df = 6-10
          2.201,  2.179, 2.160, 2.145, 2.131,  // df = 11-15
          2.120,  2.110, 2.101, 2.093, 2.086,  // df = 16-20
          2.080,  2.074, 2.069, 2.064, 2.060,  // df = 21-25
          2.056,  2.052, 2.048, 2.045          // df = 26-29
      };  // See https://en.wikipedia.org/wiki/Student%27s_t-distribution

      size_t df = n - 1;
      // Assert df is in valid range for the table
      assert(df > 0 && df <= 29);
      critical_value = t_values[df - 1];
    }

    // Compute margin of error
    double margin = critical_value * std_error;

    return std::make_pair(mean - margin, mean + margin);
  }

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
    if (Count() >= 2) {
      const auto ci = ConfidenceInterval95();
      oss << "\n  95% CI: [" << ci.first << ", " << ci.second << "]";
    } else {
      oss << "\n  95% CI: N/A (need >= 2 observations)";
    }
    return oss.str();
  }
};

}  // namespace desvu
