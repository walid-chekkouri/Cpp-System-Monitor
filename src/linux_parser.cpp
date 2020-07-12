#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream> 

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stringstream;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value{};
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() 
{
  string os;
string  kernel{};
  string line;
  string kernelVersion{};
  
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel>>kernelVersion;
  }
  return kernelVersion;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{

 
 string line, trash, memTotal,memFree;
 int memTotalVal, memFreeVal;

std::ifstream in(kProcDirectory + kMeminfoFilename);
  if(!in) 
  {
     // cout << "Cannot open input file.\n";
      return -1;
  }

  while(getline(in, line))
  {
    stringstream ss(line);

    string stringTemp;
    int stringTempVal;
    string trash;

    while(ss>>stringTemp>>stringTempVal>>trash)
    {

      if(stringTemp==filterMemTotalString)
      {
      memTotalVal = stringTempVal;
      memTotal = stringTemp;
      }
      else if(stringTemp==filterMemFreeString)
      {
        memFreeVal = stringTempVal;
        memFree = stringTemp;

      }
      else
        break;



    }
  }

    // calculations: 
    /*https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290

    Total used memory = MemTotal - MemFree
Non cache/buffer memory (green) = Total used memory - (Buffers + Cached memory)
Buffers (blue) = Buffers
Cached memory (yellow) = Cached + SReclaimable - Shmem
Swap = SwapTotal - SwapFree
    */
    float total = memTotalVal;
    float free = memFreeVal;
    float used = total - free;
    float result =  used / total;

return result;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line;
  long upTime = 0;
  
  std::ifstream ss(kProcDirectory + "." + kUptimeFilename);
  
  if (ss) 
  {
    std::getline(ss, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }

  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  string line,filter, vuser, vnice, vsystem, vidle, viowait, virq, vsoftirq, vsteal, vguest,vguest_nice;
  vector<string> cpuStats{};
  
  std::ifstream fStream(kProcDirectory + kStatFilename);
  if (fStream) 
  {
    while (std::getline(fStream, line)) 
	{
      std::istringstream ss(line);
      while (ss >> filter >> vuser >> vnice >> vsystem >> vidle >>viowait >> virq >> vsoftirq >> vsteal >> vguest >> vguest_nice) 
	  {
        if (filter == filterCpu) 
		{
          cpuStats.push_back(vuser);
          cpuStats.push_back(vnice);
          cpuStats.push_back(vsystem);
          cpuStats.push_back(vidle);
          cpuStats.push_back(viowait);
          cpuStats.push_back(virq);
          cpuStats.push_back(vsoftirq);
          cpuStats.push_back(vsteal);
          cpuStats.push_back(vguest);
          cpuStats.push_back(vguest_nice);
		  
          return cpuStats;
        }
		else{return {};}
      }
    }
  }
  
  return {};
	
}


float LinuxParser::CpuUtilization(int pid) 
{
	/*
		Operations can be found here:
		https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
	*/
	float cpuUsagePercentage=0;
	//////////
    std::string line, trash;
	std::string path = "/proc/" + std::to_string(pid) + "/stat";
	
	long uTime, sTime, cUtime, csTime, startTime, seconds;
	const int hertz=100;
	
	std::ifstream in(path);
	
	if (in)
	{
		std::getline(in,line);
		std::istringstream ss(line);
		
		for (int i = 0; i < 13; i++)
			ss >> trash;
		
		ss >> uTime >> sTime >> cUtime >> csTime;
		
		for (int i = 0; i < 4; i++)
			ss >> trash;

		ss >> startTime;
		
		
		//First we determine the total time spent for the process
		long total_time  = uTime + sTime + cUtime + csTime;
		
		
		//The we calculate the seconds = uptime - (starttime / Hertz)
		seconds = LinuxParser::UpTime(pid) - (startTime)/hertz;
		
		if(seconds==0)
			return 0;
		
		//Finally we calculate the CPU usage percentage:
		cpuUsagePercentage = (float) (( (total_time) /hertz ) /seconds);
		return  cpuUsagePercentage;

	}
	return cpuUsagePercentage;


	
	
  
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string processesString, line;
  int totalProcesses;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream) 
  {
    while (std::getline(stream, line))
    {
      std::istringstream lineSs(line);
      lineSs >> processesString >> totalProcesses;
      if (processesString == filterProcesses) 
        return totalProcesses;
      
    }
  } 
  else 
    return 0;
  
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 

  string processesString, line;
  int processesRunning;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream) 
  {
    while (std::getline(stream, line))
    {
      std::istringstream lineSs(line);
      lineSs >> processesString >> processesRunning;
      if (processesString == filterRunningProcesses) 
        return processesRunning;
      
    }
  } 
  else 
    return 0;
  
  return 0; 
 
 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{

 string commandVal {}; string path = kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename;
  std::ifstream filestream(path);
  
  if (filestream) 
  {
    std::getline(filestream, commandVal);
    return commandVal;
  }
  else 
	return commandVal;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
//Linux stores memory utilization for the process in /proc/[pid]/status.
  string line, key, path, trash;
  string value {};
  path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
  
  std::ifstream filestream(path);
  if (filestream) 
  {
    while (std::getline(filestream, line)) 
	{
      std::istringstream linestream(line);
      while (linestream >> key >> value>> trash) 
	  {
        if (key == filterProcVmSize) 
          return value;

      }
    }
  }
  
  //convert from Kb to Mb
   double ramFloatVal = std::stod(value);
   ramFloatVal *= 0.001;
  
  std::ostringstream strs;
	strs << ramFloatVal;

	value = strs.str();
  
  return value;

}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  string line, key;
  string value {};
  string path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
  
  std::ifstream filestream(path);
  if (filestream) 
  {
    while (std::getline(filestream, line)) 
	{
      std::istringstream ss(line);
      while (ss >> key >> value) 
	  {
        if (key == filterUid) 
          return value;

      }
    }
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string uid = Uid(pid);
  string line,key, trash;
  string userName {};
  
 // /etc/passwd contains the information necessary to match the UID to a username.
  std::ifstream filestream(kPasswordPath);
  if (filestream) 
  {
    while (std::getline(filestream, line)) 
	{
      std::replace(line.begin(), line.end(), ':', ' ');//replacing ':' with an empty space
      std::istringstream ss(line);
	  
      while (ss >> userName >>  trash >> key) 
	  {
        if (key == uid) //match the UID to a username
          return userName;
      }
    }
  }
  return userName;
  
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  string line, value;
  long uptime {1};
  //Linux stores the command used to launch the function in the /proc/[pid]/cmdline file
  string path = kProcDirectory + "/" + std::to_string(pid) + kStatFilename;
  std::ifstream filestream(path);
  
  if (filestream) 
  {
    while (std::getline(filestream, line))
		{
      std::istringstream ss(line);
      for (int i = 1; i <= kStartTime; ++i) 
	  {
        ss >> value;
        if (i == kStartTime) 
		{

          //divide the "clock ticks" value by sysconf(_SC_CLK_TCK)
		  
		  //TODO: REMOVE THIS 
		if(_SC_CLK_TCK ==0)
			return 3;
		
            uptime = std::stol(value) / sysconf(_SC_CLK_TCK);
            return uptime;
         
        }
      }
    }
  }
  
  return UpTime() - uptime;

}