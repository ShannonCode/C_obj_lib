#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "GnuPlot.h"



/************** "TickMap" Object Methods ************/ 


/*** Helper Method ***/
//Frees up memory used by a "TickMap" object
void TickMap_freer( void * object )
{
	((TickMap*)object)->free( ((TickMap*)object) );
}



void TickMap_free( TickMap * map )
{
	if( map->label != NULL)
		map->label->free(map->label);	
	free( map );
}


/*** Helper Method ***/
//Compares two "TickMap" Objects 
//Returns 1 if 1st element is greater than the second element, -1 if 1st element is less than the second element, 0 otherwise
int TickMap_comparator( void *element1, void *element2)
{
	if( ((TickMap*)element1)->value > ((TickMap*)element2)->value )
	{
		return 1;
	}
	else if( ((TickMap*)element1)->value < ((TickMap*)element2)->value )
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


//Initialises a new "TickMap" Object
TickMap* newTickMap( double value, String * label )
{
	//Allocate memory for struct
	TickMap * map = NULL;
	map = (TickMap*)malloc(sizeof(TickMap));
	
	//Initialise attributes
	map->value = value;
	map->label = label;

	//Assign Core Methods
	map->free = TickMap_free;

	//Assign Helper Methods
	map->comparator = TickMap_comparator;
	map->freer = TickMap_freer;

	return map;
}





/************** "FileMap" Object Methods ************/ 


/*** Helper Method ***/
//Frees up memory used by a "FileMap" object
void FileMap_freer( void * object )
{
	((FileMap*)object)->free( ((FileMap*)object) );
}



void FileMap_free( FileMap * map )
{
	if( map->label != NULL)
		map->label->free(map->label);
	if( map->file != NULL)
		map->file->free(map->file);
	free( map );
}


//Initialises a new "FileMap" Object
FileMap* newFileMap( String * file, String * label )
{
	//Allocate memory for struct
	FileMap * map = NULL;
	map = (FileMap*)malloc(sizeof(FileMap));
	
	//Initialise attributes
	map->file = file;
	map->label = label;

	//Assign Core Methods
	map->free = FileMap_free;

	//Assign Helper Methods
	map->freer = FileMap_freer;

	return map;
}







/************** "GnuPlot" Object Methods ************/ 


void GnuPlot_addFile( GnuPlot * gp, char *file, char *label )
{
	gp->fm = newFileMap( newString(file), newString(label) );

	if( gp->fmlist == NULL)
		gp->fmlist = newLinkedList( gp->fm->freer );

	gp->fmlist->addBack( gp->fmlist, gp->fm );
}

void GnuPlot_setOutFile( GnuPlot * gp, char *file )
{
	gp->outfile->set( gp->outfile, file );
}

void GnuPlot_setTitle( GnuPlot * gp, char *title )
{
	gp->title->set( gp->title, title );
}

char * GnuPlot_getTitle( GnuPlot * gp )
{
	return gp->title->value;
}

void GnuPlot_setXLabel( GnuPlot * gp, char * label )
{
	gp->xlabel->set( gp->xlabel, label );
}

void GnuPlot_setYLabel( GnuPlot * gp, char * label )
{
	gp->ylabel->set( gp->ylabel, label );
}

void GnuPlot_setXRange( GnuPlot * gp, double start, double end )
{
	gp->xstart = start;
	gp->xend = end;
	gp->flags = gp->flags | 0x3;
}

void GnuPlot_setXStartRange( GnuPlot * gp, double start )
{
	gp->xstart = start;
	gp->flags = gp->flags | 0x1;
	//gp->flags = gp->flags & 0xD;
}


void GnuPlot_setXEndRange( GnuPlot * gp, double end )
{
	gp->xend = end;
	gp->flags = gp->flags | 0x2;
	//gp->flags = gp->flags & 0xE;
}


void GnuPlot_setYRange( GnuPlot * gp, double start, double end )
{
	gp->ystart = start;
	gp->yend = end;
	gp->flags = gp->flags | 0xC;
}


void GnuPlot_setYStartRange( GnuPlot * gp, double start )
{
	gp->ystart = start;
	gp->flags = gp->flags | 0x4;
	//gp->flags = gp->flags & 0x7;
}


void GnuPlot_setYEndRange( GnuPlot * gp, double end )
{
	gp->yend = end;
	gp->flags = gp->flags | 0x8;
	//gp->flags = gp->flags & 0xB;
}


void GnuPlot_setXTicLabels( GnuPlot * gp, void mapper ( LinkedList *, double, double ) )
{
	gp->xtics = 0.0;

	//Create linkedlist if it doesn't exist
	if( gp->xtmlist == NULL)
		gp->xtmlist = newLinkedList( gp->tm->freer );
	else //empty it if it does
		gp->xtmlist->deleteAll( gp->xtmlist );

	//Populate linkedlist using specified "mapper" function
	mapper( gp->xtmlist, gp->xstart, gp->xend );

	
}


void GnuPlot_setXTics( GnuPlot * gp, double xtics )
{
	gp->xtics = xtics;
}




void GnuPlot_setYTicLabels( GnuPlot * gp, void mapper ( LinkedList *, double, double ) )
{
	gp->ytics = 0.0;

	//Create linkedlist if it doesn't exist
	if( gp->ytmlist == NULL)
		gp->ytmlist = newLinkedList( gp->tm->freer );
	else //empty it if it does
		gp->ytmlist->deleteAll( gp->ytmlist );
	
	//Populate linkedlist using specified "mapper" function
	mapper( gp->ytmlist, gp->ystart, gp->yend );
}

void GnuPlot_setYTics( GnuPlot * gp, double ytics )
{
	gp->ytics = ytics;
}



void GnuPlot_setLines( GnuPlot * gp )
{
	gp->style->set( gp->style, "lines" );
}

void GnuPlot_setLinesPoints( GnuPlot * gp )
{
	gp->style->set( gp->style, "linespoints" );
}

void GnuPlot_setPoints( GnuPlot * gp )
{
	gp->style->set( gp->style, "points" );
}

void GnuPlot_setBoxes( GnuPlot * gp, int width )
{
	gp->style->set( gp->style, "boxes" );
	gp->boxwidth = width;
}

void GnuPlot_setImpulses( GnuPlot * gp )
{
	gp->style->set( gp->style, "impulses" );
}

void GnuPlot_setSteps( GnuPlot * gp, char * type )
{
	gp->style->set( gp->style, type );
}

//Append specified text string after plot command
void GnuPlot_append( GnuPlot * gp, char * text )
{
	gp->text->set( gp->text, text );
}


void GnuPlot_create( GnuPlot * gp )
{
	ListIterator *li;
	TickMap *tm;
	FileMap *fm;
	FILE *f;

	//Check if a data file has been specified
	if( gp->fmlist == NULL )
	{
		printf("GnuPlot: No data file specified!");
		return;
	}

	//Open file
	f = fopen( gp->outfile->value, "w" );

	//Write Label Details
	fprintf(f,"set title \"%s\" textcolor lt 3", gp->title->value );
	fprintf(f,"\nset xlabel \"%s\" textcolor lt 3", gp->xlabel->value );
	fprintf(f,"\nset ylabel \"%s\" textcolor lt 3", gp->ylabel->value );


	//Write X Ranges
	if( (gp->flags & 0x3) == 0x3 )
		fprintf(f,"\nset xrange [ %f : %f ]", gp->xstart, gp->xend);

	else if( (gp->flags & 0x1) == 0x1 )
		fprintf(f,"\nset xrange [ %f :  ]", gp->xstart);

	else if( (gp->flags & 0x2) == 0x2 )
		fprintf(f,"\nset xrange [  : %f ]", gp->xend);


	//Write Y Ranges
	if( (gp->flags & 0xC) == 0xC )
		fprintf(f,"\nset yrange [ %f : %f ]", gp->ystart, gp->yend);

	else if( (gp->flags & 0x4) == 0x4 )
		fprintf(f,"\nset yrange [ %f :  ]", gp->ystart);

	else if( (gp->flags & 0x8) == 0x8 )
		fprintf(f,"\nset yrange [  : %f ]", gp->yend);



	fprintf(f,"\nset grid");
	


	//Write X Ticks
	if( gp->xtics == 0.0 && gp->xtmlist != NULL )	
	{
		li = gp->xtmlist->getIterator( gp->xtmlist );
		if( li->hasNext( li ) )
		{
			fprintf(f,"\nset xtics ( ");
			while( li->hasNext(li) )
			{
				tm = li->next( li );
				if( li->hasNext(li) )
					fprintf(f,"\"%s\" %f,\\\n", tm->label->value, tm->value );
				else
					fprintf(f,"\"%s\" %f ", tm->label->value, tm->value );
			}
			fprintf(f,")");
		}
	}
	else if( gp->xtics > 0.0 )
	{
		fprintf(f,"\nset xtics %lf", gp->xtics );
	}


	//Write Y Ticks
	if( gp->ytics == 0.0 && gp->ytmlist != NULL )	
	{
		li = gp->ytmlist->getIterator( gp->ytmlist );
		if( li->hasNext( li ) )
		{
			fprintf(f,"\nset ytics ( ");
			while( li->hasNext(li) )
			{
				tm = li->next( li );
				if( li->hasNext(li) )
					fprintf(f,"\"%s\" %f,\\\n", tm->label->value, tm->value );
				else
					fprintf(f,"\"%s\" %f ", tm->label->value, tm->value );
			}
			fprintf(f,")");
		}
	}
	else if( gp->ytics > 0.0 )
	{
		fprintf(f,"\nset ytics %lf", gp->ytics );
	}

	if( gp->style->equals( gp->style, "boxes" ) && gp->boxwidth > -1 )
		fprintf(f,"\nset boxwidth %d", gp->boxwidth );

	//Write Plot Details
	li = gp->fmlist->getIterator( gp->fmlist );
	fprintf(f,"\nplot ");
	while( li->hasNext(li) )
	{
		fm = li->next( li );
		if( li->hasNext(li) )
			fprintf(f,"\"%s\" using 1:2 title \"%s\" with %s %s,\\\n", fm->file->value, fm->label->value, gp->style->value, gp->text->value );
		else
			fprintf(f,"\"%s\" using 1:2 title \"%s\" with %s %s", fm->file->value, fm->label->value, gp->style->value, gp->text->value );
	}
	
	//Write Image Details
	fprintf(f,"\nset terminal png size 1280,800");
	//fprintf(f,"\nset terminal postscript eps color blacktext");
	fprintf(f,"\nset out \"%s.png\"", gp->title->value );
	//fprintf(f,"\nset out \"%s.eps\"", gp->title->value );
	fprintf(f,"\nreplot");

	fclose(f);
}


void GnuPlot_run( GnuPlot * gp )
{
	int pid;

	if ( (pid = fork()) == -1 )
		perror("fork error");
	else if ( pid == 0 ) {
		execlp("gnuplot", "gnuplot", "graph.gnu", NULL );
	}

	//Give Gnuplot some time to run
	sleep(2);
}


void GnuPlot_reset( GnuPlot * gp )
{
	//Delete Items in "LinkedList" Objects
	if( gp->fmlist != NULL )
		gp->fmlist->deleteAll( gp->fmlist );

	if( gp->ytmlist != NULL )
		gp->ytmlist->deleteAll( gp->ytmlist );

	if( gp->xtmlist != NULL )
		gp->xtmlist->deleteAll( gp->xtmlist );

	//Re-Initialise attributes
	gp->title->set( gp->title, "Graph" );
	gp->xlabel->set( gp->xlabel, "X" );
	gp->ylabel->set( gp->ylabel, "Y" );
	gp->outfile->set( gp->outfile, "graph.gnu" );
	gp->style->set( gp->style, "points" );
	gp->text->set( gp->text, "" );
	gp->boxwidth = -1;
	gp->flags = 0; 
	gp->xtics = 0.0;
	gp->xstart = 0.0;
	gp->xend = 0.0;
	gp->ystart = 0.0;
	gp->yend = 0.0;
	gp->ytics = 0.0;
}


//Free up memory allocated to a "GnuPlot" object
void GnuPlot_free( GnuPlot * gp )
{
	//Free "String" objects
	gp->title->free( gp->title );
	gp->xlabel->free( gp->xlabel );
	gp->ylabel->free( gp->ylabel );
	gp->outfile->free( gp->outfile );
	gp->style->free( gp->style );
	gp->text->free( gp->text );

	//Free "TickMap"	
	if( gp->tm != NULL )
		gp->tm->free( gp->tm );
	
	//Free "LinkedList" objects
	if( gp->fmlist != NULL )
		gp->fmlist->free( gp->fmlist );
	if( gp->ytmlist != NULL )
		gp->ytmlist->free( gp->ytmlist );
	if( gp->xtmlist != NULL )
		gp->xtmlist->free( gp->xtmlist );

	free( gp );
}


//Initialises a new "GnuPlot" Object
GnuPlot* newGnuPlot()
{
	//Allocate memory for struct
	GnuPlot * gp = NULL;
	gp = (GnuPlot*)malloc(sizeof(GnuPlot));
	
	//Initialise attributes
	gp->fm = NULL;
	gp->fmlist = NULL;
	gp->title = newString("Graph");
	gp->xlabel = newString("X");
	gp->ylabel = newString("Y");
	gp->outfile = newString("graph.gnu");
	gp->style = newString("points");
	gp->text = newString("");
	gp->boxwidth = -1;
	gp->flags = 0; 
	gp->xtics = 0.0;
	gp->xstart = 0.0;
	gp->xend = 0.0;
	gp->ystart = 0.0;
	gp->yend = 0.0;
	gp->ytics = 0.0;
	gp->tm = newTickMap( 0, newString(" ") );
	gp->xtmlist = NULL; 
	gp->ytmlist = NULL;

	//Assign Core Methods
	gp->addFile = GnuPlot_addFile;
	gp->setOutFile = GnuPlot_setOutFile;
	gp->setTitle = GnuPlot_setTitle;
	gp->getTitle = GnuPlot_getTitle;
	gp->setXLabel = GnuPlot_setXLabel;
	gp->setYLabel = GnuPlot_setYLabel;
	gp->setXRange = GnuPlot_setXRange;
	gp->setXStartRange = GnuPlot_setXStartRange;
	gp->setXEndRange = GnuPlot_setXEndRange;
	gp->setYRange = GnuPlot_setYRange;
	gp->setYStartRange = GnuPlot_setYStartRange;
	gp->setYEndRange = GnuPlot_setYEndRange;
	gp->setXTics = GnuPlot_setXTics;
	gp->setXTicLabels = GnuPlot_setXTicLabels;
	gp->setYTics = GnuPlot_setYTics;
	gp->setYTicLabels = GnuPlot_setYTicLabels;
	gp->setLines = GnuPlot_setLines;
	gp->setLinesPoints = GnuPlot_setLinesPoints;
	gp->setPoints = GnuPlot_setPoints;
	gp->setBoxes = GnuPlot_setBoxes;
	gp->setImpulses = GnuPlot_setImpulses;
	gp->setSteps = GnuPlot_setSteps;
	gp->append = GnuPlot_append;
	gp->create = GnuPlot_create;
	gp->run = GnuPlot_run;
	gp->reset = GnuPlot_reset;
	gp->free = GnuPlot_free;

	return gp;
}


