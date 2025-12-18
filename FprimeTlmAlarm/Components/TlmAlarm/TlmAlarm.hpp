// ======================================================================
// \title  TlmAlarm.hpp
// \author wmac
// \brief  hpp file for TlmAlarm component implementation class
// ======================================================================

#ifndef FprimeTlmAlarm_TlmAlarm_HPP
#define FprimeTlmAlarm_TlmAlarm_HPP

#include "FprimeTlmAlarm/Components/TlmAlarm/TlmAlarmComponentAc.hpp"

namespace FprimeTlmAlarm {

class TlmAlarm final : public TlmAlarmComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TlmAlarm object
    TlmAlarm(const char* const compName  //!< The component name
    );

    //! Destroy TlmAlarm object
    ~TlmAlarm();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command TODO_1
    //!
    //! TODO
    void TODO_1_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                           U32 cmdSeq            //!< The command sequence number
                           ) override;

    //! Handler implementation for command TODO_2
    //!
    //! TODO
    void TODO_2_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                           U32 cmdSeq            //!< The command sequence number
                           ) override;
};

}  // namespace FprimeTlmAlarm

#endif
