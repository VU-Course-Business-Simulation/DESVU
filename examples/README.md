# DESVU Examples

This directory contains educational examples demonstrating discrete event simulation concepts using the DESVU library.

## Example Overview

### Simple Queue (`simple_queue/`)

**Difficulty**: Beginner to Intermediate

**Description**: A professionally structured M/M/1 queueing system with exponential interarrival times and exponential service times. This example demonstrates best practices for organizing a discrete event simulation project.

**System Components**:
- Single server
- FIFO queue
- Poisson arrival process (exponential interarrival times)
- Exponential service times

**Events**:
- `ArrivalEvent`: Customer arrives at the system
- `DepartureEvent`: Customer completes service and leaves

**Statistics Collected**:
- Waiting time (event-based)
- Service time (event-based)
- Queue length (time-weighted)
- Server utilization (time-weighted)

**Code Organization**:
- `main.cpp`: Entry point, runs multiple replications
- `SimulationConfig.h`: Centralized parameters and RNG management
- `Customer.h`: Customer data structure
- `Server.h/cpp`: Queue management and business logic
- `Events.h/cpp`: Event definitions and implementations

**Learning Objectives**:
- Understanding event scheduling
- Implementing modular simulation code
- Collecting and interpreting statistics
- Running multiple replications
- Computing confidence intervals
- Comparing simulation results with theoretical values

**Key Concepts**:
- Traffic intensity (ρ = λ/μ)
- Stability condition (ρ < 1)
- Little's Law
- Time-weighted vs. event-based statistics
- Common Random Numbers technique
- Replication analysis

**To Run**:
```bash
# From build directory
cd cmake-build-debug/examples/simple_queue
./simple_queue.exe  # Windows
./simple_queue      # Linux/Mac
```

**Expected Output**:
- Results aggregated across 10 replications
- Confidence intervals for key metrics
- Theoretical M/M/1 values for comparison
- Detailed single replication example

---

## Building the Examples

### Using CMake (Recommended)

```bash
# From the project root
cmake --build cmake-build-debug

# Or rebuild from scratch
cmake -B cmake-build-debug -S .
cmake --build cmake-build-debug
```

## Adding New Examples

DESVU provides helper functions to easily add new examples:

### Option 1: Simple Single-File Example

For simple demonstrations that fit in one file:

1. Create `my_example.cpp` in the `examples/` directory
2. Edit `examples/CMakeLists.txt` and add:
   ```cmake
   add_desvu_simple_example(my_example)
   ```

### Option 2: Complex Multi-File Example (Recommended)

For well-structured examples with multiple files:

1. Create a subdirectory: `examples/my_example/`
2. Add your files:
   ```
   my_example/
   ├── CMakeLists.txt
   ├── main.cpp
   ├── SimulationConfig.h
   ├── MyClass.h
   ├── MyClass.cpp
   └── README.md (optional)
   ```

3. Create `CMakeLists.txt` in your subdirectory:
   ```cmake
   add_executable(my_example
       main.cpp
       MyClass.cpp
   )
   
   target_include_directories(my_example PRIVATE
       ${CMAKE_CURRENT_SOURCE_DIR}
   )
   
   target_link_libraries(my_example PRIVATE desvu)
   ```

4. Edit `examples/CMakeLists.txt` and add:
   ```cmake
   add_desvu_subdirectory_example(my_example)
   ```

### Recommended Structure for Complex Examples

Follow the pattern used in `simple_queue/`:

- **SimulationConfig.h**: All parameters and RNG management
- **DataStructures.h**: Customer, Job, Entity classes
- **SystemComponents.h/cpp**: Server, Queue, Resource classes
- **Events.h/cpp**: All event definitions
- **main.cpp**: Multiple replication logic and result aggregation

## Modifying the Examples

Students are encouraged to modify the examples to explore different scenarios:

### Simple Queue Modifications

