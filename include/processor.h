#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
	
	
 public:
  double Utilization(); // TODO: See src/processor.cpp
	
	// TODO: Declare any necessary private members
 private:
 
  int oldUserVar{0};//normal processes executing in user mode
  int oldNiceVar{0};//niced processes executing in user mode
  int oldSystemVar{0};//processes executing in kernel mode
  int oldIdleVar{0};//twiddling thumbs
  int oldIrqVar{0};//servicing interrupts
  int oldSoftIrqVar{0};//servicing softirqs
  int oldStealVar{0};//involuntary wait
  int oldIOwaitVar{0};//In a word, iowait stands for waiting for I/O to complete
 
  
  
  vector<double> getVectorFromFile();
  

};

#endif