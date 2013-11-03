#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "String.h"
#include "LinkedList.h"
#include "GnuPlot.h"
#include "TimeTools.h"


/*X Tics Mapper Function
	Must follow the definition -> void name( LinkedList *, double, double ) { //your code... }
	The first argument is a "LinkedList" object to which you must add "TickMap" objects
	A "TickMap" object contains 2 attributes: tick(type:double) and label(type:"String" object)
	The second argument represnts the minimum "axis range" value which can be specified using the "setXRange" or "setYRange" funations
	The third argument represnts the maximum "axis range" value which can be specified using the "setXRange" or "setYRange" funations
*/
void xmapper( LinkedList *ll, double min, double max )
{
	TickMap *tm; 					//Declare "TickMap" object
	TimeTools *t = newTimeTools();	//Create "TimeTools" Object
	int i = 0;
	long starttime = t->epochFloorHour( t, ((long)min) ); //Floor min time epoch 
	long endtime = ((long)max);							  //Floor max time epoch 

	//Create tick for every hour
	for( i=starttime; i<=endtime; i=i+3600 )
	{
		tm = newTickMap( i, newString( t->epochToString( t, i, "%H:%M" ) ) );	//Create new "TickMap" object -> newTickMap( 'tick', 'label' )
		ll->addBack( ll, tm ); 													//Add "TickMap" object to linkedlist
	}
	t->free(t);	//Free "TimeTools" Object
}




int main( int argc, char **argv )
{

	TimeTools *t = newTimeTools();

	GnuPlot *gp = newGnuPlot();
	
	//Add File which contains data( note: can add more file)
	//addFile( 'Object Pointer', 'File name', 'Legend Name' );
	gp->addFile( gp, "num_pckts_data", "" );
	
	//Set Graph Title
	//setTitle( 'Object Pointer', 'Title' );
	gp->setTitle( gp, "# Packets" );

	//Set X-axis Label
	gp->setXLabel( gp, "Time (M:S)" );

	//Set Y-axis Label
	gp->setYLabel( gp, "# Pckts" );

	//Set X-axis range ( note: can also set Y-axis range )
	//setXRange( 'Object Pointer', 'Minimum', 'Maximum' );
	gp->setXRange( gp, 1331653972, 1331713597 );
	
	//Can use "TimeTools" object for time related data
	//gp->setXRange( gp, t->stringToEpoch( t, "2012-03-13 16:00:00"), t->stringToEpoch( t, "2012-03-14 08:00:00") );
	
	//Set X-axis ticks
	//setXTicks( 'Object Pointer', 'Mapping Function' );
	gp->setXTicks( gp, xmapper );
	
	//Create Gnuplot Instruction Graph
	gp->create( gp );
	
	//Free memory allocated for "GnuPlot" Object
	gp->free(gp);
	printf("\nGraph Created!\n\n");
	
}
