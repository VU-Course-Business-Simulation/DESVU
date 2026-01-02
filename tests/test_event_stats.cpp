/**
 * @file test_event_stats.cpp
 * @brief Unit tests for the EventStats class
 */

#include <desvu/desvu.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

using namespace desvu;

// Test basic construction
TEST_CASE("EventStats construction", "[event_stats]") {
  EventStats stats("Test Stat");
  REQUIRE(stats.Name() == "Test Stat");
  REQUIRE(stats.Count() == 0);
}

// Test single observation
TEST_CASE("EventStats single observation", "[event_stats]") {
  EventStats stats("Test");
  stats.Add(5.0);

  REQUIRE(stats.Count() == 1);
  REQUIRE(stats.Average() == 5.0);
  REQUIRE(stats.Min() == 5.0);
  REQUIRE(stats.Max() == 5.0);
  REQUIRE(stats.StandardDeviation() == 0.0);
}

// Test multiple observations
TEST_CASE("EventStats multiple observations", "[event_stats]") {
  EventStats stats("Test");
  stats.Add(1.0);
  stats.Add(2.0);
  stats.Add(3.0);
  stats.Add(4.0);
  stats.Add(5.0);

  REQUIRE(stats.Count() == 5);
  REQUIRE(stats.Average() == 3.0);
  REQUIRE(stats.Min() == 1.0);
  REQUIRE(stats.Max() == 5.0);

  REQUIRE_THAT(stats.StandardDeviation(),
               Catch::Matchers::WithinAbs(1.581, 0.001));
}

// Test empty statistics
TEST_CASE("EventStats empty stats", "[event_stats]") {
  EventStats stats("Test");

  REQUIRE(stats.Count() == 0);
  REQUIRE(stats.Average() == 0.0);
  REQUIRE(stats.Min() == 0.0);
  REQUIRE(stats.Max() == 0.0);
  REQUIRE(stats.StandardDeviation() == 0.0);
}

// Test negative values
TEST_CASE("EventStats negative values", "[event_stats]") {
  EventStats stats("Test");
  stats.Add(-5.0);
  stats.Add(-3.0);
  stats.Add(-1.0);

  REQUIRE(stats.Average() == -3.0);
  REQUIRE(stats.Min() == -5.0);
  REQUIRE(stats.Max() == -1.0);
  REQUIRE(stats.StandardDeviation() == 2.0);
}

// Test mixed positive/negative values
TEST_CASE("EventStats mixed values", "[event_stats]") {
  EventStats stats("Test");
  stats.Add(-10.0);
  stats.Add(0.0);
  stats.Add(10.0);

  REQUIRE(stats.Average() == 0.0);
  REQUIRE(stats.Min() == -10.0);
  REQUIRE(stats.Max() == 10.0);
}

// Test large number of observations
TEST_CASE("EventStats large dataset", "[event_stats]") {
  EventStats stats("Test");

  for (int i = 1; i <= 1000; ++i) {
    stats.Add(static_cast<double>(i));
  }

  REQUIRE(stats.Count() == 1000);
  REQUIRE(stats.Average() == 500.5);
  REQUIRE(stats.Min() == 1.0);
  REQUIRE(stats.Max() == 1000.0);
}

// Test observations vector access
TEST_CASE("EventStats observations access", "[event_stats]") {
  EventStats stats("Test");
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
TEST_CASE("EventStats report generation", "[event_stats]") {
  EventStats stats("Waiting Time");
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
  REQUIRE(report.find("95% CI") != std::string::npos);
}

// Test identical values (zero variance)
TEST_CASE("EventStats identical values", "[event_stats]") {
  EventStats stats("Test");
  stats.Add(7.0);
  stats.Add(7.0);
  stats.Add(7.0);

  REQUIRE(stats.Average() == 7.0);
  REQUIRE(stats.StandardDeviation() == 0.0);
  REQUIRE(stats.Min() == 7.0);
  REQUIRE(stats.Max() == 7.0);
}

// Test confidence interval - large sample (uses normal approximation)
TEST_CASE("EventStats confidence interval large sample",
          "[event_stats][confidence_interval]") {
  EventStats stats("Test");

  // Add 100 observations of 0s and 100s
  for (int i = 1; i <= 50; ++i) {
    stats.Add(0);
    stats.Add(100);
  }

  auto ci = stats.ConfidenceInterval95();

  // Should successfully compute CI for large sample
  REQUIRE_THAT(ci.first, Catch::Matchers::WithinAbs(40.151, 0.001));
  REQUIRE_THAT(ci.second, Catch::Matchers::WithinAbs(59.850, 0.001));
}

// Test confidence interval - insufficient data
TEST_CASE("EventStats confidence interval insufficient data",
          "[event_stats][confidence_interval]") {
  EventStats stats("Test");

  // No observations - should throw
  REQUIRE_THROWS_AS(stats.ConfidenceInterval95(), std::invalid_argument);
  REQUIRE(stats.Report().find("N/A") != std::string::npos);

  // Only one observation - should throw
  stats.Add(5.0);
  REQUIRE_THROWS_AS(stats.ConfidenceInterval95(), std::invalid_argument);
  REQUIRE(stats.Report().find("N/A") != std::string::npos);
}

// Test confidence interval - identical values (zero variance)
TEST_CASE("EventStats confidence interval zero variance",
          "[event_stats][confidence_interval]") {
  EventStats stats("Test");
  stats.Add(10.0);
  stats.Add(10.0);
  stats.Add(10.0);
  stats.Add(10.0);

  auto ci = stats.ConfidenceInterval95();

  // With zero variance, confidence interval should be very tight around the
  // mean
  REQUIRE_THAT(ci.first, Catch::Matchers::WithinAbs(10.0, 0.001));
  REQUIRE_THAT(ci.second, Catch::Matchers::WithinAbs(10.0, 0.001));
}

// Test confidence interval - small df (df=2)
TEST_CASE("EventStats confidence interval small df",
          "[event_stats][confidence_interval]") {
  EventStats stats("Test");
  stats.Add(5.0);
  stats.Add(10.0);
  stats.Add(15.0);

  auto ci = stats.ConfidenceInterval95();
  REQUIRE_THAT(ci.first, Catch::Matchers::WithinAbs(-2.422, 0.001));
  REQUIRE_THAT(ci.second, Catch::Matchers::WithinAbs(22.422, 0.001));
}

// Test confidence interval - boundary at n=30 vs n=31
TEST_CASE("EventStats confidence interval boundary n=30 vs n=31",
          "[event_stats][confidence_interval]") {
  EventStats stats_30("n=30");
  EventStats stats_31("n=31");

  // Add same values to both, but one more to stats_31
  for (int i = 1; i <= 15; ++i) {
    stats_30.Add(0);
    stats_30.Add(100);
    stats_31.Add(0);
    stats_31.Add(100);
  }
  stats_31.Add(50);  // One extra value

  auto ci_30 = stats_30.ConfidenceInterval95();  // Uses t-distribution
  auto ci_31 =
      stats_31.ConfidenceInterval95();  // Uses normal approximation (z=1.96)

  REQUIRE_THAT(ci_30.first, Catch::Matchers::WithinAbs(31.013, 0.001));
  REQUIRE_THAT(ci_30.second, Catch::Matchers::WithinAbs(68.987, 0.001));

  REQUIRE_THAT(ci_31.first, Catch::Matchers::WithinAbs(32.399, 0.001));
  REQUIRE_THAT(ci_31.second, Catch::Matchers::WithinAbs(67.601, 0.001));
}
