#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "String.h"



/************** "String" Object Methods ************/ 



void String_set( String * s, char * value )
{
	int size = strlen(value);
	
	if( s->value == NULL )
	{
		s->value = (char *)malloc( sizeof( char[size+1] ) );
		strcpy( s->value, value );
	}
	else if( s->length != size )
	{
		free( s->value );
		s->value = (char *)malloc( sizeof( char[size+1] ) );	
	}

	strcpy( s->value, value );
	s->currentpos = s->value;
	s->length = size;
}

/*** Helper Method ***/
//Compares two "String" Objects 
//Returns 1 if 1st element is greater than the second element, -1 if 1st element is less than the second element, 0 otherwise
int String_comparator( void *object1, void *object2)
{
	return ((String*)object1)->compare( ((String*)object1), ((String*)object1) );
}


int String_compare( String *s1, String *s2)
{
	return strcmp( s1->value, s2->value );
}


int String_equals( String * s1, char * s2 )
{
	if( strcmp( s1->value, s2 ) == 0 )
		return 1;
	else
		return 0;
}


int String_same( String *s1, String *s2)
{
	if ( strcmp( s1->value, s2->value ) == 0 )
		return 1;
	else
		return 0;
}



void String_append( String * s, char * chars )
{
	int newsize = strlen(chars) + s->length;
	char * temp;

	if( s->value == NULL )
	{
		s->set(s,chars);
		return;
	}
	else if( s->length != newsize )
	{
		temp = s->value;
		s->value = (char *)malloc( sizeof( char[newsize+1] ) );
		strcpy( s->value, temp );	
		strcat( s->value, chars );
		s->currentpos = s->value;
		s->length = newsize;
		free( temp );
	}
}



int String_hasMoreInt( String * s )
{
	//If "String" has not been set
	if(s->currentpos == NULL)
		return 0;

	int i,length;

	//If the end of the string has been reached
	length = strlen(s->currentpos);
	if( length == 0 )
	{
		s->currentpos = s->value; //Reset current pointer to begining of string
		return 0;
	}

	//Search for more valid numbers in the string
	for( i=0; i<length; i++ )
	{
		//If a valid number is found
		if( s->currentpos[i] >= 48 && s->currentpos[i] <= 57 )
			return 1;
	}

	return 0;
}


int String_getNextInt( String * s )
{
	int length, i, numfound=0, track=0;
 
	//If "String" has not been set
	if(s->currentpos == NULL)
		return 0;	

	//If the end of the string has been reached
	length = strlen(s->currentpos);
	if( length == 0 )
	{
		s->currentpos = s->value; //Reset current pointer to begining of string
		return 0;
	}

	char buff[ (length+1) ];

	//Get Next Int
	for( i=0; i<length; i++ )
	{
		//If the end of the current integer has been reached
		if( (numfound == 1) && (*s->currentpos < 48 || *s->currentpos > 57 ) )
			break;

		if ( *s->currentpos >= 48 && *s->currentpos <= 57 )//If a valed number
		{
			numfound = 1;
			buff[track] = *s->currentpos;//Add to buffer
			track++;
		}
		s->currentpos++;//Move pointer to next character
	}
	buff[track] = '\0';//Add null character to buffer		

	return (atoi(buff));//Convert string to int and return 
}


/*** Helper Method ***/
//Frees up memory used by a "String" object
void String_freer( void * object )
{
	((String*)object)->free( ((String*)object) );
}



void String_free( String * s )
{
	if(s->value != NULL)
		free( s->value );
	free( s );
}




//Initialises a new "String" Object
String* newString( char * value )
{
	//Allocate memory for struct
	String * s = NULL;
	s = (String*)malloc(sizeof(String));
	
	//Initialise attributes
	s->value = NULL;
	s->length = 0;
	s->currentpos = NULL;

	//Assign Core Methods
	s->set = String_set;
	s->equals = String_equals;
	s->same = String_same;
	s->append = String_append;
	s->compare = String_compare;
	s->hasMoreInt = String_hasMoreInt;
	s->getNextInt = String_getNextInt;
	s->free = String_free;

	//Assign Helper Methods
	s->comparator = String_comparator;
	s->freer = String_freer;

	s->set(s, value);

	return s;
}

