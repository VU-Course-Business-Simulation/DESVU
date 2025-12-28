/**
 * @file test_stats_collector.cpp
 * @brief Unit tests for the StatsCollector class
 */

#include <desvu/desvu.h>
#include <catch2/catch_test_macros.hpp>

using namespace desvu;

// Test basic construction
TEST_CASE("StatsCollector construction", "[stats_collector]") {
  StatsCollector collector;
  REQUIRE_FALSE(collector.HasDiscrete("Test"));
  REQUIRE_FALSE(collector.HasTimeWeighted("Test"));
}

// Test adding discrete observations
TEST_CASE("StatsCollector add discrete observations", "[stats_collector]") {
  StatsCollector collector;

  collector.Add("Waiting Time", 5.0);
  collector.Add("Waiting Time", 7.0);
  collector.Add("Waiting Time", 3.0);

  REQUIRE(collector.HasDiscrete("Waiting Time"));

  const DiscreteStats* stats = collector.GetDiscrete("Waiting Time");
  REQUIRE(stats != nullptr);
  REQUIRE(stats->Count() == 3);
  REQUIRE(stats->Average() == 5.0);
}

// Test adding time-weighted observations
TEST_CASE("StatsCollector add time-weighted observations", "[stats_collector]") {
  StatsCollector collector;

  collector.Add("Queue Length", 0.0, 0.0);
  collector.Add("Queue Length", 2.0, 5.0);
  collector.Add("Queue Length", 5.0, 3.0);

  REQUIRE(collector.HasTimeWeighted("Queue Length"));

  const TimeWeightedStats* stats = collector.GetTimeWeighted("Queue Length");
  REQUIRE(stats != nullptr);
  REQUIRE(stats->Count() == 4);  // Constructor + 3 updates
  REQUIRE(stats->LastValue() == 3.0);
}

// Test automatic creation of statistics
TEST_CASE("StatsCollector automatic creation", "[stats_collector]") {
  StatsCollector collector;

  // First call should create the statistic
  collector.Add("New Stat", 10.0);
  REQUIRE(collector.HasDiscrete("New Stat"));

  // Second call should use existing statistic
  collector.Add("New Stat", 20.0);

  const DiscreteStats* stats = collector.GetDiscrete("New Stat");
  REQUIRE(stats->Count() == 2);
}

// Test multiple statistics
TEST_CASE("StatsCollector multiple statistics", "[stats_collector]") {
  StatsCollector collector;

  collector.Add("Stat A", 1.0);
  collector.Add("Stat B", 2.0);
  collector.Add("Stat C", 0.0, 5.0);
  collector.Add("Stat D", 0.0, 10.0);

  REQUIRE(collector.HasDiscrete("Stat A"));
  REQUIRE(collector.HasDiscrete("Stat B"));
  REQUIRE(collector.HasTimeWeighted("Stat C"));
  REQUIRE(collector.HasTimeWeighted("Stat D"));
  REQUIRE_FALSE(collector.HasDiscrete("Stat C"));
  REQUIRE_FALSE(collector.HasTimeWeighted("Stat A"));
}

// Test getting non-existent statistics
TEST_CASE("StatsCollector get non-existent", "[stats_collector]") {
  StatsCollector collector;

  REQUIRE(collector.GetDiscrete("NonExistent") == nullptr);
  REQUIRE(collector.GetTimeWeighted("NonExistent") == nullptr);
}

// Test getting discrete names
TEST_CASE("StatsCollector get discrete names", "[stats_collector]") {
  StatsCollector collector;

  collector.Add("Waiting Time", 5.0);
  collector.Add("Service Time", 3.0);
  collector.Add("Interarrival Time", 2.0);

  auto names = collector.DiscreteNames();
  REQUIRE(names.size() == 3);

  // Check all names are present (order not guaranteed with unordered_map)
  bool has_waiting = false, has_service = false, has_interarrival = false;
  for (const auto& name : names) {
    if (name == "Waiting Time") has_waiting = true;
    if (name == "Service Time") has_service = true;
    if (name == "Interarrival Time") has_interarrival = true;
  }
  REQUIRE(has_waiting);
  REQUIRE(has_service);
  REQUIRE(has_interarrival);
}

