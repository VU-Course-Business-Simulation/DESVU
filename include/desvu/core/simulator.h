#pragma once

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>

#include "event.h"

namespace desvu {

/**
 * @brief Internal implementation details (not part of public API).
 */
namespace detail {

/**
 * @brief Represents an event in the priority queue.
 *
 * Wraps an event with its scheduled time and unique ID for ordering.
 */
struct ScheduledEvent {
  double time;                   ///< Scheduled execution time
  std::size_t id;                ///< Unique event identifier
  std::shared_ptr<Event> event;  ///< The actual event

  /**
   * @brief Constructs a scheduled event.
   * @param t Execution time
   * @param i Unique identifier
   * @param e Shared pointer to the event
   */
  ScheduledEvent(double t, std::size_t i, std::shared_ptr<Event> e)
      : time(t), id(i), event(std::move(e)) {}

  /**
   * @brief Comparison operator for priority queue ordering.
   *
   * Creates a min-heap with earliest time having highest priority.
   * Ties are broken by ID (lower ID = scheduled earlier).
   *
   * @param other The other scheduled event to compare with
   * @return true if this event has lower priority than other
   */
  bool operator<(const ScheduledEvent& other) const {
    // min-heap (earliest time has highest priority)
    // Break ties by id (lower id = scheduled earlier = higher priority)
    if (time != other.time) return time > other.time;
    return id > other.id;
  }
};

}  // namespace detail

/**
 * @brief Discrete event simulator.
 *
 * Manages simulation time and executes events in chronological order.
 * Events are scheduled with delays and executed by the simulator.
 */
class Simulator {
 private:
  double time;                ///< Current simulation time
  bool log_events;            ///< Whether to log events to console
  std::size_t event_counter;  ///< Counter for unique event IDs
  std::priority_queue<detail::ScheduledEvent> event_queue;  ///< Event queue

 public:
  /**
   * @brief Constructs a new Simulator.
   * @param log_events Whether to log events to console
   */
  explicit Simulator(bool log_events = false)
      : time(0.0), log_events(log_events), event_counter(0) {}

  /**
   * @brief Returns the current simulation time.
   * @return Current time
   */
  double now() const { return time; }

  /**
   * @brief Schedules an event for future execution.
   *
   * The event will execute at time now() + event->delay.
   *
   * @param event Shared pointer to the event to schedule
   */
  void schedule(const std::shared_ptr<Event>& event) {
    event->time = time + event->delay;
    event_queue.emplace(event->time, event_counter++, event);
  }

  /**
   * @brief Runs the simulation.
   *
   * Processes events in chronological order until the queue is empty
   * or the time limit is reached.
   *
   * @param until Maximum simulation time (-1.0 for unlimited)
   */
  void run(double until = -1.0) {
    while (!event_queue.empty()) {
      auto [event_time, id, event] = event_queue.top();
      event_queue.pop();

      if (until >= 0.0 && event_time > until) {
        time = until;
        return;
      }

      if (event->cancelled) continue;

      time = event_time;

      if (log_events)
        std::cout << "t=" << std::setw(6) << std::setprecision(1) << std::fixed
                  << time << " | " << event->to_string() << "\n";

      event->action(*this);
    }
  }
};

}  // namespace desvu
