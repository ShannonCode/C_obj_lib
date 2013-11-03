#include "String.h"
#include "LinkedList.h"


/************** "TickMap" Object************/

//"TickMap" object definition

typedef struct TickMap{

	//Attributes
	double value;
	String * label;

	//Core Methods
	void (*free)( struct TickMap * );

	//Helper methods
	int (*comparator)( void *, void *);
	void (*freer)( void * );

}TickMap;


//"TickMap" method prototypes

TickMap* newTickMap( double, String * );
void TickMap_free( TickMap * );
int TickMap_comparator( void *, void *);
void TickMap_freer( void * );








/************** "FileMap" Object************/

//"FileMap" object definition

typedef struct FileMap{

	//Attributes
	String * file;
	String * label;

	//Core Methods
	void (*free)( struct FileMap * );

	//Helper methods
	void (*freer)( void * );

}FileMap;


//"FileMap" method prototypes

FileMap* newFileMap( String *,  String * );
void FileMap_free( FileMap * );
void FileMap_freer( void * );





/************** "GnuPlot" Object************/

//"GnuPlot" object definition

typedef struct GnuPlot{

	//Attributes
	FileMap *fm;
	LinkedList *fmlist;
	String *title;
	String *xlabel;
	String *ylabel;
	String *outfile;
	String *style;
	String *text;
	int boxwidth;
	unsigned short flags;//0x1 - xstart set, //0x1 - xend set, 0x4 - ystart set, //0x8 - yend set, 
	double xtics;
	double xend;
	double xstart;
	double ystart;
	double yend;
	double ytics;
	TickMap *tm;
	LinkedList *xtmlist; 
	LinkedList *ytmlist;

	//Core Methods
	void (*addFile) ( struct GnuPlot *, char *, char * );
	void (*setOutFile) ( struct GnuPlot *, char * );
	void (*setTitle) ( struct GnuPlot *, char * );
	char * (*getTitle) ( struct GnuPlot * );
	void (*setXLabel) ( struct GnuPlot *, char * );
	void (*setYLabel) ( struct GnuPlot *, char * );
	void (*setXRange) ( struct GnuPlot *, double, double );
	void (*setXStartRange) ( struct GnuPlot *, double );
	void (*setXEndRange) ( struct GnuPlot *, double );	 
	void (*setYRange) ( struct GnuPlot *, double, double );
	void (*setYStartRange) ( struct GnuPlot *, double );
	void (*setYEndRange) ( struct GnuPlot *, double );	
	void (*setXTics) ( struct GnuPlot *, double );
	void (*setXTicLabels) ( struct GnuPlot *, void (*) ( LinkedList *, double, double ) );
	void (*setYTics) ( struct GnuPlot *, double );
	void (*setYTicLabels) ( struct GnuPlot *, void (*) ( LinkedList *, double, double ) );
	void (*setLines) ( struct GnuPlot * );
	void (*setLinesPoints) ( struct GnuPlot * );
	void (*setPoints) ( struct GnuPlot * );
	void (*setBoxes) ( struct GnuPlot *, int);
	void (*setImpulses) ( struct GnuPlot * );
	void (*setSteps) ( struct GnuPlot *, char * );
	void (*append) ( struct GnuPlot *, char * );
	void (*create) ( struct GnuPlot * );
	void (*run) ( struct GnuPlot * );
	void (*reset) ( struct GnuPlot * );
	void (*free)( struct GnuPlot * );


}GnuPlot;


//"GnuPlot" method prototypes

GnuPlot* newGnuPlot();
void GnuPlot_addFile( GnuPlot *, char *, char * );
void GnuPlot_setOutFile( GnuPlot *, char * );
void GnuPlot_setTitle( GnuPlot *, char * );
char * GnuPlot_getTitle( GnuPlot * );
void GnuPlot_setXLabel( GnuPlot *, char * );
void GnuPlot_setYLabel( GnuPlot *, char * );
void GnuPlot_setXRange( GnuPlot *, double, double );
void GnuPlot_setXStartRange( GnuPlot *, double );
void GnuPlot_setXEndRange( GnuPlot *, double );
void GnuPlot_setYRange( GnuPlot *, double, double );
void GnuPlot_setYStartRange( GnuPlot *, double );
void GnuPlot_setYEndRange( GnuPlot *, double );
void GnuPlot_setXTics( GnuPlot *, double );
void GnuPlot_setXTicLabels( GnuPlot *, void (*) ( LinkedList *, double, double ) );
void GnuPlot_setYTics( GnuPlot *, double );
void GnuPlot_setYTicLabels( GnuPlot *, void (*) ( LinkedList *, double, double ) );
void GnuPlot_setLines( GnuPlot * );
void GnuPlot_setLinesPoints( GnuPlot * );
void GnuPlot_setPoints( GnuPlot * );
void GnuPlot_setBoxes( GnuPlot * , int );
void GnuPlot_setImpulses( GnuPlot * );
void GnuPlot_setSteps( GnuPlot *, char * );
void GnuPlot_append( GnuPlot *, char * );
void GnuPlot_create(  GnuPlot * );
void GnuPlot_run( GnuPlot * );
void GnuPlot_reset( GnuPlot * );
void GnuPlot_free( GnuPlot * );
