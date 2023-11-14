
#include <XViewer.h>

/*
[API]        POST /postSetRtc HTTP/1.1
[API]        Incoming request: postSetRtc
[API]        BODY: {"epoch": 1690279548}
*/

String XViewer::updateRtc(uint32_t epoch)
{
    set_time(epoch);
    Serial.println("Epoch setted: " + String(epoch));

    return getCurrentTime();
}

String XViewer::getCurrentTime()
{
    tm t;
    _rtc_localtime(time(NULL), &t, RTC_FULL_LEAP_YEAR_SUPPORT);

    t.tm_mon++;                                // zero base
    uint16_t tmpYear = t.tm_year - 100 + 2000; // don't kwon why, it's 100 + last 2 digit of current year, now YYYY

    String day = (t.tm_mday < 10) ? "0" + String(t.tm_mday) : String(t.tm_mday);
    String mon = (t.tm_mon < 10) ? "0" + String(t.tm_mon) : String(t.tm_mon);
    String year = String(tmpYear);

    String hour = (t.tm_hour < 10) ? "0" + String(t.tm_hour) : String(t.tm_hour);
    String min = (t.tm_min < 10) ? "0" + String(t.tm_min) : String(t.tm_min);
    String sec = (t.tm_sec < 10) ? "0" + String(t.tm_sec) : String(t.tm_sec);

    if (year.compareTo("1970") == 0)
    {
        Serial.println("RTC not updated.");
        return "RTC not updated.";
    }

    char buffer[25];
    sprintf(buffer, "%2s/%2s/%4s  %2s:%2s:%2s", day.c_str(), mon.c_str(), year.c_str(), hour.c_str(), min.c_str(), sec.c_str());
    Serial.println("Formatted time: " + String(buffer));

    return String(buffer);
}
