#ifndef __STATPIPE_HPP__
#define __STATPIPE_HPP__

#include "scTDC_types.h"
#include <memory>

class StatPipe {
  int pipe_desc_;
  int dev_desc_;
  std::unique_ptr<::statistics_t> sbuf_;

  void copyTo(::statistics_t* target);
  static int static_allocator_cb(void*, void**);
  int allocator_cb(void**);

public:
  StatPipe();
  int Register(int dev_desc); // call this after sc_tdc_init_inifile(...)
  int Unregister(); // optional (or let sc_tdc_deinit2 do the cleanup)
  void ZeroStats();
  void GetStatistics(::statistics_t* target);
};


#endif