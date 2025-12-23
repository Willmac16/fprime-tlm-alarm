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
            // Using the same pattern as ComSplitter, since ComBuffer and TlmBuffer both
            // have their data stored in a member var buffer
            // This protects against downstream modifying a shared TlmBuffer should the call be sync
            Fw::TlmBuffer valCopy = val;
            TlmSend_out(i, id, timeTag, valCopy);
        }
    }
}

}  // namespace FprimeTlmAlarm
