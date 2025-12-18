// ======================================================================
// \title  TlmSplitter.hpp
// \author will.maccormack
// \brief  hpp file for TlmSplitter component implementation class
// ======================================================================

#ifndef FprimeTlmAlarm_TlmSplitter_HPP
#define FprimeTlmAlarm_TlmSplitter_HPP

#include "FprimeTlmAlarm/Components/TlmSplitter/TlmSplitterComponentAc.hpp"

namespace FprimeTlmAlarm {

class TlmSplitter final : public TlmSplitterComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TlmSplitter object
    TlmSplitter(const char* const compName  //!< The component name
    );

    //! Destroy TlmSplitter object
    ~TlmSplitter();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for TlmRecv
    //!
    //! Recv Telemetry Stream
    void TlmRecv_handler(FwIndexType portNum,  //!< The port number
                         FwChanIdType id,      //!< Telemetry Channel ID
                         Fw::Time& timeTag,    //!< Time Tag
                         Fw::TlmBuffer& val    //!< Buffer containing serialized telemetry value
                         ) override;
};

}  // namespace FprimeTlmAlarm

#endif
