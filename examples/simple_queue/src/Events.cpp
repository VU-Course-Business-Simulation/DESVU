#include "Events.h"

#include <memory>

#include "Customer.h"

void ArrivalEvent::Action(desvu::Simulator& sim) {
  double arrival_time = sim.Now();

  // Schedule next arrival
  double next_interarrival = config_.NextInterarrivalTime();
  auto next_arrival =
      std::make_shared<ArrivalEvent>(next_interarrival, server_, config_);
  sim.Schedule(next_arrival);

  // Create customer and delegate to server
  Customer customer(arrival_time);
  server_->HandleArrival(customer);
}

void DepartureEvent::Action(desvu::Simulator& sim) {
  // Delegate to server to handle service completion
  server_->ServiceCompleted();
}
