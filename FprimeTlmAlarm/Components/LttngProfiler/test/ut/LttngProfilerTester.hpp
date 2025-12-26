// ======================================================================
// \title  LttngProfilerTester.hpp
// \author autocoder
// \brief  hpp file for LttngProfiler component test harness implementation class
// ======================================================================

#ifndef FprimeTlmAlarm_LttngProfilerTester_HPP
#define FprimeTlmAlarm_LttngProfilerTester_HPP

#include "FprimeTlmAlarm/Components/LttngProfiler/LttngProfiler.hpp"
#include "FprimeTlmAlarm/Components/LttngProfiler/LttngProfilerGTestBase.hpp"

namespace FprimeTlmAlarm {

class LttngProfilerTester final : public LttngProfilerGTestBase {
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

    //! Construct object LttngProfilerTester
    LttngProfilerTester();

    //! Destroy object LttngProfilerTester
    ~LttngProfilerTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! Test that schedule signal is forwarded
    void testForwarding();

    //! Test with multiple context values
    void testMultipleContexts();

    //! Test with disconnected output
    void testDisconnectedOutput();

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
    LttngProfiler component;
};

}  // namespace FprimeTlmAlarm

#endif
