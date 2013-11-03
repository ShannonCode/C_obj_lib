#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PacketForgers.h"




/************** "IEEE80211RadioTapForger" Methods ************/



void IEEE80211RadioTapForger_addBuffer( IEEE80211RadioTapForger * radiotap, u_char * buffer )
{
	//Header structure -> | Version | Padding | Length | Present | TSFT | FLAGS | RATE | ...
	//Size (bytes)     -> |    1    |    1    |    2   |    4    |   8  |   1   |   1  | ...

	radiotap->buffer = buffer;
	radiotap->rev = (u_int8_t *) buffer;
	radiotap->pad = (u_int8_t *) ( buffer + 1 );
	radiotap->len = (u_int16_t *) ( buffer + 2 );
	radiotap->present = (u_int32_t *) ( buffer + 4 );

	//Configure default IEEE80211RadioTap header
	*radiotap->rev = 0x0;
	*radiotap->pad = 0x0;
	*radiotap->len = 0x0008;
	*radiotap->present = 0x00000000;

	radiotap->length = (int)(*radiotap->len); 
}




void IEEE80211RadioTapForger_updateLength( IEEE80211RadioTapForger * radiotap )
{
	*radiotap->len = 0x0008;

	//If TSTF Field present
	if( ((*radiotap->present) & 0x1) == 0x1 )
		*radiotap->len = *radiotap->len + 8; 

	//If Rate Field present
	if( ((*radiotap->present) & 0x4) == 0x4 )
		*radiotap->len = *radiotap->len + 1; 

	radiotap->length = (int)(*radiotap->len);
}


void IEEE80211RadioTapForger_setTSTF( IEEE80211RadioTapForger * radiotap, u_int64_t tstf )
{
	//Determine location in buffer and assign value
	radiotap->tstf = (u_int64_t *) ( radiotap->buffer + 8 );
	*radiotap->tstf = tstf;

	//Set "TSTF" present flag 
	*radiotap->present = (*radiotap->present) | 0x1;

	IEEE80211RadioTapForger_updateLength( radiotap );
}


void IEEE80211RadioTapForger_setRate( IEEE80211RadioTapForger * radiotap, u_int8_t rate )
{
	int pos = 8;

	//If TSTF Field present
	if( ((*radiotap->present) & 0x1) == 0x1 )
		pos = pos + 8; 

	//Determine location in buffer and assign value
	radiotap->rate = (u_int8_t *) ( radiotap->buffer + pos );
	*radiotap->rate = rate;

	//Set "Rate" present flag 
	*radiotap->present = (*radiotap->present) | 0x4;

	IEEE80211RadioTapForger_updateLength( radiotap );
}



u_char * IEEE80211RadioTapForger_getNextHeader( IEEE80211RadioTapForger * radiotap )
{
	return ( radiotap->buffer + radiotap->length );
}




//Frees up memory used by a "IEEE80211RadioTapForger" object
void IEEE80211RadioTapForger_free( IEEE80211RadioTapForger * radiotap )
{
	free( radiotap );
}


IEEE80211RadioTapForger* newIEEE80211RadioTapForger()
{

	//Allocate memory for object
	IEEE80211RadioTapForger * radiotap = (IEEE80211RadioTapForger*)malloc(sizeof(IEEE80211RadioTapForger));

	//Initialise general attributes
	radiotap->buffer = NULL;
	radiotap->rev = NULL;
	radiotap->pad = NULL;
	radiotap->len = NULL;
	radiotap->present = NULL;
	radiotap->length = 0;

	//Initialise optional field attributes
	radiotap->tstf = NULL;
	radiotap->rate = NULL;

	//Assign Core Methods
	radiotap->addBuffer = IEEE80211RadioTapForger_addBuffer;
	radiotap->setTSTF = IEEE80211RadioTapForger_setTSTF;
	radiotap->setRate = IEEE80211RadioTapForger_setRate;
	radiotap->getNextHeader = IEEE80211RadioTapForger_getNextHeader;
	radiotap->free = IEEE80211RadioTapForger_free;

	return radiotap;
}








