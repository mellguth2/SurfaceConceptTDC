#include "StatPipe.h"
#include "scTDC.h"
#include "string.h"

StatPipe::StatPipe() 
  : pipe_desc_(-1), 
    dev_desc_(-1)
{
  sbuf_.reset(new ::statistics_t);
  ZeroStats();  
}

void StatPipe::ZeroStats() {
  ::statistics_t& s = (*sbuf_);
  for (unsigned i = 0; i < 4; i++) {
    s.events_found[i] = 0;
    s.events_in_roi[i] = 0;
    s.events_received[i] = 0;
    for (unsigned j = 0; j < 16; j++) {
      s.counters[i][j] = 0;
      s.counts_read[i][j] = 0;
      s.counts_received[i][j] = 0;
    }
  }
}

int StatPipe::Register(int dev_desc) {
  dev_desc_ = dev_desc;
  sc_pipe_statistics_params_t params;
  params.allocator_owner = this;
  params.allocator_cb = StatPipe::static_allocator_cb;
  pipe_desc_ = ::sc_pipe_open2(dev_desc, STATISTICS, &params);
  return pipe_desc_;
}

int StatPipe::Unregister() {
  int retcode = 0;
  if (dev_desc_ >= 0 && pipe_desc_ >= 0) {
     retcode = ::sc_pipe_close2(dev_desc_, pipe_desc_);
     if (retcode==0) {
       dev_desc_ = -1;
       pipe_desc_ = -1;
     }
  }
  return retcode;
}

int StatPipe::static_allocator_cb(void* priv, void** bufptr)
{
  return reinterpret_cast<StatPipe*>(priv)->allocator_cb(bufptr);
}

int StatPipe::allocator_cb(void** bufptr) 
{
  // scTDC is asking us for a buffer to place the statistics data into.
  // We always return our one and only buffer for statistics which is
  // held in sbuf_ (this buffer automatically gets deleted when this
  // object ceases to exist, thanks to unique_ptr)
  *bufptr = sbuf_.get();
  return 0; // 0 tells the scTDC that we do have a buffer available (?)
}

void StatPipe::copyTo(::statistics_t* target)
{
  ::statistics_t& t = *target;
  ::statistics_t& s = (*sbuf_);
  for (unsigned i = 0; i < 4; i++) {
    t.events_found[i] = s.events_found[i];
    t.events_in_roi[i] = s.events_in_roi[i];
    t.events_received[i] = s.events_received[i];
    for (unsigned j = 0; j < 16; j++) {
      t.counters[i][j] = s.counters[i][j];
      t.counts_read[i][j] = s.counts_read[i][j];
      t.counts_received[i][j] = s.counts_received[i][j];
    }
  }  
}

void StatPipe::GetStatistics(::statistics_t* target)
{
  copyTo(target);
}