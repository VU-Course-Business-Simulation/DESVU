#pragma once

#include <desvu/desvu.h>

#include <string>

#include "Server.h"
#include "SimulationConfig.h"

/**
 * @brief Customer arrival event.
 *
 * When a customer arrives:
 * 1. Schedule the next arrival
 * 2. Delegate to Server to handle the arrival
 */
class ArrivalEvent : public desvu::Event {
 private:
  Server* server_;               ///< Pointer to the server
  SimulationConfig& config_;     ///< Reference to simulation configuration
  int& customers_arrived_;       ///< Reference to customer counter

 public:
  /**
   * @brief Constructs an arrival event.
   * @param delay Time until this event occurs
   * @param server Pointer to the server
   * @param config Reference to simulation configuration
   * @param customers_arrived Reference to customer arrival counter
   */
  ArrivalEvent(double delay, Server* server, SimulationConfig& config,
               int& customers_arrived)
      : Event(delay),
        server_(server),
        config_(config),
        customers_arrived_(customers_arrived) {}

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

