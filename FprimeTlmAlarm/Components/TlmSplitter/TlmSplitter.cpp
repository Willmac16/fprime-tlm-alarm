// ======================================================================
// \title  TlmSplitter.cpp
// \author will.maccormack
// \brief  cpp file for TlmSplitter component implementation class
// ======================================================================

#include "FprimeTlmAlarm/Components/TlmSplitter/TlmSplitter.hpp"

namespace FprimeTlmAlarm {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TlmSplitter ::TlmSplitter(const char* const compName) : TlmSplitterComponentBase(compName) {}

TlmSplitter ::~TlmSplitter() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void TlmSplitter ::TlmRecv_handler(FwIndexType portNum, FwChanIdType id, Fw::Time& timeTag, Fw::TlmBuffer& val) {
    for (FwIndexType i = 0; i < getNum_TlmSend_OutputPorts(); i++) {
        if (isConnected_TlmSend_OutputPort(i)) {
            // Telemetry -> TlmPkt or TlmChan is sent sync by ref
            // As long as this component is sync & downstream doesn't modify
            // Things should be fine
            TlmSend_out(i, id, timeTag, val);
        }
    }
}

}  // namespace FprimeTlmAlarm
