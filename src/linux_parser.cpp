#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }