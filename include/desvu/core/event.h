#pragma once

#include <string>

namespace desvu {

// Forward declaration
class Simulator;

/**
 * @brief Abstract base class for simulation events.
 *
 * Events represent actions that occur at specific simulation times.
 * Derived classes must implement the action() method.
 */
class Event {
 public:
  double delay;    ///< Delay from scheduling time
  double time;     ///< Absolute time of execution (set by simulator)
  bool cancelled;  ///< Skip execution if true

  /**
   * @brief Constructs an event with the specified delay.
   * @param delay Time delay from scheduling to execution
   */
  explicit Event(double delay) : delay(delay), time(0.0), cancelled(false) {}

  virtual ~Event() = default;

  /**
   * @brief Cancels this event, preventing its execution.
   */
  void cancel() { cancelled = true; }

  /**
   * @brief Executes the event action.
   * @param sim Reference to the simulator
   */
  virtual void action(Simulator& sim) = 0;

  /**
   * @brief Returns a string representation of the event.
   * @return String describing the event
   */
  virtual std::string to_string() const { return "Event()"; }
};

}  // namespace desvu
