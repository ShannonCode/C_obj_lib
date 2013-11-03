#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TimeTools.h"




int main( int argc, char **argv )
{
	//Declare variables 
	char *string;
	unsigned long epoch, epochb, sec, frac;
	double epochc;

	//Create new "TimeTools" object
	TimeTools *t = newTimeTools();


	printf("\n\n********* Conversion Functions *********\n");
	
	//Convert epoch to string
	printf("\nLinux epoch to String:\t\t");
	epoch = 1330627759u;
	string = t->epochToString( t, epoch, "%Y-%m-%d %H:%M:%S" );
	printf("%ld -> \"%s\"\n", epoch, string );

	//Convert epoch to string
	printf("\nNTP Epoch to String:\t\t");
	epoch = 3539617500u;
	string = t->NTPEpochToString( t, epoch, "%Y-%m-%d %H:%M:%S" );
	printf("%lu -> \"%s\"\n", epoch, string );

	//Convert string to epoch
	printf("\nString to Linux epoch:\t\t");
	epoch = t->stringToEpoch( t, "2012-03-01 19:00:00");
	printf("\"%s\" -> %ld\n", "2012-03-01 19:00:00", epoch );


	//Convert string to NTP epoch
	printf("\nString to NTP epoch:\t\t");
	epoch = t->stringToNTPEpoch( t, "2012-03-01 19:00:00");
	printf("\"%s\" -> %lu\n", "2012-03-01 19:00:00", epoch );


	//Convert NTP epoch to Linux epoch
	printf("\nNTP epoch to Linux epoch:\t");
	epoch = 3539617500u;
	epochb = t->NTPEpochToEpoch( t, epoch );
	printf("%lu -> %lu\n", epoch, epochb );


	//Convert Linux epoch to NTP epoch
	printf("\nLinux epoch to NTP epoch:\t");
	epoch = 1330627759u;
	epochb = t->NTPEpochToEpoch( t, epoch );
	printf("%lu -> %lu\n", epoch, epochb );


	//Convert NTP Timestamp to epoch
	printf("\nNTP Timestamp to epoch:\t\t");
	sec = 3539514213u;
	frac = 1046764995u;
	//result should be 3539514213.243719
	epochc = t->NTPTimestampToEpoch( t, sec, frac );
	printf("%lu.%lu -> %lf\n", sec, frac, epochc );


	printf("\n\n********* Floor Functions *********\n");

	epoch = 1330627759u;

	//Floor to nearest Minute
	printf("\nFloor Epoch - Minute:\t\t");
	epochb = t->epochFloorMin( t, epoch );
	string = t->epochToString( t, epochb, "%Y-%m-%d %H:%M:%S" );
	printf("%ld -> \"%s\"\n", epochb, string );

	//Floor to nearest Hour
	printf("\nFloor Epoch - Hour:\t\t");
	epochb = t->epochFloorHour( t, epoch );
	string = t->epochToString( t, epochb, "%Y-%m-%d %H:%M:%S" );
	printf("%ld -> \"%s\"\n", epochb, string );

	//Floor to nearest Day
	printf("\nFloor Epoch - Day:\t\t");
	epochb = t->epochFloorDay( t, epoch );
	string = t->epochToString( t, epochb, "%Y-%m-%d %H:%M:%S" );
	printf("%ld -> \"%s\"\n", epochb, string );

	//Floor to nearest Month
	printf("\nFloor Epoch - Month:\t\t");
	epochb = t->epochFloorMon( t, epoch );
	string = t->epochToString( t, epochb, "%Y-%m-%d %H:%M:%S" );
	printf("%ld -> \"%s\"\n", epochb, string );

	//Floor to nearest Year
	printf("\nFloor Epoch - Year:\t\t");
	epochb = t->epochFloorYear( t, epoch );
	string = t->epochToString( t, epochb, "%Y-%m-%d %H:%M:%S" );
	printf("%ld -> \"%s\"\n\n", epochb, string );


	//Free "TimeTools" object
	t->free( t );

}
