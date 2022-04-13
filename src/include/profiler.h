/*************************************************************************
 * Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
 *
 * See LICENSE.txt for license information
 ************************************************************************/

#ifndef NCCL_PROFILER_H_
#define NCCL_PROFILER_H_

#include "proxy.h"

enum ncclProxyProfileState {
  ncclProxyProfileBegin = 0,

  ncclProxyProfileSendGPUWait = 1,
  ncclProxyProfileSendWait = 2,

  ncclProxyProfileRecvWait = 1,
  ncclProxyProfileRecvFlushWait = 2,
  ncclProxyProfileRecvGPUWait = 3,

  ncclProxyProfileEnd = 4,

  ncclProxyProfileSleep = 8,
  ncclProxyProfileWakeup = 9,

  ncclProxyProfileIdle = 16,
  ncclProxyProfileActive = 17,

  ncclProxyProfileAppend = 24,
  ncclProxyProfileAppendEnd = 25
};

void ncclProfilingRecord_(struct ncclProxyArgs* args, int sub, int step, int state);
void ncclProfilingDump();

#if USDT
#include <sys/sdt.h>
#define USDT_PROV(s) #s
#define USDT_PROBE0(name) DTRACE_PROBE(USDT_PROV(nccl), name)
#define USDT_PROBE1(name, a1) DTRACE_PROBE1(USDT_PROV(nccl), name, a1)
#define USDT_PROBE2(name, a1, a2) DTRACE_PROBE2(USDT_PROV(nccl), name, a1, a2)
#define USDT_PROBE6(name, a1, a2, a3, a4, a5, a6) DTRACE_PROBE6(USDT_PROV(nccl), name, a1, a2, a3, a4, a5, a6)
#define ncclProfilingRecord(args, sub, step, state) \
  do { \
    if ((state) == ncclProxyProfileAppendEnd) { \
      USDT_PROBE1(ncclProxyProfileAppendEnd, (args)->opCount); \
    } else if ((state) == ncclProxyProfileBegin) { \
      USDT_PROBE6(ncclProxyProfileBegin, (sub), (step), (args)->subs[sub].channelId, (args)->pattern, (args)->subs[sub].peer, (args)->opCount); \
    } else if ((state) <= ncclProxyProfileEnd) { \
      USDT_PROBE2(state, (sub), (step)); \
    } else { \
      USDT_PROBE0(state); \
    } \
    ncclProfilingRecord_((args), (sub), (step), (state)); \
  } while (0)
#else
#define USDT_PROBE0(name)
#define USDT_PROBE1(name, a1)
#define USDT_PROBE2(name, a1, a2)
#define USDT_PROBE6(name, a1, a2, a3, a4, a5, a6)
#define ncclProfilingRecord ncclProfilingRecord_
#endif

#endif
