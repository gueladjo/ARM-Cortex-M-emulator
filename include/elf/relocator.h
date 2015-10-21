/**
 * @file relocator.h
 * @author François Portet <francois.portet@grenoble-inp.fr>
 * @date Sun Sep 13 22:11:10 2015
 * @brief ARM 32 bits relocation.
 *
 * ARM 32 bits relocation.
 */

#ifndef _RELOCATOR_H_
#define _RELOCATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define  R_ARM_NONE   0x00
#define  R_ARM_PC24   0x01
#define  R_ARM_ABS32    0x02
#define  R_ARM_REL32    0x03
#define  R_ARM_LDR_PC_G0           0x04
#define  R_ARM_ABS16    0x05
#define  R_ARM_ABS12    0x06
#define  R_ARM_THM_ABS5            0x07
#define  R_ARM_ABS8   0x08
#define  R_ARM_SBREL32             0x09
#define  R_ARM_THM_CALL            0x0a
#define  R_ARM_THM_PC8             0x0b
#define  R_ARM_BREL_ADJ            0x0c
#define  R_ARM_TLS_DESC            0x0d
#define  R_ARM_THM_SWI8            0x0e
#define  R_ARM_XPC25    0x0f
#define  R_ARM_THM_XPC22           0x10
#define  R_ARM_TLS_DTPMOD32        0x11
#define  R_ARM_TLS_DTPOFF32        0x12
#define  R_ARM_TLS_TPOFF32         0x13
#define  R_ARM_COPY   0x14
#define  R_ARM_GLOB_DAT            0x15
#define  R_ARM_JUMP_SLOT           0x16
#define  R_ARM_RELATIVE            0x17
#define  R_ARM_GOTOFF32            0x18
#define  R_ARM_BASE_PREL           0x19
#define  R_ARM_GOT_BREL            0x1a
#define  R_ARM_PLT32    0x1b
#define  R_ARM_CALL   0x1c
#define  R_ARM_JUMP24   0x1d
#define  R_ARM_THM_JUMP24          0x1e
#define  R_ARM_BASE_ABS            0x1f
#define  R_ARM_ALU_PCREL_7_0       0x20
#define  R_ARM_ALU_PCREL_15_8      0x21
#define  R_ARM_ALU_PCREL_23_15     0x22
#define  R_ARM_LDR_SBREL_11_0_NC   0x23
#define  R_ARM_ALU_SBREL_19_12_NC  0x24
#define  R_ARM_ALU_SBREL_27_20_CK  0x25
#define  R_ARM_TARGET1             0x26
#define  R_ARM_SBREL31             0x27
#define  R_ARM_V4BX   0x28
#define  R_ARM_TARGET2             0x29
#define  R_ARM_PREL31   0x2a
#define  R_ARM_MOVW_ABS_NC         0x2b
#define  R_ARM_MOVT_ABS            0x2c
#define  R_ARM_MOVW_PREL_NC        0x2d
#define  R_ARM_MOVT_PREL           0x2e
#define  R_ARM_THM_MOVW_ABS_NC     0x2f
#define  R_ARM_THM_MOVT_ABS        0x30
#define  R_ARM_THM_MOVW_PREL_NC    0x31
#define  R_ARM_THM_MOVT_PREL       0x32
#define  R_ARM_THM_JUMP19          0x33
#define  R_ARM_THM_JUMP6           0x34
#define  R_ARM_THM_ALU_PREL_11_0   0x35
#define  R_ARM_THM_PC12            0x36
#define  R_ARM_ABS32_NOI           0x37
#define  R_ARM_REL32_NOI           0x38
#define  R_ARM_ALU_PC_G0_NC        0x39
#define  R_ARM_ALU_PC_G0           0x3a
#define  R_ARM_ALU_PC_G1_NC        0x3b
#define  R_ARM_ALU_PC_G1           0x3c
#define  R_ARM_ALU_PC_G2           0x3d
#define  R_ARM_LDR_PC_G1           0x3e
#define  R_ARM_LDR_PC_G2           0x3f
#define  R_ARM_LDRS_PC_G0          0x40
#define  R_ARM_LDRS_PC_G1          0x41
#define  R_ARM_LDRS_PC_G2          0x42
#define  R_ARM_LDC_PC_G0           0x43
#define  R_ARM_LDC_PC_G1           0x44
#define  R_ARM_LDC_PC_G2           0x45
#define  R_ARM_ALU_SB_G0_NC        0x46
#define  R_ARM_ALU_SB_G0           0x47
#define  R_ARM_ALU_SB_G1_NC        0x48
#define  R_ARM_ALU_SB_G1           0x49
#define  R_ARM_ALU_SB_G2           0x4a
#define  R_ARM_LDR_SB_G0           0x4b
#define  R_ARM_LDR_SB_G1           0x4c
#define  R_ARM_LDR_SB_G2           0x4d
#define  R_ARM_LDRS_SB_G0          0x4e
#define  R_ARM_LDRS_SB_G1          0x4f
#define  R_ARM_LDRS_SB_G2          0x50
#define  R_ARM_LDC_SB_G0           0x51
#define  R_ARM_LDC_SB_G1           0x52
#define  R_ARM_LDC_SB_G2           0x53
#define  R_ARM_MOVW_BREL_NC        0x54
#define  R_ARM_MOVT_BREL           0x55
#define  R_ARM_MOVW_BREL           0x56
#define  R_ARM_THM_MOVW_BREL_NC    0x57
#define  R_ARM_THM_MOVT_BREL       0x58
#define  R_ARM_THM_MOVW_BREL       0x59
#define  R_ARM_TLS_GOTDESC         0x5a
#define  R_ARM_TLS_CALL            0x5b
#define  R_ARM_TLS_DESCSEQ         0x5c
#define  R_ARM_THM_TLS_CALL        0x5d
#define  R_ARM_PLT32_ABS           0x5e
#define  R_ARM_GOT_ABS             0x5f
#define  R_ARM_GOT_PREL            0x60
#define  R_ARM_GOT_BREL12          0x61
#define  R_ARM_GOTOFF12            0x62
#define  R_ARM_GOTRELAX            0x63
#define  R_ARM_GNU_VTENTRY         0x64
#define  R_ARM_GNU_VTINHERIT       0x65
#define  R_ARM_THM_JUMP11          0x66
#define  R_ARM_THM_JUMP8           0x67
#define  R_ARM_TLS_GD32            0x68
#define  R_ARM_TLS_LDM32           0x69
#define  R_ARM_TLS_LDO32           0x6a
#define  R_ARM_TLS_IE32            0x6b
#define  R_ARM_TLS_LE32            0x6c
#define  R_ARM_TLS_LDO12           0x6d
#define  R_ARM_TLS_LE12            0x6e
#define  R_ARM_TLS_IE12GP          0x6f
#define  R_ARM_PRIVATE_0           0x70
#define  R_ARM_PRIVATE_1           0x71
#define  R_ARM_PRIVATE_2           0x72
#define  R_ARM_PRIVATE_3           0x73
#define  R_ARM_PRIVATE_4           0x74
#define  R_ARM_PRIVATE_5           0x75
#define  R_ARM_PRIVATE_6           0x76
#define  R_ARM_PRIVATE_7           0x77
#define  R_ARM_PRIVATE_8           0x78
#define  R_ARM_PRIVATE_9           0x79
#define  R_ARM_PRIVATE_10          0x7a
#define  R_ARM_PRIVATE_11          0x7b
#define  R_ARM_PRIVATE_12          0x7c
#define  R_ARM_PRIVATE_13          0x7d
#define  R_ARM_PRIVATE_14          0x7e
#define  R_ARM_PRIVATE_15          0x7f
#define  R_ARM_ME_TOO              0x80
#define  R_ARM_THM_TLS_DESCSEQ16   0x81
#define  R_ARM_THM_TLS_DESCSEQ32   0x82