1. **Change arrival/service rates**: 
   - Try ρ > 1 (unstable system)
   - Try ρ = 0.9 vs. ρ = 0.5

2. **Add multiple servers**: 
   - Extend to M/M/c queue
   - Track individual server utilization

3. **Different distributions**: 
   - Use uniform or normal distributions
   - Compare results with exponential

4. **Priority queues**: 
   - Implement customer priorities
   - Track waiting times by priority

5. **Balking/Reneging**:
   - Customers leave if queue is too long
   - Track lost customers

5. **Limited queue capacity**:
   - Add maximum queue size
   - Track blocked customers

6. **Customer impatience**:
   - Add reneging (leaving after waiting too long)
   - Add balking (not joining if queue is too long)

## Understanding the Output

### Event-Based Statistics
- **Count**: Number of observations
- **Average**: Arithmetic mean
- **Std Dev**: Standard deviation (measure of variability)
- **Min/Max**: Range of observed values

### Time-Weighted Statistics
- **Updates**: Number of state changes
- **Average**: Time-weighted average (accounts for duration)
- **Min/Max**: Minimum and maximum states observed

### Comparing Simulation vs. Theory

For the M/M/1 queue, theoretical formulas are:
- Average queue length: L_q = ρ²/(1-ρ)
- Average waiting time: W_q = ρ/(μ(1-ρ))
- Server utilization: ρ = λ/μ

Simulation results should approach these values as simulation time increases.

## Tips for Students

1. **Start with small simulation times** to see events in detail (enable event logging)
2. **Increase simulation time** for more accurate statistics (law of large numbers)
3. **Run multiple replications** with different random seeds to understand variability
4. **Validate your model** by comparing with theoretical results when available
5. **Document your assumptions** and modeling decisions

## Further Exercises

1. **Verify Little's Law**: L = λW (average number in system = arrival rate × average time in system)
2. **Confidence intervals**: Run multiple replications and compute 95% confidence intervals
3. **Warm-up period**: Identify and remove initial transient behavior
4. **Sensitivity analysis**: How do results change with different parameters?
5. **Model validation**: Compare simulation results with real-world data if available

## Questions or Issues?

If you encounter problems or have questions about the example:
1. Check that your C++ compiler supports C++17
2. Verify that include paths are set correctly
3. Review the API documentation in the main README
4. Examine the test files for additional usage examples

---

**Happy Simulating!** 🎲

**Difficulty**: Beginner

**Description**: A classic M/M/1 queueing system with exponential interarrival times and exponential service times.

**System Components**:
- Single server
- FIFO queue
- Poisson arrival process (exponential interarrival times)
- Exponential service times

**Events**:
- `ArrivalEvent`: Customer arrives at the system
- `DepartureEvent`: Customer completes service and leaves

**Statistics Collected**:
- Waiting time (event-based)
- Service time (event-based)
- Queue length (time-weighted)
- Server utilization (time-weighted)

**Learning Objectives**:
- Understanding event scheduling
- Implementing basic queue operations
- Collecting and interpreting statistics
- Comparing simulation results with theoretical values

**Key Concepts**:
- Traffic intensity (ρ = λ/μ)
- Stability condition (ρ < 1)
- Little's Law
- Time-weighted vs. event-based statistics

**To Run**:
```bash
# From build directory
./simple_queue
```

**Expected Output**:
- Number of customers arrived and departed
- Average waiting time
- Average queue length
- Server utilization
- Comparison with theoretical M/M/1 values

---

### 2. Workshop with Failures (`workshop_example.cpp`)

**Difficulty**: Intermediate

**Description**: A more complex system where a workshop machine processes jobs but can randomly fail during operation.

**System Components**:
- Single machine that can fail
- Job queue
- Repair mechanism
- Job restart policy (failed jobs restart from beginning)

**Events**:
- `JobArrivalEvent`: New job arrives at workshop
- `JobCompletionEvent`: Job finishes processing
- `MachineFailureEvent`: Machine breaks down during processing
- `RepairCompletionEvent`: Machine is repaired and ready

