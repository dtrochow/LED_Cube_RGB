#include "mcp23017.h"
#include <stdio.h>
#include <string.h>

static mcpData_t mcpData[MAX_NUMBER_OF_MCP_MODULES];
static uint8_t mcpBuf[2];

void mcp_init(void)
{
    memcpy(mcpData, 0, sizeof(mcpData));
    memcpy(mcpBuf, 0, sizeof(mcpBuf));
}

void mcp_set_mode(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t config)
{
    mcpBuf[0] = port;
    mcpBuf[1] = config;
    switch(port)
    {
        case GPIOA:
            mcpData[GET_ORDER_NUMBER(address)].mode.portMode.porta = config;
            break;
        case GPIOB:
            mcpData[GET_ORDER_NUMBER(address)].mode.portMode.portb = config;
            break;
        default:
            break;
    }
    i2c_write_blocking(i2c, address, mcpBuf, 2, false);
}

void mcp_write(i2c_inst_t *i2c, uint8_t address, uint8_t port, uint8_t value, bool update)
{
    switch(port)
    {
        case GPIOA:
            mcpData[GET_ORDER_NUMBER(address)].state.portState.porta = value;
            break;
        case GPIOB:
            mcpData[GET_ORDER_NUMBER(address)].state.portState.portb = value;
            break;
        default:
            break;
    }
    if(update)
    {
        mcp_update_out_state_port(i2c, address, port);
    }
}

void mcp_write_single(i2c_inst_t *i2c, uint8_t address, uint8_t output, bool value, bool update)
{
    if(HIGH == value)
    {
        // Need to check the outputs order (which pin enable which output)
        // Output needs to be in range 1-16
        // mcpData[GET_ORDER_NUMBER(address)].state.all |= (1 << (16 -output));
        mcpData[GET_ORDER_NUMBER(address)].state.all |= (1 << output);
    }
    else if (LOW == value)
    {
        mcpData[GET_ORDER_NUMBER(address)].state.all &= ~(1 << output);
    }
    if(update)
    {
        mcp_update_out_state(i2c, address);
    }
}

uint8_t mcp_get_output_value_port(uint8_t address, uint8_t port)
{
    switch(port)
    {
        case GPIOA:
            return mcpData[GET_ORDER_NUMBER(address)].state.portState.porta;
            break;
        case GPIOB:
            return mcpData[GET_ORDER_NUMBER(address)].state.portState.portb;
            break;
        default:
            return -1;
    }
}

uint16_t mcp_get_output_value(uint8_t address)
{
    return mcpData[GET_ORDER_NUMBER(address)].state.all;
}

void mcp_update_out_state_port(i2c_inst_t *i2c, uint8_t address, uint8_t port)
{
    mcpBuf[0] = port;
    mcpBuf[1] = mcpData[GET_ORDER_NUMBER(address)].state.portState.porta;
    i2c_write_blocking(i2c, address, mcpBuf, 2, false);
}

void mcp_update_out_state(i2c_inst_t *i2c, int8_t address)
{
    mcp_update_out_state_port(i2c, address, GPIOA);
    mcp_update_out_state_port(i2c, address, GPIOB);
}

void mcp_update_out_state_all(i2c_inst_t *i2c)
{
    for(int i = 0; i < MAX_NUMBER_OF_MCP_MODULES; i ++)
    {
        mcp_update_out_state(i2c, GET_ADDR_FROM_NO(i));
    }
}
