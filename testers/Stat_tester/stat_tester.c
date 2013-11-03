#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LinkedList.h"
#include "Stat.h"



int main( int argc, char **argv )
{

	//Create new "Stat" object
	Stat *stat = newStat();
	
	//Add data to "Stat" object
	//add( 'Object Pointer', 'Data' );
	stat->add( stat, 1.42 );
	stat->add( stat, 0.23 );
	stat->add( stat, 2.4 );
	stat->add( stat, 3.67 );
	stat->add( stat, 2.23 );
	
	
	
	/******************* GENERAL STATS TEST *******************/
	
	printf("\n\n*********** GENERAL STATS TEST ***********\n\n");
	
	double temp;
	
	//Get minimum value
	temp = stat->getMin(stat);
	printf("\nMin\t\t->\t%lf\n", temp);
	
	//Get maximum value
	temp = stat->getMax(stat);
	printf("\nMax\t\t->\t%lf\n", temp);
	
	//Get the mean
	temp = stat->getMean(stat);
	printf("\nMean\t\t->\t%lf\n", temp);
	
	//Get the median
	temp = stat->getMedian(stat);
	printf("\nMedian\t\t->\t%lf\n", temp);
	
	//Get the mode
	temp = stat->getMode(stat);
	printf("\nMode\t\t->\t%lf\n", temp);
	
	//Get the range
	temp = stat->getRange(stat);
	printf("\nRange\t\t->\t%lf\n", temp);
	
	//Get standard deviation
	temp = stat->getStd(stat);
	printf("\nSTD\t\t->\t%lf\n", temp);
	
	//Get variance
	temp = stat->getVariance(stat);
	printf("\nVariance\t->\t%lf\n\n\n", temp);
	
	//Print All Stats
	stat->printStats(stat);
	
	//Print Data ( data, freq )
	stat->printData(stat);
	
	
	
	/******************* RANDOM NUMBER TEST *******************/
	
	printf("\n\n*********** RANDOM NUMBER TEST ***********\n\n");
	
	//Reset "Stat" object - clears all data
	stat->reset( stat );
	
	//Generate 100 random numbers between 1 and 20 and generate stats
	int i = 0;
	for(i=0; i<100; i++)
	{
		stat->add( stat, (rand()%20) );
	}
	stat->printStats(stat);
	stat->printData(stat);
	stat->reset( stat );
	

	
	
	/******************* CORRELATION TEST *******************/
	
	printf("\n\n*********** CORRELATION TEST ***********\n\n");
	
	//Create a "StatElement" object (Used by "Stat" object to hold data)
	StatElement *se = newStatElement(0.0);
	
	//Create two linked lists to hold data
	//A "LinkedList" object can hold any object so must specifiy a "freer" function for the object it will hold
	//This "freer" function allows the "LinkedList" to delete the objects it holds
	//We will specify the "StatElement" object's freer "function"
	LinkedList *llx = newLinkedList( se->freer );
	LinkedList *lly = newLinkedList( se->freer );
	
	
	//Populate first list
	se = newStatElement(2);		//Create new "StatElement" Object -> newStatElement( 'data' )
	llx->addFront( llx, se );	//Add to front of the list 		  -> addFront( 'Object pointer', 'object' )
	
	se = newStatElement(4);		
	llx->addFront( llx, se );	
	
	se = newStatElement(5);
	llx->addFront( llx, se );
	
	se = newStatElement(6);
	llx->addFront( llx, se );
	
	
	//Populate second list
	se = newStatElement(3);
	lly->addFront( lly, se );
	
	se = newStatElement(2);
	lly->addFront( lly, se );
	
	se = newStatElement(6);
	lly->addFront( lly, se );
	
	se = newStatElement(5);
	lly->addFront( lly, se );
	

	//Get the correlation between the data contained in the 2 lists
	temp = stat->getCorrelation( stat, llx, lly );
	printf("\nCorrelation ->\t%lf\n", temp );
	
	
	
	/******************* REGRESSION TEST *******************/
	
	printf("\n\n*********** REGRESSION TEST ***********\n\n");
	
	//Empty Lists
	llx->deleteAll( llx );
	lly->deleteAll( lly );
	
	//Populate first list
	se = newStatElement(60);
	llx->addFront( llx, se );
	
	se = newStatElement(61);
	llx->addFront( llx, se );
	
	se = newStatElement(62);
	llx->addFront( llx, se );
	
	se = newStatElement(63);
	llx->addFront( llx, se );
	
	se = newStatElement(65);
	llx->addFront( llx, se );
	
	//Populate second list
	se = newStatElement(3.1);
	lly->addFront( lly, se );
	
	se = newStatElement(3.6);
	lly->addFront( lly, se );
	
	se = newStatElement(3.8);
	lly->addFront( lly, se );
	
	se = newStatElement(4.0);
	lly->addFront( lly, se );
	
	se = newStatElement(4.1);
	lly->addFront( lly, se );

	//Perform Linear Regression
	stat->doRegression(stat, llx, lly);
	
	//get slope and intercept
	double intercept = stat->getIntercept(stat);
	double slope = stat->getSlope(stat);
	
	printf("\nRegression formula (intercept + slope.x) -> %f + %fx\n\n", intercept, slope );
	
	

}
