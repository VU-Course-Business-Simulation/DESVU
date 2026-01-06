#pragma once

#include <desvu/desvu.h>

#include <queue>

#include "Customer.h"
#include "SimulationConfig.h"

/**
 * @brief Manages the server and customer queue.
 *
 * The Server class encapsulates the queue management logic,
 * tracking whether the server is busy, and handling customer
 * arrivals and service completions. It also owns and manages
 * its own statistics collection.
 */
class Server {
 private:
  std::queue<Customer> queue_;        ///< FIFO queue of waiting customers
  bool is_busy_;                      ///< Whether server is currently serving
  SimulationConfig& config_;          ///< Reference to simulation config.
  desvu::Simulator& sim_;             ///< Reference to simulator
  desvu::StatsCollector stats_;       ///< Statistics collector

 public:
  /**
   * @brief Constructs a server.
   * @param sim Reference to the simulator
   * @param config Reference to simulation configuration
   */
  Server(desvu::Simulator& sim, SimulationConfig& config);

  /**
   * @brief Handles a customer arrival.
   *
   * If the server is idle, starts service immediately.
   * Otherwise, adds customer to the waiting queue.
   *
   * @param customer The arriving customer
   */
  void HandleArrival(const Customer& customer);

  /**
   * @brief Handles service completion.
   *
   * If queue is empty, server becomes idle.
   * Otherwise, starts serving the next customer in queue.
   */
  void ServiceCompleted();

  /**
   * @brief Returns the current queue length.
   * @return Number of customers waiting in queue
   */
  size_t QueueLength() const { return queue_.size(); }

  /**
   * @brief Checks if the server is busy.
   * @return true if server is serving a customer
   */
  bool IsBusy() const { return is_busy_; }

  /**
   * @brief Gets the statistics collector.
   * @return Reference to the statistics collector
   */
  desvu::StatsCollector& GetStats() { return stats_; }
};
