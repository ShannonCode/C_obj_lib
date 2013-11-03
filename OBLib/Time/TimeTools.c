#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "TimeTools.h"



//Converts the specified epoch to a string as specified by a paticular format
//Format specifiers: "%Y" -> year, "%m" -> month, "%d" -> date, "%H" -> hour, "%M" -> minute, "%S" -> second
//e.g  TimeTools_epochToString( tt, 1330725600, "%Y-%m-%d %H:%M:%S" ) RETURNS "2012-03-02 22:00:00"
char * TimeTools_epochToString( TimeTools * tt , unsigned long epoch, char* format )
{
	tt->epoch = epoch;
	tt->t = *localtime( &tt->epoch );
	strftime( tt->buff, sizeof(tt->buff), format, &tt->t );

	return tt->buff;
}



char * TimeTools_NTPEpochToString( TimeTools * tt , unsigned long epoch, char* format )
{
	unsigned long tempepoch = epoch - 2208988800u;
	tt->t = *localtime( &tempepoch );
	strftime( tt->buff, sizeof(tt->buff), format, &tt->t );

	return tt->buff;
}



unsigned long TimeTools_NTPEpochToEpoch( TimeTools * tt, unsigned long epoch )
{
	return (epoch - 2208988800u);
}


unsigned long TimeTools_EpochToNTPEpoch( TimeTools * tt, unsigned long epoch )
{
	return (epoch + 2208988800u);
}

//Excepts a string with a date and time in the format "Y M D h m s"
//Returns the epoch of that data and time
//e.g TimeTools_stringToEpoch( tt, "2012-03-02 22:00:00" ) RETURNS 1330725600
unsigned long TimeTools_stringToEpoch( TimeTools * tt , char * time )
{
	int date[]={1900,0,1,0,0,0};//year,month,date,hour,min,sec
	int i=0;
	tt->str->set( tt->str, time );

	//Get date and time
	while( tt->str->hasMoreInt(tt->str) && i<6 )
	{
		date[i]	= tt->str->getNextInt(tt->str);
		i++;
	}

	//Validate Year
	if( date[0] < 0 )
		return 0;
		//date[0] = 1900;
	if( date[0] < 1000 )
		date[0] = date[0] + 2000;

	//Validate Month
	date[1] = date[1] - 1;
	if( date[1] < 0 || date[1] > 11 )
		date[1] = 0;
		
	
	//Validate Date
	if( date[2] < 0 || date[2] > 31 )
		date[2] = 1;
	
	//Validate Hour
	if( date[3] < 0 || date[3] > 23 )
		date[3] = 0;
	
	//Validate Minute
	if( date[4] < 0 || date[4] > 59 )
		date[4] = 0;
	
	//Validate Second
	if( date[5] < 0 || date[5] > 61 )
		date[5] = 0;
	
	//printf("\n%d/%d/%d %d:%d:%d\n",year,month,date,hour,min,sec);

	//Construct time structure
	tt->t.tm_year = date[0]-1900;
	tt->t.tm_mon = date[1];
	tt->t.tm_mday = date[2];
	tt->t.tm_hour = date[3];
	tt->t.tm_min = date[4];
	tt->t.tm_sec = date[5];
	tt->t.tm_isdst = -1;

	//printf("\nEpoch: %lu\n",epoch);

	return ( (unsigned long) mktime( &tt->t ) );
}




unsigned long TimeTools_stringToNTPEpoch( TimeTools * tt , char * time )
{
	return (tt->stringToEpoch( tt, time) + 2208988800u );
}


unsigned long TimeTools_epochFloorMin( TimeTools * tt , unsigned long epoch )
{
	tt->epoch = epoch;
	tt->t = *localtime( &tt->epoch );
	tt->t.tm_sec = 0;

	return ((unsigned long) mktime( &tt->t ));
}


unsigned long TimeTools_epochFloorHour( TimeTools * tt , unsigned long epoch )
{
	tt->epoch = epoch;
	tt->t = *localtime( &tt->epoch );
	tt->t.tm_sec = 0;
	tt->t.tm_min = 0;

	return ((unsigned long) mktime( &tt->t ));
}


unsigned long TimeTools_epochFloorDay( TimeTools * tt , unsigned long epoch )
{
	tt->epoch = epoch;
	tt->t = *localtime( &tt->epoch );
	tt->t.tm_sec = 0;
	tt->t.tm_min = 0;
	tt->t.tm_hour = 0;

	return ((unsigned long) mktime( &tt->t ));
}


unsigned long TimeTools_epochFloorMon( TimeTools * tt , unsigned long epoch )
{
	tt->epoch = epoch;
	tt->t = *localtime( &tt->epoch );
	tt->t.tm_sec = 0;
	tt->t.tm_min = 0;
	tt->t.tm_hour = 0;
	tt->t.tm_mday = 1;

	return ((unsigned long) mktime( &tt->t ));
}


unsigned long TimeTools_epochFloorYear( TimeTools * tt , unsigned long epoch )
{
	tt->epoch = epoch;
	tt->t = *localtime( &tt->epoch );
	tt->t.tm_sec = 0;
	tt->t.tm_min = 0;
	tt->t.tm_hour = 0;
	tt->t.tm_mday = 1;
	tt->t.tm_mon = 0;

	return ((unsigned long) mktime( &tt->t ));
}



double TimeTools_NTPTimestampToEpoch( TimeTools * tt, unsigned long sec, unsigned long frac )
{
	double s = ((double) sec);
	double f = (((double)frac) / UINT_MAX);

	return (s + f);
}


void TimeTools_free( TimeTools * tt )
{
	tt->str->free( tt->str );
	free( tt );
}



//Initialises a new "TimeMap" Object
TimeTools* newTimeTools( )
{
	//Allocate memory for struct
	TimeTools * tt = NULL;
	tt = (TimeTools*)malloc(sizeof(TimeTools));

	tt->epoch = 0;
	tt->str = newString("");

	//Assign Core Methods
	tt->epochToString = TimeTools_epochToString;
	tt->NTPEpochToString = TimeTools_NTPEpochToString;
	tt->NTPEpochToEpoch = TimeTools_NTPEpochToEpoch;
	tt->EpochToNTPEpoch = TimeTools_EpochToNTPEpoch;
	tt->stringToEpoch = TimeTools_stringToEpoch;
	tt->stringToNTPEpoch = TimeTools_stringToNTPEpoch;
	tt->epochFloorMin = TimeTools_epochFloorMin;
	tt->epochFloorHour = TimeTools_epochFloorHour;
	tt->epochFloorDay = TimeTools_epochFloorDay;
	tt->epochFloorMon = TimeTools_epochFloorMon;
	tt->epochFloorYear = TimeTools_epochFloorYear;

	tt->NTPTimestampToEpoch = TimeTools_NTPTimestampToEpoch;
	tt->free = TimeTools_free;


	return tt;

}