/************** "IEEE80211Forger" Methods ************/



//Configure a "IEEE80211Forger" object using the specified buffer
void IEEE80211Forger_addBuffer( IEEE80211Forger * ieee80211, u_char * buffer )
{
	ieee80211->buffer = buffer;
	ieee80211->fc = (u_int16_t *) buffer;
	*ieee80211->fc = 0x0000;
}


//Set packet to an "ACK"
void IEEE80211Forger_setAck( IEEE80211Forger * ieee80211 )
{

	//FRAME CONTROL (FC) -> | Protocol | Type | Sub-Type | ToDS | FromDs | MoreFrag | Retry | PwrMgmt | MoreData | WEP | Order |
	//Size (bits)        -> |    2     |   2  |    4     |   1  |    1   |    1     |   1   |    1    |     1    |  1  |   1   |
	//Value (dec)        -> |    0     |   1  |    13    |   0  |    0   |    0     |   0   |    0    |     0    |  0  |   0   |

	//Configure "Frame Control" Field
	*ieee80211->fc = 0x0000;
	*ieee80211->fc = (*ieee80211->fc) | 0x4;	//Set Type - 1
	*ieee80211->fc = (*ieee80211->fc) | 0xD0;	//Set Sub-Type - 13

	//Packet structure -> | FC | Duration | RA | FCS |
	//Size (bytes)     -> | 2  |    2     | 6  |  4  |

	//Configure "Body" Field
	ieee80211->duration = (u_int16_t *) (ieee80211->buffer + 2 );
	ieee80211->ra = (u_int8_t *) (ieee80211->buffer + 4 );
	ieee80211->fcs = (u_int8_t *) (ieee80211->buffer + 10 );

	//Assign values
	*ieee80211->duration = 0x00;
	*ieee80211->ra = 0x000000000000;
	*ieee80211->fcs = 0x00000000;
	
	ieee80211->length = 14;
}



//Set packet to an "Authentication Request"
void IEEE80211Forger_setAuth( IEEE80211Forger * ieee80211 )
{

	//FRAME CONTROL (FC) -> | Protocol | Type | Sub-Type | ToDS | FromDs | MoreFrag | Retry | PwrMgmt | MoreData | WEP | Order |
	//Size (bits)        -> |    2     |   2  |    4     |   1  |    1   |    1     |   1   |    1    |     1    |  1  |   1   |
	//Value (dec)        -> |    0     |   0  |    11    |   0  |    0   |    0     |   0   |    0    |     0    |  0  |   0   |

	//Configure "Frame Control" Field
	*ieee80211->fc = 0x0000;
	*ieee80211->fc = (*ieee80211->fc) | 0xB0;	//Set Sub-Type - 11

	//Packet structure -> | FC | Duration | DA | SA | BSSID | SEQ_CTL | AUTH_ALG | AUTH_SEQ | AUTH_STATUS |
	//Size (bytes)     -> | 2  |    2     | 6  | 6  |   6   |    2    |    2     |    2     |      2      |

	//Configure "Body" Field
	ieee80211->duration = (u_int16_t *) (ieee80211->buffer + 2 );
	ieee80211->da = (u_int8_t *) (ieee80211->buffer + 4 );
	ieee80211->sa = (u_int8_t *) (ieee80211->buffer + 10 );
	ieee80211->bssid = (u_int8_t *) (ieee80211->buffer + 16 );
	ieee80211->seq = (u_int16_t *) (ieee80211->buffer + 22 );

	ieee80211->authalg = (u_int16_t *) (ieee80211->buffer + 24 );
	ieee80211->authseq = (u_int16_t *) (ieee80211->buffer + 26 );
	ieee80211->authstatus = (u_int16_t *) (ieee80211->buffer + 28 );

	//Assign values
	*ieee80211->duration = 0x13A; //314 µs
	*ieee80211->da = 0x000000000000;
	*ieee80211->sa = 0x000000000000;
	*ieee80211->bssid = 0x000000000000;
	*ieee80211->seq = 0x0000;
	*ieee80211->authalg = 0x0000;
	*ieee80211->authstatus = 0x0000;

	
	ieee80211->length = 30;

}




