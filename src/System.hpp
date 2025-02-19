#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>

extern std::string img_saves_path;

#if defined(_WIN32) || defined(_WIN64)
#define LOCALTIME_S(tm_ptr, time_ptr) localtime_s((tm_ptr), (time_ptr))
#else  // For Linux
#define LOCALTIME_S(tm_ptr, time_ptr) localtime_r((time_ptr), (tm_ptr))
#endif


#endif // SYSTEM_HPP