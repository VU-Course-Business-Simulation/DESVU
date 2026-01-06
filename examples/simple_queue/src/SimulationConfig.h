#pragma once

#include <random>

/**
 * @brief Centralized configuration for the M/M/1 queue simulation.
 *
 * Contains all timing parameters and random number generators
 * for the discrete event simulation.
 */
struct SimulationConfig {
  // Simulation parameters
  double sim_time;  ///< Total simulation time
  double arrival_rate;     ///< Customer arrival rate (λ)
  double service_rate;     ///< Service rate (μ)

  // Random number generation per stream for Common Random Numbers
  std::mt19937 arrival_rng;  ///< RNG for arrivals
  std::mt19937 service_rng;  ///< RNG for service times

  // Specify distributions
  std::exponential_distribution<double> interarrival_dist;
  std::exponential_distribution<double> service_dist;

  /**
   * @brief Constructs simulation configuration for M/M/1 queue.
   *
   * Default values:
   * - Simulation time: 10000.0 time units
   * - Arrival rate (λ): 0.8 customers per time unit
   * - Service rate (μ): 1.0 customers per time unit
   * - Traffic intensity (ρ = λ/μ): 0.8
   *
   * @param sim_time Total time to run the simulation
   * @param arrival_rate Customer arrival rate (λ)
   * @param service_rate Service rate (μ)
   * @param seed Random seed for reproducibility (default: 42)
   */
  explicit SimulationConfig(double sim_time = 10000.0,
                            double arrival_rate = 0.8,
                            double service_rate = 1.0, unsigned int seed = 42)
      : sim_time(sim_time),
        arrival_rate(arrival_rate),
        service_rate(service_rate),
        arrival_rng(seed),
        service_rng(seed + 11),  // + 11 to have different seeds
        interarrival_dist(arrival_rate),
        service_dist(service_rate) {}

  /**
   * @brief Generates next inter-arrival time from exponential distribution.
   * @return Random time until next customer arrival
   */
  double NextInterarrivalTime() { return interarrival_dist(arrival_rng); }

  /**
   * @brief Generates service time from exponential distribution.
   * @return Random service duration for a customer
   */
  double NextServiceTime() { return service_dist(service_rng); }

  /**
   * @brief Gets the traffic intensity (ρ = λ/μ).
   * @return Traffic intensity
   */
  double TrafficIntensity() const { return arrival_rate / service_rate; }
};
