#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"



//CHECKS IF LIST HAS ANY MORE ELEMENTS
//RETURNS 1 IF IT DOES, 0 OTHERWISE 
int listIterator_hasNext( ListIterator * iterator )
{
	if(iterator->current != NULL )
	{
		//if( iterator->current->next != NULL )
		return 1;
	}
	else
		return 0;
}

//RETURNS THE NEXT ELEMENT IN THE LIST
//RETURNS NULL IF NO MORE ITEMS ARE AVIALABLE 
void * listIterator_next( ListIterator * iterator )
{
	void * data;

	if ( iterator->hasNext( iterator ) == 1)
	{
		data = iterator->current->data;
		iterator->current = iterator->current->next;
		iterator->position++;

		return data;
	}
	else
		return NULL;
}



//RETURNS A LIST ITERATOR TO EFFICIENTLY ITERATER THROUGH LIST
ListIterator * linkedList_getIterator( LinkedList *list )
{
	//if no iterator has been initialised
	if( list->iterator == NULL )
	{
		list->iterator = (ListIterator*)malloc(sizeof(ListIterator));
		list->iterator->current = list->head;
		list->iterator->position = 1;
		//Methods
		list->iterator->hasNext = listIterator_hasNext;
		list->iterator->next = listIterator_next;
	}	
	else //reset iterator
	{
		list->iterator->current = list->head;
		list->iterator->position = 1;
	}

	return list->iterator;
}





void linkedList_setStringer ( LinkedList *list, char* (*stringer) (void*) )
{
	list->stringer = stringer;
}

void linkedList_setComparator ( LinkedList *list, int (*comparator) (void*, void*) )
{
	list->comparator = comparator;
}


void linkedList_setCopier ( LinkedList *list, void* (*copier) (void*) )
{
	list->copier = copier;
}



//SET MAXIMUM CAPACITY OF LIST
//After setting the maximum capacity any attempt to add more elements than the maximum results in:
//	* The last element been removed if a new element is added to either the front or middle
//	* The first element been removed if a new element is added to the back
void linkedList_setMaxSize( LinkedList* list, int maxsize )
{
	list->maxsize = maxsize;

	//If list is greater than max size then trim
	if( (list->maxsize > 0) && (list->size > list->maxsize) )
	{
		while(list->size > list->maxsize)
		{
			list->deleteBack(list);
		}
	}
}




//INSERT DATA AT FRONT OF LIST
void linkedList_addFront( LinkedList *list, void *data )
{
	Node *temp;

	//Create Node
	temp = (Node*)malloc(sizeof(Node));
	temp->data = data;
	temp->next = NULL;

	if( list->head == NULL )
	{
		//Insert
		list->head = temp;
		list->back = list->head;
		list->size = 1;
	}
	else{
		//Insert
		temp->next = list->head;
		list->head = temp;
		list->size++;
	
		//Check if maximum capacity exceeded
		if( (list->maxsize > 0) && (list->size > list->maxsize) )
		{
			list->deleteBack(list);
		}
	}
	
}


//INSERT DATA AT BACK OF LIST
void linkedList_addBack( LinkedList *list, void *data )
{
	Node *temp;

	if(list->head == NULL)
		list->addFront( list, data );
	else
	{   
		//Create Node
		temp = (Node*)malloc(sizeof(Node));
		temp->data = data;
		temp->next = NULL;

		//insert Node
		list->back->next = temp;
		list->back = temp;

		list->size++;

		//Check if maximum capacity exceeded
		if( (list->maxsize > 0) && (list->size > list->maxsize) )
		{
			list->deleteFront(list);
		}
	}	
}

