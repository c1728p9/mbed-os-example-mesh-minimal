/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "rtos.h"
#include "NanostackInterface.h"
#include "socket_example.h"

#if MBED_CONF_APP_TRACE
#include "mbed-trace/mbed_trace.h"

void trace_printer(const char* str) {
    printf("%s\r\n", str);
}
#endif // MBED_CONF_APP_TRACE

#define SPI_MOSI   D11
#define SPI_MISO   D12
#define SPI_SCLK   D13
#define SPI_CS     D10
#define SPI_RST    D5
#define SPI_SLP    D7
#define SPI_IRQ    D9
#define I2C_SDA    D14
#define I2C_SCL    D15

#include "NanostackRfPhyAtmel.h"
NanostackRfPhyAtmel phy(SPI_MOSI, SPI_MISO, SPI_SCLK,
                        SPI_CS, SPI_RST, SPI_SLP, SPI_IRQ,
                        I2C_SDA, I2C_SCL);
LoWPANNDInterface mesh(&phy);
//ThreadInterface mesh;
Serial output(USBTX, USBRX);

int main()
{

#if MBED_CONF_APP_TRACE
	mbed_trace_init();
    mbed_trace_print_function_set(trace_printer);
#endif
    output.baud(115200);
    printf("\r\n\r\nConnecting...\r\n");

    if (mesh.connect()) {
        printf("Connection failed!\r\n");
        return -1;
    }

    printf("connected. IP = %s\r\n", mesh.get_ip_address());

    // Start socket example
    start_socket_example((NetworkInterface *)&mesh);
}
