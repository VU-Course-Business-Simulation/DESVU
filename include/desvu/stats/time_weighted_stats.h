#pragma once

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

namespace desvu {

/**
 * @brief Collects time-weighted statistics for values that persist over time.
 *
 * Use this for state variables like queue length, number of busy servers,
 * or any value that remains constant between updates. The average is
 * weighted by the duration each value persists.
 */
class TimeWeightedStats {
 private:
  std::string name_;
  double last_time_;
  double last_value_;
  double integral_;
  double min_;
  double max_;
  size_t update_count_;

 public:
  /**
   * @brief Constructs a new time-weighted statistics collector.
   *
   * It initializes with value 0 at time 0. Update directly if you want
   * something else to start with.
   *
   * @param name Descriptive name for this statistic
   */
  explicit TimeWeightedStats(const std::string& name)
      : name_(name),
        last_time_(0.0),
        last_value_(0.0),
        integral_(0.0),
        min_(0.0),
        max_(0.0),
        update_count_(1) {}

  /**
   * @brief Updates the statistic with a new value at the given time.
   *
   * The previous value is accumulated from last_time to the current time.
   *
   * @param time The current simulation time (must be >= last_time_)
   * @param value The new value
   */
  void Update(double time, double value) {
    if (time < last_time_) {
      throw std::invalid_argument("Update time must be >= last update time");
    }

    double duration = time - last_time_;
    integral_ += last_value_ * duration;

    min_ = std::min(min_, value);
    max_ = std::max(max_, value);
    last_time_ = time;
    last_value_ = value;
    ++update_count_;
  }

  /**
   * @brief Gets the number of updates.
   * @return Number of times Update() was called
   */
  size_t Count() const { return update_count_; }

  /**
   * @brief Computes the time-weighted average.
   *
   * Call this at the end of the simulation with the final time to include
   * the last interval.
   *
   * @param end_time The end time of the simulation
   * @return Time-weighted average
   */
  double Average(double end_time) const {
    if (end_time <= 0.0) return 0.0;
    double total_integral = integral_ + last_value_ * (end_time - last_time_);
    return total_integral / end_time;
  }

  /**
   * @brief Finds the minimum value observed.
   * @return Minimum value
   */
  double Min() const { return min_; }

  /**
   * @brief Finds the maximum value observed.
   * @return Maximum value
   */
  double Max() const { return max_; }

  /**
   * @brief Gets the name of this statistic.
   * @return The descriptive name
   */
  const std::string& Name() const { return name_; }

  /**
   * @brief Gets the current integral value.
   * @return The accumulated integral (without the last interval)
   */
  double Integral() const { return integral_; }

  /**
   * @brief Gets the last recorded value.
   * @return The most recent value
   */
  double LastValue() const { return last_value_; }

  /**
   * @brief Gets the last recorded time.
   * @return The time of the most recent update
   */
  double LastTime() const { return last_time_; }

  /**
   * @brief Generates a formatted report of statistical summaries.
   * @param end_time The end time of the simulation
   * @return String containing formatted statistics
   */
  std::string Report(double end_time) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << name_ << " (Time-Weighted)\n";
    oss << "  Updates: " << Count() << "\n";
    oss << "  Average: " << Average(end_time) << "\n";
    oss << "  Min: " << Min() << "\n";
    oss << "  Max: " << Max();
    return oss.str();
  }
};

}  // namespace desvu
