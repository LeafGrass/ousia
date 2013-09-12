#ifndef __APB_UART_DEFS_H__
#define __APB_UART_DEFS_H__

#define UART_0_DW_APB_UART_BASE 0x10009000
#define UART_1_DW_APB_UART_BASE 0x1000A000

/* uart 0 and uart 1 */
#define APB_UART_BASE(x) UART_##x##_DW_APB_UART_BASE

#define UART_ReceiveBufferReg_Offset               0x00
#define UART_TransmitHoldingReg_Offset             0x00
#define UART_DivisorLatchLow_Offset                0x00
#define UART_InterruptEnableReg_Offset             0x04
#define UART_DivisorLatchHigh_Offset               0x04
#define UART_InterruptIdentificationReg_Offset     0x08
#define UART_FIFOControlReg_Offset                 0x08
#define UART_LineControlReg_Offset                 0x0C
#define UART_ModemControlReg_Offset                0x10
#define UART_LineStatusReg_Offset                  0x14
#define UART_ModemStatusReg_Offset                 0x18
#define UART_ScratchpadReg_Offset                  0x1C
#define UART_ShadowReceiveBufferRegLow_Offset      0x30
#define UART_ShadowReceiveBufferRegHigh_Offset     0x6C
#define UART_ShadowTransmitHoldingRegLow_Offset    0x30
#define UART_ShadowTransmitHoldingRegHigh_Offset   0x6C
#define UART_FIFOAccessReg_Offset                  0x70
#define UART_TransmitFIFOReadReg_Offset            0x74
#define UART_ReceiveFIFOWriteReg_Offset            0x78
#define UART_UARTStatusReg_Offset                  0x7C
#define UART_TransmitFIFOLevelReg_Offset           0x80
#define UART_ReceiveFIFOLevelReg_Offset            0x84
#define UART_SoftwareResetReg_Offset               0x88
#define UART_ShadowRequestToSendReg_Offset         0x8C
#define UART_ShadowBreakControlReg_Offset          0x90
#define UART_ShadowDMAModeReg_Offset               0x94
#define UART_ShadowFIFOEnableReg_Offset            0x98
#define UART_ShadowRCVRTriggerReg_Offset           0x9C
#define UART_ShadowTXEmptyTriggerReg_Offset        0xA0
#define UART_HaltTXReg_Offset                      0xA4
#define UART_DMASAReg_Offset                       0xA8
#define UART_CIDReg_Offset                         0xF4
#define UART_CVReg_Offset                          0xF8
#define UART_PIDReg_Offset                         0xFC

