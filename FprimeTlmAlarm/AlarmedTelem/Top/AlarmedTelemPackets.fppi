telemetry packets AlarmedTelemPackets {

  packet CDH id 1 group 1 {
    CdhCore.cmdDisp.CommandsDispatched
    CdhCore.cmdDisp.CommandsDropped

    FileHandling.fileUplink.FilesReceived
    FileHandling.fileUplink.PacketsReceived
    FileHandling.fileDownlink.FilesSent
    FileHandling.fileDownlink.PacketsSent
    FileHandling.fileManager.CommandsExecuted

    FprimeTlmAlarm.cmdSeq.CS_LoadCommands
    FprimeTlmAlarm.cmdSeq.CS_CancelCommands
    FprimeTlmAlarm.cmdSeq.CS_CommandsExecuted
    FprimeTlmAlarm.cmdSeq.CS_SequencesCompleted
    ComCcsds.comQueue.comQueueDepth
    ComCcsds.comQueue.buffQueueDepth
    ComCcsds.commsBufferManager.TotalBuffs
    ComCcsds.commsBufferManager.CurrBuffs
    ComCcsds.commsBufferManager.HiBuffs
    #ComCcsds.tlmSend.SendLevel

    FprimeTlmAlarm.rateGroup1.RgMaxTime
    FprimeTlmAlarm.rateGroup2.RgMaxTime
    FprimeTlmAlarm.rateGroup3.RgMaxTime
  }

  packet CDHErrors id 2 group 1 {
    CdhCore.$health.PingLateWarnings

    FileHandling.fileUplink.Warnings
    FileHandling.fileDownlink.Warnings
    FileHandling.fileManager.Errors

    FprimeTlmAlarm.cmdSeq.CS_Errors
    ComCcsds.commsBufferManager.NoBuffs
    ComCcsds.commsBufferManager.EmptyBuffs

    FprimeTlmAlarm.rateGroup1.RgCycleSlips
    FprimeTlmAlarm.rateGroup2.RgCycleSlips
    FprimeTlmAlarm.rateGroup3.RgCycleSlips
  }

  packet SystemRes1 id 5 group 2 {
    FprimeTlmAlarm.systemResources.MEMORY_TOTAL
    FprimeTlmAlarm.systemResources.MEMORY_USED
    FprimeTlmAlarm.systemResources.NON_VOLATILE_TOTAL
    FprimeTlmAlarm.systemResources.NON_VOLATILE_FREE
  }

  packet SystemRes3 id 6 group 2 {
    FprimeTlmAlarm.systemResources.CPU
    FprimeTlmAlarm.systemResources.CPU_00
    FprimeTlmAlarm.systemResources.CPU_01
    FprimeTlmAlarm.systemResources.CPU_02
    FprimeTlmAlarm.systemResources.CPU_03
    FprimeTlmAlarm.systemResources.CPU_04
    FprimeTlmAlarm.systemResources.CPU_05
    FprimeTlmAlarm.systemResources.CPU_06
    FprimeTlmAlarm.systemResources.CPU_07
    FprimeTlmAlarm.systemResources.CPU_08
    FprimeTlmAlarm.systemResources.CPU_09
    FprimeTlmAlarm.systemResources.CPU_10
    FprimeTlmAlarm.systemResources.CPU_11
    FprimeTlmAlarm.systemResources.CPU_12
    FprimeTlmAlarm.systemResources.CPU_13
    FprimeTlmAlarm.systemResources.CPU_14
    FprimeTlmAlarm.systemResources.CPU_15
  }

  packet DataProducts id 21 group 3 {
    DataProducts.dpCat.CatalogDps
    DataProducts.dpCat.DpsSent

    DataProducts.dpMgr.NumSuccessfulAllocations
    DataProducts.dpMgr.NumFailedAllocations
    DataProducts.dpMgr.NumDataProducts
    DataProducts.dpMgr.NumBytes

    DataProducts.dpWriter.NumBuffersReceived
    DataProducts.dpWriter.NumBytesWritten
    DataProducts.dpWriter.NumSuccessfulWrites
    DataProducts.dpWriter.NumFailedWrites
    DataProducts.dpWriter.NumErrors

    DataProducts.dpBufferManager.TotalBuffs
    DataProducts.dpBufferManager.CurrBuffs
    DataProducts.dpBufferManager.HiBuffs
    DataProducts.dpBufferManager.NoBuffs
    DataProducts.dpBufferManager.EmptyBuffs
  }

  packet Version1 id 22 group 2 {
    CdhCore.version.FrameworkVersion
    CdhCore.version.ProjectVersion
  }

  packet Version_Library1 id 23 group 2 {
    CdhCore.version.LibraryVersion01
    CdhCore.version.LibraryVersion02
  }

  packet Version_Library2 id 24 group 2 {
    CdhCore.version.LibraryVersion03
    CdhCore.version.LibraryVersion04
  }

  packet Version_Library3 id 25 group 2 {
    CdhCore.version.LibraryVersion05
    CdhCore.version.LibraryVersion06
  }

  packet Version_Library4 id 26 group 2 {
    CdhCore.version.LibraryVersion07
    CdhCore.version.LibraryVersion08
  }

  packet Version_Library5 id 27 group 2 {
    CdhCore.version.LibraryVersion09
    CdhCore.version.LibraryVersion10
  }

  packet Version_Custom1 id 28 group 2 {
    CdhCore.version.CustomVersion01
  }

  packet Version_Custom2 id 29 group 2 {
    CdhCore.version.CustomVersion02
  }

  packet Version_Custom3 id 30 group 2 {
    CdhCore.version.CustomVersion03
  }

  packet Version_Custom4 id 31 group 2 {
    CdhCore.version.CustomVersion04
  }

  packet Version_Custom5 id 32 group 2 {
    CdhCore.version.CustomVersion05
  }

  packet Version_Custom6 id 33 group 2 {
    CdhCore.version.CustomVersion06
  }

  packet Version_Custom7 id 34 group 2 {
    CdhCore.version.CustomVersion07
  }

  packet Version_Custom8 id 35 group 2 {
    CdhCore.version.CustomVersion08
  }

  packet Version_Custom9 id 36 group 2 {
    CdhCore.version.CustomVersion09
  }

  packet Version_Custom10 id 37 group 2 {
    CdhCore.version.CustomVersion10
  }

  packet TlmAlarmSeq id 38 group 2 {
    tlmAlarmSeq.State
    tlmAlarmSeq.SequencesSucceeded
    tlmAlarmSeq.SequencesFailed
    tlmAlarmSeq.SequencesCancelled
    tlmAlarmSeq.StatementsDispatched
    tlmAlarmSeq.StatementsFailed
    tlmAlarmSeq.LastDirectiveError
    tlmAlarmSeq.DirectiveErrorIndex
    tlmAlarmSeq.DirectiveErrorId
    tlmAlarmSeq.SeqPath
    tlmAlarmSeq.Debug_ReachedEndOfFile
    tlmAlarmSeq.Debug_NextStatementReadSuccess
    tlmAlarmSeq.Debug_NextStatementOpcode
    tlmAlarmSeq.Debug_NextCmdOpcode
    tlmAlarmSeq.Debug_StackSize
    tlmAlarmSeq.BreakpointInUse
    tlmAlarmSeq.BreakpointIndex
    tlmAlarmSeq.BreakOnlyOnceOnBreakpoint
    tlmAlarmSeq.BreakBeforeNextLine
    tlmAlarmSeq.PRM_STATEMENT_TIMEOUT_SECS
    tlmAlarmSeq.PRM_FLAG_DEFAULT_EXIT_ON_CMD_FAIL
  }

} omit {
  CdhCore.cmdDisp.CommandErrors
}
