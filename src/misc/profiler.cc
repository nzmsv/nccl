#include "profiler.h"

#include <sys/sdt.h>
#define USDT_PROBE1(name, a1) \
	DTRACE_PROBE1(nccl, name, a1)

struct nccl_trace_event {
	uint64_t comm;
	uint64_t opCount;
	uint8_t state;
	uint8_t coll;
	uint8_t step;
	uint8_t channelId;
	uint16_t sliceSteps;
	uint16_t chunkSteps;
	uint16_t chunkSize;
	uint16_t nsteps;
	uint32_t nbytes;
	int peer;
};

ncclResult_t ncclProfilingRecord(struct ncclProxyArgs* args, int sub, int step, enum ncclProxyProfileState state) {
  struct nccl_trace_event e = {};
  if (state <= ncclProxyProfileRecvEnd) {
    e.state = state;
    e.comm = args->comm;
    e.coll = args->coll - NCCL_NUM_FUNCTIONS;
    e.opCount = args->opCount;
    e.peer = args->subs[sub].peer;
    e.step = step;
    e.channelId = args->subs[sub].channelId;
    e.sliceSteps = args->sliceSteps;
    e.chunkSteps = args->chunkSteps;
    e.chunkSize = args->chunkSize;
    e.nsteps = args->subs[sub].nsteps;
    e.nbytes = args->subs[sub].nbytes;
    USDT_PROBE1(sendrecv, &e);
  }
  return ncclSuccess;
}

void ncclProfilingDump() {}
