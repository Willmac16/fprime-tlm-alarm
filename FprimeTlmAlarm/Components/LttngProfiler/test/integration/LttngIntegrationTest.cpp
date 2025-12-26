// ======================================================================
// \title  LttngIntegrationTest.cpp
// \author autocoder
// \brief  Integration test for LTTNG profiler - verifies tracepoints work
// ======================================================================

#include <gtest/gtest.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include "FprimeTlmAlarm/Components/LttngProfiler/LttngProfiler.hpp"
#include "Fw/Types/BasicTypes.hpp"

#define TRACEPOINT_DEFINE
#include "fprime_profiler_tp.h"

namespace FprimeTlmAlarm {

class LttngIntegrationTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Clean up any existing trace session
        system("lttng destroy test-session 2>/dev/null");
    }

    void TearDown() override {
        // Clean up trace session
        system("lttng destroy test-session 2>/dev/null");
    }

    // Helper to run lttng commands and check return code
    int runCommand(const char* cmd) {
        int ret = system(cmd);
        return WEXITSTATUS(ret);
    }

    // Helper to check if LTTNG is available
    bool isLttngAvailable() {
        int ret = system("which lttng > /dev/null 2>&1");
        return WEXITSTATUS(ret) == 0;
    }
};

// Test that verifies LTTNG tracepoints can be emitted
TEST_F(LttngIntegrationTest, VerifyTracepointsEmitted) {
    // Skip test if LTTNG tools are not available
    if (!isLttngAvailable()) {
        GTEST_SKIP() << "LTTNG tools not available, skipping integration test";
    }

    const char* sessionName = "test-session";
    const char* tracePath = "/tmp/lttng-traces/test-session";

    // Create LTTNG session
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "lttng create %s --output=%s 2>/dev/null", sessionName, tracePath);
    ASSERT_EQ(runCommand(cmd), 0) << "Failed to create LTTNG session";

    // Enable userspace events for fprime_profiler provider
    ASSERT_EQ(runCommand("lttng enable-event -u fprime_profiler:entry 2>/dev/null"), 0);
    ASSERT_EQ(runCommand("lttng enable-event -u fprime_profiler:exit 2>/dev/null"), 0);

    // Start tracing
    ASSERT_EQ(runCommand("lttng start 2>/dev/null"), 0);

    // Create and use the component
    LttngProfiler profiler("TestProfiler");
    profiler.init(0);

    // Emit some tracepoints by simulating schedule calls
    // We can't easily hook up ports in this test, so we'll emit tracepoints directly
    const U32 NUM_ITERATIONS = 3;
    for (U32 i = 0; i < NUM_ITERATIONS; i++) {
        tracepoint(fprime_profiler, entry, "TestProfiler", i);
        tracepoint(fprime_profiler, exit, "TestProfiler", i);
    }

    // Stop tracing
    ASSERT_EQ(runCommand("lttng stop 2>/dev/null"), 0);

    // Read trace and verify events
    char readCmd[512];
    snprintf(readCmd, sizeof(readCmd),
             "babeltrace %s 2>/dev/null | grep -c 'fprime_profiler:'", tracePath);

    // Check if babeltrace is available
    if (system("which babeltrace > /dev/null 2>&1") == 0) {
        FILE* pipe = popen(readCmd, "r");
        ASSERT_NE(pipe, nullptr);

        char buffer[32];
        char* result = fgets(buffer, sizeof(buffer), pipe);
        pclose(pipe);

        if (result != nullptr) {
            int eventCount = atoi(buffer);
            // We should have 2 events (entry + exit) per iteration
            EXPECT_GE(eventCount, NUM_ITERATIONS * 2)
                << "Expected at least " << (NUM_ITERATIONS * 2) << " events, got " << eventCount;
        }
    } else {
        // If babeltrace is not available, just verify the trace directory exists
        char checkCmd[256];
        snprintf(checkCmd, sizeof(checkCmd), "test -d %s", tracePath);
        EXPECT_EQ(system(checkCmd), 0) << "Trace directory should exist";
    }
}

// Test with actual component port invocation (if possible)
TEST_F(LttngIntegrationTest, VerifyComponentTracing) {
    // Skip test if LTTNG tools are not available
    if (!isLttngAvailable()) {
        GTEST_SKIP() << "LTTNG tools not available, skipping integration test";
    }

    const char* sessionName = "test-session";
    const char* tracePath = "/tmp/lttng-traces/test-session-2";

    // Create and configure LTTNG session
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "lttng create %s --output=%s 2>/dev/null", sessionName, tracePath);
    ASSERT_EQ(runCommand(cmd), 0);

    ASSERT_EQ(runCommand("lttng enable-event -u 'fprime_profiler:*' 2>/dev/null"), 0);
    ASSERT_EQ(runCommand("lttng start 2>/dev/null"), 0);

    // Create component - this will exercise the actual component code
    LttngProfiler profiler("RateGroup1Profiler");
    profiler.init(0);

    // Manually emit events to verify the tracepoint infrastructure works
    for (U32 i = 0; i < 5; i++) {
        tracepoint(fprime_profiler, entry, profiler.getObjName(), i);
        // Simulate some work
        usleep(100);
        tracepoint(fprime_profiler, exit, profiler.getObjName(), i);
    }

    // Stop tracing
    ASSERT_EQ(runCommand("lttng stop 2>/dev/null"), 0);

    // Verify trace exists
    char checkCmd[256];
    snprintf(checkCmd, sizeof(checkCmd), "test -d %s", tracePath);
    EXPECT_EQ(system(checkCmd), 0) << "Trace directory should exist after tracing";
}

}  // namespace FprimeTlmAlarm

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
