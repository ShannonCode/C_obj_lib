#ifndef LINKEDLIST_H
#define LINKEDLIST_H


/************** "Node" Object************/

//"Node" object definition

typedef struct Node{
	void *data;
	struct Node *next;
}Node;





/************** "ListIterator" Object************/

//"ListIterator" object definition

typedef struct ListIterator{

	//Attributes
	Node *current;
	int position;

	//Methods
	int (*hasNext)( struct ListIterator* );
	void* (*next)( struct ListIterator* );

}ListIterator; 


//"ListIterator" method prototypes

int listIterator_hasNext( ListIterator * );
void * listIterator_next( ListIterator * );






/************** "LinkedList" Object************/

//"LinkedList" object definition

typedef struct LinkedList{

	//Attributes
	Node *head;
	Node *back;
	ListIterator *iterator;
	int size;
	int maxsize;


	//Setter Methods
	void (*setStringer) ( struct LinkedList*, char* (*) (void*) );
	void (*setComparator) ( struct LinkedList*, int (*) (void*, void*) );
	void (*setCopier) ( struct LinkedList*, void* (*) (void*) );


	//Core Methods	
	void (*setMaxSize)( struct LinkedList*, int );
	void (*addFront)( struct LinkedList*, void* );
	void (*addBack)( struct LinkedList*, void* );
	int (*addAt)( struct LinkedList*, void*, int );
	void (*copy)( struct LinkedList*, struct LinkedList* );
	int (*setAt)( struct LinkedList*, void*, int );
	void (*deleteFront)( struct LinkedList* );
	void (*deleteBack)( struct LinkedList* );
	int (*deleteAt)( struct LinkedList*, int );
	int (*deleteData)( struct LinkedList*, void* );
	void (*deleteAll)( struct LinkedList* );
	void* (*getFront)( struct LinkedList* );
	void* (*getAt)( struct LinkedList*, int );
	Node* (*getNode)( struct LinkedList*, int );
	int (*indexOf)( struct LinkedList*, void* );
	int (*lastIndexOf)( struct LinkedList*, void*  );
	ListIterator* (*getIterator)( struct LinkedList* );	
	int (*sortAsc)( struct LinkedList* );
	int (*sortDesc)( struct LinkedList* );
	void (*printAll)( struct LinkedList* );
	void (*printAt)( struct LinkedList*, int );
	void (*printAllFile)( struct LinkedList*, FILE * );
	void (*releaseAll)( struct LinkedList * );
	void (*free)( struct LinkedList * );


	//Helper Methods
	void (*freer) (void*);
	char* (*stringer) (void*);
	int (*comparator) (void*, void*);
	void* (*copier) (void*);

}LinkedList;



//"LinkedList" method prototypes

LinkedList* newLinkedList( void (*) (void*) );
void linkedList_setStringer ( LinkedList*, char* (*) (void*) );
void linkedList_setComparator ( LinkedList*, int (*) (void*, void*) );
void linkedList_setCopier ( LinkedList*, void* (*) (void*) );
void linkedList_setMaxSize( LinkedList*, int );
ListIterator* linkedList_getIterator( LinkedList* );
void linkedList_addFront( LinkedList*, void* );
void linkedList_addBack( LinkedList*, void* );
int linkedList_addAt( LinkedList*, void*, int );
void linkedList_copy( LinkedList*, LinkedList* );
int linkedList_setAt( LinkedList*, void*, int );
void linkedList_deleteFront( LinkedList* );
void linkedList_deleteBack( LinkedList* );
int linkedList_deleteAt( LinkedList*, int );
int linkedList_deleteData( LinkedList*, void* );
void linkedList_deleteAll( LinkedList* );
void* linkedList_getFront( LinkedList* );
void* linkedList_getAt( LinkedList*, int );
Node* linkedList_getNode( LinkedList*, int );
int linkedList_indexOf( LinkedList*, void* );
int linkedList_lastIndexOf( LinkedList*, void* );	
int linkedList_sortAsc( LinkedList* );
int linkedList_sortDesc( LinkedList* );
void linkedList_printAll( LinkedList* );
void linkedList_printAt( LinkedList*, int );
void linkedList_printAllFile( LinkedList*, FILE* );
void linkedList_releaseAll( LinkedList * );
void linkedList_free( LinkedList * );




#endif
