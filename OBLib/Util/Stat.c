#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Stat.h"




/************** "STATELEMENT" OBJECT METHODS ************/


//Returns a string describing a "StatElement" object
char * statElementToString ( struct StatElement * se )
{
	char string[100];
	int size;

	size = sprintf(string, "%f\t%d", se->data, se->freq );

	//printf("\n\n%s\t\tis the result which is %d long\n\n", string, size);

	if( se->description == NULL )
	{
		se->description = (char *)malloc( sizeof( char[size+1] ) );
	}
	else if( strlen(se->description) != size )
	{
		free( se->description );
		se->description = (char *)malloc( sizeof( char[size+1] ) );	
	}

	strcpy( se->description, string );

	return se->description;
}


/*** Helper Method ***/
//Compare two "StatElement" objects
//Returns 1 if 1st element is greater than the second element, -1 if 1st element is less than the second element, 0 otherwise
int statElementComparator( void *element1, void *element2)
{
	char buffa[40];
	char buffb[40];
	int size;

	//Covert data to strings to check for equality (comparing double values is error prone)
	sprintf( buffa, "%f", ((StatElement*)element1)->data );
	sprintf( buffb, "%f", ((StatElement*)element2)->data );

	//printf("\ne1-> %s \te2-> %s",buffa, buffb);

	//Check for equality first
	if( strcmp(buffa,buffb) == 0 )
	{
		//printf("\tequal");
		return 0;
	}
	else if( ((StatElement*)element1)->data < ((StatElement*)element2)->data )
	{
		//printf("\tless than");
		return -1;
	}
	else
	{
		//printf("\tgreater");
		return 1;
	}
}


void statElementFree( StatElement * se)
{
	free(se);
}


/*** Helper Method ***/
//Frees up memory used by a "StatElement" object
void statElementFreer( void * element )
{
	((StatElement*)element)->free( ((StatElement*)element) );
}


/*** Helper Method ***/
//Returns a string describing a "StatElement" object
char * statElementStringer( void *element )
{
	return ((StatElement*)element)->toString( ((StatElement*)element) );
}


//Initialises a new "StatElement" object
StatElement* newStatElement( double data )
{
	//Allocate memory for object
	StatElement *se = (StatElement*)malloc(sizeof(StatElement));

	//Initialise attributes
	se->data = data;
	se->freq = 1;
	se->description = NULL;

	//Assign Core Methods
	se->toString = statElementToString;
	se->free = statElementFree;

	//Assign Helper Methods (used by "LinkedList" object)
	se->stringer = statElementStringer;
	se->comparator = statElementComparator;
	se->freer = statElementFreer;

	return se;
}




/************** "STAT" OBJECT METHODS ************/


//Prints all data contained within a "Stat" object
void statPrintData( Stat *stat )
{
	printf("\nSTATS DATA:");
	if( stat->numdata > 0)
		stat->datalist->printAll( stat->datalist );
	else
		printf("\tNo Data!");
}


//Prints all stats related to the data contained within a "Stat" object
void statPrintStats( Stat *stat )
{
	if( stat->numdata >= 2)
	{
		//Update stats
		stat->getMean(stat);
		stat->getMedian(stat);
		stat->getMode(stat);
		stat->getRange(stat);
		stat->getStd(stat);
		stat->getVariance(stat);

		//Print
		printf("\nSTATS:\n#\t->\t%d\nMax\t->\t%f\nMin\t->\t%f\nMean\t->\t%f\nMedian\t->\t%f\nMode\t->\t%f (%d)", stat->numdata, stat->max, stat->min, stat->mean, stat->median, stat->mode, stat->modefreq );

		printf("\nRange\t->\t%f\nStd\t->\t%f\nVar\t->\t%f",stat->range, stat->std, stat->variance);
		

		printf("\n");
	}
	else if( stat->numdata == 1 )
		printf("\nSTATS:\tOnly 1 Data Element!");
	else
		printf("\nSTATS:\tNo Data!");
}




