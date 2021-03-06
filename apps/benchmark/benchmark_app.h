/*
 * Copyright (c) 2018, Torsten Hueter, Jens Dede, VIRTENIO GmbH.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the VIRTENIO GmbH nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE VIRTENIO GMBH BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 /*
  * Benchmark test for Preon32 (simplified variant)
  */

#ifndef __BENCHMARK_APP_H__
#define __BENCHMARK_APP_H__

#include "contiki.h"
#include <dev/watchdog.h>
#include <net/ip/uip.h>
#include <net/ipv6/uip-ds6.h>

#include <string.h>
#include <stdbool.h>

typedef struct statistic_t {
    uint32_t lastReceivedPacket;             ///< Last number of received packet (for statistics)
    uint16_t lostPackets;                    ///< Number of lost packets (for statistics)
    uint32_t firstReceivedPacket;            ///< First received packet (for statistics)
} statistic_t;

// Determining of the device role via pin
#define GPIO_ROOT GPIO_32

#define MAX_STATISTIC_DATA 20

/// Prints PRR statistic if set to 1, NOTE: only useful if all packets arrive ordered
#define PRINT_PRR                   1

/// Print some more messages for debugging
#define NODE_DEBUG                  1

/// The default pushing period in clock ticks
#define DEFAULT_PUSHING_PERIOD      (5 * CLOCK_SECOND)
/// Random offset for sending clock ticks
#define RANDOM_OFFSET               (5 * CLOCK_SECOND)
/// UDP port for measurements
#define MEAS_PORT                   1111
/// UDP port for control messages
#define CONTROL_PORT                1234
/// Maximum number of nodes for statistic
#define MAX_NODE_STATISTIC          20
/// The TX Power for the benchmark, use a value between [-17, 3]
#define BENCHMARK_TX_POWER          -8
/// Drop first packets from the statistic
#define STAT_MINIMUM_COUNT          5

// Restart interval [s] or 0 if no reboot, example: (60 * CLOCK_SECOND)
#define NODE_RESTART_INTERVAL  (0 * CLOCK_SECOND)
// Restart interval [s] or 0 if no reboot, example: (60 * CLOCK_SECOND)
#define ROOT_RESTART_INTERVAL  (0 * CLOCK_SECOND)

// Enables latency measurement using reply packets from the root
#define USE_LATENCY_MEASUREMENT 0

// Enable Adpative Scheduler
#define USE_ADAPTIVE 1

// Payload length: NOTE a minimum of 5 is required (type + counter).
// Use 74 for maximal length (TSCH + Encryption)
//#define PAYLOAD_LENGTH              5
#define PAYLOAD_LENGTH              74

// Trigger interval for blacklist distribution process
#define BLACKLIST_NOTIFY_INTERVAL   (5 * 60 * CLOCK_SECOND)

// Trigger interval for Adaptive Scheduling
#define ADAPTVIE_SCHEDULE_INTERVAL   (1 * 60 * CLOCK_SECOND)

// helper macros to compare addresses
// compare two ipv6 addresses
#define cmp__ip6addr(ipaddr, addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7) \
        (ipaddr.u16[0] == UIP_HTONS(addr0) && \
        ipaddr.u16[1] == UIP_HTONS(addr1) && \
        ipaddr.u16[2] == UIP_HTONS(addr2) && \
        ipaddr.u16[3] == UIP_HTONS(addr3) && \
        ipaddr.u16[4] == UIP_HTONS(addr4) && \
        ipaddr.u16[5] == UIP_HTONS(addr5) && \
        ipaddr.u16[6] == UIP_HTONS(addr6) && \
        ipaddr.u16[7] == UIP_HTONS(addr7))

#define cmp__lladdr(lladdr, addr0, addr1, addr2, addr3, addr4, addr5, addr6, addr7) \
        (lladdr.addr[0] == addr0 && \
        lladdr.addr[1] == addr1 && \
        lladdr.addr[2] == addr2 && \
        lladdr.addr[3] == addr3 && \
        lladdr.addr[4] == addr4 && \
        lladdr.addr[5] == addr5 && \
        lladdr.addr[6] == addr6 && \
        lladdr.addr[7] == addr7)

#define ip6addr(destAddr, ipAddr) uip_ip6addr(destAddr, ipAddr)
#define cmp_ip6addr(addr1, ...) cmp__ip6addr(addr1, __VA_ARGS__)
#define cmp_lladdr(addr1, ...) cmp__lladdr(addr1, __VA_ARGS__)

#define IP6ADDR_SET(addr, ...) uip_ip6addr(&addr, __VA_ARGS__)
#define IP6ADDR_LL_TO_IP6_(    prefix, \
                            ll0, ll1, ll2, ll3, ll4, ll5, ll6, ll7 )    \
                             prefix, 0, 0, 0,                           \
                             ( ( ( ll0 ^ 0x02 )  << 8) | ll1 ),         \
                             (   ( ll2 << 8 )          | ll3 ),         \
                             (   ( ll4 << 8 )          | ll5 ),         \
                             (   ( ll6 << 8 )          | ll7 )

#define IP6ADDR_LL_TO_IP6(     prefix0, ... )    IP6ADDR_LL_TO_IP6_(    prefix0, __VA_ARGS__ )

// Used prefix (Upper 16 Bit)
#define PREFIXA                0xAAAA
#define PREFIXB                0xFE80

#endif //__BENCHMARK_APP_H__
