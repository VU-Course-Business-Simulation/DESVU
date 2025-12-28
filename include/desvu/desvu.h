/**
 * @file desvu.h
 * @brief DESVU - Discrete Event Simulation for Vrije Universiteit Amsterdam
 *
 * This is a convenience header that includes all components of the DESVU
 * library. You can include individual headers from desvu/core/ or desvu/stats/
 * if you prefer fine-grained control.
 *
 * Core simulation components:
 *   - Event: Base class for simulation events
 *   - Simulator: Discrete event simulation engine
 *
 * Statistics collection:
 *   - DiscreteStats: Statistics for discrete observations (e.g., waiting times)
 *   - TimeWeightedStats: Statistics for time-weighted values (e.g., queue length)
 *   - StatsCollector: Manages multiple statistics with a unified interface
 *
 * @example
 * @code
 * #include <desvu/desvu.h>
 *
 * class ArrivalEvent : public desvu::Event {
 *  public:
 *   explicit ArrivalEvent(double delay) : Event(delay) {}
 *   void action(desvu::Simulator& sim) override {
 *     // Handle customer arrival
 *   }
 * };
 *
 * int main() {
 *   desvu::Simulator sim;
 *   sim.schedule(std::make_shared<ArrivalEvent>(0.0));
 *   sim.run(100.0);
 *   return 0;
 * }
 * @endcode
 */

#pragma once

// Core simulation
#include "core/event.h"
#include "core/simulator.h"

// Statistics
#include "stats/discrete_stats.h"
#include "stats/stats_collector.h"
#include "stats/time_weighted_stats.h"

/**
 * @namespace desvu
 * @brief DESVU - Discrete Event Simulation for Vrije Universiteit Amsterdam
 *
 * A lightweight, header-only C++ library for teaching discrete event
 * simulation concepts. The library provides:
 *
 * - **Event-driven simulation**: Schedule and execute events in chronological order
 * - **Statistics collection**: Built-in tools for collecting and analyzing simulation data
 * - **Educational design**: Clear APIs and documentation for learning DES concepts
 *
 * The library follows the three-phase simulation approach:
 * 1. Initialize: Set up initial events and state
 * 2. Advance time: Move to the next scheduled event
 * 3. Execute event: Perform the event's action, possibly scheduling new events
 *
 * @see Simulator for the main simulation engine
 * @see Event for creating custom event types
 */