void statPrintStatsFile( Stat * stat, FILE * f )
{

	if( stat->numdata >= 2)
	{
		//Update stats
		stat->getMean(stat);
		stat->getMedian(stat);
		stat->getMode(stat);
		stat->getRange(stat);
		stat->getStd(stat);
		stat->getVariance(stat);

		//Print
		fprintf(f,"\nSTATS:\n#\t->\t%d\nMax\t->\t%f\nMin\t->\t%f\nMean\t->\t%f\nMedian\t->\t%f\nMode\t->\t%f (%d)", stat->numdata, stat->max, stat->min, stat->mean, stat->median, stat->mode, stat->modefreq );

		fprintf(f,"\nRange\t->\t%f\nStd\t->\t%f\nVar\t->\t%f",stat->range, stat->std, stat->variance);
		

		fprintf(f,"\n");
	}
	else if( stat->numdata == 1 )
		fprintf(f,"\nSTATS:\tOnly 1 Data Element!");
	else
		fprintf(f,"\nSTATS:\tNo Data!");

}



//Prints all data contained within a "Stat" object to File
void statPrintDataFile( Stat *stat, FILE *f )
{
	fprintf(f,"\nSTATS DATA:");
	if( stat->numdata > 0)
		stat->datalist->printAllFile( stat->datalist, f );
	else
		fprintf(f,"\tNo Data!");
}



//Prints Frequencey Histogram of Data
//Range of histogram is between "start" and "bar_size*num_bars"
void statPrintFreqHist( Stat * stat, double start, double bar_size, int num_bars )
{
	double a = start, temp_por;
	int i, temp_num;

	printf("(x  <  %lf)\t\t\t(%d)\t(%.2lf %%)\n", a, stat->getNumLessThan(stat, a ), stat->getPorLessThan(stat, a )*100 );
	
	for(i=0;i<num_bars;i++)	
	{
		temp_num = stat->getNumInRange(stat, a, (a + bar_size) ) - stat->getNumEqualTo(stat, (a + bar_size) );
		temp_por = stat->getPorInRange(stat, a, (a + bar_size) ) - stat->getPorEqualTo(stat, (a + bar_size) );	
		
		printf("(%lf  <= x <  %lf)\t\t(%d)\t(%.2lf %%)\n", a, (a + bar_size), temp_num, (temp_por*100) );
	
		a = a + bar_size;
	}

	temp_num = stat->getNumGreaterThan(stat, a ) + stat->getNumEqualTo(stat, a );
	temp_por = stat->getPorGreaterThan(stat, a ) + stat->getPorEqualTo(stat, a );
	printf("(%lf  <=  x )\t\t\t(%d)\t(%.2lf %%)\n", a, temp_num, (temp_por*100) );
}


//Accepts a "LinkedList" object containing "StatElement" objects and adds them to a "Stat" object
void statAddList( Stat *stat, LinkedList * list )
{
	ListIterator *li;

	//Traverse list and add to Stat object's datalist
	li = list->getIterator( list );
	while( li->hasNext( li ) )
	{
		stat->tempse = li->next( li );
		stat->add( stat, stat->tempse->data );
	}
	
	stat->tempse = stat->se;
}


//Adds the specified data to a "Stat" object
void statAdd( Stat *stat, double data )
{
	ListIterator *li; 
	int index = 0;


	//If first data element
	if( stat->datalist == NULL )
	{
		stat->tempse = newStatElement( data );	
		stat->datalist->addFront( stat->datalist, stat->tempse);
	}
	else 
	{
		stat->tempse->data = data;
		index = stat->datalist->indexOf( stat->datalist, stat->tempse );

		//If found
		if(index>0)
		{
			//Get reference to data element
			stat->tempse = stat->datalist->getAt(stat->datalist, index );
			stat->tempse->freq++;
		}
		else//If no similar data was found
		{
			stat->tempse = newStatElement( data );
			stat->datalist->addFront( stat->datalist, stat->tempse );
		}
	}

	stat->tempse = stat->se;
	stat->numdata++;

	//Mark attributes for update
	stat->update = 0xff;
}
		

