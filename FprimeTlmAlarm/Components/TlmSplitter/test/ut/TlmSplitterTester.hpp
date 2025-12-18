// ======================================================================
// \title  TlmSplitterTester.hpp
// \author wmac
// \brief  hpp file for TlmSplitter component test harness implementation class
// ======================================================================

#ifndef FprimeTlmAlarm_TlmSplitterTester_HPP
#define FprimeTlmAlarm_TlmSplitterTester_HPP

#include "FprimeTlmAlarm/Components/TlmSplitter/TlmSplitter.hpp"
#include "FprimeTlmAlarm/Components/TlmSplitter/TlmSplitterGTestBase.hpp"

namespace FprimeTlmAlarm {

class TlmSplitterTester final : public TlmSplitterGTestBase {
  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    // Maximum size of histories storing events, telemetry, and port outputs
    static const FwSizeType MAX_HISTORY_SIZE = 10;

    // Instance ID supplied to the component instance under test
    static const FwEnumStoreType TEST_INSTANCE_ID = 0;

  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Construct object TlmSplitterTester
    TlmSplitterTester();

    //! Destroy object TlmSplitterTester
    ~TlmSplitterTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! It splits the telem
    void splits();

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    TlmSplitter component;

    // ----------------------------------------------------------------------
    // Helper Functions
    // ----------------------------------------------------------------------

    //! Assert an element of TlmSend History
    void assert_TlmSend(const U32 historyInd,
                        const FwChanIdType chanId,
                        const Fw::Time& timeTag,
                        const Fw::TlmBuffer& tlm);
};

}  // namespace FprimeTlmAlarm

#endif
