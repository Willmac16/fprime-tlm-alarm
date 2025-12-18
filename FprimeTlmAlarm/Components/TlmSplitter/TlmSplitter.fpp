module FprimeTlmAlarm {
    @ Repeats
    passive component TlmSplitter {

        constant NUM_OUTPUTS = 5

        @ Recv Telemetry Stream
        sync input port TlmRecv: Fw.Tlm

        @ Duplicate Output Telemetry Streams
        output port TlmSend: [NUM_OUTPUTS] Fw.Tlm
    }
}