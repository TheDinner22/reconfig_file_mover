// Greg Stitt
// Wes Piard
// University of Florida
// main.cpp
//

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "Board.h"

using namespace std;

unsigned is_perfect_square(unsigned n) {
  unsigned k = 1;
  unsigned square = 0;

  while (square < n) {
    square = k * k;
    k++;
  }

  return (square == n ? 1 : 0);
}

enum mmapAddr {
  GO_ADDR = 0,
  N_ADDR,
  OUTPUT_ADDR,
  DONE_ADDR
};

int main(int argc, char *argv[]) {

  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " bitfile" << endl;
    return -1;
  }

  vector<float> clocks(Board::NUM_FPGA_CLOCKS);
  clocks[0] = 100.0;
  clocks[1] = 100.0;
  clocks[2] = 100.0;
  clocks[3] = 100.0;

  cout << "Programming FPGA...." << endl;

  // initialize board
  Board *board;
  try {
    board = new Board(argv[1], clocks);
  } catch (...) {
    exit(-1);
  }

  // TODO: Create a loop that tests the software and hardware `is_perfect_square`
  for (unsigned i = 0; i < 50; i++) {

    // Use the output format specified in the lab instructions.
    //
    // To check for the done signal, do not use your own for/while loop.
    // Instead, use the board->waitUntilNotZero function.
    //
    // e.g. board->waitUntilNotZero(DONE_ADDR, 2.0);
    //
    // This function takes an MMIO address and a timeout value (in seconds).
    // The function will continually read from the specified address,
    // until the returned value is non-zero, or until the timeout occurs.
    // If the timeout occurs, the function throws a TimeoutException.
  }

  return 0;
}
