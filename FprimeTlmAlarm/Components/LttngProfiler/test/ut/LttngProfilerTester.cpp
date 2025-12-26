// ======================================================================
// \title  LttngProfilerTester.cpp
// \author autocoder
// \brief  cpp file for LttngProfiler component test harness implementation class
// ======================================================================

#include "LttngProfilerTester.hpp"

namespace FprimeTlmAlarm {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

LttngProfilerTester::LttngProfilerTester()
    : LttngProfilerGTestBase("LttngProfilerTester", LttngProfilerTester::MAX_HISTORY_SIZE),
      component("LttngProfiler") {
    this->initComponents();
    this->connectPorts();
}

LttngProfilerTester::~LttngProfilerTester() {}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void LttngProfilerTester::testForwarding() {
    const U32 CONTEXT = 0x12345678;

    // Invoke schedIn with a context value
    invoke_to_schedIn(0, CONTEXT);

    // Assert schedOut was called once with the same context
    ASSERT_from_schedOut_SIZE(1);
    ASSERT_from_schedOut(0, CONTEXT);
}

void LttngProfilerTester::testMultipleContexts() {
    const U32 NUM_CALLS = 5;

    // Invoke schedIn multiple times with different contexts
    for (U32 i = 0; i < NUM_CALLS; i++) {
        invoke_to_schedIn(0, i * 100);
    }

    // Assert schedOut was called NUM_CALLS times with correct contexts
    ASSERT_from_schedOut_SIZE(NUM_CALLS);
    for (U32 i = 0; i < NUM_CALLS; i++) {
        ASSERT_from_schedOut(i, i * 100);
    }
}

void LttngProfilerTester::testDisconnectedOutput() {
    const U32 CONTEXT = 0xDEADBEEF;

    // Disconnect the output port
    this->component.set_schedOut_OutputPort(0, nullptr);

    // Invoke schedIn - should not crash even with disconnected output
    invoke_to_schedIn(0, CONTEXT);

    // Assert schedOut was not called
    ASSERT_from_schedOut_SIZE(0);
}

// ----------------------------------------------------------------------
// Helper methods
// ----------------------------------------------------------------------

void LttngProfilerTester::connectPorts() {
    // Input schedIn
    this->connect_to_schedIn(0, this->component.get_schedIn_InputPort(0));

    // Output schedOut
    this->component.set_schedOut_OutputPort(0, this->get_from_schedOut(0));
}

void LttngProfilerTester::initComponents() {
    this->init();
    this->component.init(LttngProfilerTester::TEST_INSTANCE_ID);
}

}  // namespace FprimeTlmAlarm
