#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "PacketHandlers.h"



/************** "IEEE80211RadioTap" Methods ************/



void IEEE80211RadioTap_addPacket( IEEE80211RadioTap * radiotap, const u_char * packet )
{
	//Header structure -> | Version | Padding | Length | Present | TSFT | FLAGS | RATE | ...
	//Size (bytes)     -> |    1    |    1    |    2   |    4    |   8  |   1   |   1  | ...

	radiotap->packet = packet;
	radiotap->len = (u_int16_t *) ( packet + 2 );
	radiotap->present = (u_int32_t *) ( packet + 4 );

	radiotap->length = *radiotap->len; 
}



int IEEE80211RadioTap_hasTSTF( IEEE80211RadioTap * radiotap )
{
	if( ( (*radiotap->present) & 0X1 ) == 0X1 )
		return 1;
	else
		return 0; 
}


u_int64_t IEEE80211RadioTap_getTSTF( IEEE80211RadioTap * radiotap )
{
	if( radiotap->hasTSTF(radiotap) == 1 )
	{	
		return * ( (u_int64_t *) (radiotap->packet + 8 ) );
	}
	else
		return 0;
}



int IEEE80211RadioTap_hasRate( IEEE80211RadioTap * radiotap )
{
	if( ( (*radiotap->present) & 0X4 ) == 0X4 )
		return 1;
	else
		return 0; 
}


u_int8_t IEEE80211RadioTap_getRate( IEEE80211RadioTap * radiotap )
{
	int pos = 8;

	//Find position in packet
	if( radiotap->hasTSTF(radiotap) == 1 )
		pos = pos + 8;		

	if( radiotap->hasRate(radiotap) == 1 )
	{	
		return * ( (u_int8_t *) (radiotap->packet + pos ) );
	}
	else
		return 0;
}




const u_char * IEEE80211RadioTap_getNextHeader( IEEE80211RadioTap * radiotap )
{
	return ( radiotap->packet + radiotap->length );
}


//Prints a description of a "IEEE80211RadioTap" object
void IEEE80211RadioTap_print( IEEE80211RadioTap * radiotap )
{	
	int i = 0;

	printf("\nRADIOTAP HEADER ->\n");

	printf("\n\tLength\t%d (bytes)\n", radiotap->length );
	
	printf("\n\tFIELD\t\t\tPRESENT\t\t\tVALUE\n");
	
	//Fields
	for(i=0; i<32; i++)
	{
		if(i<18 || i==30)
		{
			printf("\n\t%s\t%d", radiotap->fieldnames[i], ( (*radiotap->present) & ((int)pow(2, i)) ) >> i );
			switch(i)
			{
				case 0:
					if( radiotap->hasTSTF(radiotap) )
						printf("\t\t\t%llu (µs)", radiotap->getTSTF(radiotap) );
					break;
				case 2:
					if( radiotap->hasRate(radiotap) )
						printf("\t\t\t%u (Kb/s)", (radiotap->getRate(radiotap)*500) );
					break;
			}
		}
	}

	printf("\n\n");
	
}


void IEEE80211RadioTap_printFile( IEEE80211RadioTap * radiotap, FILE * f )
{

	int i = 0;

	fprintf(f,"\nRADIOTAP HEADER ->\n");

	fprintf(f,"\n\tLength\t%d (bytes)\n", radiotap->length );
	
	fprintf(f,"\n\tFIELD\t\t\tPRESENT\t\t\tVALUE\n");
	
	//Fields
	for(i=0; i<32; i++)
	{
		if(i<18 || i==30)
		{
			fprintf(f,"\n\t%s\t%d", radiotap->fieldnames[i], ( (*radiotap->present) & ((int)pow(2, i)) ) >> i );
			switch(i)
			{
				case 0:
					if( radiotap->hasTSTF(radiotap) )
						fprintf(f,"\t\t\t%llu (µs)", radiotap->getTSTF(radiotap) );
					break;
				case 2:
					if( radiotap->hasRate(radiotap) )
						fprintf(f,"\t\t\t%u (Kb/s)", (radiotap->getRate(radiotap)*500) );
					break;
			}
		}
	}

	printf("\n\n");

}




/*** Helper Method ***/
//Frees up memory used by a "IEEE80211RadioTap" object
void IEEE80211RadioTap_freer( void * object )
{
	((IEEE80211RadioTap*)object)->free( ((IEEE80211RadioTap*)object) );
}


void IEEE80211RadioTap_free( IEEE80211RadioTap * radiotap )
{
	free( radiotap );
}


IEEE80211RadioTap* newIEEE80211RadioTap()
{

	//Allocate memory for object
	IEEE80211RadioTap * radiotap = (IEEE80211RadioTap*)malloc(sizeof(IEEE80211RadioTap));

	radiotap->packet = NULL;
	radiotap->len = NULL;
	radiotap->present = NULL;
	radiotap->length = 0;

	//Assign Field Names
	strcpy( radiotap->fieldnames[0], "Tstf             ");
	strcpy( radiotap->fieldnames[1], "Flags            ");
	strcpy( radiotap->fieldnames[2], "Rate             ");
	strcpy( radiotap->fieldnames[3], "Channel          ");
	strcpy( radiotap->fieldnames[4], "Fhss             ");
	strcpy( radiotap->fieldnames[5], "Dbm antsignal    ");
	strcpy( radiotap->fieldnames[6], "Dbm antnoise     ");
	strcpy( radiotap->fieldnames[7], "Lock quality     ");
	strcpy( radiotap->fieldnames[8], "Tx attenuation   ");
	strcpy( radiotap->fieldnames[9], "Db tx attenuation");
	strcpy( radiotap->fieldnames[10], "Dbm tx power     ");
	strcpy( radiotap->fieldnames[11], "Antenna          ");
	strcpy( radiotap->fieldnames[12], "Db antsignal     ");
	strcpy( radiotap->fieldnames[13], "Db antnoise      ");
	strcpy( radiotap->fieldnames[14], "Rx flags         ");
	strcpy( radiotap->fieldnames[15], "Tx flags         ");
	strcpy( radiotap->fieldnames[16], "Rts retries      ");
	strcpy( radiotap->fieldnames[17], "Data retries     ");
	strcpy( radiotap->fieldnames[30], "Ext              ");

	//Assign Core Methods
	radiotap->addPacket = IEEE80211RadioTap_addPacket;
	radiotap->hasTSTF = IEEE80211RadioTap_hasTSTF;
	radiotap->getTSTF = IEEE80211RadioTap_getTSTF;
	radiotap->hasRate = IEEE80211RadioTap_hasRate;
	radiotap->getRate = IEEE80211RadioTap_getRate;
	radiotap->getNextHeader = IEEE80211RadioTap_getNextHeader;
	radiotap->print = IEEE80211RadioTap_print;
	radiotap->printFile = IEEE80211RadioTap_printFile;
	radiotap->free = IEEE80211RadioTap_free;

	//Assign Helper Methods
	radiotap->freer = IEEE80211RadioTap_freer;

	return radiotap;
}







/************** "IEEE80211" Methods ************/



