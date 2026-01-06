/**
 *
 * TODO: Refine examples.
 *
 * @file simple_queue.cpp
 * @brief Educational example of an M/M/1 queueing system
 *
 * This example demonstrates:
 * - Event scheduling (arrivals and departures)
 * - Queue management (FIFO)
 * - Statistics collection (waiting time, queue length, utilization)
 * - Using random distributions (exponential for both arrivals and service)
 *
 * M/M/1 Queue Model:
 * - M: Markovian (exponential) interarrival times
 * - M: Markovian (exponential) service times
 * - 1: Single server
 *
 * Learning Objectives:
 * 1. Understanding discrete event simulation fundamentals
 * 2. Implementing event-driven logic
 * 3. Collecting and interpreting simulation statistics
 */

#include <desvu/desvu.h>

#include <iostream>
#include <memory>
#include <queue>
#include <random>

// ============================================================================
// Global Simulation State
// ============================================================================

// Random number generator (seeded for reproducibility)
std::mt19937 rng(42);

// Arrival rate (λ) and service rate (μ)
const double ARRIVAL_RATE = 0.8;  // customers per time unit
const double SERVICE_RATE = 1.0;  // customers per time unit

// Random distributions
std::exponential_distribution<> interarrival_dist(ARRIVAL_RATE);
std::exponential_distribution<> service_dist(SERVICE_RATE);

// System state
std::queue<double> waiting_queue;  // Queue of arrival times
bool server_busy = false;

// Statistics
desvu::StatsCollector stats;
int customers_arrived = 0;
int customers_departed = 0;

// ============================================================================
// Event Definitions
// ============================================================================

/**
 * @brief Customer arrival event
 *
 * When a customer arrives:
 * 1. Schedule the next arrival
 * 2. If server is idle, start service immediately
 * 3. Otherwise, join the queue
 */
class ArrivalEvent : public desvu::Event {
 public:
  explicit ArrivalEvent(double delay) : Event(delay) {}
  void Action(desvu::Simulator& sim) override;
  std::string ToString() const override { return "Arrival"; }
};

/**
 * @brief Customer departure event
 *
 * When a customer completes service:
 * 1. If queue is empty, server becomes idle
 * 2. Otherwise, start serving the next customer in queue
 */
class DepartureEvent : public desvu::Event {
 public:
  explicit DepartureEvent(double delay) : Event(delay) {}
  void Action(desvu::Simulator& sim) override;
  std::string ToString() const override { return "Departure"; }
};

// ============================================================================
// Event Implementations
// ============================================================================

void ArrivalEvent::Action(desvu::Simulator& sim) {
  customers_arrived++;
  double arrival_time = sim.Now();

  // Schedule next arrival (Poisson process)
  double next_interarrival = interarrival_dist(rng);
  auto next_arrival = std::make_shared<ArrivalEvent>(next_interarrival);
  sim.Schedule(next_arrival);

  // Update queue length statistics
  stats.Add("Queue Length", sim.Now(),
            static_cast<double>(waiting_queue.size()));

  if (!server_busy) {
    // Server is idle - start service immediately
    server_busy = true;
    stats.Add("Server Utilization", sim.Now(), 1.0);

    // Waiting time is zero
    stats.Add("Waiting Time", 0.0);

    // Schedule departure
    double service_time = service_dist(rng);
    stats.Add("Service Time", service_time);
    auto departure = std::make_shared<DepartureEvent>(service_time);
    sim.Schedule(departure);
  } else {
    // Server is busy - join the queue
    waiting_queue.push(arrival_time);

    // Update queue length
    stats.Add("Queue Length", sim.Now(),
              static_cast<double>(waiting_queue.size()));
  }
}

void DepartureEvent::Action(desvu::Simulator& sim) {
  customers_departed++;

  if (waiting_queue.empty()) {
    // No one waiting - server becomes idle
    server_busy = false;
    stats.Add("Server Utilization", sim.Now(), 0.0);
  } else {
    // Serve next customer in queue
    double arrival_time = waiting_queue.front();
    waiting_queue.pop();

    // Update queue length
    stats.Add("Queue Length", sim.Now(),
              static_cast<double>(waiting_queue.size()));

    // Calculate and record waiting time
    double waiting_time = sim.Now() - arrival_time;
    stats.Add("Waiting Time", waiting_time);

    // Schedule next departure
    double service_time = service_dist(rng);
    stats.Add("Service Time", service_time);
    auto next_departure = std::make_shared<DepartureEvent>(service_time);
    sim.Schedule(next_departure);
  }
}

// ============================================================================
// Main Simulation
// ============================================================================

int main() {
  std::cout << "===========================================\n";
  std::cout << "  M/M/1 Queue Simulation\n";
  std::cout << "===========================================\n";
  std::cout << "Arrival rate (λ): " << ARRIVAL_RATE << "\n";
  std::cout << "Service rate (μ): " << SERVICE_RATE << "\n";
  std::cout << "Traffic intensity (ρ = λ/μ): " << (ARRIVAL_RATE / SERVICE_RATE)
            << "\n";
  std::cout << "\n";

  // Create simulator
  desvu::Simulator sim(false);  // Set to true to see event log

  // Initialize statistics
  stats.Add("Queue Length", 0.0, 0.0);
  stats.Add("Server Utilization", 0.0, 0.0);

  // Schedule first arrival at time 0
  auto first_arrival = std::make_shared<ArrivalEvent>(0.0);
  sim.Schedule(first_arrival);

  // Run simulation
  const double SIM_TIME = 10000.0;
  std::cout << "Running simulation for " << SIM_TIME << " time units...\n\n";
  sim.Run(SIM_TIME);

  // Print results
  std::cout << "===========================================\n";
  std::cout << "  Simulation Results\n";
  std::cout << "===========================================\n";
  std::cout << "Customers arrived: " << customers_arrived << "\n";
  std::cout << "Customers departed: " << customers_departed << "\n";
  std::cout << "Customers still in system: "
            << (customers_arrived - customers_departed) << "\n";
  std::cout << "\n";

  // Print statistics
  std::cout << stats.Report(sim.Now()) << "\n\n";

  // Theoretical comparison (for stable M/M/1 with ρ < 1)
  if (ARRIVAL_RATE < SERVICE_RATE) {
    double rho = ARRIVAL_RATE / SERVICE_RATE;
    double theoretical_avg_queue = (rho * rho) / (1 - rho);
    double theoretical_avg_waiting = rho / (SERVICE_RATE * (1 - rho));
    double theoretical_utilization = rho;

    std::cout << "===========================================\n";
    std::cout << "  Theoretical Values (M/M/1)\n";
    std::cout << "===========================================\n";
    std::cout << "Average queue length (L_q): " << theoretical_avg_queue
              << "\n";
    std::cout << "Average waiting time (W_q): " << theoretical_avg_waiting
              << "\n";
    std::cout << "Server utilization (ρ): " << theoretical_utilization << "\n";
  }

  return 0;
}
