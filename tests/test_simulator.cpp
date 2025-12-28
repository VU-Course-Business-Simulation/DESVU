/**
 * @file test_simulator.cpp
 * @brief Unit tests for the Simulator class
 */

#include <desvu/desvu.h>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

using namespace desvu;

// Simple test event that records when it executes
class TestEvent : public Event {
 public:
  double* execution_time;

  explicit TestEvent(double delay, double* exec_time)
      : Event(delay), execution_time(exec_time) {}

  void action(Simulator& sim) override {
    *execution_time = sim.now();
  }
};

// Event that schedules another event
class ChainEvent : public Event {
 public:
  int* counter;
  int max_count;

  ChainEvent(double delay, int* cnt, int max)
      : Event(delay), counter(cnt), max_count(max) {}

  void action(Simulator& sim) override {
    (*counter)++;
    if (*counter < max_count) {
      sim.schedule(std::make_shared<ChainEvent>(1.0, counter, max_count));
    }
  }
};

// Test basic simulator creation and time
TEST_CASE("Simulator initial time", "[simulator]") {
  Simulator sim;
  REQUIRE(sim.now() == 0.0);
}

// Test single event scheduling and execution
TEST_CASE("Simulator single event", "[simulator]") {
  Simulator sim;
  double exec_time = -1.0;

  sim.schedule(std::make_shared<TestEvent>(5.0, &exec_time));
  sim.run();

  REQUIRE(exec_time == 5.0);
  REQUIRE(sim.now() == 5.0);
}

// Test multiple events execute in correct order
TEST_CASE("Simulator event order", "[simulator]") {
  Simulator sim;
  double exec_time1 = -1.0, exec_time2 = -1.0, exec_time3 = -1.0;

  sim.schedule(std::make_shared<TestEvent>(10.0, &exec_time1));
  sim.schedule(std::make_shared<TestEvent>(5.0, &exec_time2));
  sim.schedule(std::make_shared<TestEvent>(15.0, &exec_time3));

  sim.run();

  REQUIRE(exec_time2 == 5.0);   // Second scheduled, first executed
  REQUIRE(exec_time1 == 10.0);
  REQUIRE(exec_time3 == 15.0);
  REQUIRE(sim.now() == 15.0);
}

// Test events scheduled at same time execute in schedule order
TEST_CASE("Simulator simultaneous events", "[simulator]") {
  Simulator sim;
  double exec_time1 = -1.0, exec_time2 = -1.0;

  sim.schedule(std::make_shared<TestEvent>(5.0, &exec_time1));
  sim.schedule(std::make_shared<TestEvent>(5.0, &exec_time2));

  sim.run();

  REQUIRE(exec_time1 == 5.0);
  REQUIRE(exec_time2 == 5.0);
}

// Test run with time limit
TEST_CASE("Simulator run with time limit", "[simulator]") {
  Simulator sim;
  double exec_time1 = -1.0, exec_time2 = -1.0;

  sim.schedule(std::make_shared<TestEvent>(5.0, &exec_time1));
  sim.schedule(std::make_shared<TestEvent>(15.0, &exec_time2));

  sim.run(10.0);  // Stop before second event

  REQUIRE(exec_time1 == 5.0);
  REQUIRE(exec_time2 == -1.0);  // Not executed
  REQUIRE(sim.now() == 10.0);
}

// Test event cancellation
TEST_CASE("Simulator cancel event", "[simulator]") {
  Simulator sim;
  double exec_time = -1.0;

  auto event = std::make_shared<TestEvent>(5.0, &exec_time);
  sim.schedule(event);
  event->cancel();

  sim.run();

  REQUIRE(exec_time == -1.0);  // Event should not execute
}

// Test chained event scheduling
TEST_CASE("Simulator chained events", "[simulator]") {
  Simulator sim;
  int counter = 0;

  sim.schedule(std::make_shared<ChainEvent>(1.0, &counter, 5));
  sim.run();

  REQUIRE(counter == 5);
  REQUIRE(sim.now() == 5.0);
}

// Test empty simulator
TEST_CASE("Simulator empty run", "[simulator]") {
  Simulator sim;
  sim.run();  // Should not crash
  REQUIRE(sim.now() == 0.0);
}

// Test run with no time limit (until queue empty)
TEST_CASE("Simulator run until empty", "[simulator]") {
  Simulator sim;
  double exec_time = -1.0;

  sim.schedule(std::make_shared<TestEvent>(100.0, &exec_time));
  sim.run(-1.0);  // Negative means no limit

  REQUIRE(exec_time == 100.0);
  REQUIRE(sim.now() == 100.0);
}

