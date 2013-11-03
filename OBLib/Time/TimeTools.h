#include <time.h>
#include "String.h"


/************** "TimeTools" Object************/

//"TimeMap" object definition

typedef struct TimeTools{

	//Attributes
	time_t epoch;
	struct tm t;
	char buff[80];
	String *str;

	//Core Methods
	char * (*epochToString)( struct TimeTools * , unsigned long, char* );
	char * (*NTPEpochToString)( struct TimeTools * , unsigned long, char* );
	unsigned long (*NTPEpochToEpoch)( struct TimeTools * , unsigned long );
	unsigned long (*EpochToNTPEpoch)( struct TimeTools * , unsigned long );
	unsigned long (*stringToEpoch)( struct TimeTools *, char * );
	unsigned long (*stringToNTPEpoch)( struct TimeTools *, char * );
	unsigned long (*epochFloorMin)( struct TimeTools *, unsigned long );
	unsigned long (*epochFloorHour)( struct TimeTools *, unsigned long );
	unsigned long (*epochFloorDay)( struct TimeTools *, unsigned long );
	unsigned long (*epochFloorMon)( struct TimeTools *, unsigned long );
	unsigned long (*epochFloorYear)( struct TimeTools *, unsigned long );

	double (*NTPTimestampToEpoch)( struct TimeTools *, unsigned long, unsigned long );
	void (*free)( struct TimeTools * );


}TimeTools;


//"TimeTools" method prototypes

TimeTools* newTimeTools();
char * TimeTools_epochToString( TimeTools * , unsigned long, char* );
char * TimeTools_NTPEpochToString( TimeTools * , unsigned long, char* );
unsigned long NTPEpochToEpoch( TimeTools * , unsigned long );
unsigned long EpochToNTPEpoch( TimeTools * , unsigned long );
unsigned long TimeTools_stringToEpoch( TimeTools *, char * );
unsigned long TimeTools_stringToNTPEpoch( TimeTools *, char * );
unsigned long epochFloorMin( TimeTools *, unsigned long );
unsigned long epochFloorHour( TimeTools *, unsigned long );
unsigned long epochFloorDay( TimeTools *, unsigned long );
unsigned long epochFloorMon( TimeTools *, unsigned long );
unsigned long epochFloorYear( TimeTools *, unsigned long );

double NTPTimestampToEpoch( TimeTools *, unsigned long, unsigned long );
void TimeTools_free( TimeTools * );

