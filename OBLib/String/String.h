#ifndef STRING_H
#define STRING_H

/************** "String" Object************/

//"String" object definition{
typedef struct String{

	//Attributes
	char * value;
	int length;
	char * currentpos;

	//Core Methods
	void (*set)( struct String *, char * );
	void (*append)( struct String *, char * );
	int (*equals)( struct String *, char * );
	int (*same)( struct String *, struct String * );
	int (*compare)( struct String *, struct String *);
	int (*hasMoreInt)( struct String * );
	int (*getNextInt)( struct String * );
	void (*free)( struct String * );

	//Helper methods
	int (*comparator)( void *, void *);
	void (*freer)( void * );

}String;


//"String" method prototypes
String* newString( char * );
void String_set( String * , char * );
void String_append( String *, char * );
int String_equals( String *, char * );
int String_same ( String *, String *);
int String_compare ( String *, String *);
int hasMoreInt( String * );
int String_getNextInt( String * );
void String_free( String * );
int String_comparator( void *, void *);
void String_freer( void * );


#endif