//Removes the first occurance of the specified data from a "Stat" object
void statRemove( Stat *stat, double data )
{
	ListIterator *li;
	int index = -1;

	stat->tempse->data = data; 
	index = stat->datalist->indexOf( stat->datalist, stat->tempse );	

	//If found
	if(index>0)
	{
		//Get reference to data element
		stat->tempse = stat->datalist->getAt(stat->datalist, index );
	
		if(stat->tempse->freq == 1)
		{
			stat->datalist->deleteData( stat->datalist, stat->tempse ); //Remove from list
		}
		else
		{
			stat->tempse->freq--;
		}

		stat->tempse = stat->se;
		stat->numdata--;

		//Mark attributes for update
		stat->update = 0xff;
	}
	
}


//Removes all occurances of the specified data from a "Stat" object
void statRemoveAll( Stat *stat, double data )
{
	ListIterator *li;
	int index = -1;

	stat->tempse->data = data; 
	index = stat->datalist->indexOf( stat->datalist, stat->tempse );

	//While there are still more elements
	while(index != -1)
	{
		//Get reference to data element
		stat->tempse = stat->datalist->getAt(stat->datalist, index );

		if(stat->tempse->freq == 1)
			stat->datalist->deleteData( stat->datalist, stat->tempse ); //Remove from list
		else
			stat->tempse->freq--;

		stat->tempse = stat->se;
		stat->numdata--;

		//Mark attributes for update
		stat->update = 0xff;

		index = stat->datalist->indexOf( stat->datalist, stat->tempse );
	}

	
}


//Returns the minimum value of the data contained within a "Stat" object
double statGetMin( Stat * stat )
{
	double min = 0.0;
	ListIterator *li;

	//Check if Min needs updating
	if( (stat->update & 0x1) == 0 || stat->numdata < 1)
		return stat->min;
	else
	{
		//Search min
		li = stat->datalist->getIterator( stat->datalist );
		if(li->hasNext( li ))
		{
			stat->tempse = li->next( li );
			min = stat->tempse->data;
		}
		while( li->hasNext( li ) )
		{
			stat->tempse = li->next( li );
			if( stat->tempse->data < min)
				min = stat->tempse->data;
		}
	
		//Update Min
		stat->min = min;
		stat->update = (stat->update & 0xFE);
		stat->tempse = stat->se;	
		return stat->min;
	}

}


//Returns the maximum value of the data contained within a "Stat" object
double statGetMax( Stat * stat )
{
	double max = 0.0;
	ListIterator *li;

	//Check if Max needs updating
	if( (stat->update & 0x2) == 0 || stat->numdata < 1)
		return stat->max;
	else
	{
		//Search for min
		li = stat->datalist->getIterator( stat->datalist );
		if(li->hasNext( li ))
		{
			stat->tempse = li->next( li );
			max = stat->tempse->data;
		}
		while( li->hasNext( li ) )
		{
			stat->tempse = li->next( li );
			if( stat->tempse->data > max)
				max = stat->tempse->data;
		}
	
		//Update Min
		stat->max = max;
		stat->update = (stat->update & 0xFD);
		stat->tempse = stat->se;
		return stat->max;
	}
}





//Returns the mean of the data contained within a "Stat" object
double statGetMean( Stat * stat )
{
	ListIterator *li; 
	int numelements = 0;
	double sum = 0.0;
	
	//Check if Mean needs updating
	if( (stat->update & 0x4) == 0 || stat->numdata < 1)
		return stat->mean;
	else
	{
		//Traverse data list
		li = stat->datalist->getIterator( stat->datalist );
		while( li->hasNext( li ) )
		{
			stat->tempse = li->next( li );
			numelements = numelements + stat->tempse->freq;
			sum = sum + (stat->tempse->data * stat->tempse->freq);
		}
		
		//Calculate mean
		stat->mean = ((double)sum)/numelements;
		stat->update = (stat->update & 0xFB);
		stat->tempse = stat->se;		
		return stat->mean;
	}

}



