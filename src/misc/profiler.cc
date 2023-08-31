#include "profiler.h"

#include <sys/sdt.h>
#define USDT_PROBE12(name, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
	DTRACE_PROBE12(nccl, name, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)

ncclResult_t ncclProfilingRecord(struct ncclProxyArgs* args, int sub, int step, enum ncclProxyProfileState state) {
  if (state <= ncclProxyProfileSendEnd) {
  //if (state <= ncclProxyProfileRecvEnd) {
    USDT_PROBE12(sendrecv,
      state,
      args->comm,
      args->coll - NCCL_NUM_FUNCTIONS,
      args->opCount,
      args->subs[sub].peer,
      step,
      args->subs[sub].channelId,
      args->sliceSteps,
      args->chunkSteps,
      args->chunkSize,
      args->subs[sub].nsteps,
      args->subs[sub].nbytes);
  }
  return ncclSuccess;
}

void ncclProfilingDump() {}