//INSERT DATA AT SPECIFIED POSITION 
//Return 1 for success, 0 otherwise
int linkedList_addAt( LinkedList *list, void *data, int position )
{

	Node *temp = list->head;
	Node *tempb;
	int i = 0;

	if( position <= 1 )
		list->addFront( list, data );	

	else if( position == (list->size+1) )
		list->addBack( list, data );

	else if( position > (list->size+1) )
	{
		printf("\n\nThere are no nodes near position %d to add element!\n\n",position);
		return 0;	
	}
	else
	{
		//Traverse to position
		for(i=1;i<(position-1);i++)
		{
			temp = temp->next;
		}

		//Create Node
		tempb = (Node*)malloc(sizeof(Node));
		tempb->data = data;

		//Insert Node
		tempb->next = temp->next;
		temp->next = tempb;

		list->size++;

		//Check if maximum capacity exceeded
		if( (list->maxsize > 0) && (list->size > list->maxsize) )
		{
			list->deleteBack(list);
		}
	}

	return 1;
	
}


//COPIES THE CONTENTS OF THE CURRENT LIST INTO THE SPECIFIED LIST
void linkedList_copy ( struct LinkedList* list, struct LinkedList* copy )
{

	int i;

	for( i=1; i<=list->size; i++ )
	{
		copy->addBack( copy, list->copier( list->getAt( list, i ) ) );		
	}

}




//REPLACE DATA AT SPECIFIED POSITION 
//Return 1 for success, 0 otherwise
int linkedList_setAt( LinkedList *list, void *data, int position )
{

	Node *temp = list->head;
	int i = 0;


	if( position == 1 )
		list->head = data;	

	else if( position == list->size )
		list->back = data;	

	else if( position < 1 || position > list->size )
	{
		printf("\n\nThere are no nodes near position %d to set element!\n\n",position);
		return 0;
	}
	else
	{
		//Traverse to position
		for(i=1;i<=(position-1);i++)
		{
			temp = temp->next;
		}

		//Replace
		temp->data = data;
	}

	return 1;
	
}



//DELETE FROM FRONT
void linkedList_deleteFront( LinkedList *list )
{
	Node *temp = list->head;

	if( list->size > 0)
	{
		list->head = temp->next;
		
		//Free memory	
		list->freer(temp->data);
		free(temp);

		list->size--;	
	}
}



//DELETE FROM BACK
void linkedList_deleteBack( LinkedList *list )
{
	Node *temp = list->head;
	Node *tempb;

	if( list->size > 0 )
	{
		while(temp->next != NULL)
		{
			tempb = temp;
			temp = temp->next;
		}
		tempb->next = NULL;
		
		//Free memory
		list->freer(temp->data);	
		free(temp);
		
		list->back = tempb;
		list->size--;
	}	
}


//DELETE NODE AT SPECIFIED POSITION 
//Return 1 for success, 0 otherwise
int linkedList_deleteAt( LinkedList *list, int position )
{
	Node *temp = list->head;
	Node *tempb = list->head;
	int i = 0;

	if( position == 1 )
		list->deleteFront( list );
	
	else if( position == list->size )
		list->deleteBack( list );
	
	else if( position < 0 || position > list->size )
	{
		printf("\n\nThere is no node to delete position %d!\n\n",position);
		return 0;
	}
	else
	{
		for(i=1;i<position;i++)
		{
			tempb = temp;
			temp = temp->next;
		}
		tempb->next = temp->next;
		
		//Free memory
		list->freer(temp->data);
		free(temp);

		list->size--;
	}

	return 1;
		
}

//Removes the first occurrence of the specified element from the list, if it is present.
//Uses the specified compare function to compare elements
//returns 1 is element is removed, 0 otherwise
int linkedList_deleteData( LinkedList *list, void * data )
{

	Node *temp = list->head;
	Node *tempb = list->head;
	int position = 0;


	while(temp != NULL)
	{
		position++;

		if( list->comparator( temp->data, data ) == 0 )
		{
			if( position == 1 )
				list->deleteFront( list );

			else if( position == list->size )
				list->deleteBack( list );

			else
			{
				tempb->next = temp->next;
				
				//Free memory				
				list->freer(temp->data);
				free(temp);

				list->size--;
			}
			return 1;
		}

		tempb = temp;
		temp = temp->next;
	}

	return 0;	

}



