#include "m10s.h"

#include <stdio.h>
#include <string.h>

#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

// I2C address
static const uint8_t GPS_ADDR = 0x0F;

// Registers
static const uint8_t REG_NUM_BYTES_MSB = 0xFD;
static const uint8_t REG_NUM_BYTES_LSB = 0xFE;
static const uint8_t REG_DATA = 0xFF;

void ubx_inf_debug() {
    const uint8_t sync1 = 0xB5;
    const uint8_t sync2 = 0x62;
    const uint8_t msg_class = 0x04;
    const uint8_t id = 0x04;
    const char *payload = "hey";
    uint8_t CK_A = 0;
    uint8_t CK_B = 0;

    for (int i = 0; i < 10; i++) {
        CK_A = CK_A + payload[i];
        CK_B = CK_B + CK_A;
    }

    const uint8_t message[9] = {sync1,      sync2,      msg_class,
                                id,         payload[0], payload[1],
                                payload[2], CK_A,       CK_B};

    i2c_write_blocking(i2c, GPS_ADDR, message, 9, false);
}

void ubx_cfg_prt() {
    const uint8_t sync1 = 0xB5;
    const uint8_t sync2 = 0x62;
    const uint8_t msg_class = 0x06;
    const uint8_t id = 0x00;
    const uint8_t len = 0x01;
    const uint8_t payload = 0x00;
    uint8_t CK_A = 0;
    uint8_t CK_B = 0;
    uint result1 = 0;
    uint result2 = 0;
    uint8_t rx_msg = 0x00;

    const uint8_t scratch[8] = {sync1, sync2,   msg_class, id,
                                len,   payload, CK_A,      CK_B};

    for (int i = 2; i < 6; i++) {
        CK_A = CK_A + scratch[i];
        CK_B = CK_B + CK_A;
    }

    const uint8_t message[8] = {sync1, sync2,   msg_class, id,
                                len,   payload, CK_A,      CK_B};

    printf("Writing UBX-CFG-PRT Message...\n");

    i2c_write_blocking(i2c, GPS_ADDR, message, 8, false);

    sleep_ms(1500);

    // i2c_write_blocking(i2c, GPS_ADDR, &REG_NUM_BYTES_MSB, 1, true);

    printf("received msg:");

    for (int i = 0; i < 20; i++) {
        result2 = i2c_read_blocking(i2c, GPS_ADDR, &rx_msg, 1, false);
        if (result2 == PICO_ERROR_GENERIC)
            printf("\ni2c error occurred %x\n\n", result2);
        else
            printf(" %x", rx_msg);
    }

    printf("\n\n");
}

void ubx_cfg_cfg() {
    const uint8_t sync1 = 0xB5;
    const uint8_t sync2 = 0x62;
    const uint8_t msg_class = 0x06;
    const uint8_t id = 0x09;
    const uint8_t len = 0x0C;
    const uint8_t payload[12] = {0x00, 0x01, 0xF1, 0xFF, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x01, 0xF1, 0xFF};
    uint8_t CK_A = 0;
    uint8_t CK_B = 0;
    uint result1 = 0;
    uint result2 = 0;
    uint8_t rx_msg = 0x00;

    const uint8_t scratch[19] = {
        sync1,       sync2,       msg_class,  id,         len,
        payload[0],  payload[1],  payload[2], payload[3], payload[4],
        payload[5],  payload[6],  payload[7], payload[8], payload[9],
        payload[10], payload[11], CK_A,       CK_B};

    for (int i = 2; i < 6; i++) {
        CK_A = CK_A + scratch[i];
        CK_B = CK_B + CK_A;
    }

    const uint8_t message[21] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF,
                                 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0xFF, 0xFF, 0x00, 0x00, 0x17, 0x2F, 0xAE};

    printf("Writing UBX-CFG-CFG Message...\n");

    i2c_write_blocking(i2c, GPS_ADDR, message, 21, false);

    sleep_ms(1500);

    // i2c_write_blocking(i2c, GPS_ADDR, &REG_NUM_BYTES_MSB, 1, true);

    printf("received msg:");

    for (int i = 0; i < 20; i++) {
        result2 = i2c_read_blocking(i2c, GPS_ADDR, &rx_msg, 1, false);
        if (result2 == PICO_ERROR_GENERIC)
            printf("\ni2c error occurred %x\n\n", result2);
        else
            printf(" %x", rx_msg);
    }

    printf("\n\n");
}

void ubx_cfg_dat() {
    uint result1 = 0;
    uint result2 = 0;
    uint8_t rx_msg = 0x00;

    const uint8_t message[10] = {0xB5, 0x62, 0x06, 0x06, 0x02,
                                 0x00, 0x00, 0x00, 0x0E, 0x4A};

    printf("Writing UBX-CFG-DAT Message...\n");

    i2c_write_blocking(i2c, GPS_ADDR, message, 10, false);

    sleep_ms(1500);

    printf("received msg:");

    for (int i = 0; i < 20; i++) {
        result2 = i2c_read_blocking(i2c, GPS_ADDR, &rx_msg, 1, false);
        if (result2 == PICO_ERROR_GENERIC)
            printf("\ni2c error occurred %x\n\n", result2);
        else
            printf(" %x", rx_msg);
    }

    printf("\n\n");
}