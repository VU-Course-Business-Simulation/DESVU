# M/M/1 Queue Simulation Example

This is a well-structured example demonstrating how to organize a discrete event simulation project.

## File Structure

```
simple_queue/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
└── src/
    ├── main.cpp                # Main entry point, runs multiple replications
    ├── simulation_config.h      # Configuration parameters and RNG management
    ├── customer.h              # Customer data structure
    ├── server.h                # Server class declaration
    ├── Server.cpp              # Server class implementation
    ├── events.h                # Event class declarations
    └── events.cpp              # Event class implementations
```

## Architecture

### Components

1. **simulation_config.h**
   - Centralizes all simulation parameters
   - Manages random number generators (separate streams for arrivals and service)
   - Provides methods to generate random variates
   - Supports Common Random Numbers technique

2. **customer.h**
   - Simple data structure storing customer information
   - Tracks arrival time
   - Calculates waiting time

3. **Server** (server.h/cpp)
   - Manages the customer queue
   - Tracks server state (busy/idle)
   - Handles arrival logic via `HandleArrival()`
   - Handles service completion logic via `ServiceCompleted()`

4. **Events** (events.h/cpp)
   - **ArrivalEvent**: Schedules next arrival and delegates to Server
   - **DepartureEvent**: Delegates to Server for service completion

5. **main.cpp**
   - Contains `RunSimulation()` function for single replication
   - Runs multiple replications for statistical analysis
   - Aggregates results across replications
   - Compares simulation results with theoretical M/M/1 values

## Design Patterns

### Separation of Concerns
- **Events**: Handle timing and scheduling logic
- **Server**: Handles queue management and business logic
- **SimulationConfig**: Isolates parameters and randomness

### Dependency Injection
- Server receives references to `SimulationConfig` and `StatsCollector`
- Events receive pointers to Server and references to configuration
- Enables easy testing and configuration changes

### Common Random Numbers
- Each random process has its own RNG stream
- Enables variance reduction techniques in advanced studies

## Building

From the project root:
```bash
cmake --build cmake-build-debug
```

The executable will be created at:
```
cmake-build-debug/examples/simple_queue/simple_queue.exe  # Windows
cmake-build-debug/examples/simple_queue/simple_queue      # Linux/Mac
```

## Running

```bash
cd cmake-build-debug/examples/simple_queue
./simple_queue.exe  # Windows
./simple_queue      # Linux/Mac
```

## Customization

To modify the simulation:

1. **Change parameters**: Edit values in `main.cpp` when creating `SimulationConfig`
2. **Add new statistics**: Add calls to `stats.Add()` in Server methods
3. **Modify server logic**: Edit `Server::HandleArrival()` or `Server::ServiceCompleted()`
4. **Add new events**: Create new event classes in events.h/cpp

## Learning Objectives

This example demonstrates:
- ✅ Modular code organization
- ✅ Separation of simulation logic from event scheduling
- ✅ Multiple replication analysis
- ✅ Confidence interval computation
- ✅ Comparison with theoretical results
- ✅ Professional C++ project structure