//DELETE ALL ELEMENTS
void linkedList_deleteAll( LinkedList *list )
{
	Node *temp = list->head;
	Node *tempb;

	if( temp != NULL )
	{
		while(temp->next != NULL)
		{
			tempb = temp;
			temp = temp->next;
		
			//Free memory
			if( tempb->data != NULL )
				list->freer(tempb->data);

			free(tempb);

		}

		//Free memory
		if( temp->data != NULL )
			list->freer(temp->data);
		

		free(temp);
	}

	list->head = NULL;
	list->back = NULL;
	list->size = 0;
}

	



//RETURNS THE INDEX OF THE FIRST OCCURANCE OF THE SPECIFIED DATA ELEMENT
//A COMPARATOR FUNCTION MUST EXIST IN ORDER TO COMPARE DATA OBJECT
//RETURNS -1 IF DATA ELEMENT IS NOT FOUND OR A COMPARATOR FUNCTION HAS NOT BEEN SPECIFIED
int linkedList_indexOf( LinkedList *list, void * data )
{

	Node *temp = list->head;
	int index = 0;

	if(list->comparator == NULL)
		return -1;

	while(temp != NULL)
	{
		index++;

		if( list->comparator( temp->data, data ) == 0 )
		{
			return index;
		}

		temp = temp->next;
	}

	return -1;	

}


//RETURNS THE INDEX OF THE LAST OCCURANCE OF THE SPECIFIED DATA ELEMENT
//RETURNS -1 IF DATA ELEMENT IS NOT FOUND OR A COMPARATOR FUNCTION HAS NOT BEEN SPECIFIED
int linkedList_lastIndexOf( struct LinkedList* list, void * data )
{
	Node *temp = list->head;
	int position = 0;
	int index = -1;

	if(list->comparator == NULL)
		return -1;

	while(temp != NULL)
	{
		position++;

		if( list->comparator( temp->data, data ) == 0 )
		{
			index = position;
		}

		temp = temp->next;
	}

	return index;
}



void* linkedList_getFront( LinkedList* list )
{
	return list->head->data;
}


//RETURNS THE NODE AT THE SPECIFIED POSITION
Node* linkedList_getNode( LinkedList *list, int position )
{
	Node *temp = list->head;
	int i = 0;	

	if( position == 1 )
		return list->head;

	else if( position == list->size )
		return list->back;

	else if( position < 0 || position > list->size )
	{
		printf("\n\nThere is no node at this position %d!\n\n",position);
		return NULL;	
	}
	else
	{
		for(i=1;i<=(position-1);i++)
		{
			temp = temp->next;
		}
		return temp;
	}
}


//RETURNS THE DATA AT THE SPECIFIED POSITION
void * linkedList_getAt( LinkedList *list, int position )
{
	Node *temp;

	if( (temp = list->getNode( list, position )) != NULL )
	{
		return temp->data;
	}
	else
		return NULL;

}




//BUBBLE SORTS LIST USING THE SPECIFIED COMPARATOR FUNCTION 
//RETURNS -1 IF A COMPARATOR FUNCTION HAS NOT BEEN SPECIFIED, 1 otherwise
int linkedList_sortDesc( LinkedList *list )
{
	Node *temp;
	Node *tempb;
	void *data;

	if(list->comparator == NULL)
		return -1;

	for(temp = list->head; temp != NULL; temp = temp->next)
	{
		for(tempb = temp->next; tempb != NULL; tempb = tempb->next)
		{
			if( list->comparator( temp->data, tempb->data ) == 1 )
			{
				data = temp->data;
				temp->data = tempb->data;
				tempb->data = data;
			}
		}
	}

	return 1;

}




//BUBBLE SORTS LIST USING THE SPECIFIED COMPARATOR FUNCTION 
//RETURNS -1 IF A COMPARATOR FUNCTION HAS NOT BEEN SPECIFIED, 1 otherwise
int linkedList_sortAsc( LinkedList *list )
{
	Node *temp;
	Node *tempb;
	void *data;

	if(list->comparator == NULL)
		return -1;

	for(temp = list->head; temp != NULL; temp = temp->next)
	{
		for(tempb = temp->next; tempb != NULL; tempb = tempb->next)
		{
			if( list->comparator( temp->data, tempb->data ) == -1 )
			{
				data = temp->data;
				temp->data = tempb->data;
				tempb->data = data;
			}
		}
	}

	return 1;

}