//Returns the median of the data contained within a "Stat" object
double statGetMedian( Stat * stat )
{
	ListIterator *li;
	int num = 0; 
	int even = 0;
	int midpos;

	if(stat->numdata%2 == 0)
	{
		even = 1;
		midpos = stat->numdata/2;
	}
	else
	{
		midpos = (stat->numdata+1)/2;
	}

	//Check if Mode needs updating
	if( (stat->update & 0x8) == 0 || stat->numdata < 1)
		return stat->median;
	else
	{
		//Sort List
		stat->datalist->sortDesc( stat->datalist );
		
		//Traverse data list
		li = stat->datalist->getIterator( stat->datalist );
		while( li->hasNext( li ) )
		{
			stat->tempse = li->next( li );
			num = num + stat->tempse->freq;

			if( num == midpos && even == 1)
			{
				stat->median = stat->tempse->data;
				stat->tempse = li->next( li );
				stat->median = (stat->median + stat->tempse->data)/2;
				break;
			}
			else if ( num > midpos || (num == midpos && even == 0) )
			{
				stat->median = stat->tempse->data;
				break;
			}
		}


		stat->update = (stat->update & 0xF7);
		stat->tempse = stat->se;
		return stat->median;
	}
}


//Returns the mode of the data contained within a "Stat" object
double statGetMode( Stat * stat )
{
	ListIterator *li;
	int max = -1;

	//Check if Mode needs updating
	if( (stat->update & 0x10) == 0 || stat->numdata < 1)
		return stat->mode;
	else
	{
		//Search for mode
		li = stat->datalist->getIterator( stat->datalist );
		while( li->hasNext( li ) )
		{
			stat->tempse = li->next( li );
			if( stat->tempse->freq > max )
			{
				stat->mode = stat->tempse->data;
				max = stat->tempse->freq;
				stat->modefreq = max;
			}
		}

		stat->update = (stat->update & 0xEF);
		stat->tempse = stat->se;
		return stat->mode;
	}
}


//Returns the range of the data contained within a "Stat" object
double statGetRange( Stat * stat )
{	
	ListIterator *li;
	double max = 0;
	double min = 0;

	//Check if Range needs updating
	if( (stat->update & 0x20) == 0 || stat->numdata < 1)
		return stat->range;
	else
	{
		max = stat->getMax(stat);
		min = stat->getMin(stat);

		//Calculate range
		if(min <= 0.0 && max <= 0.0)
			stat->range = -(min - max);
		else if(min <= 0.0 && max >= 0.0)
			stat->range = ((-min) + max);
		else if(min >= 0.0 && max <= 0.0)
			stat->range = (min + (-max));
		else
			stat->range = max - min;

		stat->update = (stat->update & 0xDF);

		return stat->range;
	}
}


//Returns the standard deviation of the data contained within a "Stat" object
double statGetStd( Stat * stat )
{
	ListIterator *li;
	double sum = 0;

	//Check if Std needs updating
	if( (stat->update & 0x40) == 0 || stat->numdata < 1 )
		return stat->std;
	else
	{
		//Get mean
		stat->getMean(stat);
		
		li = stat->datalist->getIterator( stat->datalist );
		while( li->hasNext( li ) )
		{
			stat->tempse = li->next( li );
			sum = sum + ( ((stat->tempse->data - stat->mean) * (stat->tempse->data - stat->mean)) * stat->tempse->freq);
		}

		stat->variance = sum/(stat->numdata-1);
		stat->std = sqrt(stat->variance);
		stat->update = (stat->update & 0xBF);
		stat->tempse = stat->se;
		return stat->std;
	}
}




