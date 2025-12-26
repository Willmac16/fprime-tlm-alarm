# TlmAlarm Alternative Implementation Summary

## Overview

This implementation provides a complete alternative to the FpySequencer-based telemetry alarm monitoring system. The new approach performs all monitoring logic directly in C++ within the TlmAlarm component.

## Key Changes

### 1. New Custom Port Definition

**File: `FprimeTlmAlarm/Components/TlmAlarm/ResponseOut.fpp`**
- Defines `ResponseOut` port for monitor trigger notifications
- Port signature: `ResponseOut(monitorId: U32)`

### 2. Updated Component Definition

**File: `FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.fpp`**

**Removed:**
- All FpySequencer interface ports (`tlmMock`, `paramMock`, `seqRunOut`, `seqStartIn`, `seqDoneIn`)
- Dependencies on FpySequencer component

**Added:**
- `ComparisonOp` enum (6 comparison operators: >, >=, <, <=, ==, !=)
- `ResponseOut` output port array [10 ports]
- Events for monitor lifecycle and triggers
- Telemetry channels for monitoring statistics

### 3. Comprehensive Implementation

**File: `FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.hpp`**

**Data Structures:**
- `TlmDataType` enum: 14 supported types (U8-U64, I8-I64, F32, F64, vectors, quaternions)
- `VectorF32`, `VectorF64`: 3-component vectors with magnitude calculation
- `QuaternionF32`, `QuaternionF64`: 4-component quaternions with magnitude calculation
- `ThresholdValue` union: Holds threshold values for all supported types
- `MonitorConfig` struct: Complete monitor configuration with runtime state

**Public API:**
- `configureMonitor()`: Set up a new monitor
- `enableMonitor()`: Enable an existing monitor
- `disableMonitor()`: Disable a monitor
- `updateThreshold()`: Runtime threshold modification

**Private Methods:**
- `evaluateMonitor()`: Deserialize and compare telemetry value
- `compareNumeric()`: Template for numeric comparisons
- `compareVector()`: Template for vector comparisons (component-wise or magnitude)
- `compareQuaternion()`: Template for quaternion comparisons
- `setThresholdValue()`: Convert F64 to appropriate type
- `findMonitorByChannel()`: Lookup monitor by channel ID
- `findMonitorById()`: Lookup monitor by monitor ID

### 4. Full Implementation

**File: `FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.cpp`**

**Features:**
- Automatic deserialization from `Fw::TlmBuffer` for all 14 supported types
- Debounce/persistence logic with consecutive trigger counting
- `ResponseOut` port invocation on trigger
- Event logging for all monitor operations
- Efficient linear search for monitor lookup (O(n) where n ≤ 256)

**Handler Flow:**
1. `TlmRecv_handler()` receives telemetry update
2. Find monitor configuration for channel
3. Deserialize value from buffer
4. Perform comparison based on data type and operator
5. Update consecutive trigger count
6. Trigger response if debounce threshold met
7. Emit events and call ResponseOut port

### 5. Topology Updates

**File: `FprimeTlmAlarm/AlarmedTelem/Top/topology.fpp`**

**Removed:**
- `AlarmedTelemSeq` connection block (FpySequencer wiring)
- Rate group connections to `tlmAlarmSeq.checkTimers` and `tlmAlarmSeq.tlmWrite`
- `tlmAlarmSeq` instance reference

**Retained:**
- `TlmRecv` connection from TlmSplitter
- Rate group connection to `tlmAlarm.run`

**File: `FprimeTlmAlarm/AlarmedTelem/Top/instances.fpp`**
- Commented out `tlmAlarmSeq` instance definition

### 6. Build Configuration

**File: `FprimeTlmAlarm/Components/TlmAlarm/CMakeLists.txt`**
- Added `ResponseOut.fpp` to autocoder inputs
- Maintained existing source file registration

## Supported Data Types

### Basic Numeric Types (11 types)
- Unsigned: U8, U16, U32, U64
- Signed: I8, I16, I32, I64
- Floating: F32, F64

### Vector Types (2 types)
- VEC_F32_TYPE: 3D vector (x, y, z) using F32
- VEC_F64_TYPE: 3D vector (x, y, z) using F64

