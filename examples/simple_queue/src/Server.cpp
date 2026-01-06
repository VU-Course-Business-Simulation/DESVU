#include "Server.h"

#include "Events.h"

void Server::HandleArrival(desvu::Simulator& sim, const Customer& customer) {
  // Update queue length statistics
  stats_.Add("Queue Length", sim.Now(),
             static_cast<double>(waiting_queue_.size()));

  if (!is_busy_) {
    // Server is idle - start service immediately
    is_busy_ = true;
    stats_.Add("Server Utilization", sim.Now(), 1.0);

    // Waiting time is zero
    stats_.Add("Waiting Time", 0.0);

    // Schedule departure
    double service_time = config_.NextServiceTime();
    stats_.Add("Service Time", service_time);
    auto departure = std::make_shared<DepartureEvent>(service_time, this);
    sim.Schedule(departure);
  } else {
    // Server is busy - join the queue
    waiting_queue_.push(customer);

    // Update queue length
    stats_.Add("Queue Length", sim.Now(),
               static_cast<double>(waiting_queue_.size()));
  }
}

void Server::ServiceCompleted(desvu::Simulator& sim) {
  if (waiting_queue_.empty()) {
    // No one waiting - server becomes idle
    is_busy_ = false;
    stats_.Add("Server Utilization", sim.Now(), 0.0);
  } else {
    // Serve next customer in queue
    Customer next_customer = waiting_queue_.front();
    waiting_queue_.pop();

    // Update queue length
    stats_.Add("Queue Length", sim.Now(),
               static_cast<double>(waiting_queue_.size()));

    // Calculate and record waiting time
    double waiting_time = next_customer.WaitingTime(sim.Now());
    stats_.Add("Waiting Time", waiting_time);

    // Schedule next departure
    double service_time = config_.NextServiceTime();
    stats_.Add("Service Time", service_time);
    auto next_departure = std::make_shared<DepartureEvent>(service_time, this);
    sim.Schedule(next_departure);
  }
}