//Accepts two "LinkedList" objects containing "StatElement" objects
//Returns the correlation between each list of data
double statGetCorrelation( Stat * stat, LinkedList *listx, LinkedList *listy )
{
	ListIterator *lix;
	ListIterator *liy;
	StatElement * dex;
	StatElement * dey;
	double sumX = 0.0, sumY = 0.0, sumXsq = 0.0, sumYsq = 0.0, sumXY = 0.0, numer = 0.0, denom = 0.0;	
	int n = 0;

	//Get Iterators
	lix = listx->getIterator( listx );
	liy = listy->getIterator( listy );	
	
	//Traverse Lists	
	while( lix->hasNext( lix ) && liy->hasNext( liy ) )
	{
		//Get data		
		dex = lix->next( lix );
		dey = liy->next( liy );
		
		//Calculate sums
		sumX = sumX + dex->data;
		sumXsq = sumXsq + ( dex->data * dex->data );
		
		sumY = sumY + dey->data;
		sumYsq = sumYsq + ( dey->data * dey->data );		

		sumXY = sumXY + ( dex->data * dey->data ); 
		
		n++;
	}

	numer = ( n * sumXY ) - ( sumX * sumY );
	denom = sqrt( ( ( n * sumXsq ) - ( sumX * sumX ) ) * ( ( n * sumYsq ) - ( sumY * sumY ) ) );

	//printf("\nsumx -> %f",sumX);	
	//printf("\nsumy -> %f",sumY);
	//printf("\nsumx^2 -> %f",sumXsq);
	//printf("\nsumy^2 -> %f",sumYsq);
	//printf("\nsumxy -> %f",sumXY);
	//printf("\nn -> %d",n);

	return (numer/denom);
}



//Returns the variance of the data contained within a "Stat" object
double statGetVariance( Stat * stat )
{
	stat->getStd;
	return stat->variance;
}



//Accepts two "LinkedList" objects containing "StatElement" objects
//Calculates the relationship between the two lists using Regression Analysis
void statDoRegression( Stat * stat, LinkedList *listx, LinkedList *listy )
{
	ListIterator *lix;
	ListIterator *liy;
	StatElement * dex;
	StatElement * dey;
	double sumX = 0.0, sumY = 0.0, sumXsq = 0.0, sumXY = 0.0, numer = 0.0, denom = 0.0;	
	int n = 0;

	//Get Iterators
	lix = listx->getIterator( listx );
	liy = listy->getIterator( listy );	
	
	//Traverse Lists	
	while( lix->hasNext( lix ) && liy->hasNext( liy ) )
	{
		//Get data		
		dex = lix->next( lix );
		dey = liy->next( liy );
		
		//Calculate sums
		sumX = sumX + dex->data;
		sumXsq = sumXsq + ( dex->data * dex->data );
		
		sumY = sumY + dey->data;		

		sumXY = sumXY + ( dex->data * dey->data ); 
		
		n++;
	}

	//printf("\nsumx -> %f",sumX);	
	//printf("\nsumy -> %f",sumY);
	//printf("\nsumx^2 -> %f",sumXsq);
	//printf("\nsumxy -> %f",sumXY);
	//printf("\nn -> %d",n);
	
	numer = ( n * sumXY ) - ( sumX * sumY );
	denom = ( n * sumXsq ) - ( sumX * sumX );
	stat->slope = numer/denom;
	stat->intercept = ( sumY - ( stat->slope * sumX ) ) / n;
}


//Returns the "slope" as calculated from the previous call to "doRegression" 
double statGetSlope( Stat * stat )
{
	return stat->slope;
}

//Returns the "intercept" as calculated from the previous call to "doRegression" 
double statGetIntercept( Stat * stat )
{
	return stat->intercept;
}