//Set packet to an "Deauthentication" Frame
void IEEE80211Forger_setDeAuth( IEEE80211Forger * ieee80211 )
{

	//FRAME CONTROL (FC) -> | Protocol | Type | Sub-Type | ToDS | FromDs | MoreFrag | Retry | PwrMgmt | MoreData | WEP | Order |
	//Size (bits)        -> |    2     |   2  |    4     |   1  |    1   |    1     |   1   |    1    |     1    |  1  |   1   |
	//Value (dec)        -> |    0     |   0  |    12    |   0  |    0   |    0     |   0   |    0    |     0    |  0  |   0   |

	//Configure "Frame Control" Field
	*ieee80211->fc = 0x0000;
	*ieee80211->fc = (*ieee80211->fc) | 0xC0;	//Set Sub-Type - 12

	//Packet structure -> | FC | Duration | DA | SA | BSSID | SEQ_CTL | REASON_CODE |
	//Size (bytes)     -> | 2  |    2     | 6  | 6  |   6   |    2    |      2      |

	//Configure "Body" Field
	ieee80211->duration = (u_int16_t *) (ieee80211->buffer + 2 );
	ieee80211->da = (u_int8_t *) (ieee80211->buffer + 4 );
	ieee80211->sa = (u_int8_t *) (ieee80211->buffer + 10 );
	ieee80211->bssid = (u_int8_t *) (ieee80211->buffer + 16 );
	ieee80211->seq = (u_int16_t *) (ieee80211->buffer + 22 );

	ieee80211->reason = (u_int16_t *) (ieee80211->buffer + 24 );

	//Assign values
	*ieee80211->duration = 0x75; //117 µs
	*ieee80211->da = 0x000000000000;
	*ieee80211->sa = 0x000000000000;
	*ieee80211->bssid = 0x000000000000;
	*ieee80211->seq = 0x0000;
	*ieee80211->reason = 0x0001;

	ieee80211->length = 26;

}



//Set packet to an "Authentication Request"
void IEEE80211Forger_setAuthReq( IEEE80211Forger * ieee80211 )
{
	IEEE80211Forger_setAuth( ieee80211 );
	*ieee80211->authseq = 0x0001;
}

//Set packet to an "Authentication Response"
void IEEE80211Forger_setAuthRes( IEEE80211Forger * ieee80211 )
{
	IEEE80211Forger_setAuth( ieee80211 );
	*ieee80211->authseq = 0x0002;
}



//Set packet to an "Deauthentication" Frame
void IEEE80211Forger_setDataOut( IEEE80211Forger * ieee80211 )
{

	//FRAME CONTROL (FC) -> | Protocol | Type | Sub-Type | ToDS | FromDs | MoreFrag | Retry | PwrMgmt | MoreData | WEP | Order |
	//Size (bits)        -> |    2     |   2  |    4     |   1  |    1   |    1     |   1   |    1    |     1    |  1  |   1   |
	//Value (dec)        -> |    0     |   2  |    0     |   1  |    0   |    0     |   0   |    0    |     0    |  0  |   0   |

	//Configure "Frame Control" Field
	*ieee80211->fc = 0x0000;
	*ieee80211->fc = (*ieee80211->fc) | 0x8;	//Set type = 2 and Sub-Type = 0
	*ieee80211->fc = (*ieee80211->fc) | 0x100;	//Set ToDS

	//Packet structure -> | FC | Duration | BSSID | SA | DA | SEQ_CTL | Frame Body |
	//Size (bytes)     -> | 2  |    2     |   6   | 6  | 6  |    2    |     x      |

	//Configure "Body" Field
	ieee80211->duration = (u_int16_t *) (ieee80211->buffer + 2 );
	ieee80211->bssid = (u_int8_t *) (ieee80211->buffer + 4 );
	ieee80211->sa = (u_int8_t *) (ieee80211->buffer + 10 );
	ieee80211->da = (u_int8_t *) (ieee80211->buffer + 16 );
	ieee80211->seq = (u_int16_t *) (ieee80211->buffer + 22 );

	//Assign values
	*ieee80211->duration = 0x13A; //314 µs
	*ieee80211->bssid = 0x000000000000;
	*ieee80211->sa = 0x000000000000;
	*ieee80211->da = 0x000000000000;
	*ieee80211->seq = 0x0000;

	ieee80211->length = 24;

}





