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
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for TlmRecv
    void TlmRecv_handler(FwIndexType portNum,  //!< The port number
                         FwChanIdType id,      //!< Telemetry Channel ID
                         Fw::Time& timeTag,    //!< Time Tag
                         Fw::TlmBuffer& val    //!< Buffer containing serialized telemetry value
                         ) override;

    //! Handler implementation for paramMock
    //!
    //! port for feeding channel comparison seq thresholds and receiving debounce/persistence
    Fw::ParamValid paramMock_handler(FwIndexType portNum,  //!< The port number
                                     FwPrmIdType id,       //!< Parameter ID
                                     Fw::ParamBuffer& val  //!< Buffer containing serialized parameter value.
                                                           //!< Unmodified if param not found.
                                     ) override;

    //! Handler implementation for run
    //!
    //! Example port: receiving calls from the rate group
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

    //! Handler implementation for seqDoneIn
    //!
    //! called when a sequence finishes running, either successfully or not
    void seqDoneIn_handler(FwIndexType portNum,             //!< The port number
                           FwOpcodeType opCode,             //!< Command Op Code
                           U32 cmdSeq,                      //!< Command Sequence
                           const Fw::CmdResponse& response  //!< The command response argument
                           ) override;

    //! Handler implementation for seqStartIn
    //!
    //! called when a sequence begins running
    void seqStartIn_handler(FwIndexType portNum,            //!< The port number
                            const Fw::StringBase& filename  //!< The sequence file
                            ) override;

    //! Handler implementation for tlmMock
    //!
    //! port for feeding channel comparison seq tlm values
    Fw::TlmValid tlmMock_handler(FwIndexType portNum,  //!< The port number
                                 FwChanIdType id,      //!< Telemetry Channel ID
                                 Fw::Time& timeTag,    //!< Time Tag
                                 Fw::TlmBuffer& val    //!< Buffer containing serialized telemetry value.
                                                       //!< Size set to 0 if channel not found, or if no value
                                                       //!< has been received for this channel yet.
                                 ) override;
};

}  // namespace FprimeTlmAlarm

#endif