// Test getting time-weighted names
TEST_CASE("StatsCollector get time-weighted names", "[stats_collector]") {
  StatsCollector collector;

  collector.Add("Queue Length", 0.0, 0.0);
  collector.Add("Server Utilization", 0.0, 0.0);

  auto names = collector.TimeWeightedNames();
  REQUIRE(names.size() == 2);

  bool has_queue = false, has_util = false;
  for (const auto& name : names) {
    if (name == "Queue Length") has_queue = true;
    if (name == "Server Utilization") has_util = true;
  }
  REQUIRE(has_queue);
  REQUIRE(has_util);
}

// Test report generation
TEST_CASE("StatsCollector report generation", "[stats_collector]") {
  StatsCollector collector;

  collector.Add("Waiting Time", 5.0);
  collector.Add("Waiting Time", 10.0);
  collector.Add("Queue Length", 0.0, 0.0);
  collector.Add("Queue Length", 5.0, 2.0);

  std::string report = collector.Report(10.0);

  // Check report contains key elements
  REQUIRE(report.find("Statistics Report") != std::string::npos);
  REQUIRE(report.find("Waiting Time") != std::string::npos);
  REQUIRE(report.find("Queue Length") != std::string::npos);
}

// Test empty report
TEST_CASE("StatsCollector empty report", "[stats_collector]") {
  StatsCollector collector;

  std::string report = collector.Report(10.0);
  REQUIRE(report.find("Statistics Report") != std::string::npos);
}

// Test mixed discrete and time-weighted
TEST_CASE("StatsCollector mixed statistics", "[stats_collector]") {
  StatsCollector collector;

  // Add several discrete
  collector.Add("Waiting Time", 5.0);
  collector.Add("Service Time", 3.0);

  // Add several time-weighted
  collector.Add("Queue Length", 0.0, 0.0);
  collector.Add("Server Busy", 0.0, 0.0);

  REQUIRE(collector.DiscreteNames().size() == 2);
  REQUIRE(collector.TimeWeightedNames().size() == 2);

  REQUIRE(collector.GetDiscrete("Waiting Time") != nullptr);
  REQUIRE(collector.GetTimeWeighted("Queue Length") != nullptr);
  REQUIRE(collector.GetTimeWeighted("Waiting Time") == nullptr);
  REQUIRE(collector.GetDiscrete("Queue Length") == nullptr);
}

// Test realistic simulation scenario
TEST_CASE("StatsCollector realistic simulation scenario", "[stats_collector]") {
  StatsCollector stats;

  // Simulate a queue system
  // Customer 1
  stats.Add("Interarrival Time", 1.5);
  stats.Add("Queue Length", 0.0, 0);
  stats.Add("Queue Length", 1.5, 1);
  stats.Add("Waiting Time", 0.0);
  stats.Add("Service Time", 2.0);
  stats.Add("Queue Length", 3.5, 0);

  // Customer 2
  stats.Add("Interarrival Time", 1.0);
  stats.Add("Queue Length", 2.5, 1);
  stats.Add("Waiting Time", 1.0);
  stats.Add("Service Time", 1.5);
  stats.Add("Queue Length", 5.0, 0);

  // Verify statistics exist
  REQUIRE(stats.HasDiscrete("Waiting Time"));
  REQUIRE(stats.HasDiscrete("Service Time"));
  REQUIRE(stats.HasDiscrete("Interarrival Time"));
  REQUIRE(stats.HasTimeWeighted("Queue Length"));

  // Verify counts
  const auto* waiting = stats.GetDiscrete("Waiting Time");
  REQUIRE(waiting->Count() == 2);

  const auto* queue = stats.GetTimeWeighted("Queue Length");
  REQUIRE(queue->Count() == 6);  // Constructor + 5 updates

  // Generate report
  std::string report = stats.Report(10.0);
  REQUIRE_FALSE(report.empty());
}