void IEEE80211Forger_setReciever( IEEE80211Forger * ieee80211, u_int8_t * reciever )
{
	int i;
	if( ieee80211->ra != NULL )
	{
		for( i=0; i<6; i++)
			ieee80211->ra[i] = reciever[i];
	}
}


void IEEE80211Forger_setSender( IEEE80211Forger * ieee80211, u_int8_t * sender )
{
	int i;
	if( ieee80211->sa != NULL )
	{
		for( i=0; i<6; i++)
			ieee80211->sa[i] = sender[i];
	}
}


void IEEE80211Forger_setBSSID( IEEE80211Forger * ieee80211, u_int8_t * bssid )
{
	int i;
	if( ieee80211->bssid != NULL )
	{
		for( i=0; i<6; i++)
			ieee80211->bssid[i] = bssid[i];
	}
}


void IEEE80211Forger_setDestination( IEEE80211Forger * ieee80211, u_int8_t * destination )
{
	int i;
	if( ieee80211->da != NULL )
	{
		for( i=0; i<6; i++)
			ieee80211->da[i] = destination[i];
	}
}


void IEEE80211Forger_setFCSeq( IEEE80211Forger * ieee80211, u_int16_t seq )
{
	if( ieee80211->fc != NULL )
	{
		*ieee80211->seq = (*ieee80211->seq) | ( seq << 4 );
	}
}


void IEEE80211Forger_setFCFrag( IEEE80211Forger * ieee80211, u_int8_t frag )
{
	if( ieee80211->fc != NULL )
	{
		*ieee80211->seq = (*ieee80211->seq) | frag;
	}
}



u_char * IEEE80211Forger_getNextHeader( IEEE80211Forger * ieee80211 )
{
	return ( ieee80211->buffer + ieee80211->length );
}



void IEEE80211Forger_free( IEEE80211Forger * ieee80211 )
{
	free( ieee80211 );
}


//Initialises a new "IEEE80211Forger" object
IEEE80211Forger* newIEEE80211Forger()
{

	//Allocate memory for object
	IEEE80211Forger * ieee80211 = (IEEE80211Forger*)malloc(sizeof(IEEE80211Forger));

	ieee80211->buffer = NULL;
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

	ieee80211->length = 0;

	//Assign Core Methods
	ieee80211->addBuffer = IEEE80211Forger_addBuffer;
	ieee80211->setAck = IEEE80211Forger_setAck;
	//ieee80211->setDataIn = IEEE80211Forger_setDataIn;
	ieee80211->setDataOut = IEEE80211Forger_setDataOut;
	ieee80211->setAuthReq = IEEE80211Forger_setAuthReq;
	ieee80211->setAuthRes = IEEE80211Forger_setAuthRes;
	ieee80211->setDeAuth = IEEE80211Forger_setDeAuth;
	ieee80211->setReciever = IEEE80211Forger_setReciever;
	ieee80211->setSender = IEEE80211Forger_setSender;
	ieee80211->setBSSID = IEEE80211Forger_setBSSID;
	ieee80211->setDestination = IEEE80211Forger_setDestination;
	ieee80211->setFCSeq = IEEE80211Forger_setFCSeq;
	ieee80211->setFCFrag = IEEE80211Forger_setFCFrag;
	ieee80211->getNextHeader = IEEE80211Forger_getNextHeader;
	ieee80211->free = IEEE80211Forger_free;;

	return ieee80211;
}







