/**
 * @file test_discrete_stats.cpp
 * @brief Unit tests for the DiscreteStats class
 */

#include <desvu/desvu.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

using namespace desvu;

// Test basic construction
TEST_CASE("DiscreteStats construction", "[discrete_stats]") {
  DiscreteStats stats("Test Stat");
  REQUIRE(stats.Name() == "Test Stat");
  REQUIRE(stats.Count() == 0);
}

// Test single observation
TEST_CASE("DiscreteStats single observation", "[discrete_stats]") {
  DiscreteStats stats("Test");
  stats.Add(5.0);

  REQUIRE(stats.Count() == 1);
  REQUIRE(stats.Average() == 5.0);
  REQUIRE(stats.Min() == 5.0);
  REQUIRE(stats.Max() == 5.0);
  REQUIRE(stats.StandardDeviation() == 0.0);
}

// Test multiple observations
TEST_CASE("DiscreteStats multiple observations", "[discrete_stats]") {
  DiscreteStats stats("Test");
  stats.Add(1.0);
  stats.Add(2.0);
  stats.Add(3.0);
  stats.Add(4.0);
  stats.Add(5.0);

  REQUIRE(stats.Count() == 5);
  REQUIRE(stats.Average() == 3.0);
  REQUIRE(stats.Min() == 1.0);
  REQUIRE(stats.Max() == 5.0);

  // Standard deviation of [1,2,3,4,5] is sqrt(2) ≈ 1.414
  REQUIRE_THAT(stats.StandardDeviation(), Catch::Matchers::WithinAbs(std::sqrt(2.0), 0.001));
}

// Test empty statistics
TEST_CASE("DiscreteStats empty stats", "[discrete_stats]") {
  DiscreteStats stats("Test");

  REQUIRE(stats.Count() == 0);
  REQUIRE(stats.Average() == 0.0);
  REQUIRE(stats.Min() == 0.0);
  REQUIRE(stats.Max() == 0.0);
  REQUIRE(stats.StandardDeviation() == 0.0);
}

// Test negative values
TEST_CASE("DiscreteStats negative values", "[discrete_stats]") {
  DiscreteStats stats("Test");
  stats.Add(-5.0);
  stats.Add(-3.0);
  stats.Add(-1.0);

  REQUIRE(stats.Average() == -3.0);
  REQUIRE(stats.Min() == -5.0);
  REQUIRE(stats.Max() == -1.0);
}

// Test mixed positive/negative values
TEST_CASE("DiscreteStats mixed values", "[discrete_stats]") {
  DiscreteStats stats("Test");
  stats.Add(-10.0);
  stats.Add(0.0);
  stats.Add(10.0);

  REQUIRE(stats.Average() == 0.0);
  REQUIRE(stats.Min() == -10.0);
  REQUIRE(stats.Max() == 10.0);
}

// Test large number of observations
TEST_CASE("DiscreteStats large dataset", "[discrete_stats]") {
  DiscreteStats stats("Test");

  for (int i = 1; i <= 1000; ++i) {
    stats.Add(static_cast<double>(i));
  }

  REQUIRE(stats.Count() == 1000);
  REQUIRE(stats.Average() == 500.5);
  REQUIRE(stats.Min() == 1.0);
  REQUIRE(stats.Max() == 1000.0);
}

// Test observations vector access
TEST_CASE("DiscreteStats observations access", "[discrete_stats]") {
  DiscreteStats stats("Test");
  stats.Add(1.0);
  stats.Add(2.0);
  stats.Add(3.0);

  const auto& obs = stats.Observations();
  REQUIRE(obs.size() == 3);
  REQUIRE(obs[0] == 1.0);
  REQUIRE(obs[1] == 2.0);
  REQUIRE(obs[2] == 3.0);
}

// Test report generation
TEST_CASE("DiscreteStats report generation", "[discrete_stats]") {
  DiscreteStats stats("Waiting Time");
  stats.Add(1.0);
  stats.Add(2.0);
  stats.Add(3.0);

  std::string report = stats.Report();

  // Check that report contains expected elements
  REQUIRE(report.find("Waiting Time") != std::string::npos);
  REQUIRE(report.find("Count") != std::string::npos);
  REQUIRE(report.find("Average") != std::string::npos);
  REQUIRE(report.find("Std Dev") != std::string::npos);
  REQUIRE(report.find("Min") != std::string::npos);
  REQUIRE(report.find("Max") != std::string::npos);
}

// Test identical values (zero variance)
TEST_CASE("DiscreteStats identical values", "[discrete_stats]") {
  DiscreteStats stats("Test");
  stats.Add(7.0);
  stats.Add(7.0);
  stats.Add(7.0);

  REQUIRE(stats.Average() == 7.0);
  REQUIRE(stats.StandardDeviation() == 0.0);
  REQUIRE(stats.Min() == 7.0);
  REQUIRE(stats.Max() == 7.0);
}