//Returns the perpendicular distance of a point (x,y) from a line (Ax + By + C = 0)
double statGetDistFromLine( Stat * stat, double A, double B, double C, double x, double y )
{
	double numer, denom;

	numer = (A*x) + (B*y) + C;
	denom = sqrt( (A*A) + (B*B) );

	return (numer/denom);
}


int statGetNumElements( Stat * stat )
{
	ListIterator *li; 
	int numelements = 0;
	
	//Traverse data list
	li = stat->datalist->getIterator( stat->datalist );
	while( li->hasNext( li ) )
	{
		stat->tempse = li->next( li );
		numelements = numelements + stat->tempse->freq;
	}

	//Store value
	stat->tempint = numelements;

	return numelements;
}


//Return proportion of values that fall within the specified range inclusive 
double statGetPorInRange( Stat * stat, double start, double end )
{
	int count = stat->getNumInRange( stat, start, end );

	//Return proportion		
	return ((double)count/(double)stat->tempint) + stat->getPorEqualTo( stat, start ) + stat->getPorEqualTo( stat, end );
}


//Return proportion of values that are equal to the specified value 
double statGetPorEqualTo( Stat * stat, double value )
{
	int count = stat->getNumEqualTo( stat, value );

	//Return proportion		
	return ((double)count/(double)stat->tempint);
}


//Return proportion of values that are greater than the specified value 
double statGetPorGreaterThan( Stat * stat, double value )
{
	int count = stat->getNumGreaterThan( stat, value );

	//Return proportion		
	return ((double)count/(double)stat->tempint);
}

//Return proportion of values that are less than the specified value 
double statGetPorLessThan( Stat * stat, double value )
{
	int count = stat->getNumLessThan( stat, value );

	//Return proportion		
	return ((double)count/(double)stat->tempint);
}



//Return number of values that fall within the specified range inclusive 
int statGetNumInRange( Stat * stat, double start, double end )
{
	ListIterator *li; 
	int count = 0;
	
	//Traverse data list
	li = stat->datalist->getIterator( stat->datalist );
	while( li->hasNext( li ) )
	{
		stat->tempse = li->next( li );

		//Check if in range
		if( (stat->tempse->data > start && stat->tempse->data < end) )
			count = count + stat->tempse->freq;
	}
			
	return count + stat->getNumEqualTo( stat, start ) + stat->getNumEqualTo( stat, end );
}


//Return number of values that are equal to the specified value 
int statGetNumEqualTo( Stat * stat, double value )
{
	ListIterator *li; 
	int numelements = 0;
	int count = 0;
	char buffa[40];
	char buffb[40];

	//Covert data to string to check for equality (comparing double values is error prone)
	sprintf( buffa, "%lf", value );

	//Traverse data list
	li = stat->datalist->getIterator( stat->datalist );
	while( li->hasNext( li ) )
	{
		stat->tempse = li->next( li );
		numelements = numelements + stat->tempse->freq;

		//Check if equal
		sprintf( buffb, "%lf", stat->tempse->data );
		if( strcmp( buffa, buffb ) == 0 )
			count = count + stat->tempse->freq;
		
	}
	
	//Store "numelements"
	stat->tempint = numelements;

	//Return proportion		
	return count;
}

//Return numer of values that are greater than the specified value 
int statGetNumGreaterThan( Stat * stat, double value )
{
	ListIterator *li; 
	int numelements = 0;	
	int count = 0;
	
	//Traverse data list
	li = stat->datalist->getIterator( stat->datalist );
	while( li->hasNext( li ) )
	{
		stat->tempse = li->next( li );
		numelements = numelements + stat->tempse->freq;

		//Check if less
		if( stat->tempse->data > value )
			count = count + stat->tempse->freq;
	}

	//Store "numelements"
	stat->tempint = numelements;

	return count;
}


