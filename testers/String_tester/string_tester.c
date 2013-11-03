#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "String.h"


int main( int argc, char **argv )
{

	//****** CREATE ********

	//Create new "String" object
	String *s = newString("Hello");

	//Print Details
	printf("\n\n******* CREATION *******\n");
	printf("\nSTRING\t->\t%s", s->value );
	printf("\nLENGTH\t->\t%d", s->length );



	//****** APPEND ********
	s->append( s, " world!" );

	//Print Details
	printf("\n\n\n******* APPENDING *******\n");
	printf("\nSTRING\t->\t%s", s->value );
	printf("\nLENGTH\t->\t%d", s->length );


	//****** COMPARISON ********
	printf("\n\n\n******* CHARCTER COMPARISON *******\n");

	printf("\nIs \"%s\" equal to \"bitzh\"?", s->value );

	if( s->equals( s, "bitzh" ) )
		printf("\t\tYes");
	else
		printf("\t\tNo");



	printf("\n\nIs \"%s\" equal to \"Hello world!\"?", s->value );

	if( s->equals( s, "Hello world!" ) )
		printf("\tYes");
	else
		printf("\tNo");



	//****** STRING COMPARISON ********

	//Create new "String" object
	String *s2 = newString("yo");
	
	printf("\n\n\n******* STRING COMPARISON *******\n");

	printf("\nIs \"%s\" equal to \"%s\"?", s->value, s2->value );

	if( s->same( s, s2 ) )
		printf("\t\tYes");
	else
		printf("\t\tNo");


	//Reset string
	s2->set( s2, "Hello world!" );

	printf("\n\nIs \"%s\" equal to \"%s\"?", s->value, s2->value );

	if( s->same( s, s2 ) )
		printf("\tYes");
	else
		printf("\tNo");



	//****** INTEGER EXTRACTION ********

	printf("\n\n\n******* INTEGER EXTRACTION *******\n");

	int temp;

	//Reset string
	s2->set( s2, "20 on the 10th october 1985" );

	printf("\nSTRING\t->\t%s\n", s2->value );
	//get integers
	while( s2->hasMoreInt(s2) )
	{
		temp = s2->getNextInt( s2 );
		printf("\nINTEGER\t->\t%d",temp);
	}





	printf("\n\n");

	

	
}
