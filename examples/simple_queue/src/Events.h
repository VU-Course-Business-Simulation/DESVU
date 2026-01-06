#pragma once

#include <desvu/desvu.h>

#include <string>

#include "server.h"
#include "simulation_config.h"

/**
 * @brief Customer arrival event.
 *
 * When a customer arrives:
 * 1. Delegate to Server
 * 2. Schedule the next arrival
 */
class ArrivalEvent : public desvu::Event {
 private:
  Server* server_;            ///< Pointer to the server
  SimulationConfig& config_;  ///< Reference to simulation configuration

 public:
  /**
   * @brief Constructs an arrival event.
   * @param delay Time until this event occurs
   * @param server Pointer to the server
   * @param config Reference to simulation configuration
   */
  ArrivalEvent(double delay, Server* server, SimulationConfig& config)
      : Event(delay), server_(server), config_(config) {}

  void Action(desvu::Simulator& sim) override;
  std::string ToString() const override { return "Arrival"; }
};
/**
 * @brief Customer departure event.
 *
 * When a customer completes service:
 * 1. Increment departure counter
 * 2. Delegate to Server to handle service completion
 */
class DepartureEvent : public desvu::Event {
 private:
  Server* server_;  ///< Pointer to the server

 public:
  /**
   * @brief Constructs a departure event.
   * @param delay Time until this event occurs
   * @param server Pointer to the server
   */
  DepartureEvent(double delay, Server* server)
      : Event(delay), server_(server) {}

  void Action(desvu::Simulator& sim) override;
  std::string ToString() const override { return "Departure"; }
};
