#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "discrete_stats.h"
#include "time_weighted_stats.h"

namespace desvu {

/**
 * @brief Container for managing multiple statistics collectors.
 *
 * Provides a unified interface for collecting both discrete observations
 * and time-weighted statistics. Statistics are automatically created on
 * first use.
 */
class StatsCollector {
 private:
  std::unordered_map<std::string, std::unique_ptr<DiscreteStats>>
      discrete_stats_;
  std::unordered_map<std::string, std::unique_ptr<TimeWeightedStats>>
      time_weighted_stats_;

 public:
  /**
   * @brief Default constructor.
   */
  StatsCollector() = default;

  /**
   * @brief Adds a discrete observation.
   *
   * Creates the statistic if it doesn't exist.
   *
   * @param name The name of the statistic
   * @param value The observed value
   */
  void Add(const std::string& name, double value) {
    auto it = discrete_stats_.find(name);
    if (it == discrete_stats_.end()) {
      discrete_stats_[name] = std::make_unique<DiscreteStats>(name);
    }
    discrete_stats_[name]->Add(value);
  }

  /**
   * @brief Adds a time-weighted observation.
   *
   * Creates the statistic if it doesn't exist.
   *
   * @param name The name of the statistic
   * @param time The current simulation time
   * @param value The new value
   */
  void Add(const std::string& name, double time, double value) {
    auto it = time_weighted_stats_.find(name);
    if (it == time_weighted_stats_.end()) {
      time_weighted_stats_[name] = std::make_unique<TimeWeightedStats>(name);
    }
    time_weighted_stats_[name]->Update(time, value);
  }

  /**
   * @brief Gets a discrete statistic by name.
   * @param name The name of the statistic
   * @return Pointer to the DiscreteStats, or nullptr if not found
   */
  const DiscreteStats* GetDiscrete(const std::string& name) const {
    auto it = discrete_stats_.find(name);
    if (it != discrete_stats_.end()) {
      return it->second.get();
    }
    return nullptr;
  }

  /**
   * @brief Gets a time-weighted statistic by name.
   * @param name The name of the statistic
   * @return Pointer to the TimeWeightedStats, or nullptr if not found
   */
  const TimeWeightedStats* GetTimeWeighted(const std::string& name) const {
    auto it = time_weighted_stats_.find(name);
    if (it != time_weighted_stats_.end()) {
      return it->second.get();
    }
    return nullptr;
  }

  /**
   * @brief Checks if a discrete statistic exists.
   * @param name The name to check
   * @return true if the statistic exists
   */
  bool HasDiscrete(const std::string& name) const {
    return discrete_stats_.find(name) != discrete_stats_.end();
  }

  /**
   * @brief Checks if a time-weighted statistic exists.
   * @param name The name to check
   * @return true if the statistic exists
   */
  bool HasTimeWeighted(const std::string& name) const {
    return time_weighted_stats_.find(name) != time_weighted_stats_.end();
  }

  /**
   * @brief Gets all discrete statistic names.
   * @return Vector of names
   */
  std::vector<std::string> DiscreteNames() const {
    std::vector<std::string> names;
    names.reserve(discrete_stats_.size());
    for (const auto& pair : discrete_stats_) {
      names.push_back(pair.first);
    }
    return names;
  }

  /**
   * @brief Gets all time-weighted statistic names.
   * @return Vector of names
   */
  std::vector<std::string> TimeWeightedNames() const {
    std::vector<std::string> names;
    names.reserve(time_weighted_stats_.size());
    for (const auto& pair : time_weighted_stats_) {
      names.push_back(pair.first);
    }
    return names;
  }

  /**
   * @brief Generates a full report of all statistics.
   * @param end_time The end time of the simulation (for time-weighted stats)
   * @return Formatted string with all statistics
   */
  std::string Report(double end_time) const {
    std::ostringstream oss;
    oss << "=== Statistics Report ===\n";

    bool first = true;
    for (const auto& pair : discrete_stats_) {
      if (!first) oss << "\n\n";
      oss << pair.second->Report();
      first = false;
    }

    for (const auto& pair : time_weighted_stats_) {
      if (!first) oss << "\n\n";
      oss << pair.second->Report(end_time);
      first = false;
    }

    return oss.str();
  }
};

}  // namespace desvu