//PRINTS LIST USING THE SPECIFIED PRINTER FUNCTION  
void linkedList_printAll( LinkedList *list )
{
	Node *temp;

	temp = list->head;

	int i = 1;

	if(list->stringer == NULL)
		return;

	printf("\n");
	while(temp != NULL)
	{
		printf("\n%s", list->stringer( temp->data ) );
		temp = temp->next;
		i++;
	}
	printf("\n");

}



void linkedList_printAt( LinkedList *list, int position )
{
	Node *temp;
	temp = list->head;
	int i = 0;

	if(list->stringer == NULL)
		return;

	if( position < 0 || position > list->size )
	{
		printf("\n\nThere are no nodes near position %d to print!\n\n",position);
	}
	else
	{
		for( i=1; i<=(position-1); i++ )
		{
			temp = temp->next;
		}
		printf("%s", list->stringer( temp->data ) );
	}
}



void linkedList_printAllFile( LinkedList *list, FILE *f )
{
	Node *temp;

	temp = list->head;

	int i = 1;

	if(list->stringer == NULL)
		return;

	fprintf(f,"\n");
	while(temp != NULL)
	{
		fprintf(f,"\n%s", list->stringer( temp->data ) );
		temp = temp->next;
		i++;
	}
	fprintf(f,"\n");
}


//Releases all data objects contained in a "LinkedList"
//NOTE: This function does not free the data objects themselves, one must use the "deleteAll" function for that
void linkedList_releaseAll( LinkedList * list )
{
	if( list->iterator != NULL )
		free( list->iterator );

	list->head = NULL;
	list->back = NULL;
	list->iterator = NULL;
	list->size = 0;
	list->maxsize = 0;
}


void linkedList_free( LinkedList * list )
{
	list->deleteAll( list );
	if( list->iterator != NULL )
		free( list->iterator );
	free( list );
}



//INITIALISE A NEW LINKEDLIST OBJECT
LinkedList* newLinkedList( void (*freer) (void*) )
{
	//Allocate memory for struct
	LinkedList *list = NULL;
	list = (LinkedList*)malloc(sizeof(LinkedList));
	
	//Initialise attributes
	list->head = NULL;
	list->back = NULL;
	list->iterator = NULL;
	list->size = 0;
	list->maxsize = 0;
	list->freer = freer;

	/* Assign Methods */

	//Setters
	list->setComparator = linkedList_setComparator;
	list->setStringer = linkedList_setStringer;
	list->setCopier = linkedList_setCopier;
	list->setMaxSize = linkedList_setMaxSize;
	
	//Core
	list->getIterator = linkedList_getIterator;
	list->addFront = linkedList_addFront;
	list->addBack =linkedList_addBack;
	list->addAt = linkedList_addAt;
	list->copy = linkedList_copy;
	list->setAt = linkedList_setAt;	
	list->deleteFront = linkedList_deleteFront;
	list->deleteBack = linkedList_deleteBack;
	list->deleteAt = linkedList_deleteAt;
	list->deleteData = linkedList_deleteData;
	list->deleteAll = linkedList_deleteAll;
	list->sortAsc = linkedList_sortAsc;
	list->sortDesc = linkedList_sortDesc;
	list->getNode = linkedList_getNode;
	list->indexOf = linkedList_indexOf;	
	list->lastIndexOf = linkedList_lastIndexOf;
	list->getFront = linkedList_getFront;
	list->getAt = linkedList_getAt;
	list->printAll = linkedList_printAll;
	list->printAt = linkedList_printAt;
	list->printAllFile = linkedList_printAllFile;
	list->releaseAll = linkedList_releaseAll;
	list->free = linkedList_free;

	return list;
}