// UART 0
#define UART_ReceiveBufferReg(x)             (APB_UART_BASE(x) + UART_ReceiveBufferReg_Offset            )
#define UART_TransmitHoldingReg(x)           (APB_UART_BASE(x) + UART_TransmitHoldingReg_Offset          )
#define UART_DivisorLatchLow(x)              (APB_UART_BASE(x) + UART_DivisorLatchLow_Offset             )
#define UART_InterruptEnableReg(x)           (APB_UART_BASE(x) + UART_InterruptEnableReg_Offset          )
#define UART_DivisorLatchHigh(x)             (APB_UART_BASE(x) + UART_DivisorLatchHigh_Offset            )
#define UART_InterruptIdentificationReg(x)   (APB_UART_BASE(x) + UART_InterruptIdentificationReg_Offset  )
#define UART_FIFOControlReg(x)               (APB_UART_BASE(x) + UART_FIFOControlReg_Offset              )
#define UART_LineControlReg(x)               (APB_UART_BASE(x) + UART_LineControlReg_Offset              )
#define UART_ModemControlReg(x)              (APB_UART_BASE(x) + UART_ModemControlReg_Offset             )
#define UART_LineStatusReg(x)                (APB_UART_BASE(x) + UART_LineStatusReg_Offset               )
#define UART_ModemStatusReg(x)               (APB_UART_BASE(x) + UART_ModemStatusReg_Offset              )
#define UART_ScratchpadReg(x)                (APB_UART_BASE(x) + UART_ScratchpadReg_Offset               )
#define UART_ShadowReceiveBufferRegLow(x)    (APB_UART_BASE(x) + UART_ShadowReceiveBufferRegLow_Offset   )
#define UART_ShadowReceiveBufferRegHigh(x)   (APB_UART_BASE(x) + UART_ShadowReceiveBufferRegHigh_Offset  )
#define UART_ShadowTransmitHoldingRegLow(x)  (APB_UART_BASE(x) + UART_ShadowTransmitHoldingRegLow_Offset )
#define UART_ShadowTransmitHoldingRegHigh(x) (APB_UART_BASE(x) + UART_ShadowTransmitHoldingRegHigh_Offset)
#define UART_FIFOAccessReg(x)                (APB_UART_BASE(x) + UART_FIFOAccessReg_Offset               )
#define UART_TransmitFIFOReadReg(x)          (APB_UART_BASE(x) + UART_TransmitFIFOReadReg_Offset         )
#define UART_ReceiveFIFOWriteReg(x)          (APB_UART_BASE(x) + UART_ReceiveFIFOWriteReg_Offset         )
#define UART_UARTStatusReg(x)                (APB_UART_BASE(x) + UART_UARTStatusReg_Offset               )
#define UART_TransmitFIFOLevelReg(x)         (APB_UART_BASE(x) + UART_TransmitFIFOLevelReg_Offset        )
#define UART_ReceiveFIFOLevelReg(x)          (APB_UART_BASE(x) + UART_ReceiveFIFOLevelReg_Offset         )
#define UART_SoftwareResetReg(x)             (APB_UART_BASE(x) + UART_SoftwareResetReg_Offset            )
#define UART_ShadowRequestToSendReg(x)       (APB_UART_BASE(x) + UART_ShadowRequestToSendReg_Offset      )
#define UART_ShadowBreakControlReg(x)        (APB_UART_BASE(x) + UART_ShadowBreakControlReg_Offset       )
#define UART_ShadowDMAModeReg(x)             (APB_UART_BASE(x) + UART_ShadowDMAModeReg_Offset            )
#define UART_ShadowFIFOEnableReg(x)          (APB_UART_BASE(x) + UART_ShadowFIFOEnableReg_Offset         )
#define UART_ShadowRCVRTriggerReg(x)         (APB_UART_BASE(x) + UART_ShadowRCVRTriggerReg_Offset        )
#define UART_ShadowTXEmptyTriggerReg(x)      (APB_UART_BASE(x) + UART_ShadowTXEmptyTriggerReg_Offset     )
#define UART_HaltTXReg(x)                    (APB_UART_BASE(x) + UART_HaltTXReg_Offset                   )
#define UART_DMASAReg(x)                     (APB_UART_BASE(x) + UART_DMASAReg_Offset                    )
#define UART_CIDReg(x)                       (APB_UART_BASE(x) + UART_CIDReg_Offset                      )
#define UART_CVReg(x)                        (APB_UART_BASE(x) + UART_CVReg_Offset                       )
#define UART_PIDReg(x)                       (APB_UART_BASE(x) + UART_PIDReg_Offset                      )

#define UART_RBR(x)                          (APB_UART_BASE(x) + UART_ReceiveBufferReg_Offset            )
#define UART_THR(x)                          (APB_UART_BASE(x) + UART_TransmitHoldingReg_Offset          )
#define UART_DLL(x)                          (APB_UART_BASE(x) + UART_DivisorLatchLow_Offset             )
#define UART_IER(x)                          (APB_UART_BASE(x) + UART_InterruptEnableReg_Offset          )
#define UART_DLH(x)                          (APB_UART_BASE(x) + UART_DivisorLatchHigh_Offset            )
#define UART_IIR(x)                          (APB_UART_BASE(x) + UART_InterruptIdentificationReg_Offset  )
#define UART_FCR(x)                          (APB_UART_BASE(x) + UART_FIFOControlReg_Offset              )
#define UART_LCR(x)                          (APB_UART_BASE(x) + UART_LineControlReg_Offset              )
#define UART_MCR(x)                          (APB_UART_BASE(x) + UART_ModemControlReg_Offset             )
#define UART_LSR(x)                          (APB_UART_BASE(x) + UART_LineStatusReg_Offset               )
#define UART_MSR(x)                          (APB_UART_BASE(x) + UART_ModemStatusReg_Offset              )
#define UART_SCR(x)                          (APB_UART_BASE(x) + UART_ScratchpadReg_Offset               )

#endif /* __APB_UART_DEFS_H__ */
