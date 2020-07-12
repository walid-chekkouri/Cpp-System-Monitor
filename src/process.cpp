#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//Constructor
Process::Process(int pid)
 {
  this->pid = pid;
  this->userName = LinuxParser::User(pid);
  this->command = LinuxParser::Command(pid);
  this->ram = LinuxParser::Ram(pid);
  this->upTime = LinuxParser::UpTime(pid);
  this->cpuUtilization = LinuxParser::CpuUtilization(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization; }

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return userName; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {return upTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const
 { 
	return (cpuUtilization > a.cpuUtilization)? true:false;
 
 }
 
 