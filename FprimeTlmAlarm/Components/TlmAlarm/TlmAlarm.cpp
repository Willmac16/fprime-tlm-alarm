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
// Handler implementations for commands
// ----------------------------------------------------------------------

void TlmAlarm ::TODO_1_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void TlmAlarm ::TODO_2_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace FprimeTlmAlarm
