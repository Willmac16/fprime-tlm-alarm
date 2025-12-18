// ======================================================================
// \title  TlmSplitterTester.cpp
// \author Will MacCormack
// \brief  cpp file for TlmSplitter component test harness implementation class
// ======================================================================

#include "TlmSplitterTester.hpp"

namespace FprimeTlmAlarm {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

TlmSplitterTester ::TlmSplitterTester()
    : TlmSplitterGTestBase("TlmSplitterTester", TlmSplitterTester::MAX_HISTORY_SIZE), component("TlmSplitter") {
    this->initComponents();
    this->connectPorts();
}

TlmSplitterTester ::~TlmSplitterTester() {}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void TlmSplitterTester ::assert_TlmSend(const U32 historyInd,
                                        const FwChanIdType chanId,
                                        const Fw::Time& timeTag,
                                        const Fw::TlmBuffer& tlm) {}

void TlmSplitterTester ::splits() {
    const FwChanIdType ID = 0x1700;
    Fw::Time base_time = {1717, 7171};

    U8 buf[4] = {0xDE, 0xAD, 0xC0, 0xDE};

    const U8 ITERS = 3;
    const U8 OUTS = 3;

    Fw::TlmBuffer tlm(buf, sizeof(buf));

    for (U8 ind = 0; ind < ITERS; ind++) {
        Fw::Time time = base_time;
        time.add(ind, ind);

        buf[0] = ind;

        invoke_to_TlmRecv(0, ID + ind, time, tlm);
    }

    ASSERT_from_TlmSend_SIZE(OUTS * ITERS);

    for (U8 ind = 0; ind < ITERS; ind++) {
        Fw::Time time = base_time;
        time.add(ind, ind);

        buf[0] = ind;

        for (U8 port = 0; port < TlmSplitter::NUM_TLMSEND_OUTPUT_PORTS; port++) {
            assert_TlmSend(ind * OUTS + port, ID + ind, time, tlm);
        }
    }
}

// ----------------------------------------------------------------------
// Helper methods
// ----------------------------------------------------------------------

void TlmSplitterTester ::connectPorts() {
    // Input TlmRecv
    this->connect_to_TlmRecv(0, this->component.get_TlmRecv_InputPort(0));

    // Connect every other TlmSend output port
    for (FwIndexType i = 0; i < TlmSplitter::NUM_TLMSEND_OUTPUT_PORTS; i += 2) {
        this->component.set_TlmSend_OutputPort(i, this->get_from_TlmSend(i));
    }
}

void TlmSplitterTester ::initComponents() {
    this->init();
    this->component.init(TlmSplitterTester::TEST_INSTANCE_ID);
}

}  // namespace FprimeTlmAlarm