//Check if the packet pointed to by a "IEEE80211" object is an "Acknowledgement" frame
int IEEE80211_isAck( IEEE80211 * ieee80211 )
{

	//FRAME CONTROL (FC) -> | Protocol | Type | Sub-Type | ToDS | FromDs | MoreFrag | Retry | PwrMgmt | MoreData | WEP | Order |
	//Size (bits)        -> |    2     |   2  |    4     |   1  |    1   |    1     |   1   |    1    |     1    |  1  |   1   |
	//Value (dec)        -> |    0     |   1  |    13    |   0  |    0   |    0     |   0   |    0    |     0    |  0  |   0   |


	if( ieee80211->fc == NULL )
		return 0;

	if( ((*(ieee80211->fc) & 0XC) >> 2) == 1 && ((*(ieee80211->fc) & 0XF0) >> 4 ) == 13 )
		return 1;
	
	else
		return 0;
}

//Check if the packet pointed to by a "IEEE80211" object is an "incoming Data" frame
int IEEE80211_isDataIn( IEEE80211 * ieee80211 )
{
	if( ieee80211->fc == NULL )
		return 0;

	//Check type and subtype
	if( ieee80211->fc != NULL && ((*(ieee80211->fc) & 0XC) >> 2) == 2 && ((*(ieee80211->fc) & 0XF0) >> 4 ) == 0 )
	{
		//to_DS and from_DS
		if( ((*(ieee80211->fc) & 0X100) >> 8) == 0 && ((*(ieee80211->fc) & 0X200) >> 9 ) == 1 )
		{
			return 1;
		}
	}

	return 0;
}

//Check if the packet pointed to by a "IEEE80211" object is an "outgoing Data" frame
int IEEE80211_isDataOut( IEEE80211 * ieee80211 )
{
	if( ieee80211->fc == NULL )
		return 0;

	//Check type and subtype
	if( ((*(ieee80211->fc) & 0XC) >> 2) == 2 && ((*(ieee80211->fc) & 0XF0) >> 4 ) == 0 )
	{
		//to_DS and from_DS
		if( ((*(ieee80211->fc) & 0X100) >> 8) == 1 && ((*(ieee80211->fc) & 0X200) >> 9 ) == 0 )
		{
			return 1;
		}
	}

	return 0;
}

//Check if the packet pointed to by a "IEEE80211" object is an "Data" frame
int IEEE80211_isData( IEEE80211 * ieee80211 )
{
	if( ieee80211->isDataIn( ieee80211 ) || ieee80211->isDataOut( ieee80211 ) )
		return 1;
	else
		return 0;
}


//Check if the packet pointed to by a "IEEE80211" object is an "Authentication" frame
int IEEE80211_isAuth( IEEE80211 * ieee80211 )
{
	//FRAME CONTROL (FC) -> | Protocol | Type | Sub-Type | ToDS | FromDs | MoreFrag | Retry | PwrMgmt | MoreData | WEP | Order |
	//Size (bits)        -> |    2     |   2  |    4     |   1  |    1   |    1     |   1   |    1    |     1    |  1  |   1   |
	//Value (dec)        -> |    0     |   0  |    11    |   0  |    0   |    0     |   0   |    0    |     0    |  0  |   0   |

	if( ieee80211->fc == NULL )
		return 0;

	//Check type and subtype
	if( ((*(ieee80211->fc) & 0XC) >> 2) == 0 && ((*(ieee80211->fc) & 0XF0) >> 4 ) == 11 )
	{
		return 1;
	}

	return 0;
}


//Check if the packet pointed to by a "IEEE80211" object is an "Authentication Request" frame
int IEEE80211_isAuthReq( IEEE80211 * ieee80211 )
{
	if( ieee80211->isAuth(ieee80211) == 0 )
		return 0;

	//Check SEQ: 0x0001 => Request, 0x0002 => Response
	if( (*ieee80211->authseq) == 1 )
	{
		return 1;
	}

	return 0;
}


//Check if the packet pointed to by a "IEEE80211" object is an "Authentication Response" frame
int IEEE80211_isAuthRes( IEEE80211 * ieee80211 )
{
	if( ieee80211->isAuth(ieee80211) == 0 )
		return 0;

	//Check SEQ: 0x0001 => Request, 0x0002 => Response
	if( (*ieee80211->authseq) == 2 )
	{
		return 1;
	}

	return 0;
}


//Check if the packet pointed to by a "IEEE80211" object is an "Deauthentication" frame
int IEEE80211_isDeAuth( IEEE80211 * ieee80211 )
{
	//FRAME CONTROL (FC) -> | Protocol | Type | Sub-Type | ToDS | FromDs | MoreFrag | Retry | PwrMgmt | MoreData | WEP | Order |
	//Size (bits)        -> |    2     |   2  |    4     |   1  |    1   |    1     |   1   |    1    |     1    |  1  |   1   |
	//Value (dec)        -> |    0     |   0  |    12    |   0  |    0   |    0     |   0   |    0    |     0    |  0  |   0   |

	if( ieee80211->fc == NULL )
		return 0;

	//Check type and subtype
	if( ((*(ieee80211->fc) & 0XC) >> 2) == 0 && ((*(ieee80211->fc) & 0XF0) >> 4 ) == 12 )
	{
		return 1;
	}

	return 0;
}


int IEEE80211_isReciever( IEEE80211 * ieee80211, u_int8_t * reciever )
{
	int i;
	if( ieee80211->ra == NULL )
		return 0;

	for( i=0; i<6; i++)
	{
		if( reciever[i] != ieee80211->ra[i] )
			return 0;
	}
	
	return 1;
}

int IEEE80211_isSender( IEEE80211 * ieee80211, u_int8_t * sender )
{
	int i;
	if( ieee80211->sa == NULL )
		return 0;

	for( i=0; i<6; i++)
	{
		if( sender[i] != ieee80211->sa[i] )
			return 0;
	}
	
	return 1;
}


int IEEE80211_isBSSID( IEEE80211 * ieee80211, u_int8_t * bssid )
{
	int i;
	if( ieee80211->bssid == NULL )
		return 0;

	for( i=0; i<6; i++)
	{
		if( bssid[i] != ieee80211->bssid[i] )
			return 0;
	}
	
	return 1;
}


int IEEE80211_isDestination( IEEE80211 * ieee80211, u_int8_t * destination )
{
	int i;
	if( ieee80211->da == NULL )
		return 0;

	for( i=0; i<6; i++)
	{
		if( destination[i] != ieee80211->da[i] )
			return 0;
	}
	
	return 1;
}


//Prints the details of the frame pointed to by a "IEEE80211" object
void IEEE80211_print( IEEE80211 * ieee80211 )
{	
	printf("\n802.11 HEADER ->\n");
	
	if( ieee80211->fc != NULL )
	{
		//Print Frame Control
		printf("\n\tFRAME CONTROL:\n");
		printf("\n\tVersion\t\t%u", *(ieee80211->fc) & 0X2 );
		printf("\n\tType\t\t%u\t(%s)", (*(ieee80211->fc) & 0XC) >> 2, ieee80211->types[((*(ieee80211->fc) & 0XC) >> 2)] );
		printf("\n\tSubType\t\t%u\t(%s)", (*(ieee80211->fc) & 0XF0) >> 4, ieee80211->subtypes[((*(ieee80211->fc) & 0XC) >> 2)][((*(ieee80211->fc) & 0XF0) >> 4)]);
		printf("\n\tTo DS\t\t%u", (*(ieee80211->fc) & 0X100) >> 8 );
		printf("\n\tFrom DS\t\t%u", (*(ieee80211->fc) & 0X200) >> 9 );
		printf("\n\tMore Frag\t%u", (*(ieee80211->fc) & 0X400) >> 10 );
		printf("\n\tRetry\t\t%u", (*(ieee80211->fc) & 0X800) >> 11 );
		printf("\n\tPwr Mgmt\t%u", (*(ieee80211->fc) & 0X1000) >> 12 );
		printf("\n\tMore Data\t%u", (*(ieee80211->fc) & 0X2000) >> 13 );
		printf("\n\tWEP\t\t%u", (*(ieee80211->fc) & 0X4000) >> 14 );
		printf("\n\tOrder\t\t%u", (*(ieee80211->fc) & 0X8000) >> 15 );

		//Print Body
		if( ieee80211->printBody != NULL )
			ieee80211->printBody( ieee80211 );
		

		printf("\n\n");
	}	
}



