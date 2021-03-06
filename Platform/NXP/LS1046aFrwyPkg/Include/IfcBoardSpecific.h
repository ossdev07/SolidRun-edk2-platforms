/** IfcBoardSpecificLib.h

  IFC Flash Board Specific Macros and structure

  Copyright 2019 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#ifndef __IFC__BOARD_SPECIFIC_H__
#define __IFC__BOARD_SPECIFIC_H__

#include <Ifc.h>

// On board flash support
#define IFC_NAND_BUF_BASE    0x7E800000

// On board Inegrated flash Controller chip select configuration
#define IFC_NAND_CS   IFC_CS0

// On board NAND flash properties
#define NAND_PG_SZ        SIZE_4KB //NAND flash page size
#define NAND_SP_SZ        (BIT8 - BIT5) //NAND flash spare area 224 Bytes
#define NAND_BK_SZ        SIZE_256KB //NAND flash block size
#define NAND_LAST_BLOCK   ((SIZE_1KB / BIT1) - BIT0) //Block Count 512

// board-specific NAND timing
#define NAND_FTIM0    (IFC_FTIM0_NAND_TCCST(0x7) | \
                      IFC_FTIM0_NAND_TWP(0x18)   | \
                      IFC_FTIM0_NAND_TWCHT(0x7) | \
                      IFC_FTIM0_NAND_TWH(0xa))

#define NAND_FTIM1    (IFC_FTIM1_NAND_TADLE(0x32) | \
                      IFC_FTIM1_NAND_TWBE(0x39)  | \
                      IFC_FTIM1_NAND_TRR(0xe)   | \
                      IFC_FTIM1_NAND_TRP(0x18))

#define NAND_FTIM2    (IFC_FTIM2_NAND_TRAD(0xf) | \
                      IFC_FTIM2_NAND_TREH(0xa) | \
                      IFC_FTIM2_NAND_TWHRE(0x1e))

#define NAND_FTIM3    0x0

#define NAND_CSPR   (IFC_CSPR_PHYS_ADDR(IFC_NAND_BUF_BASE) \
                            | IFC_CSPR_PORT_SIZE_8 \
                            | IFC_CSPR_MSEL_NAND \
                            | IFC_CSPR_V)

#define NAND_CSPR_EXT   0x0
#define NAND_AMASK      0xFFFF0000

#define NAND_CSOR     (IFC_CSOR_NAND_ECC_ENC_EN /* ECC on encode */ \
                      | IFC_CSOR_NAND_ECC_DEC_EN /* ECC on decode */ \
                      | IFC_CSOR_NAND_ECC_MODE_8 /* 8-bit ECC */ \
                      | IFC_CSOR_NAND_RAL_3       /* RAL = 3 Bytes */ \
                      | IFC_CSOR_NAND_PGS_4K      /* Page Size = 4K */ \
                      | IFC_CSOR_NAND_SPRZ_224     /* Spare size = 224 */ \
                      | IFC_CSOR_NAND_PB(6))     /* 2^6 Pages Per Block */

#endif //__IFC__BOARD_SPECIFIC_H__
