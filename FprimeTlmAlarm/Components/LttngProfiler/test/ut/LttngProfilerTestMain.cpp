// ======================================================================
// \title  LttngProfilerTestMain.cpp
// \author autocoder
// \brief  cpp file for LttngProfiler component test main function
// ======================================================================

#include "LttngProfilerTester.hpp"

TEST(Nominal, testForwarding) {
    FprimeTlmAlarm::LttngProfilerTester tester;
    tester.testForwarding();
}

TEST(Nominal, testMultipleContexts) {
    FprimeTlmAlarm::LttngProfilerTester tester;
    tester.testMultipleContexts();
}

TEST(EdgeCase, testDisconnectedOutput) {
    FprimeTlmAlarm::LttngProfilerTester tester;
    tester.testDisconnectedOutput();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