/************** "LLCForger" Methods ************/



//Configure a "IEEE80211Forger" object using the specified buffer
void LLCForger_addBuffer( LLCForger * llc, u_char * buffer )
{

	//Header structure -> | dsap | ssap | control | code | type |
	//Size (bytes)     -> |  1   |  1   |    1    |   3  |  2   |
	//value            -> | 0xaa | 0xaa |  0x03   |   0  |  ?   |

	llc->buffer = buffer;
	llc->dsap = (u_int8_t *) llc->buffer;
	llc->ssap = (u_int8_t *) (llc->buffer + 1 );
	llc->control = (u_int8_t *) (llc->buffer + 2 );
	llc->code = (u_int8_t *) (llc->buffer + 3 );
	llc->type = (u_int16_t *) (llc->buffer + 6 );

	*llc->dsap = 0xaa;
	*llc->ssap = 0xaa;
	*llc->control = 0x03;
	llc->code[0] = 0x00;
	llc->code[1] = 0x00;
	llc->code[2] = 0x00;
	*llc->type = 0x0000;

	llc->length = 8;
}



void LLCForger_setTypeIP( LLCForger * llc )
{
	*llc->type = 0x0008;
}


u_char * LLCForger_getNextHeader( LLCForger * llc )
{
	return ( llc->buffer + llc->length );
}



/*** Helper Method ***/
//Frees up memory used by a "LLCForger" object
void LLCForger_freer( void * object )
{
	((LLCForger*)object)->free( ((LLCForger*)object) );
}


void LLCForger_free( LLCForger * llc )
{
	free( llc );
}


LLCForger* newLLCForger()
{

	//Allocate memory for object
	LLCForger * llc = (LLCForger*)malloc(sizeof(LLCForger));

	llc->buffer = NULL;
	llc->dsap = NULL;
	llc->ssap = NULL;
	llc->control = NULL;
	llc->code = NULL;
	llc->type = NULL;
	llc->length = 0;

	//Assign Core Methods
	llc->addBuffer = LLCForger_addBuffer;
	llc->setTypeIP = LLCForger_setTypeIP;
	llc->getNextHeader = LLCForger_getNextHeader;
	llc->free = LLCForger_free;

	//Assign Helper Methods
	llc->freer = LLCForger_freer;

	return llc;
}





/************** "IPForger" Methods ************/


void IPForger_addBuffer( IPForger * ip, u_char * buffer )
{

	//Header structure -> | vhl | tos | tlen | id | off | ttl | prot | hc | src | dst |
	//Size (bytes)     -> |  1  |  1  |   2  |  2 |  2  |  1  |   1  | 2  |  4  |  4  |

	ip->buffer = buffer;

	ip->vhl    = (u_int8_t *) ip->buffer;
	ip->tos    = (u_int8_t *) ( ip->buffer + 1 );
	ip->tlen   = (u_int16_t *) ( ip->buffer + 2 );
	ip->id     = (u_int16_t *) ( ip->buffer + 4 );
	ip->off    = (u_int16_t *) ( ip->buffer + 6 );
	ip->ttl    = (u_int8_t *) ( ip->buffer + 8 );
	ip->prot   = (u_int8_t *) ( ip->buffer + 9 );
	ip->hc     = (u_int16_t *) ( ip->buffer + 10 );
	ip->src    = (u_int8_t *) ( ip->buffer + 12 );
	ip->dst    = (u_int8_t *) ( ip->buffer + 16 );

	*ip->vhl   = 0x45;
	*ip->tos   = 0x00;
	*ip->tlen  = htons(20);
	*ip->id    = 0x00;
	*ip->off   = 0x00;
	*ip->ttl   = 0x20;
	*ip->prot  = 0x00;
	*ip->hc    = 0x0000;
	ip->src[0] = 1;
	ip->src[1] = 1;
	ip->src[2] = 1;
	ip->src[3] = 1;
	ip->dst[0] = 1;
	ip->dst[1] = 1;
	ip->dst[2] = 1;
	ip->dst[3] = 1;

	ip->length = 20;
}



