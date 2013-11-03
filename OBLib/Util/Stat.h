#include "LinkedList.h"


/************** "StatElement" Object************/

//"StatElement" object definition

typedef struct StatElement{
	
	//Attributes
	double data;
	int freq;
	char * description;

	//Core Methods
	char * (*toString)( struct StatElement * );
	void (*free)( struct StatElement * );

	//Helper Methods
	char * (*stringer)( void * );
	int (*comparator)( void *, void *);
	void (*freer)( void * );

}StatElement;


//"StatElement" method prototypes

StatElement* newStatElement( double );
char * statElementToString( StatElement * );
void statElementFree( StatElement * );
char * statElementStringer( void * );
int statElementComparator( void *, void *);
void statElementFreer( void * );




/************** "Stat" Object************/

//"Stat" object definition

typedef struct Stat{

	//Attributes
	LinkedList *datalist;
	StatElement *se; //Used to access StatElement functions
	StatElement *tempse; //Used in functions to store temparary StatElements
	int tempint;

	int numdata;
	int modefreq;
	
	double max;
	double min;
	double mean;
	double median;
	double mode;
	double range;
	double std;
	double variance;
	unsigned char update;	//Flag which represent which attriutes need to be updated 
				//min =0x1, max = 0x2, mean = 0x4, median = 0x8, mode = 0x10, range = 0x20, std = 0x40

	//Regression variables
	double slope;
	double intercept;

	//Core Methods
	void (*add)( struct Stat *, double );
	void (*addList)( struct Stat *, LinkedList* );
	void (*remove)( struct Stat *, double );
	void (*removeAll)( struct Stat *, double );
	double (*getMax)( struct Stat * );
	double (*getMin)( struct Stat * );
	double (*getMean)( struct Stat * );
	double (*getMedian)( struct Stat * );
	double (*getMode)( struct Stat * );
	double (*getRange)( struct Stat * );
	double (*getStd)( struct Stat * );
	double (*getVariance)( struct Stat * );
	double (*getCorrelation)( struct Stat *, LinkedList*, LinkedList* );
	void (*doRegression)( struct Stat *, LinkedList*, LinkedList* );
	double (*getSlope)( struct Stat * );
	double (*getIntercept)( struct Stat * );
	double (*getDistFromLine)( struct Stat *, double, double, double, double, double );
	int (*getNumElements)( struct Stat * );	
	double (*getPorInRange)( struct Stat *, double, double );
	double (*getPorEqualTo)( struct Stat *, double );	
	double (*getPorGreaterThan)( struct Stat *, double );	
	double (*getPorLessThan)( struct Stat *, double );	
	int (*getNumInRange)( struct Stat *, double, double );
	int (*getNumEqualTo)( struct Stat *, double );	
	int (*getNumGreaterThan)( struct Stat *, double );	
	int (*getNumLessThan)( struct Stat *, double );	
	void (*printStats)( struct Stat * );
	void (*printData)( struct Stat * );
	void (*printStatsFile)( struct Stat * , FILE * );
	void (*printDataFile)( struct Stat * , FILE * );
	void (*printFreqHist)( struct Stat *, double, double, int );
	void (*reset)( struct Stat * );
	void (*free)( struct Stat * );

}Stat;


//"StatElement" method prototypes

Stat* newStat();
void statAdd( Stat *, double );
void statAddList( Stat *, LinkedList* );
void statRemove( Stat *, double ); 
void statRemoveAll( Stat *, double );
double statGetMax( Stat * );
double statGetMin( Stat * );
double statGetMean( Stat * );
double statGetMedian( Stat * );
double statGetMode( Stat * );
double statGetRange( Stat * );
double statGetStd( Stat * );
double statGetVariance( Stat * );
double statGetCorrelation( Stat *, LinkedList*, LinkedList* );
void statDoRegression( Stat *, LinkedList*, LinkedList* );
double statGetSlope( Stat * );
double statGetIntercept( Stat * );
double statGetDistFromLine( Stat *, double, double, double, double, double );
int statGetNumElements( Stat * );
double statGetPorInRange( Stat *, double, double );
double statGetPorEqualTo( Stat *, double );
double statGetPorGreaterThan( Stat *, double );
double statGetPorLessThan( Stat *, double );
int statGetNumInRange( Stat *, double, double );
int statGetNumEqualTo( Stat *, double );
int statGetNumGreaterThan( Stat *, double );
int statGetNumLessThan( Stat *, double );
void statPrintStats( Stat * );
void statPrintData( Stat * );
void statPrintStatsFile( Stat * , FILE * );
void statPrintDataFile( Stat * , FILE * );
void statPrintFreqHist( Stat *, double, double, int );
void statReset( Stat * );
void statFree( Stat * );


