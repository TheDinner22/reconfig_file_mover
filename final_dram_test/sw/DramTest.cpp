// Greg Stitt
// University of Florida

#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include "DramTest.h"
#include "Timer.h"

using namespace std;

DramTest::DramTest(Board &board) : App(board) {

}

DramTest::~DramTest() {
  
}

void DramTest::waitUntilDone(float timeout) {

  Timer waitTime;
  unsigned value = 0;
  waitTime.start();
  while (!value && waitTime.elapsedTime() < timeout) {
    this->read(&value, DONE_ADDR, 1);
  }
  waitTime.stop();

  if (value == 0) {
    throw TimeoutException();
  }
}

bool DramTest::start(unsigned int size, unsigned int addr) {
 
  // Make sure test doesn't exceed dram address space in memory map.
  assert(size/2 + addr <= RAM_WORDS);

  appWord_t go;
  //appWord_t *input, *output;  
  //input = (appWord_t *) safeMalloc(size*sizeof(appWord_t));
  //output = (appWord_t *) safeMalloc(size*sizeof(boardWord_t));

  boardWord_t *input, *output;
  input = (boardWord_t *) safeMalloc(RAM_WORDS*sizeof(boardWord_t));
  output = (boardWord_t *) safeMalloc(RAM_WORDS*sizeof(boardWord_t));
  assert(input != NULL);
  assert(output != NULL);

  // Initialize input and output arrays
  for (unsigned i=0; i < RAM_WORDS; i++) {
   
    input[i] = rand();
    //input[i] = i;
    output[i] = 0;    
  }

  // Disable user mode
  write(0, MODE_ADDR);

  // Transfer all inputs
  write(input, MEM_IN_ADDR, RAM_WORDS);
  
  // Enable user mode.
  write(1, MODE_ADDR);

  write(size, SIZE_ADDR); 
  write(addr, RAM0_ADDR_ADDR);

  // Always start writes at address 0 for now. There is a strange paging bug
  // that needs to be debugging to change this.
  write(0, RAM1_ADDR_ADDR); 
    
  // Assert go, cleared by memory map
  go = 1;
  write(go, GO_ADDR);
  
  // Wait for the board to assert done
  try {
    waitUntilDone(2.0);
  }
  catch(TimeoutException e) {
   
    cerr << "Error: Done was not asserted before timeout." << endl;

    // Print dma debugging information.
    unsigned int count, start_addr, dma_size, dma_addr, prog_full, empty;
    read(count, DMA_RD_COUNT_ADDR);
    read(start_addr, DMA_RD_START_ADDR_ADDR);
    read(dma_addr, DMA_RD_ADDR_ADDR);
    read(dma_size, DMA_RD_SIZE_ADDR);
    read(prog_full, DMA_RD_PROG_FULL_ADDR);
    read(empty, DMA_RD_EMPTY_ADDR);
    cout << "Count = " << count << endl;
    cout << "Start Addr = " << start_addr << endl;
    cout << "DMA Addr = " << dma_addr << endl;
    cout << "DMA Size = " << dma_size << endl;
    cout << "Prog full = " << prog_full << endl;
    cout << "DMA Empty = " << empty << endl;

    throw e;
  }

  // Disable user mode.
  write(0, MODE_ADDR);

  // read the outputs back from the FPGA
  read(output, MEM_OUT_ADDR, size/2 + size%2);

  bool result = true;
  for (unsigned i=0; i < size/2 + size%2; i++) {   
    if (input[i+addr] != output[i]) {
      cout << "ERROR: Input = " << input[i+addr] << " output = " << output[i] << endl;
      result = false;      
    }
  }

  delete[] input;
  delete[] output;
  return result;
}

