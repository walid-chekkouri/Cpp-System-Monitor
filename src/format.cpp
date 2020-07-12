#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
  long currentResult = 0;
  string elapsedTime {"00:00:00"};// HH:MM:SS format
 
  if (seconds > 0) 
  {
    // Hours
    currentResult = seconds / secondsInAnHour;
    elapsedTime = TimeToStringConverter(currentResult) + ":";
	
    // Minutes
    currentResult = (seconds / minutesInAnHour) % minutesInAnHour;
    elapsedTime += TimeToStringConverter(currentResult) + ":";
	
    //Seconds
    currentResult = seconds %  secondesInAnHour;
   elapsedTime += TimeToStringConverter(currentResult);
  }

  return elapsedTime;

 }
 
 std::string Format::TimeToStringConverter(long time) 
 {
  if (time < 10)
    return "0" + std::to_string(time);
  else
    return std::to_string(time);
return (time < 10) ? "0" + std::to_string(time) : std::to_string(time);

}