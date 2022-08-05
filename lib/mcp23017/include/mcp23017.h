#include "hardware/i2c.h"

#define GET_ORDER_NUMBER(ADDR)   ((int)ADDR - 32)
#define GET_ADDR_FROM_NO(NUMBER) ((int)NUMBER + 32)

#define IODIRA                    0x00
#define IODIRB                    0x01
#define GPIOA                     0x12
#define GPIOB                     0x13

#define IPOLA                     0x02
#define IPOLB                     0x03
#define GPINTENA                  0x04
#define GPINTENB                  0x05
#define DEFVALA                   0x06
#define DEFVALB                   0x07
#define INTCONA                   0x08
#define INTCONB                   0x09
#define IOCON1                    0x0A
#define IOCON2                    0x0B
#define GPPUA                     0x0C
#define GPPUB                     0x0D
#define INTFA                     0x0E
#define INTFB                     0x0F
#define INTCAPA                   0x10
#define INTCAPB                   0x11
#define OLATA                     0x14
#define OLATB                     0x15

#define MAX_NUMBER_OF_MCP_MODULES (8)


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADDRESS1 = 0x20,
    ADDRESS2 = 0x21,
    ADDRESS3 = 0x22,
    ADDRESS4 = 0x23,
    ADDRESS5 = 0x24,
    ADDRESS6 = 0x25,
    ADDRESS7 = 0x26,
    ALL_ADDR = 0xFF // Not defined in datasheet
} mcpAddress_e;

typedef enum {
    MCP_DISABLED = 0,
    MCP_ENABLED
} mcpStatus_e;

typedef enum {
    OUTPUT_MODE = 0,
    INPUT_MODE  = 1
} mcpIOMode_e;

typedef enum {
    LOW  = 0,
    HIGH = 1
} mcpIOState_e;

typedef struct {
    uint8_t porta;
    uint8_t portb;
} ports_t;

union IOStates_u {
    ports_t portState;
    uint16_t all;
};

union IOMode_u {
    ports_t portMode;
    uint16_t all;
};

typedef struct {
    union IOStates_u state;
    union IOMode_u mode;
    mcpStatus_e status;
} mcpData_t;

/**
 * @brief
 *
 */
void mcp_init(void);

/**
 * @brief
 *
 * @param address
 */
void mcp_enable(mcpAddress_e address);

/**
 * @brief
 *
 * @param address
 */
void mcp_disable(mcpAddress_e address);

/**
 * @brief
 *
 * @param i2c
 * @param port
 * @param config
 */
void mcp_set_mode(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t config);

/**
 * @brief
 *
 * @param i2c
 * @param port
 * @param value
 */
void mcp_write(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t value, bool update);

/**
 * @brief
 *
 * @param i2c
 * @param address
 * @param output
 * @param value
 * @param update
 */
void mcp_write_single(i2c_inst_t *i2c, uint8_t address, uint8_t output, bool value, bool update);

/**
 * @brief
 *
 * @param address
 * @param port
 * @return uint8_t
 */
uint8_t mcp_get_output_value_port(uint8_t address, uint8_t port);

/**
 * @brief
 *
 * @param address
 * @return uint16_t
 */
uint16_t mcp_get_output_value(uint8_t address);

/**
 * @brief
 *
 * @param i2c
 * @param address
 * @param port
 */
void mcp_update_out_state_port(i2c_inst_t *i2c, uint8_t address, uint8_t port);

/**
 * @brief
 *
 * @param i2c
 * @param address
 */
void mcp_update_out_state(i2c_inst_t *i2c, int8_t address);

/**
 * @brief
 *
 * @param i2c
 */
void mcp_update_out_state_all(i2c_inst_t *i2c);

// Declarations of this file
#ifdef __cplusplus
}
#endif
