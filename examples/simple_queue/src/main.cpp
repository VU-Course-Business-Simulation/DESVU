/**
 * @file main.cpp
 * @brief Educational example of an M/M/1 queueing system
 *
 * This example demonstrates:
 * - Event scheduling (arrivals and departures)
 * - Queue management (FIFO)
 * - Statistics collection (waiting time, queue length, utilization)
 * - Using random distributions (exponential for both arrivals and service)
 * - Running multiple replications for statistical accuracy
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
 * 4. Running multiple replications for confidence intervals
 */

#include <desvu/desvu.h>

#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include "Events.h"
#include "Server.h"
#include "SimulationConfig.h"

/**
 * @brief Runs a single simulation replication.
 *
 * @param config Simulation configuration
 * @param verbose Whether to print event log
 * @return StatsCollector containing simulation results
 */
desvu::StatsCollector RunSimulation(SimulationConfig& config,
                                    bool verbose = false) {
  // Create simulator
  desvu::Simulator sim(verbose);

  // Create statistics collector
  desvu::StatsCollector stats;

  // Initialize statistics
  stats.Add("Queue Length", 0.0, 0.0);
  stats.Add("Server Utilization", 0.0, 0.0);

  // Create server
  Server server(config, stats);

  // Customer counter
  int customers_arrived = 0;

  // Schedule first arrival at time 0
  auto first_arrival = std::make_shared<ArrivalEvent>(0.0, &server, config,
                                                      customers_arrived);
  sim.Schedule(first_arrival);

  // Run simulation
  sim.Run(config.simulation_time);

  return stats;
}

/**
 * @brief Prints theoretical M/M/1 queue results.
 *
 * @param arrival_rate Customer arrival rate (λ)
 * @param service_rate Service rate (μ)
 */
void PrintTheoreticalResults(double arrival_rate, double service_rate) {
  if (arrival_rate >= service_rate) {
    std::cout << "\nNote: System is unstable (ρ >= 1), theoretical values "
                 "not applicable.\n";
    return;
  }

  double rho = arrival_rate / service_rate;
  double theoretical_avg_queue = (rho * rho) / (1 - rho);
  double theoretical_avg_waiting = rho / (service_rate * (1 - rho));
  double theoretical_utilization = rho;

  std::cout << "\n===========================================\n";
  std::cout << "  Theoretical Values (M/M/1)\n";
  std::cout << "===========================================\n";
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Average queue length (L_q): " << theoretical_avg_queue << "\n";
  std::cout << "Average waiting time (W_q): " << theoretical_avg_waiting
            << "\n";
  std::cout << "Server utilization (ρ): " << theoretical_utilization << "\n";
}

/**
 * @brief Aggregates results from multiple replications.
 *
 * @param all_stats Vector of StatsCollector from each replication
 * @param sim_time Simulation time used for time-weighted statistics
 */
void PrintAggregatedResults(
    const std::vector<desvu::StatsCollector>& all_stats, double sim_time) {
  // Collect waiting times from all replications
  desvu::EventStats aggregated_waiting("Waiting Time (All Replications)");
  desvu::EventStats aggregated_service("Service Time (All Replications)");

  for (const auto& stats : all_stats) {
    const auto* waiting = stats.GetEvent("Waiting Time");
    if (waiting) {
      for (double obs : waiting->Observations()) {
        aggregated_waiting.Add(obs);
      }
    }

    const auto* service = stats.GetEvent("Service Time");
    if (service) {
      for (double obs : service->Observations()) {
        aggregated_service.Add(obs);
      }
    }
  }

  // Collect averages for time-weighted statistics
  desvu::EventStats avg_queue_length("Average Queue Length per Replication");
  desvu::EventStats avg_utilization("Average Utilization per Replication");

  for (const auto& stats : all_stats) {
    const auto* queue = stats.GetTimeWeighted("Queue Length");
    if (queue) {
      avg_queue_length.Add(queue->Average(sim_time));
    }

    const auto* util = stats.GetTimeWeighted("Server Utilization");
    if (util) {
      avg_utilization.Add(util->Average(sim_time));
    }
  }

  std::cout << "\n===========================================\n";
  std::cout << "  Aggregated Results Across Replications\n";
  std::cout << "===========================================\n";
  std::cout << "\n" << aggregated_waiting.Report() << "\n\n";
  std::cout << aggregated_service.Report() << "\n\n";
  std::cout << avg_queue_length.Report() << "\n\n";
  std::cout << avg_utilization.Report() << "\n";
}

int main() {
  std::cout << "===========================================\n";
  std::cout << "  M/M/1 Queue Simulation\n";
  std::cout << "===========================================\n";

  // Create configuration
  SimulationConfig config(
      10000.0,  // simulation_time
      0.8,      // arrival_rate
      1.0,      // service_rate
      42        // random seed
  );

  std::cout << "Arrival rate: " << config.arrival_rate << "\n";
  std::cout << "Service rate: " << config.service_rate << "\n";
  std::cout << "Simulation time: " << config.simulation_time << "\n";

  // Run multiple replications
  const int NUM_REPLICATIONS = 100;
  std::cout << "Number of replications: " << NUM_REPLICATIONS << "\n";
  std::cout << "\n";

  std::vector<desvu::StatsCollector> all_stats;
  all_stats.reserve(NUM_REPLICATIONS);

  std::cout << "Running " << NUM_REPLICATIONS << " replications...\n";
  for (int i = 0; i < NUM_REPLICATIONS; ++i) {
    // Create new config with different seed for each replication
    SimulationConfig rep_config(config.simulation_time, config.arrival_rate,
                                config.service_rate, 42 + i * 100);
    auto stats = RunSimulation(rep_config, false);
    all_stats.push_back(std::move(stats));
    std::cout << "  Completed replication " << (i + 1) << "/"
              << NUM_REPLICATIONS << "\n";
  }

  // Print aggregated results
  PrintAggregatedResults(all_stats, config.simulation_time);

  // Print theoretical comparison
  PrintTheoreticalResults(config.arrival_rate, config.service_rate);

  std::cout << "\n===========================================\n";
  std::cout << "  Single Replication Example\n";
  std::cout << "===========================================\n";
  std::cout << "Running one detailed simulation...\n\n";

  SimulationConfig single_config(10000.0, 0.8, 1.0, 999);
  auto single_stats = RunSimulation(single_config, false);
  std::cout << single_stats.Report(single_config.simulation_time) << "\n";

  return 0;
}

