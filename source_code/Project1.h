// DESCRIPTION

#define REG_SIZE 1 //size of a register is 1 byte (8 bits)
#define NUM_CTRL_REGS 1 //number of control reg
#define NUM_STS_REGS 5 //number of status reg
#define NUM_DATA_REGS 256 //number of data reg
#define NUM_DEV_REGS (NUM_CTRL_REGS + NUM_STS_REGS + NUM_DATA_REGS) //total number of reg

/****************** STATUS REG: START ******************/
/*
 * [READ_COUNT_H_REG:READ_COUNT_L_REG]:
 * - Initial value: 0x0000
 * - Increase 1 once read successfully from data register
 */
#define READ_COUNT_H_REG 0
#define READ_COUNT_L_REG 1

/*
 * [WRITE_COUNT_H_REG:WRITE_COUNT_L_REG]:
 * - Initial value: 0x0000
 * - Increase 1 once write successfully into data register
 */
#define WRITE_COUNT_H_REG 2
#define WRITE_COUNT_L_REG 3

/*
 * DEVICE_STATUS_REG:
 * - Initial value: 0x03
 * - Bit meaninf:
 *   bit0:
 *       0: data reg is unready to read
 *       1: data reg is ready to read
 *   bit1:
 *       0: data reg is unready to write
 *       1: data reg is ready to write
 *   bit2:
 *       0: when data reg is released
 *       1: when all data reg is written
 *   bit3~7: unused
 */
#define DEVICE_STATUS_REG 4

#define STS_READ_ACCESS_BIT (1 << 0)
#define STS_WRITE_ACCESS_BIT (1 << 1)
#define STS_DATAREGS_OVERFLOW_BIT (1 << 2)

#define READY 1
#define NOT_READY 0
#define OVERFLOW 1
#define NOT_OVERFLOW 0
/****************** STATUS REG: END ******************/


/****************** CONTROL REG: START ******************/
/*
 * CONTROL_ACCESS_REG:
 * - Role: contains bits that control ability to read/write data reg
 * - Initial value: 0x03
 * - Meaning:
 *   bit0:
 *       0: not allowed to read from data reg
 *       1: allowed to read from data reg
 *   bit1:
 *       0: not allowed to write into data reg
 *       1: allowed to write into data reg
 *   bit2~7: unused
 */
#define CONTROL_ACCESS_REG 0

#define CTRL_READ_DATA_BIT (1 << 0)
#define CTRL_WRITE_DATA_BIT (1 << 1)

#define ENABLE 1
#define DISABLE 0
/****************** CONTROL REG: END ******************/