//Prints the details of the frame pointed to by a "IEEE80211" object
void IEEE80211_printFile( IEEE80211 * ieee80211, FILE * f )
{	
	fprintf(f,"\n802.11 HEADER ->\n");
	
	if( ieee80211->fc != NULL )
	{
		//Print Frame Control
		fprintf(f,"\n\tFRAME CONTROL:\n");
		fprintf(f,"\n\tVersion\t\t%u", *(ieee80211->fc) & 0X2 );
		fprintf(f,"\n\tType\t\t%u\t(%s)", (*(ieee80211->fc) & 0XC) >> 2, ieee80211->types[((*(ieee80211->fc) & 0XC) >> 2)] );
		fprintf(f,"\n\tSubType\t\t%u\t(%s)", (*(ieee80211->fc) & 0XF0) >> 4, ieee80211->subtypes[((*(ieee80211->fc) & 0XC) >> 2)][((*(ieee80211->fc) & 0XF0) >> 4)]);
		fprintf(f,"\n\tTo DS\t\t%u", (*(ieee80211->fc) & 0X100) >> 8 );
		fprintf(f,"\n\tFrom DS\t\t%u", (*(ieee80211->fc) & 0X200) >> 9 );
		fprintf(f,"\n\tMore Frag\t%u", (*(ieee80211->fc) & 0X400) >> 10 );
		fprintf(f,"\n\tRetry\t\t%u", (*(ieee80211->fc) & 0X800) >> 11 );
		fprintf(f,"\n\tPwr Mgmt\t%u", (*(ieee80211->fc) & 0X1000) >> 12 );
		fprintf(f,"\n\tMore Data\t%u", (*(ieee80211->fc) & 0X2000) >> 13 );
		fprintf(f,"\n\tWEP\t\t%u", (*(ieee80211->fc) & 0X4000) >> 14 );
		fprintf(f,"\n\tOrder\t\t%u", (*(ieee80211->fc) & 0X8000) >> 15 );

		//Print Body
		if( ieee80211->printBody != NULL )
			ieee80211->printBodyFile( ieee80211, f );
		

		fprintf(f,"\n\n");
	}	
}