static char *ARM32_REL[] = {
    "R_ARM_NONE","R_ARM_PC24","R_ARM_ABS32","R_ARM_REL32",
    "R_ARM_LDR_PC_G0","R_ARM_ABS16","R_ARM_ABS12","R_ARM_THM_ABS5",
    "R_ARM_ABS8","R_ARM_SBREL32","R_ARM_THM_CALL","R_ARM_THM_PC8",
    "R_ARM_BREL_ADJ","R_ARM_TLS_DESC","R_ARM_THM_SWI8","R_ARM_XPC25",
    "R_ARM_THM_XPC22","R_ARM_TLS_DTPMOD32","R_ARM_TLS_DTPOFF32",
    "R_ARM_TLS_TPOFF32","R_ARM_COPY","R_ARM_GLOB_DAT","R_ARM_JUMP_SLOT",
    "R_ARM_RELATIVE","R_ARM_GOTOFF32","R_ARM_BASE_PREL","R_ARM_GOT_BREL",
    "R_ARM_PLT32","R_ARM_CALL","R_ARM_JUMP24","R_ARM_THM_JUMP24",
    "R_ARM_BASE_ABS","R_ARM_ALU_PCREL_7_0","R_ARM_ALU_PCREL_15_8",
    "R_ARM_ALU_PCREL_23_15","R_ARM_LDR_SBREL_11_0_NC",
    "R_ARM_ALU_SBREL_19_12_NC","R_ARM_ALU_SBREL_27_20_CK",
    "R_ARM_TARGET1","R_ARM_SBREL31","R_ARM_V4BX","R_ARM_TARGET2",
    "R_ARM_PREL31","R_ARM_MOVW_ABS_NC","R_ARM_MOVT_ABS",
    "R_ARM_MOVW_PREL_NC","R_ARM_MOVT_PREL","R_ARM_THM_MOVW_ABS_NC",
    "R_ARM_THM_MOVT_ABS","R_ARM_THM_MOVW_PREL_NC","R_ARM_THM_MOVT_PREL",
    "R_ARM_THM_JUMP19","R_ARM_THM_JUMP6","R_ARM_THM_ALU_PREL_11_0",
    "R_ARM_THM_PC12","R_ARM_ABS32_NOI","R_ARM_REL32_NOI","R_ARM_ALU_PC_G0_NC",
    "R_ARM_ALU_PC_G0","R_ARM_ALU_PC_G1_NC","R_ARM_ALU_PC_G1","R_ARM_ALU_PC_G2",
    "R_ARM_LDR_PC_G1","R_ARM_LDR_PC_G2","R_ARM_LDRS_PC_G0","R_ARM_LDRS_PC_G1",
    "R_ARM_LDRS_PC_G2","R_ARM_LDC_PC_G0","R_ARM_LDC_PC_G1","R_ARM_LDC_PC_G2",
    "R_ARM_ALU_SB_G0_NC","R_ARM_ALU_SB_G0","R_ARM_ALU_SB_G1_NC","R_ARM_ALU_SB_G1",
    "R_ARM_ALU_SB_G2","R_ARM_LDR_SB_G0","R_ARM_LDR_SB_G1","R_ARM_LDR_SB_G2",
    "R_ARM_LDRS_SB_G0","R_ARM_LDRS_SB_G1","R_ARM_LDRS_SB_G2","R_ARM_LDC_SB_G0",
    "R_ARM_LDC_SB_G1","R_ARM_LDC_SB_G2","R_ARM_MOVW_BREL_NC","R_ARM_MOVT_BREL",
    "R_ARM_MOVW_BREL","R_ARM_THM_MOVW_BREL_NC","R_ARM_THM_MOVT_BREL",
    "R_ARM_THM_MOVW_BREL","R_ARM_TLS_GOTDESC","R_ARM_TLS_CALL","R_ARM_TLS_DESCSEQ",
    "R_ARM_THM_TLS_CALL","R_ARM_PLT32_ABS","R_ARM_GOT_ABS","R_ARM_GOT_PREL",
    "R_ARM_GOT_BREL12","R_ARM_GOTOFF12","R_ARM_GOTRELAX","R_ARM_GNU_VTENTRY",
    "R_ARM_GNU_VTINHERIT","R_ARM_THM_JUMP11","R_ARM_THM_JUMP8","R_ARM_TLS_GD32",
    "R_ARM_TLS_LDM32","R_ARM_TLS_LDO32","R_ARM_TLS_IE32","R_ARM_TLS_LE32",
    "R_ARM_TLS_LDO12","R_ARM_TLS_LE12","R_ARM_TLS_IE12GP","R_ARM_PRIVATE_0",
    "R_ARM_PRIVATE_1","R_ARM_PRIVATE_2","R_ARM_PRIVATE_3","R_ARM_PRIVATE_4",
    "R_ARM_PRIVATE_5","R_ARM_PRIVATE_6","R_ARM_PRIVATE_7","R_ARM_PRIVATE_8",
    "R_ARM_PRIVATE_9","R_ARM_PRIVATE_10","R_ARM_PRIVATE_11","R_ARM_PRIVATE_12",
    "R_ARM_PRIVATE_13","R_ARM_PRIVATE_14","R_ARM_PRIVATE_15","R_ARM_ME_TOO",
    "R_ARM_THM_TLS_DESCSEQ16","R_ARM_THM_TLS_DESCSEQ32"
};

#ifdef __cplusplus
}
#endif

#endif /* _RELOCATOR_H_ */
