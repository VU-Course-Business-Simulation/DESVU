#include "events.h"

#include <memory>

#include "customer.h"

void ArrivalEvent::Action(desvu::Simulator& sim) {
  // Create new customer and delegate to server
  Customer new_customer(sim.Now());
  server_->HandleArrival(new_customer);

  // Schedule next arrival
  double next_interarrival = config_.NextInterarrivalTime();
  auto next_arrival =
      std::make_shared<ArrivalEvent>(next_interarrival, server_, config_);
  sim.Schedule(next_arrival);
}

void DepartureEvent::Action(desvu::Simulator& sim) {
  // Delegate to server to handle service completion
  server_->HandleServiceCompletion();
}
