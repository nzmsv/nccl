/*************************************************************************
 * Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
 *
 * See LICENSE.txt for license information
 ************************************************************************/

#ifndef NCCL_PROFILER_H_
#define NCCL_PROFILER_H_

#include "proxy.h"

enum ncclProxyProfileState {
  ncclProxyProfileSendBegin = 0,
  ncclProxyProfileSendGPUWait = 1,
  ncclProxyProfileSendWait = 2,
  ncclProxyProfileSendEnd = 3,

  ncclProxyProfileRecvBegin = 8,
  ncclProxyProfileRecvWait = 9,
  ncclProxyProfileRecvFlushWait = 10,
  ncclProxyProfileRecvGPUWait = 11,
  ncclProxyProfileRecvEnd = 12,

  ncclProxyProfileSleep = 16,
  ncclProxyProfileWakeup = 17,

  ncclProxyProfileIdle = 24,
  ncclProxyProfileActive = 25,

  ncclProxyProfileAppend = 32,
  ncclProxyProfileAppendEnd = 33
};

ncclResult_t ncclProfilingRecord(struct ncclProxyArgs* args, int sub, int step, enum ncclProxyProfileState state);

void ncclProfilingDump();

#endif
