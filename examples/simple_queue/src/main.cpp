/**
 * @file main.cpp
 * @brief Educational example of an M/M/1 queueing system
 *
 * This example demonstrates:
 * - Event scheduling (arrivals and departures)
 * - Queue management (FIFO)
 * - Statistics collection (waiting time, queue length, utilization)
 * - Using random distributions (exponential for both arrivals and service)
 * - Running multiple runs for statistical accuracy
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
 * 4. Running multiple runs for confidence intervals
 */

#include <desvu/desvu.h>

#include <iomanip>
#include <iostream>
#include <memory>

#include "Events.h"
#include "Server.h"
#include "SimulationConfig.h"

/**
 * @brief Result structure returned by RunSimulation.
 */
struct SimRunResults {
  double mean_waiting_time;  ///< Mean waiting time of jobs
  double mean_queue_length;  ///< Mean queue length
};

/**
 * @brief Runs a single simulation run.
 *
 * @param config Simulation configuration
 * @param verbose Whether to print event log
 * @return StatsCollector containing simulation results
 */
SimRunResults RunSimulation(SimulationConfig& config, bool verbose = false) {
  // Create simulator
  desvu::Simulator sim(verbose);

  // Create server
  Server server(sim, config);

  // Schedule first arrival
  double first_delay = config.NextInterarrivalTime();
  auto first_arrival =
      std::make_shared<ArrivalEvent>(first_delay, &server, config);
  sim.Schedule(first_arrival);

  // Run simulation
  sim.Run(config.sim_time);

  // Get statistics from server
  auto& stats = server.GetStats();
  double mean_wait_time = stats.GetEvent("Waiting Time")->Average();
  double mean_queue_length =
      stats.GetTimeWeighted("Queue Length")->Average(config.sim_time);

  return SimRunResults{mean_wait_time, mean_queue_length};
}

/**
 * @brief Prints theoretical M/M/1 queue results.
 *
 * @param arrival_rate Customer arrival rate (λ)
 * @param service_rate Service rate (μ)
 */
void PrintTheoreticalResults(double arrival_rate, double service_rate) {
  if (arrival_rate >= service_rate) {
    std::cout << "\nSystem unstable (ρ >= 1), no theoretical values.\n";
    return;
  }

  double rho = arrival_rate / service_rate;
  double theory_mean_queue = (rho * rho) / (1 - rho);
  double theory_mean_wait = rho / (service_rate * (1 - rho));

  std::cout << "\n\nTheoretical Values (M/M/1):\n";
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "  Mean queue length: " << theory_mean_queue << "\n";
  std::cout << "  Mean waiting time: " << theory_mean_wait  << "\n";
}

int main() {

  // Create simulation configuration
  SimulationConfig config(10000.0,  // sim_time
                          0.8,      // arrival_rate
                          1.0,      // service_rate
                          0         // random seed
  );

  // Run multiple simulation runs
  desvu::StatsCollector stats;
  const int num_runs = 100;
  for (int i = 0; i < num_runs; ++i) {

    // Run simulation with config with new seed i
    SimulationConfig run_config(config.sim_time, config.arrival_rate,
                                config.service_rate, i);
    auto run_stats = RunSimulation(run_config, false);

    // Store statistics from this run
    stats.Add("Mean Waiting Time", run_stats.mean_waiting_time);
    stats.Add("Mean Queue Length", run_stats.mean_queue_length);
  }

  std::cout << stats.Report(0.0);

  // Print theoretical comparison
  PrintTheoreticalResults(config.arrival_rate, config.service_rate);

  return 0;
}
