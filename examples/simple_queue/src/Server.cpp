#include "Server.h"

#include "Events.h"

Server::Server(desvu::Simulator& sim, SimulationConfig& config)
    : sim_(sim), config_(config), is_busy_(false), stats_() {}

void Server::HandleArrival(const Customer& customer) {
  if (is_busy_) {
    // Server is busy - join the queue
    queue_.push(customer);
    stats_.Add("Queue Length", sim_.Now(),
               static_cast<double>(queue_.size()));
  } else {
    // Server is idle - start service immediately
    is_busy_ = true;
    stats_.Add("Server Utilization", sim_.Now(), 1.0);
    stats_.Add("Waiting Time", 0.0);

    ScheduleServiceCompletion();
  }
}

void Server::HandleServiceCompletion() {
  if (queue_.empty()) {
    // No one waiting - server becomes idle
    is_busy_ = false;
  } else {
    // Serve next customer in queue
    Customer next_customer = queue_.front();
    queue_.pop();
    stats_.Add("Queue Length", sim_.Now(),
               static_cast<double>(queue_.size()));

    // Calculate and record waiting time
    double waiting_time = next_customer.WaitingTime(sim_.Now());
    stats_.Add("Waiting Time", waiting_time);

    ScheduleServiceCompletion();
  }
}

void Server::ScheduleServiceCompletion() {
  double service_time = config_.NextServiceTime();
  auto departure = std::make_shared<DepartureEvent>(service_time, this);
  sim_.Schedule(departure);
}