void IPForger_setSrc( IPForger * ip, u_int8_t * src )
{
	int i;
	if( ip->src != NULL )
	{
		for( i=0; i<4; i++)
			ip->src[i] = src[i];
	}
}


void IPForger_setDst( IPForger * ip, u_int8_t * dst )
{
	int i;
	if( ip->dst != NULL )
	{
		for( i=0; i<4; i++)
			ip->dst[i] = dst[i];
	}
}


void IPForger_setUDP( IPForger * ip )
{
	*ip->prot  = 0x11;
}

void IPForger_setTCP( IPForger * ip )
{
	*ip->prot  = 0x06;
}


u_char * IPForger_getNextHeader( IPForger * ip )
{
	return ( ip->buffer + ip->length );
}


/*** Helper Method ***/
//Frees up memory used by a "IPForger" object
void IPForger_freer( void * object )
{
	((IPForger*)object)->free( ((IPForger*)object) );
}


void IPForger_free( IPForger * ip )
{
	free( ip );
}


IPForger* newIPForger()
{
	//Allocate memory for object
	IPForger * ip = (IPForger*)malloc(sizeof(IPForger));

	ip->buffer = NULL;
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
	ip->addBuffer = IPForger_addBuffer;
	ip->setDst = IPForger_setDst;
	ip->setSrc = IPForger_setSrc;
	ip->setUDP = IPForger_setUDP;
	ip->setTCP = IPForger_setTCP;
	ip->getNextHeader = IPForger_getNextHeader;
	ip->free = IPForger_free;

	//Assign Helper Methods
	ip->freer = IPForger_freer;

	return ip;
}









/************** "UDPForger" Methods ************/



void UDPForger_addBuffer( UDPForger * udp, u_char * buffer )
{

	//Header structure -> | sport | dport | len | check | 
	//Size (bytes)     -> |   2   |   2   |  2  |   2   |

	udp->buffer  = buffer;

	udp->sport   = (u_int16_t *) buffer;
	udp->dport   = (u_int16_t *) ( buffer + 2 );
	udp->len     = (u_int16_t *) ( buffer + 4 );
	udp->check   = (u_int16_t *) ( buffer + 6 );

	*udp->sport   = 0x0000;
	*udp->dport   = 0x0000;
	*udp->len     = htons(8);
	*udp->check   = 0x0000;

	udp->length  = 8;

}


void UDPForger_setSrcPort( UDPForger * udp, u_int16_t sport )
{
	if( udp->sport != NULL )
	{
		*udp->sport = sport;
	}
}


void UDPForger_setDstPort( UDPForger * udp, u_int16_t dport )
{
	if( udp->dport != NULL )
	{
		*udp->dport = dport;
	}
}



u_char * UDPForger_getNextHeader( UDPForger * udp )
{
	return ( udp->buffer + udp->length );
}


/*** Helper Method ***/
//Frees up memory used by a "UDPForger" object
void UDPForger_freer( void * object )
{
	((UDPForger*)object)->free( ((UDPForger*)object) );
}


void UDPForger_free( UDPForger * udp )
{
	free( udp );
}


UDPForger* newUDPForger()
{
	//Allocate memory for object
	UDPForger * udp = (UDPForger*)malloc(sizeof(UDPForger));

	udp->buffer = NULL;
	udp->sport    = NULL;
	udp->dport    = NULL;
	udp->len    = NULL;
	udp->check    = NULL;

	udp->length = 0;

	//Assign Core Methods
	udp->addBuffer = UDPForger_addBuffer;
	udp->setDstPort = UDPForger_setDstPort;
	udp->setSrcPort = UDPForger_setSrcPort;
	udp->getNextHeader = UDPForger_getNextHeader;
	udp->free = UDPForger_free;

	//Assign Helper Methods
	udp->freer = UDPForger_freer;

	return udp;
}


