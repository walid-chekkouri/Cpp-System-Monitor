#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
	
std::string TimeToStringConverter(long time);
std::string ElapsedTime(long times);  // TODO: See src/format.cpp

const int secondsInAnHour = 3600;
const int minutesInAnHour = 60;
const int secondesInAnHour = 60;

};                                    // namespace Format

#endif