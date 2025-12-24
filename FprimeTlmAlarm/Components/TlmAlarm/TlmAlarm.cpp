// ======================================================================
// \title  TlmAlarm.cpp
// \author wmac
// \brief  cpp file for TlmAlarm component implementation class
// ======================================================================

#include "FprimeTlmAlarm/Components/TlmAlarm/TlmAlarm.hpp"

namespace FprimeTlmAlarm {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

TlmAlarm ::TlmAlarm(const char* const compName) : TlmAlarmComponentBase(compName) {}

TlmAlarm ::~TlmAlarm() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void TlmAlarm ::TlmRecv_handler(FwIndexType portNum, FwChanIdType id, Fw::Time& timeTag, Fw::TlmBuffer& val) {
    // TODO
}

Fw::ParamValid TlmAlarm ::paramMock_handler(FwIndexType portNum, FwPrmIdType id, Fw::ParamBuffer& val) {
    return Fw::ParamValid::VALID;
}

void TlmAlarm ::run_handler(FwIndexType portNum, U32 context) {
    // TODO
}

void TlmAlarm ::seqDoneIn_handler(FwIndexType portNum,
                                  FwOpcodeType opCode,
                                  U32 cmdSeq,
                                  const Fw::CmdResponse& response) {
    // TODO
}

void TlmAlarm ::seqStartIn_handler(FwIndexType portNum, const Fw::StringBase& filename) {
    // TODO
}

Fw::TlmValid TlmAlarm ::tlmMock_handler(FwIndexType portNum, FwChanIdType id, Fw::Time& timeTag, Fw::TlmBuffer& val) {
    return Fw::TlmValid::VALID;
}

}  // namespace FprimeTlmAlarm
