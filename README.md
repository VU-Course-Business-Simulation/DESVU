# DESVU - Discrete Event Simulation Library

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Header-Only](https://img.shields.io/badge/header--only-yes-green.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

A lightweight, educational discrete event simulation (DES) library designed for teaching simulation concepts at Vrije Universiteit Amsterdam (VU).

## Features

- ✅ **Header-only** - Easy integration, just include and go
- ✅ **Event-based simulation engine** - Schedule and execute events in chronological order
- ✅ **Built-in statistics collection** - Event-based and time-weighted statistics
- ✅ **Clear, documented API** - Designed specifically for learning
- ✅ **Educational examples** - Complete working simulation with explanation
- ✅ **Modern C++17** - Clean, modern code following best practices
- ✅ **Comprehensive tests** - 43 unit tests using Catch2

---

## Table of Contents

- [Quick Start](#quick-start)
- [Installation](#installation)
- [Core Concepts](#core-concepts)
- [API Reference](#api-reference)
- [Quick Reference](#quick-reference)
- [Building and Testing](#building-and-testing)
- [Contributing](#contributing)
- [License](#license)

---

## Quick Start

Here's a simple discrete event simulation that prints a message:

```cpp
#include <desvu/desvu.h>
#include <iostream>
#include <memory>

class PrintEvent : public desvu::Event {
public:
    explicit PrintEvent(double delay) : Event(delay) {}
    
    void action(desvu::Simulator& sim) override {
        std::cout << "Event executed at time " << sim.now() << "\n";
    }
};

int main() {
    desvu::Simulator sim;
    auto event = std::make_shared<PrintEvent>(5.0);
    sim.schedule(event);
    sim.run(10.0);
    return 0;
}
```

**Output:**
```
Event executed at time 5
```

For a complete M/M/1 queueing system example with arrivals, service, and statistics, see [`examples/simple_queue.cpp`](examples/simple_queue.cpp). TODO: Add more examples (G/G/c).

---

## Installation

### Method 1: CMake FetchContent (Recommended)

```cmake
# Disable building tests and examples for DESVU.
# These options are defined in the DESVU repository's CMakeLists.txt.
# The description strings (3rd parameter) appear in CMake GUI tools.
set(DESVU_BUILD_TESTS OFF CACHE BOOL "Build DESVU tests" FORCE)
set(DESVU_BUILD_EXAMPLES OFF CACHE BOOL "Build DESVU examples" FORCE)

# Declare the DESVU library with its Git repository location
# GIT_TAG specifies the branch or version to fetch
FetchContent_Declare(
    desvu
    GIT_REPOSITORY https://github.com/VU-Course-Business-Simulation/DESVU.git
    GIT_TAG main  # Using main branch; could be changed to a specific version tag like v1.0.0
)

# Download and make the DESVU library available to the project
# The library target 'desvu' will be created by the repository's CMakeLists.txt
# as an INTERFACE library with the correct include directories
FetchContent_MakeAvailable(desvu)

add_executable(your_target main.cpp)

target_link_libraries(your_target PRIVATE desvu)
```

### Method 2: Direct Include

Copy `include/desvu/` to your project:

```cmake
target_include_directories(your_target PRIVATE path/to/include)
```

---

## Core Concepts

### Events

Events represent actions at specific simulation times. Create custom events by inheriting from `desvu::Event`:

```cpp
class MyEvent : public desvu::Event {
public:
    explicit MyEvent(double delay) : Event(delay) {}
    
    void action(desvu::Simulator& sim) override {
        // Your event logic here
        // Can schedule new events using sim.schedule()
    }
};
```

**Key Pattern**: When scheduling an event of a type that has not been fully defined yet, you must first declare all event classes before implementing their `action()` methods. This allows forward references:

```cpp
// Declare both classes first
class ArrivalEvent : public desvu::Event {
public:
    explicit ArrivalEvent(double delay) : Event(delay) {}
    void action(desvu::Simulator& sim) override;
};

class DepartureEvent : public desvu::Event {
public:
    explicit DepartureEvent(double delay) : Event(delay) {}
    void action(desvu::Simulator& sim) override;
};

// Then implement action() methods
void ArrivalEvent::action(desvu::Simulator& sim) {
    // Can now create and schedule DepartureEvent
    auto departure = std::make_shared<DepartureEvent>(service_time);
    sim.schedule(departure);
}
```

### Simulator

The simulator manages time and executes events in chronological order:

- `sim.now()` - Get current simulation time
- `sim.schedule(event)` - Schedule an event for future execution
- `sim.run(until)` - Run simulation until specified time or queue empty

### Statistics

#### Event-Based Statistics
For measurements taken at specific events (e.g., waiting times, service times):

```cpp
desvu::EventStats waiting_times("Waiting Time");
waiting_times.Add(5.2);
waiting_times.Add(3.8);
waiting_times.Add(4.5);
waiting_times.Add(6.1);
std::cout << "Average: " << waiting_times.Average() << "\n";

// Compute 95% confidence interval for the mean
auto [lower, upper] = waiting_times.ConfidenceInterval95();
std::cout << "95% CI: [" << lower << ", " << upper << "]\n";
```

#### Time-Weighted Statistics
For state variables that persist over time (e.g., queue length, server utilization):

```cpp
desvu::TimeWeightedStats queue_length("Queue Length");
queue_length.Update(0.0, 0);   // 0 customers at time 0
queue_length.Update(1.5, 1);   // 1 customer at time 1.5
queue_length.Update(3.2, 0);   // 0 customers at time 3.2
std::cout << "Average: " << queue_length.Average(10.0) << "\n";
```

#### Stats Collector
Manage multiple statistics with a unified interface:

```cpp
desvu::StatsCollector stats;
stats.Add("Waiting Time", 5.2);              // Event-based
stats.Add("Queue Length", sim.now(), 3);      // Time-weighted
std::cout << stats.Report(sim.now()) << "\n";
```

---

## API Reference

TODO: Organize elsewhere.

### `desvu::Event`
Abstract base class for simulation events.

- **Constructor**: `Event(double delay)` - Create event with specified delay
- **Virtual method**: `void action(Simulator& sim)` - Implement event behavior
- **Method**: `void cancel()` - Cancel this event

### `desvu::Simulator`
Discrete event simulation engine.

- **Constructor**: `Simulator(bool log_events = false)`
- **Method**: `double now() const` - Get current simulation time
- **Method**: `void schedule(std::shared_ptr<Event> event)` - Schedule an event
- **Method**: `void run(double until = -1.0)` - Run simulation

### `desvu::EventStats`
Statistics for event-based observations (recorded at specific events).

- **Constructor**: `EventStats(const std::string& name)`
- **Method**: `void Add(double value)` - Add observation
- **Method**: `double Average() const` - Compute mean
- **Method**: `double StandardDeviation() const` - Compute std dev
- **Method**: `double Min() const` / `double Max() const` - Get extremes
- **Method**: `size_t Count() const` - Get number of observations
- **Method**: `std::pair<double, double> ConfidenceInterval(double confidence_level = 0.95) const` - Compute 95% confidence interval (only 95% supported)
- **Method**: `std::string Report() const` - Generate formatted report

### `desvu::TimeWeightedStats`
Statistics for time-weighted values.

- **Constructor**: `TimeWeightedStats(const std::string& name)`
- **Method**: `void Update(double time, double value)` - Update value at time
- **Method**: `double Average(double end_time) const` - Compute time-weighted average
- **Method**: `double Min() const` / `double Max() const` - Get extremes
- **Method**: `std::string Report(double end_time) const` - Generate report

### `desvu::StatsCollector`
Container for managing multiple statistics.

- **Method**: `void Add(const std::string& name, double value)` - Add event-based observation
- **Method**: `void Add(const std::string& name, double time, double value)` - Add time-weighted observation
- **Method**: `const EventStats* GetEvent(const std::string& name) const` - Get event-based statistic
- **Method**: `const EventStats* GetDiscrete(const std::string& name) const` - Legacy name for GetEvent
- **Method**: `const TimeWeightedStats* GetTimeWeighted(const std::string& name) const`
- **Method**: `std::string Report(double end_time) const` - Generate full report

---

## Quick Reference

### Common Patterns

#### Scheduling Events
```cpp
// Store in variable first (enables implicit conversion)
auto event = std::make_shared<MyEvent>(delay);
sim.schedule(event);

// Cancel a scheduled event (the action will not be executed)
event->cancel();
```

#### Managing Queues
```cpp
std::queue<Customer> waiting_queue;

waiting_queue.push(customer);    // Add
Customer next = waiting_queue.front();
waiting_queue.pop();              // Remove
if (waiting_queue.empty()) { }   // Check
size_t length = waiting_queue.size();
```

#### Random Number Generation
```cpp
std::mt19937 rng(42);  // Seed for reproducibility
std::exponential_distribution<> exp_dist(rate);
std::uniform_real_distribution<> unif_dist(min, max);
std::normal_distribution<> norm_dist(mean, std_dev);

double value = exp_dist(rng);
```

#### Statistics Collection Patterns
```cpp
// Queue length (time-weighted)
stats.Add("Queue Length", sim.now(), queue.size());

// Server utilization (time-weighted)
stats.Add("Utilization", sim.now(), server_busy ? 1.0 : 0.0);

// Waiting time (event-based)
stats.Add("Waiting Time", sim.now() - arrival_time);
```

---

## Building and Testing

TODO: Check correctness.

### Clone and Build

```bash
git clone https://github.com/VU-Course-Business-Simulation/DESVU.git
cd desvu
mkdir build && cd build
cmake ..
cmake --build .
```

### Running Tests

```bash
# In build directory
ctest --output-on-failure

# Or run individual tests
./test_simulator
./test_event_stats
./test_time_weighted_stats
./test_stats_collector
```

### Running Examples

```bash
# In build directory
./examples/simple_queue

# Or from project root
./cmake-build-debug/examples/simple_queue.exe
```

---

## Contributing

Contributions are welcome! This is an educational project, so clarity and documentation are paramount.

### Guidelines

- **Follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)**
- **Add comprehensive Doxygen comments**
- **Include tests for new features** (use Catch2)
- **Keep examples simple** and well-explained
- **Update documentation**

### Development Workflow

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Make your changes
4. Write tests and ensure they pass: `ctest`
5. Update documentation
6. Submit a pull request

### Code Style

This project follows the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

Quick reference:
- Files: `snake_case.h`
- Classes: `PascalCase`
- Functions: `PascalCase` (public), `snake_case` (private)
- Variables: `snake_case`
- Constants: `UPPER_CASE`

### Testing

Use Catch2 framework:

```cpp
#include <desvu/desvu.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Description", "[category]") {
    desvu::Simulator sim;
    sim.run(10.0);
    REQUIRE(sim.now() == 10.0);
}
```

Examine existing tests in `tests/` for more examples.

---

## Educational Use

This library is designed for teaching discrete event simulation in operations research and computer science courses. It demonstrates:

- **Event-driven programming**: Understanding event handlers
- **Priority queues**: How events are ordered by time
- **Statistical analysis**: Collecting and interpreting simulation results
- **Stochastic modeling**: Using random variables in simulation
- **Performance metrics**: Queue length, waiting times, utilization

### For Students

1. Study the header files to understand DES implementation
2. Modify examples to explore different scenarios
3. Build your own simulation models from scratch
4. Experiment with different statistical distributions
5. Verify results against theoretical formulas

### For Instructors

- Use as a teaching tool for DES concepts
- Assign modification exercises
- Compare simulation vs. analytical results

---

## Requirements

- **C++17 or later**
- **CMake 3.15+** (for building tests and examples)
- **Catch2** (automatically fetched for testing)

Compatible with GCC 7+, Clang 5+, MSVC 2017+, AppleClang 10+

TODO: Check correctness of compatibability -> Github Actions?

---

## Project Structure

```
desvu/
├── CMakeLists.txt              # Root build configuration
├── README.md                   # This file
├── LICENSE                     # MIT License
├── include/
│   └── desvu/
│       ├── desvu.h            # Main header (includes all)
│       ├── core/
│       │   ├── event.h        # Event base class
│       │   └── simulator.h    # Simulation engine
│       └── stats/
│           ├── event_stats.h           # Event-based statistics
│           ├── time_weighted_stats.h   # Time-weighted statistics
│           └── stats_collector.h       # Statistics container
├── tests/
│   ├── CMakeLists.txt
│   ├── test_simulator.cpp
│   ├── test_event_stats.cpp
│   ├── test_time_weighted_stats.cpp
│   └── test_stats_collector.cpp
└── examples/
    ├── CMakeLists.txt
    ├── simple_queue.cpp
    └── README.md
```

---

## License

MIT License - see [LICENSE](LICENSE) file for details.

---

## Authors

Created by Joost Berkhout, Assistant Professor at Vrije Universiteit Amsterdam, for teaching discrete event simulation.

---

## Acknowledgments

Inspired by the SSJ (Stochastic Simulation in Java) library, adapted for C++ education.
