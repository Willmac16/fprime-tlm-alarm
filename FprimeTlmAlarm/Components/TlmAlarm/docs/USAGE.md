# TlmAlarm Component - Direct Monitoring Implementation

## Overview

This is an alternative implementation of the TlmAlarm component that performs telemetry monitoring directly in C++ without using an FpySequencer. The component evaluates telemetry values against configurable thresholds using comparison operators and supports debouncing/persistence logic.

## Features

- **Direct C++ monitoring logic** - No dependency on FpySequencer
- **Comprehensive type support**:
  - Basic unsigned integers: U8, U16, U32, U64
  - Basic signed integers: I8, I16, I32, I64
  - Floating point: F32, F64
  - 3D vectors (F32 and F64)
  - Quaternions (F32 and F64)
- **Configurable comparison operators**: >, >=, <, <=, ==, !=
- **Debounce/persistence logic** - Trigger only after N consecutive violations
- **Runtime threshold updates** - Modify thresholds without recompilation
- **Enable/disable monitors** - Turn monitors on/off dynamically
- **Vector/quaternion comparison modes**:
  - Component-wise: All components must satisfy condition
  - Magnitude-based: Compare magnitudes only

## Architecture

```
System Telemetry
    |
    v
TlmSplitter
    |
    +----> TlmAlarm.TlmRecv (async queued)
               |
               v
        Monitor Evaluation
               |
               +---> If triggered: ResponseOut port call
               +---> Event: MonitorTriggered
```

## Configuration API

### Configure a Monitor

```cpp
bool configureMonitor(
    FwChanIdType channelId,    // Telemetry channel ID to monitor
    U32 monitorId,             // Unique monitor ID
    TlmDataType dataType,      // Data type of the channel
    ComparisonOp comparisonOp, // Comparison operator
    F64 threshold,             // Threshold value
    U32 debounceCount = 1,     // Consecutive triggers needed
    bool usesMagnitude = false // For vectors/quats: compare magnitude
);
```

**Example - Monitor temperature above 50°C:**
```cpp
tlmAlarm.configureMonitor(
    TEMP_SENSOR_CH_ID,          // Channel ID
    1,                           // Monitor ID
    TlmDataType::F32_TYPE,       // Float32 data
    ComparisonOp::GREATER_THAN,  // >
    50.0,                        // 50 degrees threshold
    3                            // Trigger after 3 consecutive violations
);
```

**Example - Monitor velocity magnitude:**
```cpp
tlmAlarm.configureMonitor(
    VELOCITY_CH_ID,              // Channel ID
    2,                           // Monitor ID
    TlmDataType::VEC_F64_TYPE,   // 3D vector (F64)
    ComparisonOp::GREATER_THAN,  // >
    100.0,                       // 100 m/s threshold
    1,                           // Immediate trigger
    true                         // Compare magnitude, not components
);
```

### Runtime Control

**Enable a monitor:**
```cpp
tlmAlarm.enableMonitor(monitorId);
```

**Disable a monitor:**
```cpp
tlmAlarm.disableMonitor(monitorId);
```

**Update threshold:**
```cpp
tlmAlarm.updateThreshold(monitorId, newThreshold);
```

## Data Types and Enumerations

### TlmDataType Enum

```cpp
enum class TlmDataType : U8 {
    U8_TYPE,
    U16_TYPE,
    U32_TYPE,
    U64_TYPE,
    I8_TYPE,
    I16_TYPE,
    I32_TYPE,
    I64_TYPE,
    F32_TYPE,
    F64_TYPE,
    VEC_F32_TYPE,   // 3-element float vector
    VEC_F64_TYPE,   // 3-element double vector
    QUAT_F32_TYPE,  // 4-element float quaternion
    QUAT_F64_TYPE   // 4-element double quaternion
};
```

### ComparisonOp Enum

Defined in TlmAlarm.fpp:
```fpp
enum ComparisonOp {
    GREATER_THAN = 0         @< >
    GREATER_THAN_OR_EQUAL = 1 @< >=
    LESS_THAN = 2            @< <
    LESS_THAN_OR_EQUAL = 3   @< <=
    EQUAL = 4                @< ==
    NOT_EQUAL = 5            @< !=
}
```

## Vector and Quaternion Support

### Vector Structures

**VectorF32:**
```cpp
struct VectorF32 {
    F32 x, y, z;
    F32 magnitude() const;
};
```

**VectorF64:**
```cpp
struct VectorF64 {
    F64 x, y, z;
    F64 magnitude() const;
};
```

### Quaternion Structures

**QuaternionF32:**
```cpp
struct QuaternionF32 {
    F32 w, x, y, z;
    F32 magnitude() const;
};
```

**QuaternionF64:**
```cpp
struct QuaternionF64 {
    F64 w, x, y, z;
    F64 magnitude() const;
};
```

### Comparison Modes