//Prints the details of an "Acknowledgement" frame pointed to by a "IEEE80211" object
void IEEE80211_printAck( IEEE80211 * ieee80211 )
{
	printf("\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	printf("\n\tRA\t\t%X:%X:%X:%X:%X:%X", ieee80211->ra[0], ieee80211->ra[1], ieee80211->ra[2], ieee80211->ra[3], ieee80211->ra[4], ieee80211->ra[5] );
	printf("\n\tFCS\t\t0x%X%X%X%X", ieee80211->fcs[0], ieee80211->fcs[1], ieee80211->fcs[2], ieee80211->fcs[3] );
}

//Prints the details of an "Acknowledgement" frame pointed to by a "IEEE80211" object
void IEEE80211_printAckFile( IEEE80211 * ieee80211, FILE * f )
{
	fprintf(f,"\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	fprintf(f,"\n\tRA\t\t%X:%X:%X:%X:%X:%X", ieee80211->ra[0], ieee80211->ra[1], ieee80211->ra[2], ieee80211->ra[3], ieee80211->ra[4], ieee80211->ra[5] );
	fprintf(f,"\n\tFCS\t\t0x%X%X%X%X", ieee80211->fcs[0], ieee80211->fcs[1], ieee80211->fcs[2], ieee80211->fcs[3] );
}


//Prints the details of an "incoming Data" frame pointed to by a "IEEE80211" object
void IEEE80211_printDataIn( IEEE80211 * ieee80211 )
{
	printf("\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	printf("\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	printf("\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	printf("\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	printf("\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	printf("\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );
}


//Prints the details of an "incoming Data" frame pointed to by a "IEEE80211" object
void IEEE80211_printDataInFile( IEEE80211 * ieee80211, FILE * f )
{
	fprintf(f,"\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	fprintf(f,"\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	fprintf(f,"\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	fprintf(f,"\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	fprintf(f,"\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	fprintf(f,"\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );
}



//Prints the details of an "outgoing Data" frame pointed to by a "IEEE80211" object
void IEEE80211_printDataOut( IEEE80211 * ieee80211 )
{
	printf("\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	printf("\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	printf("\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	printf("\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	printf("\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	printf("\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );
}


//Prints the details of an "outgoing Data" frame pointed to by a "IEEE80211" object
void IEEE80211_printDataOutFile( IEEE80211 * ieee80211, FILE * f )
{
	fprintf(f,"\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	fprintf(f,"\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	fprintf(f,"\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	fprintf(f,"\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	fprintf(f,"\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	fprintf(f,"\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );
}




//Prints the details of an "Authentication" frame pointed to by a "IEEE80211" object
void IEEE80211_printAuth( IEEE80211 * ieee80211 )
{
	printf("\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	printf("\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	printf("\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	printf("\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	printf("\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	printf("\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );

	printf("\n\n\tFIXED PARAMETERS:\n");
	printf("\n\tAlg.\t\t%u", *ieee80211->authalg );

	if( ieee80211->isAuthReq( ieee80211 ) )
		printf("\n\tSeq\t\t%u\t(Request)", *ieee80211->authseq );
	else
		printf("\n\tSeq\t\t%u\t(Response)", *ieee80211->authseq );

	printf("\n\tStatus\t\t%u", *ieee80211->authstatus );
}



//Prints the details of an "Authentication" frame pointed to by a "IEEE80211" object to file
void IEEE80211_printAuthFile( IEEE80211 * ieee80211, FILE * f )
{
	fprintf(f,"\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	fprintf(f,"\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	fprintf(f,"\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	fprintf(f,"\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	fprintf(f,"\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	fprintf(f,"\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );

	fprintf(f,"\n\n\tFIXED PARAMETERS:\n");
	fprintf(f,"\n\tAlg.\t\t%u", *ieee80211->authalg );
	if( ieee80211->isAuthReq( ieee80211 ) )
		fprintf(f,"\n\tSeq\t\t%u\t(Request)", *ieee80211->authseq );
	else
		fprintf(f,"\n\tSeq\t\t%u\t(Response)", *ieee80211->authseq );

	fprintf(f,"\n\tStatus\t\t%u", *ieee80211->authstatus );
}




//Prints the details of an "Deauthentication" frame pointed to by a "IEEE80211" object
void IEEE80211_printDeAuth( IEEE80211 * ieee80211 )
{
	printf("\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	printf("\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	printf("\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	printf("\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	printf("\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	printf("\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );

	if( (*ieee80211->reason) >= 0 && (*ieee80211->reason) < 10 )
		printf("\n\tReason\t\t%u\t(%s)", *ieee80211->reason, ieee80211->reasoncodes[(*ieee80211->reason)] );
	else
		printf("\n\tReason\t\t%u\t(?)", *ieee80211->reason);

}



//Prints the details of an "Deauthentication" frame pointed to by a "IEEE80211" object to file
void IEEE80211_printDeAuthFile( IEEE80211 * ieee80211, FILE * f )
{
	fprintf(f,"\n\n\tDuration\t%u (µs)", (*ieee80211->duration) );
	fprintf(f,"\n\tDA\t\t%X:%X:%X:%X:%X:%X", ieee80211->da[0], ieee80211->da[1], ieee80211->da[2], ieee80211->da[3], ieee80211->da[4], ieee80211->da[5] );
	fprintf(f,"\n\tSA\t\t%X:%X:%X:%X:%X:%X", ieee80211->sa[0], ieee80211->sa[1], ieee80211->sa[2], ieee80211->sa[3], ieee80211->sa[4], ieee80211->sa[5] );
	fprintf(f,"\n\tBSSID\t\t%X:%X:%X:%X:%X:%X", ieee80211->bssid[0], ieee80211->bssid[1], ieee80211->bssid[2], ieee80211->bssid[3], ieee80211->bssid[4], ieee80211->bssid[5] );
	fprintf(f,"\n\tFrag #\t\t%u", (*ieee80211->seq) & 0XF );
	fprintf(f,"\n\tSeq #\t\t%u", ((*ieee80211->seq) & 0XFFF0) >> 4 );

	if( (*ieee80211->reason) >= 0 && (*ieee80211->reason) < 10 )
		fprintf(f,"\n\tReason\t\t%u\t(%s)", *ieee80211->reason, ieee80211->reasoncodes[(*ieee80211->reason)] );
	else
		fprintf(f,"\n\tReason\t\t%u\t(?)", *ieee80211->reason);

}







//Configure a "IEEE80211" object using the specified packet pointer
void IEEE80211_addPacket( IEEE80211 * ieee80211, const u_char * packet )
{
	ieee80211->packet = packet;
	ieee80211->fc = (u_int16_t *) packet;
	
	//Reset
	ieee80211->configure = NULL;
	ieee80211->printBody = NULL;

	if( ieee80211->isAck( ieee80211 ) )//If ACK
	{
		ieee80211->configure = IEEE80211_configureAck;
		ieee80211->printBody = IEEE80211_printAck;
		ieee80211->printBodyFile = IEEE80211_printAckFile;
	}
	else if( ieee80211->isDataIn( ieee80211 ) )//If DATA IN
	{
		ieee80211->configure = IEEE80211_configureDataIn;
		ieee80211->printBody = IEEE80211_printDataIn;
		ieee80211->printBodyFile = IEEE80211_printDataInFile;
	}
	else if( ( ieee80211->isDataOut( ieee80211 ) ))//If DATA OUT
	{
		ieee80211->configure = IEEE80211_configureDataOut;
		ieee80211->printBody = IEEE80211_printDataOut;
		ieee80211->printBodyFile = IEEE80211_printDataOutFile;
	}
	else if( ieee80211->isAuth( ieee80211 ) )//If AUTHENTICATION
	{
		ieee80211->configure = IEEE80211_configureAuth;
		ieee80211->printBody = IEEE80211_printAuth;
		ieee80211->printBodyFile = IEEE80211_printAuthFile;
	}
	else if( ieee80211->isDeAuth( ieee80211 ) )//If DEAUTHENTICATION
	{
		ieee80211->configure = IEEE80211_configureDeAuth;
		ieee80211->printBody = IEEE80211_printDeAuth;
		ieee80211->printBodyFile = IEEE80211_printDeAuthFile;
	}

	//Configure
	if( ieee80211->configure != NULL )
		ieee80211->configure( ieee80211 );

}


//Configures an "IEEE80211" object based on the structure of an "Acknowledgement" frame
void IEEE80211_configureAck( IEEE80211 * ieee80211 )
{
	//Packet structure -> | FC | Duration | RA | FCS |
	//Size (bytes)     -> | 2  |    2     | 6  |  4  |

	ieee80211->duration = (u_int16_t *) (ieee80211->packet + 2 );
	ieee80211->ra = (u_int8_t *) (ieee80211->packet + 4 );
	ieee80211->fcs = (u_int8_t *) (ieee80211->packet + 10 );
	
	ieee80211->length = 14;
}

//Configures an "IEEE80211" object based on the structure of an "incoming Data" frame
void IEEE80211_configureDataIn( IEEE80211 * ieee80211 )
{
	//Packet structure -> | FC | Duration | DA | BSSID | SA | SEQ_CTL |
	//Size (bytes)     -> | 2  |    2     | 6  |   6   | 6  |    2    |

	ieee80211->duration = (u_int16_t *) (ieee80211->packet + 2 );
	ieee80211->da = (u_int8_t *) (ieee80211->packet + 4 );
	ieee80211->bssid = (u_int8_t *) (ieee80211->packet + 10 );
	ieee80211->sa = (u_int8_t *) (ieee80211->packet + 16 );
	ieee80211->seq = (u_int16_t *) (ieee80211->packet + 22 );

	ieee80211->length = 24;
}

//Configures an "IEEE80211" object based on the structure of an "outcoming Data" frame
void IEEE80211_configureDataOut( IEEE80211 * ieee80211 )
{
	//Packet structure -> | FC | Duration | BSSID | SA | DA | SEQ_CTL |
	//Size (bytes)     -> | 2  |    2     |   6   | 6  | 6  |    2    |

	ieee80211->duration = (u_int16_t *) (ieee80211->packet + 2 );
	ieee80211->bssid = (u_int8_t *) (ieee80211->packet + 4 );
	ieee80211->sa = (u_int8_t *) (ieee80211->packet + 10 );
	ieee80211->da = (u_int8_t *) (ieee80211->packet + 16 );
	ieee80211->seq = (u_int16_t *) (ieee80211->packet + 22 );

	ieee80211->length = 24;
}


//Configures an "IEEE80211" object based on the structure of an "Authentication" frame
void IEEE80211_configureAuth( IEEE80211 * ieee80211 )
{
	//Packet structure -> | FC | Duration | DA | SA | BSSID | SEQ_CTL | AUTH_ALG | AUTH_SEQ | AUTH_STATUS |
	//Size (bytes)     -> | 2  |    2     | 6  | 6  |   6   |    2    |    2     |    2     |      2      |

	ieee80211->duration = (u_int16_t *) (ieee80211->packet + 2 );
	ieee80211->da = (u_int8_t *) (ieee80211->packet + 4 );
	ieee80211->sa = (u_int8_t *) (ieee80211->packet + 10 );
	ieee80211->bssid = (u_int8_t *) (ieee80211->packet + 16 );
	ieee80211->seq = (u_int16_t *) (ieee80211->packet + 22 );

	ieee80211->authalg = (u_int16_t *) (ieee80211->packet + 24 );
	ieee80211->authseq = (u_int16_t *) (ieee80211->packet + 26 );
	ieee80211->authstatus = (u_int16_t *) (ieee80211->packet + 28 );

	ieee80211->length = 30;
}



//Configures an "IEEE80211" object based on the structure of an "Deauthentication" frame
void IEEE80211_configureDeAuth( IEEE80211 * ieee80211 )
{
	//Packet structure -> | FC | Duration | DA | SA | BSSID | SEQ_CTL | REASON_CODE |
	//Size (bytes)     -> | 2  |    2     | 6  | 6  |   6   |    2    |      2      |

	ieee80211->duration = (u_int16_t *) (ieee80211->packet + 2 );
	ieee80211->da = (u_int8_t *) (ieee80211->packet + 4 );
	ieee80211->sa = (u_int8_t *) (ieee80211->packet + 10 );
	ieee80211->bssid = (u_int8_t *) (ieee80211->packet + 16 );
	ieee80211->seq = (u_int16_t *) (ieee80211->packet + 22 );

	ieee80211->reason = (u_int16_t *) (ieee80211->packet + 24 );

	ieee80211->length = 26;
}




const u_char * IEEE80211_getNextHeader( IEEE80211 * ieee80211 )
{
	return ( ieee80211->packet + ieee80211->length );
}



/*** Helper Method ***/
//Frees up memory used by a "IEEE80211" object
void IEEE80211_freer( void * object )
{
	((IEEE80211*)object)->free( ((IEEE80211*)object) );
}


void IEEE80211_free( IEEE80211 * ieee80211 )
{
	free( ieee80211 );
}


//Initialises a new "IEEE80211" object
IEEE80211* newIEEE80211()
{

	//Allocate memory for object
	IEEE80211 * ieee80211 = (IEEE80211*)malloc(sizeof(IEEE80211));

	ieee80211->packet = NULL;
	ieee80211->fc = NULL;
	ieee80211->duration = NULL;
	ieee80211->bssid = NULL;
	ieee80211->da = NULL;
	ieee80211->sa = NULL;
	ieee80211->ra = NULL;
	ieee80211->seq = NULL;
	ieee80211->fcs = NULL;

	ieee80211->authalg = NULL;
	ieee80211->authseq = NULL;
	ieee80211->authstatus = NULL;

	ieee80211->reason = NULL;

	ieee80211->length = 0;

	//Set Type Meanings
	strcpy( ieee80211->types[0], "Management Frame");
	strcpy( ieee80211->types[1], "Control Frame");
	strcpy( ieee80211->types[2], "Data Frame");
	strcpy( ieee80211->types[3], "Reserved Frame");

	//set SubType Meanings
	strcpy( ieee80211->subtypes[0][0], "Association Request");
	strcpy( ieee80211->subtypes[0][1], "Association Response");
	strcpy( ieee80211->subtypes[0][2], "Reassociation Request");
	strcpy( ieee80211->subtypes[0][3], "Reassociation Response");
	strcpy( ieee80211->subtypes[0][4], "Probe Request");
	strcpy( ieee80211->subtypes[0][5], "Probe Response");
	strcpy( ieee80211->subtypes[0][6], "?");
	strcpy( ieee80211->subtypes[0][7], "?");
	strcpy( ieee80211->subtypes[0][8], "Beacon");
	strcpy( ieee80211->subtypes[0][9], "ATIM");
	strcpy( ieee80211->subtypes[0][10], "Disassociation");
	strcpy( ieee80211->subtypes[0][11], "Authentication");
	strcpy( ieee80211->subtypes[0][12], "Deauthentication");
	strcpy( ieee80211->subtypes[0][13], "?");
	strcpy( ieee80211->subtypes[0][14], "?");
	strcpy( ieee80211->subtypes[0][15], "?");

	strcpy( ieee80211->subtypes[1][0], "?");
	strcpy( ieee80211->subtypes[1][1], "?");
	strcpy( ieee80211->subtypes[1][2], "?");
	strcpy( ieee80211->subtypes[1][3], "?");
	strcpy( ieee80211->subtypes[1][4], "?");
	strcpy( ieee80211->subtypes[1][5], "?");
	strcpy( ieee80211->subtypes[1][6], "?");
	strcpy( ieee80211->subtypes[1][7], "?");
	strcpy( ieee80211->subtypes[1][8], "?");
	strcpy( ieee80211->subtypes[1][9], "?");
	strcpy( ieee80211->subtypes[1][10], "PS-Poll");
	strcpy( ieee80211->subtypes[1][11], "RTS");
	strcpy( ieee80211->subtypes[1][12], "CTS");
	strcpy( ieee80211->subtypes[1][13], "ACK");
	strcpy( ieee80211->subtypes[1][14], "CF-End");
	strcpy( ieee80211->subtypes[1][15], "CF-End+CF-Ack");

	strcpy( ieee80211->subtypes[2][0], "Data");
	strcpy( ieee80211->subtypes[2][1], "Data+CF-Ack");
	strcpy( ieee80211->subtypes[2][2], "Data+CF-Poll");
	strcpy( ieee80211->subtypes[2][3], "Data+CF-Ack+CF-Poll");
	strcpy( ieee80211->subtypes[2][4], "Null Data");
	strcpy( ieee80211->subtypes[2][5], "CF-Ack");
	strcpy( ieee80211->subtypes[2][6], "CF-Poll");
	strcpy( ieee80211->subtypes[2][7], "Data+CF-Ack+CF-Poll");
	strcpy( ieee80211->subtypes[2][8], "?");
	strcpy( ieee80211->subtypes[2][9], "?");
	strcpy( ieee80211->subtypes[2][10], "?");
	strcpy( ieee80211->subtypes[2][11], "?");
	strcpy( ieee80211->subtypes[2][12], "?");
	strcpy( ieee80211->subtypes[2][13], "?");
	strcpy( ieee80211->subtypes[2][14], "?");
	strcpy( ieee80211->subtypes[2][15], "?");

	//Set Deautentication Reason Codes
	strcpy( ieee80211->reasoncodes[0], "Reserved; unused");
	strcpy( ieee80211->reasoncodes[1], "Unspecified");
	strcpy( ieee80211->reasoncodes[2], "Prior authentication is not valid");
	strcpy( ieee80211->reasoncodes[3], "Station has left the basic service area or extended service area and is deauthenticated");
	strcpy( ieee80211->reasoncodes[4], "Inactivity timer expired and station was disassociated");
	strcpy( ieee80211->reasoncodes[5], "Disassociated due to insufficient resources at the access point");
	strcpy( ieee80211->reasoncodes[6], "Incorrect frame type or subtype received from unauthenticated station");
	strcpy( ieee80211->reasoncodes[7], "Incorrect frame type or subtype received from unassociated station");
	strcpy( ieee80211->reasoncodes[8], "Station has left the basic service area or extended service area and is disassociated");
	strcpy( ieee80211->reasoncodes[9], "Association or reassociation requested before authentication is complete");
	

	//Assign Core Methods
	ieee80211->addPacket = IEEE80211_addPacket;
	ieee80211->configure = NULL;
	ieee80211->isAck = IEEE80211_isAck;
	ieee80211->isData = IEEE80211_isData;
	ieee80211->isDataIn = IEEE80211_isDataIn;
	ieee80211->isDataOut = IEEE80211_isDataOut;
	ieee80211->isAuth = IEEE80211_isAuth;
	ieee80211->isAuthReq = IEEE80211_isAuthReq;
	ieee80211->isAuthRes = IEEE80211_isAuthRes;
	ieee80211->isDeAuth = IEEE80211_isDeAuth;
	ieee80211->isReciever = IEEE80211_isReciever;
	ieee80211->isSender = IEEE80211_isSender;
	ieee80211->isBSSID = IEEE80211_isBSSID;
	ieee80211->isDestination = IEEE80211_isDestination;
	ieee80211->getNextHeader = IEEE80211_getNextHeader;
	ieee80211->print = IEEE80211_print;
	ieee80211->printFile = IEEE80211_printFile;
	ieee80211->printBody = NULL;
	ieee80211->printBodyFile = NULL;
	ieee80211->free = IEEE80211_free;
	

	//Assign Helper Methods
	ieee80211->freer = IEEE80211_freer;

	return ieee80211;
}






/************** "LLC" Methods ************/



void LLC_addPacket( LLC * llc, const u_char * packet )
{
	//Header structure -> | dsap | ssap | control | code | type |
	//Size (bytes)     -> |  1   |  1   |    1    |   3  |  2   |

	llc->packet = packet;
	llc->dsap    = (u_int8_t *) packet;
	llc->ssap    = (u_int8_t *) ( packet + 1 );
	llc->control = (u_int8_t *) ( packet + 2 );
	llc->code    = (u_int8_t *) ( packet + 3 );
	llc->type    = (u_int16_t *) ( packet + 6 );

	llc->length = 8;
}


//Prints the details of the frame pointed to by a "LLC" object
void LLC_print( LLC * llc )
{	
	printf("\nLLC HEADER ->\n");
	
	printf("\n\tDSAP\t\t%X", *llc->dsap );
	printf("\n\tSSAP\t\t%X", *llc->ssap );
	printf("\n\tControl\t\t%X", *llc->control );
	printf("\n\tCode\t\t%X%X%X", llc->code[0], llc->code[1], llc->code[2] );
	printf("\n\tType\t\t0x%X", ntohs(*llc->type) );

	printf("\n\n");	
}


//Check if the packet pointed to by a "LLC" object is an "IP" packet
int LLC_isIP( LLC * llc )
{
	if( llc->type != NULL)
	{
		if( ntohs(*llc->type) == 0x0800 )
			return 1;
	}

	return 0;
}


const u_char * LLC_getNextHeader( LLC * llc )
{
	return ( llc->packet + llc->length );
}


/*** Helper Method ***/
//Frees up memory used by a "LLC" object
void LLC_freer( void * object )
{
	((LLC*)object)->free( ((LLC*)object) );
}


void LLC_free( LLC * llc )
{
	free( llc );
}


LLC* newLLC()
{

	//Allocate memory for object
	LLC * llc = (LLC*)malloc(sizeof(LLC));

	llc->packet = NULL;
	llc->dsap = NULL;
	llc->ssap = NULL;
	llc->control = NULL;
	llc->code = NULL;
	llc->type = NULL;
	llc->length = 0;

	//Assign Core Methods
	llc->addPacket = LLC_addPacket;
	llc->isIP = LLC_isIP;
	llc->getNextHeader = LLC_getNextHeader;
	llc->print = LLC_print;
	llc->free = LLC_free;

	//Assign Helper Methods
	llc->freer = LLC_freer;

	return llc;
}














/************** "IP" Methods ************/


void IP_addPacket( IP * ip, const u_char * packet )
{
	//Header structure -> | vhl | tos | tlen | id | off | ttl | prot | hc | src | dst |
	//Size (bytes)     -> |  1  |  1  |   2  |  2 |  2  |  1  |   1  | 2  |  4  |  4  |

	ip->packet = packet;

	ip->vhl    = (u_int8_t *) packet;
	ip->tos    = (u_int8_t *) ( packet + 1 );
	ip->tlen   = (u_int16_t *) ( packet + 2 );
	ip->id     = (u_int16_t *) ( packet + 4 );
	ip->off    = (u_int16_t *) ( packet + 6 );
	ip->ttl    = (u_int8_t *) ( packet + 8 );
	ip->prot   = (u_int8_t *) ( packet + 9 );
	ip->hc     = (u_int16_t *) ( packet + 10 );
	ip->src    = (u_int8_t *) ( packet + 12 );
	ip->dst    = (u_int8_t *) ( packet + 16 );

	ip->length = (*ip->vhl & 0x0f) * 4;
}


//Prints the details of the frame pointed to by a "IP" object
void IP_print( IP * ip )
{	
	printf("\nIP HEADER ->\n");

	printf("\n\tVersion\t\t%u", (*ip->vhl & 0xf0) >> 4 );
	printf("\n\tIHL\t\t%u (words)", (*ip->vhl & 0x0f) );
	printf("\n\tDSCP\t\t%u", (*ip->tos & 0xfc) >> 2 );
	printf("\n\tECN\t\t%u", *ip->tos & 0x2 );
	printf("\n\tLength\t\t%u (bytes)", ntohs(*ip->tlen) );
	printf("\n\tID\t\t%u", ntohs(*ip->id) );
	printf("\n\tFlags\t\t%u", (ntohs(*ip->off) & 0xE000) >> 13 );
	printf("\n\tOffset\t\t%u (bytes)", ntohs(*ip->off) & 0x1FFF);
	printf("\n\tTTL\t\t%u", *ip->ttl );
	printf("\n\tProt.\t\t%u", *ip->prot );
	printf("\n\tCheck.\t\t%u", ntohs(*ip->hc) );
	printf("\n\tSrc.\t\t%u.%u.%u.%u", ip->src[0], ip->src[1], ip->src[2], ip->src[3] );
	printf("\n\tDest.\t\t%u.%u.%u.%u", ip->dst[0], ip->dst[1], ip->dst[2], ip->dst[3] );

	printf("\n\n");	
}



int IP_isSource( IP * ip, u_int8_t * source )
{
	int i;
	if( ip->src == NULL )
		return 0;

	for( i=0; i<4; i++)
	{
		if( source[i] != ip->src[i] )
			return 0;
	}
	
	return 1;
}

u_int8_t * IP_getSrc( IP * ip )
{
	return ip->src;
}


int IP_isDestination( IP * ip, u_int8_t * destination )
{
	int i;
	if( ip->dst == NULL )
		return 0;

	for( i=0; i<4; i++)
	{
		if( destination[i] != ip->dst[i] )
			return 0;
	}
	
	return 1;
}


u_int8_t * IP_getDst( IP * ip )
{
	return ip->dst;
}


//Check if the packet pointed to by a "IP" object is an "UDP" packet
int IP_isUDP( IP * ip )
{
	if( ip->prot != NULL)
	{
		if( *ip->prot == 0x11 )
			return 1;
	}

	return 0;
}


//Check if the packet pointed to by a "IP" object is an "TCP" packet
int IP_isTCP( IP * ip )
{
	if( ip->prot != NULL)
	{
		if( *ip->prot == 0x06 )
			return 1;
	}

	return 0;
}


const u_char * IP_getNextHeader( IP * ip )
{
	return ( ip->packet + ip->length );
}


/*** Helper Method ***/
//Frees up memory used by a "IP" object
void IP_freer( void * object )
{
	((IP*)object)->free( ((IP*)object) );
}


void IP_free( IP * ip )
{
	free( ip );
}




IP* newIP()
{
	//Allocate memory for object
	IP * ip = (IP*)malloc(sizeof(IP));

	ip->packet = NULL;
	ip->vhl    = NULL;
	ip->tos    = NULL;
	ip->tlen   = NULL;
	ip->id     = NULL;
	ip->off    = NULL;
	ip->ttl    = NULL;
	ip->prot   = NULL;
	ip->hc     = NULL;
	ip->src    = NULL;
	ip->dst    = NULL;

	ip->length = 0;

	//Assign Core Methods
	ip->addPacket = IP_addPacket;
	ip->isSource = IP_isSource;
	ip->isDestination = IP_isDestination;
	ip->getDst = IP_getDst;
	ip->getSrc = IP_getSrc;
	ip->isUDP = IP_isUDP;
	ip->isTCP = IP_isTCP;
	ip->getNextHeader = IP_getNextHeader;
	ip->print = IP_print;
	ip->free = IP_free;

	//Assign Helper Methods
	ip->freer = IP_freer;

	return ip;
}









/************** "UDP" Methods ************/


void UDP_addPacket( UDP * udp, const u_char * packet )
{
	//Header structure -> | sport | dport | len | check | 
	//Size (bytes)     -> |   2   |   2   |  2  |   2   |

	udp->packet  = packet;

	udp->sport   = (u_int16_t *) packet;
	udp->dport   = (u_int16_t *) ( packet + 2 );
	udp->len     = (u_int16_t *) ( packet + 4 );
	udp->check   = (u_int16_t *) ( packet + 6 );

	udp->length  = 8;
}


//Prints the details of the frame pointed to by a "UDP" object
void UDP_print( UDP * udp )
{	
	printf("\nUDP HEADER ->\n");

	printf("\n\tSrc Port\t%u", ntohs(*udp->sport) );
	printf("\n\tDst Port\t%u", ntohs(*udp->dport) );
	printf("\n\tLength\t\t%u (bytes)", ntohs(*udp->len) );
	printf("\n\tCheck\t\t%u", ntohs(*udp->check) );


	printf("\n\n");	
}



int UDP_isSrcPort( UDP * udp, u_int16_t sport )
{
	int i;
	if( udp->sport == NULL )
		return 0;

	if( sport != ntohs(*udp->sport) )
		return 0;

	return 1;
}


int UDP_isDstPort( UDP * udp, u_int16_t dport )
{
	int i;
	if( udp->dport == NULL )
		return 0;

	if( dport != ntohs(*udp->dport) )
		return 0;

	return 1;
}



const u_char * UDP_getNextHeader( UDP * udp )
{
	return ( udp->packet + udp->length );
}


/*** Helper Method ***/
//Frees up memory used by a "UDP" object
void UDP_freer( void * object )
{
	((UDP*)object)->free( ((UDP*)object) );
}


void UDP_free( UDP * udp )
{
	free( udp );
}


UDP* newUDP()
{
	//Allocate memory for object
	UDP * udp = (UDP*)malloc(sizeof(UDP));

	udp->packet = NULL;
	udp->sport   = NULL;
	udp->dport   = NULL;
	udp->len    = NULL;
	udp->check  = NULL;

	udp->length = 0;

	//Assign Core Methods
	udp->addPacket = UDP_addPacket;
	udp->isSrcPort = UDP_isSrcPort;
	udp->isDstPort = UDP_isDstPort;
	udp->getNextHeader = UDP_getNextHeader;
	udp->print = UDP_print;
	udp->free = UDP_free;

	//Assign Helper Methods
	udp->freer = UDP_freer;

	return udp;
}






/************** "NTP" Methods ************/


void NTP_addPacket( NTP * ntp, const u_char * packet )
{
	//Header structure -> | flags | stratum | poll | precision | delay | Dispersion | clockid | Ref Time | Org Time | Rec Time | Trans Time |
	//Size (bytes)     -> |   1   |    1    |  1   |     1     |   4   |     4      |    4    |    8     |    8     |    8     |      8     | 

	ntp->packet  	= packet;
	ntp->flags	= (u_int8_t *) packet;
	ntp->stratum	= (u_int8_t *) (packet + 1);
	ntp->poll	= (u_int8_t *) (packet + 2);
	ntp->precision	= (int8_t *) 	(packet + 3);
	ntp->delay_sec	= (int16_t *) 	(packet + 4);
	ntp->delay_frac	= (u_int16_t *) (packet + 6);
	ntp->disp_sec	= (u_int16_t *) (packet + 8);
	ntp->disp_frac	= (u_int16_t *) (packet + 10);
	ntp->clockid	= (u_int8_t *) (packet + 12);
	ntp->ref_sec	= (u_int32_t *) (packet + 16);
	ntp->ref_frac	= (u_int32_t *) (packet + 20);
	ntp->org_sec	= (u_int32_t *) (packet + 24);
	ntp->org_frac	= (u_int32_t *) (packet + 28);
	ntp->rec_sec	= (u_int32_t *) (packet + 32);
	ntp->rec_frac	= (u_int32_t *) (packet + 36);
	ntp->trans_sec	= (u_int32_t *) (packet + 40);
	ntp->trans_frac	= (u_int32_t *) (packet + 44);

}


//Prints the details of the frame pointed to by a "NTP" object
void NTP_print( NTP * ntp )
{	
	printf("\nNTP HEADER ->\n");

	printf("\n\tVersion\t%u\n", (*ntp->flags & 0x38) >> 3 ); 
	printf("\tLeap\t\t%u\t(%s)\n", (*ntp->flags & 0xc0) >> 6, ntp->leaps[((*ntp->flags & 0xc0) >> 6)] );
	printf("\tMode\t\t%u\t(%s)\n", *ntp->flags & 0x7, ntp->modes[(*ntp->flags & 0x7)] );
	printf("\tStratum\t\t%u\t(%s)\n", *ntp->stratum, ntp->stratums[(*ntp->stratum)] );
	printf("\tPoll\t\t%u\t(%.0f s)\n", *ntp->poll, ntp->getPoll(ntp) );
	printf("\tPrecision\t%d\t(%f s)\n", *ntp->precision, ntp->getPrecision(ntp) );
	printf("\tDelay\t\t%d.%u s\n", ntohs(*ntp->delay_sec), ntohs(*ntp->delay_frac) );
	printf("\tDispersion\t%u.%u s\n", ntohs(*ntp->disp_sec), ntohs(*ntp->disp_frac) );
	printf("\tClock ID\t%c%c%c%c\n",ntp->clockid[0], ntp->clockid[1], ntp->clockid[2], ntp->clockid[3] );
	//printf("\tReference\t%u.%u s\n", ntohl(*ntp->ref_sec), ntohl(*ntp->ref_frac) );
	//printf("\tOriginate\t%u.%u s\n", ntohl(*ntp->org_sec), ntohl(*ntp->org_frac) );
	//printf("\tRecieve\t\t%u.%u s\n", ntohl(*ntp->rec_sec), ntohl(*ntp->rec_frac) );
	//printf("\tTransmit\t%u.%u s\n", ntohl(*ntp->trans_sec), ntohl(*ntp->trans_frac) );
	printf("\tReference\t%lf s\n", ntp->getRefTime(ntp) );
	printf("\tOriginate\t%lf s\n", ntp->getOrgTime(ntp) );
	printf("\tRecieve\t\t%lf s\n", ntp->getRecTime(ntp) );
	printf("\tTransmit\t%lf s\n", ntp->getTransTime(ntp) );


	printf("\n\n");	
}


void NTP_printFile( NTP * ntp, FILE * f )
{

	fprintf(f,"\nNTP HEADER ->\n");

	fprintf(f,"\n\tVersion\t%u\n", (*ntp->flags & 0x38) >> 3 ); 
	fprintf(f,"\tLeap\t\t%u\t(%s)\n", (*ntp->flags & 0xc0) >> 6, ntp->leaps[((*ntp->flags & 0xc0) >> 6)] );
	fprintf(f,"\tMode\t\t%u\t(%s)\n", *ntp->flags & 0x7, ntp->modes[(*ntp->flags & 0x7)] );
	fprintf(f,"\tStratum\t\t%u\t(%s)\n", *ntp->stratum, ntp->stratums[(*ntp->stratum)] );
	fprintf(f,"\tPoll\t\t%u\t(%.0f s)\n", *ntp->poll, ntp->getPoll(ntp) );
	fprintf(f,"\tPrecision\t%d\t(%f s)\n", *ntp->precision, ntp->getPrecision(ntp) );
	fprintf(f,"\tDelay\t\t%d.%u s\n", ntohs(*ntp->delay_sec), ntohs(*ntp->delay_frac) );
	fprintf(f,"\tDispersion\t%u.%u s\n", ntohs(*ntp->disp_sec), ntohs(*ntp->disp_frac) );
	fprintf(f,"\tClock ID\t%c%c%c%c\n",ntp->clockid[0], ntp->clockid[1], ntp->clockid[2], ntp->clockid[3] );
	//fprintf(f,"\tReference\t%u.%u s\n", ntohl(*ntp->ref_sec), ntohl(*ntp->ref_frac) );
	//fprintf(f,"\tOriginate\t%u.%u s\n", ntohl(*ntp->org_sec), ntohl(*ntp->org_frac) );
	//fprintf(f,"\tRecieve\t\t%u.%u s\n", ntohl(*ntp->rec_sec), ntohl(*ntp->rec_frac) );
	//fprintf(f,"\tTransmit\t%u.%u s\n", ntohl(*ntp->trans_sec), ntohl(*ntp->trans_frac) );
	fprintf(f,"\tReference\t%lf s\n", ntp->getRefTime(ntp) );
	fprintf(f,"\tOriginate\t%lf s\n", ntp->getOrgTime(ntp) );
	fprintf(f,"\tRecieve\t\t%lf s\n", ntp->getRecTime(ntp) );
	fprintf(f,"\tTransmit\t%lf s\n", ntp->getTransTime(ntp) );

	fprintf(f,"\n\n");	

}


double NTP_getPoll( NTP * ntp )
{
	return pow(2.0, ((double) *ntp->poll) );
}


double NTP_getPrecision( NTP * ntp )
{
	return pow(2.0, ((double) *ntp->precision) );
}

double NTP_getRefTime( NTP * ntp )
{
	unsigned long int s =  ((unsigned long int)ntohl(*ntp->ref_sec));
	double sec = ((double) s);
	unsigned long int f = 	((unsigned long int)ntohl(*ntp->ref_frac));	
	double frac = (((double)f) / 4294967296.0);

	return (sec + frac);
}

//Returns the Complete Originate Timestamp as a Floating Point Value (i.e. a double)
double NTP_getOrgTime( NTP * ntp )
{
	unsigned long int s =  ((unsigned long int)ntohl(*ntp->org_sec));
	double sec = ((double) s);
	unsigned long int f = 	((unsigned long int)ntohl(*ntp->org_frac));	
	double frac = (((double)f) / 4294967296.0);

	return (sec + frac);
}

double NTP_getRecTime( NTP * ntp )
{
	unsigned long int s =  ((unsigned long int)ntohl(*ntp->rec_sec));
	double sec = ((double) s);
	unsigned long int f = 	((unsigned long int)ntohl(*ntp->rec_frac));	
	double frac = (((double)f) / 4294967296.0);

	return (sec + frac);
}

//Returns the Complete Transmission Timestamp as a Floating Point Value (i.e. a double)
double NTP_getTransTime( NTP * ntp )
{
	unsigned long int s =  ((unsigned long int)ntohl(*ntp->trans_sec));
	double sec = ((double) s);
	unsigned long int f = 	((unsigned long int)ntohl(*ntp->trans_frac));	
	double frac = (((double)f) / 4294967296.0);

	return (sec + frac);
}

//Returns First Part (the seconds) of the Originate Timestamp in Host Byte Order
u_int32_t NTP_getOrgS( NTP * ntp )
{
	return ntohl( *(ntp->org_sec) );
}

//Returns Second Part (the fractions)  of the Originate Timestamp in Host Byte Order
u_int32_t NTP_getOrgF( NTP * ntp )
{
	return ntohl( *(ntp->org_frac) );
}

//Returns First Part (the seconds) of the Transmission Timestamp in Host Byte Order
u_int32_t NTP_getTransS( NTP * ntp )
{
	return ntohl( *(ntp->trans_sec) );
}

//Returns Second Part (the fractions)  of the Transmission Timestamp in Host Byte Order
u_int32_t NTP_getTransF( NTP * ntp )
{
	return ntohl( *(ntp->trans_frac) );
}


/*** Helper Method ***/
//Frees up memory used by a "NTP" object
void NTP_freer( void * object )
{
	((NTP*)object)->free( ((NTP*)object) );
}


void NTP_free( NTP * ntp )
{
	free( ntp );
}


NTP* newNTP()
{
	//Allocate memory for object
	NTP * ntp = (NTP*)malloc(sizeof(NTP));

	ntp->packet = NULL;

	//Leap Indicator Meaning
	strcpy( ntp->leaps[0], "No Warning");
	strcpy( ntp->leaps[1], "Last minute has 61 seconds");
	strcpy( ntp->leaps[2], "Last minute has 59 seconds");
	strcpy( ntp->leaps[3], "Alarm - Clock not Synchronised");

	//Mode Meaning
	strcpy( ntp->modes[0], "Reserved");
	strcpy( ntp->modes[1], "Symmetric Active");
	strcpy( ntp->modes[2], "Symmetric Passive");
	strcpy( ntp->modes[3], "Client");
	strcpy( ntp->modes[4], "Server");
	strcpy( ntp->modes[5], "Broadcast");
	strcpy( ntp->modes[6], "Reserved - Control Message");
	strcpy( ntp->modes[7], "Reserved - private use");

	//Stratum Meaning
	strcpy( ntp->stratums[0], "Unspecified");
	strcpy( ntp->stratums[1], "Primary");
	strcpy( ntp->stratums[2], "Secondary");

	//Assign Core Methods
	ntp->addPacket = NTP_addPacket;
	ntp->getPoll = NTP_getPoll;
	ntp->getPrecision = NTP_getPrecision;
	ntp->getRefTime = NTP_getRefTime;
	ntp->getOrgTime = NTP_getOrgTime;
	ntp->getRecTime = NTP_getRecTime;
	ntp->getTransTime = NTP_getTransTime;
	ntp->getOrgS = NTP_getOrgS;
	ntp->getOrgF = NTP_getOrgF;
	ntp->getTransS = NTP_getTransS;
	ntp->getTransF = NTP_getTransF;
	ntp->print = NTP_print;
	ntp->printFile = NTP_printFile;
	ntp->free = NTP_free;

	//Assign Helper Methods
	ntp->freer = NTP_freer;

	return ntp;
}


