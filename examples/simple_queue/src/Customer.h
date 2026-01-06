#pragma once

/**
 * @brief Represents a customer in the queueing system.
 *
 * Stores information about a customer including their arrival time.
 */
class Customer {
 public:
  double arrival_time;  ///< Time when customer arrived

  /**
   * @brief Constructs a customer with given arrival time.
   * @param arrival_time Time when customer entered the system
   */
  explicit Customer(double arrival_time) : arrival_time(arrival_time) {}

  /**
   * @brief Calculates waiting time based on current time.
   * @param current_time Current simulation time
   * @return Time spent waiting in queue
   */
  double WaitingTime(double current_time) const {
    return current_time - arrival_time;
  }
};
