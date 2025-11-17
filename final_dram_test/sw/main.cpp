// Greg Stitt
// University of Florida
// main.cpp
//
// Description: This file is the software portion of the DRAM test application.

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cstdio>

#include <unistd.h>

#include "Board.h"
#include "Timer.h"
#include "DramTest.h"

using namespace std;


//#define DEBUG


void replaceMessage(string oldMsg, string newMsg) {

    for (unsigned j=0; j < oldMsg.size(); j++) {
      cout << "\b";
    }
    for (unsigned j=0; j < oldMsg.size(); j++) {
      cout << " ";
    }
    for (unsigned j=0; j < oldMsg.size(); j++) {
      cout << "\b";
    }
    
    cout << newMsg;
    fflush(stdout);
}


int main(int argc, char* argv[]) {

  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " bitfile" << endl;
    return -1;
  }

  // Set up clock frequencies.
  vector<float> clocks(Board::NUM_FPGA_CLOCKS);
  clocks[0] = 100.0;
  clocks[1] = 100.0;
  clocks[2] = 100.0;
  clocks[3] = 100.0;
  
  cout << "Programming FPGA....";
 
  // Initialize board
  Board *board;
  try {
    board = new Board(argv[1], clocks);
  }
  catch(...) {
    exit(-1);
  }

  cout << "SUCCESS" << endl;

  DramTest dramTest(*board);
  string msg;
  
  cout << "Testing transfers to/from address 0....";

  for (unsigned i=1; i*i <= DramTest::MAX_SIZE; i++) {

    stringstream msgStream;
    msgStream << "(Size = " << i*i << ")";
    string newMsg = msgStream.str();
    replaceMessage(msg, newMsg); 
    msg = newMsg;
    
    if (!dramTest.start(i*i, 0)) {
      cout << "ERROR: Failed test for size " << i*i << " and address " << 0 << endl;   
      return -1;
    }
  }
  
  replaceMessage(msg, "SUCCESS\n"); 
  cout << "Testing max transfer size....";

  if (!dramTest.start(DramTest::MAX_SIZE, 0)) {
      cout << "ERROR: Failed test for size " << DramTest::MAX_SIZE << " and address " << 0 << endl;   
      return -1;
  }

  msg = "";
  cout << "SUCCESS" << endl;
  cout << "Testing random sizes and addresses....";

  for (unsigned i=0; i < NUM_RAND_TESTS; i ++) {

    unsigned size = (rand() % DramTest::MAX_SIZE) + 1;
    unsigned addr = rand() % RAM_WORDS;

    // Cap the size so the transfer doesn't overflow memory.
    if (size/2 + size % 2 + addr > RAM_WORDS) {
      size = (RAM_WORDS-addr)*2;
    }

    stringstream msgStream;
    msgStream << "(Size = " << size << ", addr = " << addr << ")";
    string newMsg = msgStream.str();
    replaceMessage(msg, newMsg); 
    msg = newMsg;

    if (!dramTest.start(size, addr)) {
      cout << endl << "ERROR: Failed test for size " << size << " and address " << addr << endl;   
      return -1;
    }
  }

  replaceMessage(msg, "SUCCESS\n"); 
  delete board;
  return 0;
}


