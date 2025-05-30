#ifndef TIME_API_CALL_H
#define TIME_API_CALL_H

//these are all the time related constants
//this is the link to the public api
#define NTP_SERVER "time.google.com"
//this will define the timezone
//the offset is in seconds (ie if your Timezone is UTC+1 you need to put 3600 here)
// int utcOffset = 0;

//this will define whether or not there is daylight savings
//again this offset is in seconds. During the summer put 3600 here, and 0 in the winter
// int dstOffset = 0;


//here are all the functions declarations

//this will print out the time which was stored after getting it back from the api. This should be replaced later with
//a function that outputs the time correctly onto the screen.
void printLocalTime();

#endif