// ======================================================================
// \title  TlmSplitterTestMain.cpp
// \author wmac
// \brief  cpp file for TlmSplitter component test main function
// ======================================================================

#include "TlmSplitterTester.hpp"

TEST(Nominal, splits) {
    FprimeTlmAlarm::TlmSplitterTester tester;
    tester.splits();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
