#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <string>
using namespace std;

class TimeUtils {
public:
    // Convert time string "HH:MM" to minutes since midnight
    static int timeToMinutes(const string& time) {
        int hours = (time[0] - '0') * 10 + (time[1] - '0');
        int minutes = (time[3] - '0') * 10 + (time[4] - '0');
        return hours * 60 + minutes;
    }
    
    // Convert date string "DD/MM/YYYY" to days since epoch
    static int dateToDays(const string& date) {
        int day = (date[0] - '0') * 10 + (date[1] - '0');
        int month = (date[3] - '0') * 10 + (date[4] - '0');
        int year = ((date[6] - '0') * 1000 + (date[7] - '0') * 100 + 
                    (date[8] - '0') * 10 + (date[9] - '0'));
        
        // Simple approximation: assume 30 days per month
        return year * 365 + month * 30 + day;
    }

    // Convert a date and time to absolute minutes (rough epoch)
    static int toAbsoluteMinutes(const string& date, const string& time) {
        return dateToDays(date) * 24 * 60 + timeToMinutes(time);
    }
    
    // Calculate time difference in minutes (handles day overflow)
    static int timeDifference(const string& date1, const string& time1,
                             const string& date2, const string& time2) {
        int days1 = dateToDays(date1);
        int days2 = dateToDays(date2);
        int mins1 = timeToMinutes(time1);
        int mins2 = timeToMinutes(time2);
        
        return (days2 - days1) * 24 * 60 + (mins2 - mins1);
    }
    
    // Check if connection is feasible (minimum 1 hour transfer time)
    static bool isConnectionFeasible(const string& arrDate, const string& arrTime,
                                     const string& depDate, const string& depTime,
                                     int minLayoverMinutes = 60) {
        int diff = timeDifference(arrDate, arrTime, depDate, depTime);
        return diff >= minLayoverMinutes;
    }

    // Compute arrival absolute minutes, adding 24h if arrival clock is earlier than departure clock.
    static int absoluteArrivalMinutes(const string& depDate, const string& depTime,
                                      const string& arrDate, const string& arrTime) {
        int depAbs = toAbsoluteMinutes(depDate, depTime);
        int arrAbs = toAbsoluteMinutes(arrDate, arrTime);
        if (arrAbs < depAbs) {
            // crossed midnight (or date omission), roll to next day
            arrAbs += 24 * 60;
        }
        return arrAbs;
    }
    
    // Convert minutes to "HH:MM" format
    static string minutesToTime(int minutes) {
        int hours = (minutes / 60) % 24;
        int mins = minutes % 60;
        string result = "";
        if (hours < 10) result += "0";
        result += to_string(hours) + ":";
        if (mins < 10) result += "0";
        result += to_string(mins);
        return result;
    }
    
    // Format duration in hours and minutes
    static string formatDuration(int minutes) {
        int hours = minutes / 60;
        int mins = minutes % 60;
        return to_string(hours) + "h " + to_string(mins) + "m";
    }
};

#endif