**Component-wise mode (usesMagnitude = false):**
- For vectors: All three components (x, y, z) must satisfy the condition
- For quaternions: All four components (w, x, y, z) must satisfy the condition
- Example: VectorF32{10, 20, 30} > VectorF32{5, 15, 25} → true (all components satisfy >)

**Magnitude mode (usesMagnitude = true):**
- Compares only the magnitude (Euclidean norm)
- For vectors: sqrt(x² + y² + z²)
- For quaternions: sqrt(w² + x² + y² + z²)
- Example: VectorF32{3, 4, 0}.magnitude() = 5.0

## Events

The component emits the following events:

| Event | Severity | Description |
|-------|----------|-------------|
| `MonitorTriggered` | WARNING_HI | Monitor condition met after debounce |
| `MonitorConfigured` | ACTIVITY_HI | New monitor successfully configured |
| `MonitorEnabled` | ACTIVITY_LO | Monitor enabled |
| `MonitorDisabled` | ACTIVITY_LO | Monitor disabled |
| `ThresholdUpdated` | ACTIVITY_LO | Threshold value updated |
| `ConfigurationFailed` | WARNING_HI | Configuration failed (max monitors, etc.) |

## Telemetry Channels

| Channel | Type | Description |
|---------|------|-------------|
| `NumActiveMonitors` | U32 | Number of currently active monitors |
| `TotalTriggers` | U32 | Total number of monitor triggers |

## ResponseOut Port

When a monitor triggers, the component calls the `ResponseOut` port with the monitor ID:

```fpp
port ResponseOut(
    monitorId: U32 @< The ID of the monitor that triggered
)
```

This port can be connected to custom response handlers in your topology to take actions when monitors trigger (e.g., safing modes, corrective commands, etc.).

## Deserialization

The component automatically deserializes telemetry values from `Fw::TlmBuffer` using F' serialization:

1. For basic types: Single `deserialize()` call
2. For vectors: Three sequential deserializations (x, y, z)
3. For quaternions: Four sequential deserializations (w, x, y, z)

Deserialization order matches F' standard serialization order.

## Limitations

- **Maximum monitors**: 256 (configurable via `MAX_MONITORS` constant)
- **One monitor per channel**: Each telemetry channel can have one monitor
- **Threshold as F64**: All thresholds configured as F64 and cast to appropriate type
- **Component-wise thresholds for vectors/quats**: When setting threshold for vectors/quaternions, the same value is applied to all components

## Comparison with FpySequencer Approach

| Aspect | FpySequencer Approach | Direct C++ Approach (This) |
|--------|----------------------|---------------------------|
| **Performance** | Python interpreter overhead | Native C++ execution |
| **Type safety** | Runtime Python types | Compile-time C++ types |
| **Flexibility** | Scriptable logic | Compiled logic |
| **Complexity** | External Python sequences | Self-contained C++ |
| **Dependencies** | FpySequencer component | None |
| **Debugging** | Python + C++ | C++ only |

## Example Usage Scenario

**Spacecraft Attitude Monitoring:**

```cpp
// Initialize component
TlmAlarm tlmAlarm("tlmAlarm");

// Monitor angular velocity magnitude - trigger if exceeds 0.1 rad/s
tlmAlarm.configureMonitor(
    ANG_VEL_CH_ID,
    MONITOR_ANG_VEL,
    TlmDataType::VEC_F64_TYPE,
    ComparisonOp::GREATER_THAN,
    0.1,
    5,      // Trigger after 5 consecutive violations (debounce)
    true    // Compare magnitude
);

// Monitor battery voltage - trigger if below 24V
tlmAlarm.configureMonitor(
    BATTERY_V_CH_ID,
    MONITOR_BATTERY,
    TlmDataType::F32_TYPE,
    ComparisonOp::LESS_THAN,
    24.0,
    3       // Trigger after 3 consecutive violations
);

// Later: Disable angular velocity monitor during maneuver
tlmAlarm.disableMonitor(MONITOR_ANG_VEL);

// After maneuver: Re-enable
tlmAlarm.enableMonitor(MONITOR_ANG_VEL);

// Update battery threshold for different mission phase
tlmAlarm.updateThreshold(MONITOR_BATTERY, 22.0);
```

## Building and Integration

1. The component is defined in `TlmAlarm.fpp`
2. Port definition in `ResponseOut.fpp`
3. Implementation in `TlmAlarm.hpp` and `TlmAlarm.cpp`
4. Both FPP files are registered in `CMakeLists.txt`
5. Component is instantiated and wired in the topology
6. Connect `ResponseOut` ports to appropriate handlers as needed

## Future Enhancements

Potential improvements:
- Command interface for runtime monitor configuration
- Per-component threshold values for vectors/quaternions
- Hysteresis support (different thresholds for triggering vs. clearing)
- Statistics tracking (min/max observed values, trigger counts per monitor)
- Persistence to non-volatile storage for monitor configurations
