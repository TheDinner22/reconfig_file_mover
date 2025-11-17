// Greg Stitt
// University of Florida

#ifndef _DRAM_TEST_H_
#define _DRAM_TEST_H_

#include "App.h"

#define ADDR_WIDTH 15
#define RAM_WORDS (1 << ADDR_WIDTH)

#define MODE_ADDR ((1<<MMAP_ADDR_WIDTH)-1)

#define MEM_IN_ADDR 0
#define MEM_OUT_ADDR 0
#define GO_ADDR 1
#define RAM0_ADDR_ADDR 2
#define RAM1_ADDR_ADDR 3
#define SIZE_ADDR 4
#define DONE_ADDR 5

#define DMA_RD_COUNT_ADDR 6
#define DMA_RD_START_ADDR_ADDR 7
#define DMA_RD_ADDR_ADDR 8
#define DMA_RD_SIZE_ADDR 9
#define DMA_RD_PROG_FULL_ADDR 10
#define DMA_RD_EMPTY_ADDR 11

#define NUM_RAND_TESTS 500

class DramTest : public App {

 public:  

  typedef unsigned short appWord_t;

  DramTest(Board &board);
  ~DramTest();

  void waitUntilDone(float timeout);
  bool start(unsigned int input, unsigned int addr);

  static const unsigned int MAX_SIZE = RAM_WORDS*sizeof(boardWord_t)/sizeof(appWord_t);

};

#endif
