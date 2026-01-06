#pragma once

#include <desvu/desvu.h>

#include <queue>

#include "Customer.h"
#include "SimulationConfig.h"

// Forward declaration
class DepartureEvent;

/**
 * @brief Manages the server and customer queue.
 *
 * The Server class encapsulates the queue management logic,
 * tracking whether the server is busy, and handling customer
 * arrivals and service completions.
 */
class Server {
 private:
  std::queue<Customer> waiting_queue_;  ///< FIFO queue of waiting customers
  bool is_busy_;                        ///< Whether server is currently serving
  SimulationConfig& config_;            ///< Reference to simulation configuration
  desvu::StatsCollector& stats_;        ///< Reference to statistics collector

 public:
  /**
   * @brief Constructs a server.
   * @param config Reference to simulation configuration
   * @param stats Reference to statistics collector
   */
  Server(SimulationConfig& config, desvu::StatsCollector& stats)
      : is_busy_(false), config_(config), stats_(stats) {}

  /**
   * @brief Handles a customer arrival.
   *
   * If the server is idle, starts service immediately.
   * Otherwise, adds customer to the waiting queue.
   *
   * @param sim Reference to the simulator
   * @param customer The arriving customer
   */
  void HandleArrival(desvu::Simulator& sim, const Customer& customer);

  /**
   * @brief Handles service completion.
   *
   * If queue is empty, server becomes idle.
   * Otherwise, starts serving the next customer in queue.
   *
   * @param sim Reference to the simulator
   */
  void ServiceCompleted(desvu::Simulator& sim);

  /**
   * @brief Returns the current queue length.
   * @return Number of customers waiting in queue
   */
  size_t QueueLength() const { return waiting_queue_.size(); }

  /**
   * @brief Checks if the server is busy.
   * @return true if server is serving a customer
   */
  bool IsBusy() const { return is_busy_; }
};

