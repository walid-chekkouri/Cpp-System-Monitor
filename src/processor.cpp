#include "processor.h"
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"


using std::string;
using std::vector;
using std::ifstream;
using std::getline;
using std::istringstream;


/*
Calculation formulas can found here:
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
*/
double Processor::Utilization() 
{
  vector<double> processorStats = getVectorFromFile();
  
  double userVar = processorStats[0];//normal processes executing in user mode
  double niceVar= processorStats[1];//niced processes executing in user mode
  double systemVar = processorStats[2];//processes executing in kernel mode
  double idleVar = processorStats[3];//twiddling thumbs
  
  double irqVar = processorStats[5];//servicing interrupts
  double softIrqVar =processorStats[6];//servicing softirqs
  double stealVar = processorStats[7];//involuntary wait
  double ioWaitVar= processorStats[4];//In a word, iowait stands for waiting for I/O to complete
  
 
/*
Example of display:

 user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0

*/

  double oldIdle = oldIdleVar + oldIOwaitVar;
  double idle = idleVar + ioWaitVar;

 
  double oldNotIdle = oldUserVar + oldNiceVar + oldSystemVar + oldIrqVar + oldSoftIrqVar + oldStealVar;
  double notIdle = userVar + niceVar + systemVar + irqVar + softIrqVar + stealVar;
 
 
  double oldTotal = oldIdleVar +oldNotIdle;
  double total = idleVar + notIdle;//Total = Idle + NonIdle
  
 
  

  //differentiate: actual value minus the previous one
  double totalDelta {1};
  totalDelta =  total - oldTotal;
  double idleDelta = idle - oldIdle;
  
  //Calculating Percentage
  double utilizationP = (totalDelta - idleDelta) / totalDelta;
  
  
  //Setting the new stats:
  oldUserVar = processorStats[0];
  
  oldNiceVar = processorStats[1];
  
  oldSystemVar = processorStats[2];
  
  oldIdleVar =processorStats[3];
  
  oldIrqVar = processorStats[5];
  oldSoftIrqVar = processorStats[6];
  
  oldStealVar =processorStats[7];
  
  oldIOwaitVar = processorStats[4];
  
  //final value is returned
  return utilizationP;
}


vector<double> Processor::getVectorFromFile() 
{
	
	vector<double> cpuStats;
	double cpuValue;
	string line, filterKey;
  
 
  
  ifstream myFilestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  
  if (myFilestream) 
  {
	  
    while (getline(myFilestream, line)) 
	{
      istringstream ss(line);
      while (ss >> filterKey) 
	  {
        if (filterKey == LinuxParser::filterCpu) 
		{
          while (ss >> cpuValue) cpuStats.push_back(cpuValue);
          
        }
      }
    }
	
	
  }
  
  return cpuStats;
}