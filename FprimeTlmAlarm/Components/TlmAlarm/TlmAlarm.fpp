module FprimeTlmAlarm {
    @ Monitor Tlm Mnemonics Onboard
    queued component TlmAlarm {
        # RX Tlm from the system (Likely a TlmSplitter)
        async input port TlmRecv: Fw.Tlm

        @ Example port: receiving calls from the rate group
        sync input port run: Svc.Sched

        ##############################################################################
        #### Ports for interfacing w/ the FpySeq                                     #
        ##############################################################################

        # Mock getters
        @ port for feeding channel comparison seq tlm values
        guarded input port tlmMock: Fw.TlmGet

        @ port for feeding channel comparison seq thresholds and receiving debounce/persistence
        guarded input port paramMock: Fw.PrmGet

        # Seq Commanding
        @ port for requests to run sequences
        output port seqRunOut: Svc.CmdSeqIn

        @ called when a sequence begins running
        guarded input port seqStartIn: Svc.CmdSeqIn

        @ called when a sequence finishes running, either successfully or not
        guarded input port seqDoneIn: Fw.CmdResponse

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}