/**
 * @file test_time_weighted_stats.cpp
 * @brief Unit tests for the TimeWeightedStats class
 */

#include <desvu/desvu.h>
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

using namespace desvu;

// Test basic construction
TEST_CASE("TimeWeightedStats construction", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test Stat");
  REQUIRE(stats.Name() == "Test Stat");
  REQUIRE(stats.Count() == 1);  // Constructor counts as first update
  REQUIRE(stats.LastTime() == 0.0);
  REQUIRE(stats.LastValue() == 0.0);
}

// Test single update
TEST_CASE("TimeWeightedStats single update", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(5.0, 10.0);

  REQUIRE(stats.Count() == 2);
  REQUIRE(stats.LastTime() == 5.0);
  REQUIRE(stats.LastValue() == 10.0);

  // Average over [0, 10): value 0 for 5 units, value 10 for 5 units
  REQUIRE(stats.Average(10.0) == 5.0);
}

// Test multiple updates
TEST_CASE("TimeWeightedStats multiple updates", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(0.0, 0.0);   // 0 for time [0, 2)
  stats.Update(2.0, 5.0);   // 5 for time [2, 5)
  stats.Update(5.0, 10.0);  // 10 for time [5, 10)

  // Average = (0*2 + 5*3 + 10*5) / 10 = 65/10 = 6.5
  REQUIRE(stats.Average(10.0) == 6.5);
}

// Test time-weighted average calculation
TEST_CASE("TimeWeightedStats time-weighted average", "[time_weighted_stats]") {
  TimeWeightedStats stats("Queue Length");
  stats.Update(0.0, 0);   // 0 customers from t=0 to t=1
  stats.Update(1.0, 2);   // 2 customers from t=1 to t=4
  stats.Update(4.0, 1);   // 1 customer from t=4 to t=10

  // Average = (0*1 + 2*3 + 1*6) / 10 = 12/10 = 1.2
  REQUIRE(stats.Average(10.0) == 1.2);
}

// Test min/max tracking
TEST_CASE("TimeWeightedStats min/max", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(1.0, 5.0);
  stats.Update(2.0, 3.0);
  stats.Update(3.0, 8.0);
  stats.Update(4.0, 2.0);

  REQUIRE(stats.Min() == 0.0);  // Initial value
  REQUIRE(stats.Max() == 8.0);
}

// Test zero duration (all updates at same time)
TEST_CASE("TimeWeightedStats zero duration", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(0.0, 5.0);
  stats.Update(0.0, 10.0);
  stats.Update(0.0, 15.0);

  // No time passes, so average depends on end_time
  REQUIRE(stats.Average(0.0) == 0.0);  // No time passed
  REQUIRE(stats.LastValue() == 15.0);
}

// Test backward time throws error
TEST_CASE("TimeWeightedStats backward time error", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(5.0, 10.0);

  REQUIRE_THROWS_AS(stats.Update(3.0, 5.0), std::invalid_argument);
}

// Test constant value over time
TEST_CASE("TimeWeightedStats constant value", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(0.0, 7.0);

  // Value is 7 from t=0 to t=100
  REQUIRE(stats.Average(100.0) == 7.0);
}

// Test integral tracking
TEST_CASE("TimeWeightedStats integral tracking", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(0.0, 0.0);
  stats.Update(2.0, 5.0);   // Integral += 0*2 = 0
  stats.Update(5.0, 10.0);  // Integral += 5*3 = 15

  REQUIRE(stats.Integral() == 15.0);
  REQUIRE(stats.LastValue() == 10.0);
}

// Test report generation
TEST_CASE("TimeWeightedStats report generation", "[time_weighted_stats]") {
  TimeWeightedStats stats("Queue Length");
  stats.Update(1.0, 2.0);
  stats.Update(5.0, 3.0);

  std::string report = stats.Report(10.0);

  // Check that report contains expected elements
  REQUIRE(report.find("Queue Length") != std::string::npos);
  REQUIRE(report.find("Time-Weighted") != std::string::npos);
  REQUIRE(report.find("Updates") != std::string::npos);
  REQUIRE(report.find("Average") != std::string::npos);
  REQUIRE(report.find("Min") != std::string::npos);
  REQUIRE(report.find("Max") != std::string::npos);
}

// Test with end time before last update
TEST_CASE("TimeWeightedStats end time before last update", "[time_weighted_stats]") {
  TimeWeightedStats stats("Test");
  stats.Update(0.0, 10.0);
  stats.Update(5.0, 20.0);

  // Trying to get average at time 3 (before second update at time 5) should throw
  REQUIRE_THROWS_AS(stats.Average(3.0), std::invalid_argument);

  // Average at time equal to last update should work
  REQUIRE(stats.Average(5.0) == 10.0);  // (10*5) / 5 = 10

  // Average after last update should work
  REQUIRE(stats.Average(10.0) == 15.0);  // (10*5 + 20*5) / 10 = 150/10 = 15
}

// Test realistic queue example
TEST_CASE("TimeWeightedStats realistic queue example", "[time_weighted_stats]") {
  TimeWeightedStats queue_length("Queue Length");

  // Simulate queue changes
  queue_length.Update(0.0, 0);    // Start empty
  queue_length.Update(1.5, 1);    // Customer arrives
  queue_length.Update(2.0, 2);    // Another arrives
  queue_length.Update(3.5, 1);    // One leaves
  queue_length.Update(5.0, 0);    // Queue empty

  // Average = (0*1.5 + 1*0.5 + 2*1.5 + 1*1.5 + 0*5) / 10
  //         = (0 + 0.5 + 3 + 1.5 + 0) / 10 = 5/10 = 0.5
  REQUIRE(queue_length.Average(10.0) == 0.5);
}