### Quaternion Types (2 types)
- QUAT_F32_TYPE: Quaternion (w, x, y, z) using F32
- QUAT_F64_TYPE: Quaternion (w, x, y, z) using F64

## Comparison Logic

### Numeric Types
Standard comparison operators applied directly to values.

### Vectors and Quaternions
Two modes available via `usesMagnitude` flag:

**Component-wise mode (`usesMagnitude = false`):**
- ALL components must satisfy the comparison
- Vector: x, y, z all satisfy condition
- Quaternion: w, x, y, z all satisfy condition

**Magnitude mode (`usesMagnitude = true`):**
- Compare Euclidean norms only
- Vector magnitude: √(x² + y² + z²)
- Quaternion magnitude: √(w² + x² + y² + z²)

## Debounce/Persistence Logic

- Each monitor maintains a `currentCount` of consecutive triggers
- Condition must be violated for `debounceCount` consecutive telemetry updates
- Counter resets to 0 if condition is not met
- `ResponseOut` port called and event emitted only when count reaches threshold
- Counter resets after successful trigger

## Events

| Event | Severity | When Emitted |
|-------|----------|--------------|
| `MonitorTriggered` | WARNING_HI | Monitor condition met after debounce |
| `MonitorConfigured` | ACTIVITY_HI | New monitor configured |
| `MonitorEnabled` | ACTIVITY_LO | Monitor enabled |
| `MonitorDisabled` | ACTIVITY_LO | Monitor disabled |
| `ThresholdUpdated` | ACTIVITY_LO | Threshold updated |
| `ConfigurationFailed` | WARNING_HI | Configuration error |

## Files Modified

1. **Created:**
   - `FprimeTlmAlarm/Components/TlmAlarm/ResponseOut.fpp` (new port definition)
   - `FprimeTlmAlarm/Components/TlmAlarm/docs/USAGE.md` (documentation)

2. **Modified:**
   - `FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.fpp` (component definition)
   - `FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.hpp` (class definition)
   - `FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.cpp` (implementation)
   - `FprimeTlmAlarm/Components/TlmAlarm/CMakeLists.txt` (build config)
   - `FprimeTlmAlarm/AlarmedTelem/Top/topology.fpp` (topology)
   - `FprimeTlmAlarm/AlarmedTelem/Top/instances.fpp` (instances)

## Advantages Over FpySequencer Approach

1. **Performance**: Native C++ execution vs. Python interpreter overhead
2. **Type Safety**: Compile-time type checking
3. **Simplicity**: No external sequencer component dependency
4. **Maintainability**: Single-component solution
5. **Determinism**: Predictable execution timing
6. **Debugging**: C++-only debugging flow

## Configuration Example

```cpp
// Monitor temperature (F32) above 50°C with 3-sample debounce
tlmAlarm.configureMonitor(
    TEMP_CH_ID,                  // Channel ID
    1,                           // Monitor ID
    TlmDataType::F32_TYPE,       // Data type
    ComparisonOp::GREATER_THAN,  // Operator
    50.0,                        // Threshold
    3                            // Debounce count
);

// Monitor velocity vector magnitude (F64) above 100 m/s
tlmAlarm.configureMonitor(
    VEL_CH_ID,
    2,
    TlmDataType::VEC_F64_TYPE,
    ComparisonOp::GREATER_THAN,
    100.0,
    1,
    true  // Use magnitude comparison
);
```

## Testing Recommendations

1. **Unit Tests:** Test each data type deserialization and comparison
2. **Debounce Tests:** Verify consecutive trigger counting
3. **Edge Cases:** Test boundary values for each type
4. **Vector/Quaternion Tests:** Test both comparison modes
5. **Runtime Tests:** Verify enable/disable and threshold updates
6. **Integration Tests:** Verify ResponseOut port invocation

## Future Enhancements

- Command interface for runtime configuration
- Per-component thresholds for vectors/quaternions
- Hysteresis support
- Monitor statistics tracking
- Configuration persistence to NVM
- Rate limiting for trigger notifications