**Statistics Collected**:
- Waiting time (event-based)
- Processing time (event-based)
- Time in system (event-based)
- Repair time (event-based)
- Queue length (time-weighted)
- Machine state (time-weighted): idle, processing, or failed

**Learning Objectives**:
- Modeling systems with multiple event types
- Handling event cancellation and interruption
- State management in complex systems
- Reliability and maintenance modeling
- Analyzing system performance under failures

**Key Concepts**:
- Event interactions and dependencies
- MTBF (Mean Time Between Failures)
- System availability and downtime
- Impact of failures on throughput
- Job restart policies

**To Run**:
```bash
# From build directory
./workshop_example
```

**Expected Output**:
- Jobs arrived, completed, and restarted
- Number of failures
- System throughput
- Average waiting time and queue length
- Machine performance metrics

---

## Building the Examples

### Using CMake

```bash
# From the project root
mkdir build
cd build
cmake ..
cmake --build .

# Examples will be in the build directory
./simple_queue
./workshop_example
```

### Manual Compilation

If you prefer to compile manually:

```bash
# Simple Queue
g++ -std=c++17 -I../include simple_queue.cpp -o simple_queue

# Workshop Example
g++ -std=c++17 -I../include workshop_example.cpp -o workshop_example
```

## Modifying the Examples

Students are encouraged to modify these examples to explore different scenarios:

### Simple Queue Modifications

1. **Change arrival/service rates**: 
   - Try ρ > 1 (unstable system)
   - Try ρ = 0.9 vs. ρ = 0.5

2. **Add multiple servers**: 
   - Extend to M/M/c queue
   - Track individual server utilization

3. **Different distributions**: 
   - Use uniform or normal distributions
   - Compare results with exponential

4. **Priority queues**: 
   - Implement customer priorities
   - Track waiting times by priority

### Workshop Modifications

1. **Multiple machines**: 
   - Add parallel machines
   - Implement load balancing

2. **Preventive maintenance**: 
   - Schedule regular maintenance
   - Compare with corrective maintenance

3. **Different failure models**: 
   - Age-dependent failures (Weibull distribution)
   - Wear-out effects

4. **Job priorities**: 
   - Important jobs bypass queue
   - Different job types with different processing times

## Understanding the Output

### Event Statistics
- **Count**: Number of observations
- **Average**: Arithmetic mean
- **Std Dev**: Standard deviation (measure of variability)
- **Min/Max**: Range of observed values

### Time-Weighted Statistics
- **Updates**: Number of state changes
- **Average**: Time-weighted average (accounts for duration)
- **Min/Max**: Minimum and maximum states observed

### Comparing Simulation vs. Theory

For the M/M/1 queue, theoretical formulas are:
- Average queue length: L_q = ρ²/(1-ρ)
- Average waiting time: W_q = ρ/(μ(1-ρ))
- Server utilization: ρ = λ/μ

Simulation results should approach these values as simulation time increases.

## Tips for Students

1. **Start with small simulation times** to see events in detail (enable event logging)
2. **Increase simulation time** for more accurate statistics (law of large numbers)
3. **Run multiple replications** with different random seeds to understand variability
4. **Validate your model** by comparing with theoretical results when available
5. **Document your assumptions** and modeling decisions

## Further Exercises

1. **Verify Little's Law**: L = λW (average number in system = arrival rate × average time in system)
2. **Confidence intervals**: Run multiple replications and compute 95% confidence intervals
3. **Warm-up period**: Identify and remove initial transient behavior
4. **Sensitivity analysis**: How do results change with different parameters?
5. **Model validation**: Compare simulation results with real-world data if available

## Questions or Issues?

If you encounter problems or have questions about the examples:
1. Check that your C++ compiler supports C++17
2. Verify that include paths are set correctly
3. Review the API documentation in the main README
4. Examine the test files for additional usage examples

---

**Happy Simulating!** 🎲
