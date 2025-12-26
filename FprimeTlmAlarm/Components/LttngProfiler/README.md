# LttngProfiler Component

## Overview

The LttngProfiler is an FPrime component that provides profiling capabilities using LTTNG (Linux Trace Toolkit Next Generation). It sits between a rate group and the component driven by it, emitting LTTNG tracepoint events upon entry and exit for profiling and performance analysis.

## Architecture

The component acts as a transparent pass-through for scheduler signals:

```
RateGroup -> LttngProfiler -> Driven Component
                   |
                   v
            LTTNG Tracepoints
            (entry/exit events)
```

## Dependencies

### System Requirements

- LTTNG UST (User Space Tracing) library
- LTTNG Tools (for trace capture and analysis)
- Babeltrace (for trace reading, optional but recommended)

### Installation on Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y lttng-tools liblttng-ust-dev babeltrace
```

### Installation on RHEL/CentOS/Fedora

```bash
sudo yum install -y lttng-tools lttng-ust-devel babeltrace
```

## Component Interface

### Ports

- **schedIn** (sync input): Receives schedule signals from rate group
- **schedOut** (output): Forwards schedule signals to driven component

### Tracepoints

The component defines two LTTNG tracepoints:

1. **fprime_profiler:entry**
   - Fields: `component_name` (string), `context` (unsigned int)
   - Emitted when schedIn handler is called

2. **fprime_profiler:exit**
   - Fields: `component_name` (string), `context` (unsigned int)
   - Emitted after forwarding to driven component

## Usage

### 1. Component Integration

In your topology, insert the profiler between a rate group and its driven component:

```fpp
# instances.fpp
instance rateGroup1Profiler: FprimeTlmAlarm.LttngProfiler base id 0x10003000

# topology.fpp
connections RateGroups {
    rateGroup1.RateGroupMemberOut[0] -> rateGroup1Profiler.schedIn
    rateGroup1Profiler.schedOut -> myComponent.run
}
```

### 2. Capture Traces

Start an LTTNG tracing session before running your application:

```bash
# Create session
lttng create my-session --output=/tmp/traces

# Enable fprime_profiler events
lttng enable-event -u 'fprime_profiler:*'

# Start tracing
lttng start

# Run your FPrime application
./AlarmedTelem

# Stop tracing when done
lttng stop
lttng destroy
```

### 3. Analyze Traces

View traces using babeltrace:

```bash
babeltrace /tmp/traces
```

Or analyze timing between entry/exit events:

```bash
babeltrace /tmp/traces | grep fprime_profiler | awk '{print $4, $5, $6, $7}'
```

## Testing

### Unit Tests

The component includes unit tests that verify:
- Schedule signals are correctly forwarded
- Multiple contexts are handled properly
- Disconnected outputs are handled gracefully

Run unit tests:

```bash
cd FprimeTlmAlarm/Components/LttngProfiler
fprime-util build --ut
fprime-util check
```

### Integration Tests

Integration tests verify that LTTNG tracepoints are actually emitted and can be captured. These tests require LTTNG tools to be installed.

Run integration tests:

```bash
fprime-util build --ut
./build-fprime-automatic-native-ut/FprimeTlmAlarm/Components/LttngProfiler/LttngProfilerIntegration
```

**Note:** Integration tests will be skipped if LTTNG tools are not available.

## Profiling Multiple Components

You can insert profilers at multiple points in your topology:

```fpp
instance rg1_profiler_1: FprimeTlmAlarm.LttngProfiler base id 0x10003000
instance rg1_profiler_2: FprimeTlmAlarm.LttngProfiler base id 0x10003100
instance rg1_profiler_3: FprimeTlmAlarm.LttngProfiler base id 0x10003200

connections RateGroups {
    rateGroup1.RateGroupMemberOut[0] -> rg1_profiler_1.schedIn
    rg1_profiler_1.schedOut -> component1.run

    rateGroup1.RateGroupMemberOut[1] -> rg1_profiler_2.schedIn
    rg1_profiler_2.schedOut -> component2.run

    rateGroup1.RateGroupMemberOut[2] -> rg1_profiler_3.schedIn
    rg1_profiler_3.schedOut -> component3.run
}
```

Each profiler will emit events with its component name, allowing you to track execution of different components.

## Performance Overhead

LTTNG is designed for low overhead:
- Entry/exit tracepoints add ~100-200ns per call
- Negligible impact on real-time performance
- No overhead when tracing is not enabled
- Traces stored in memory-mapped buffers

## Troubleshooting

### Build Errors

If you get build errors about missing LTTNG headers:

```
fatal error: lttng/tracepoint.h: No such file or directory
```

Install the LTTNG development packages (see Dependencies above).

### Runtime Warnings

If you see warnings about failed tracepoints at runtime, LTTNG may not be properly configured. Ensure:
- `lttng-sessiond` daemon is running
- You have permissions to create trace sessions
- `/tmp` has sufficient space for traces

### No Events in Trace

If babeltrace shows no events:
- Verify events were enabled: `lttng list my-session`
- Check the component name matches the tracepoint provider
- Ensure the profiled component actually ran

## References

- [LTTNG Documentation](https://lttng.org/docs/)
- [FPrime Documentation](https://fprime.jpl.nasa.gov/)
- [LTTNG UST Guide](https://lttng.org/docs/v2.13/#doc-c-application)