//Return numer of values that are less than the specified value 
int statGetNumLessThan( Stat * stat, double value )
{
	ListIterator *li; 
	int numelements = 0;
	int count = 0;
	
	//Traverse data list
	li = stat->datalist->getIterator( stat->datalist );
	while( li->hasNext( li ) )
	{
		stat->tempse = li->next( li );		
		numelements = numelements + stat->tempse->freq;

		//Check if less
		if( stat->tempse->data < value )
			count = count + stat->tempse->freq;
	}
	
	//Store "numelements"
	stat->tempint = numelements;
	
	return count;
}



//Resets all data contained in a "Stat" object
void statReset( Stat * stat )
{
	if( stat->datalist->size > 0)
		stat->datalist->deleteAll( stat->datalist );

	stat->tempse = stat->se;
	stat->tempint = 0;	
	stat->numdata = 0;
	stat->update = 0xff;
	stat->modefreq = 0;
	stat->max = 0.0;
	stat->min = 0.0;
	stat->mean = 0.0;
	stat->median = 0.0;
	stat->mode = 0.0;
	stat->range = 0.0;
	stat->std = 0.0;
	stat->variance = 0.0;

	stat->slope = 0.0;
	stat->intercept = 0.0;

}

//Frees all memory allocated by a "Stat" object
void statFree( Stat * stat )
{
	if( stat->datalist != NULL)
		stat->datalist->free( stat->datalist );

	free( stat->se );
	free( stat );
}


//INITIALISES A NEW STAT OBJECT
Stat* newStat()
{
	//Allocate memory for object
	Stat * stat = (Stat*)malloc(sizeof(Stat));

	//Initialise attributes
	stat->se = newStatElement( 0.0 );
	stat->datalist = newLinkedList( stat->se->freer );
	stat->datalist->setComparator( stat->datalist, stat->se->comparator );
	stat->datalist->setStringer( stat->datalist, stat->se->stringer);
	stat->tempse = stat->se;
	stat->tempint = 0;	
	stat->numdata = 0;
	stat->update = 0xff;
	stat->modefreq = 0;

	stat->max = 0.0;
	stat->min = 0.0;
	stat->mean = 0.0;
	stat->median = 0.0;
	stat->mode = 0.0;
	stat->range = 0.0;
	stat->std = 0.0;
	stat->variance = 0.0;

	stat->slope = 0.0;
	stat->intercept = 0.0;

	//Assign Core Methods
	stat->addList = statAddList;	
	stat->add = statAdd;
	stat->remove = statRemove;
	stat->removeAll = statRemoveAll;
	stat->getMax = statGetMax;
	stat->getMin = statGetMin;
	stat->getMean = statGetMean;
	stat->getMedian = statGetMedian;
	stat->getMode = statGetMode;
	stat->getRange = statGetRange;
	stat->getStd = statGetStd;
	stat->getVariance = statGetVariance;
	stat->getCorrelation = statGetCorrelation;
	stat->doRegression = statDoRegression;
	stat->getSlope = statGetSlope;
	stat->getIntercept = statGetIntercept;
	stat->getDistFromLine = statGetDistFromLine;
	stat->getNumElements = statGetNumElements;
	stat->getPorInRange = statGetPorInRange;
	stat->getPorEqualTo = statGetPorEqualTo;	
	stat->getPorGreaterThan = statGetPorGreaterThan;
	stat->getPorLessThan = statGetPorLessThan;	
	stat->getNumInRange = statGetNumInRange;
	stat->getNumEqualTo = statGetNumEqualTo;	
	stat->getNumGreaterThan = statGetNumGreaterThan;
	stat->getNumLessThan = statGetNumLessThan;		
	stat->printStats = statPrintStats;
	stat->printData = statPrintData;
	stat->printStatsFile = statPrintStatsFile;
	stat->printDataFile = statPrintDataFile;
	stat->printFreqHist = statPrintFreqHist;
	stat->reset = statReset;
	stat->free = statFree;

	return stat;
}


