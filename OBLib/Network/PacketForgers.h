

/************** "IEEE80211RadioTapForger" Object ************/

//"IEEE80211RadioTapForger" object definition

typedef struct IEEE80211RadioTapForger{
	
	//General attributes
	u_char	     *buffer;
	u_int8_t     *rev;
	u_int8_t     *pad;
	u_int16_t    *len;
	u_int32_t    *present;
	int 	     length;

	//Optional field attributes
	u_int64_t    *tstf;
	u_int8_t     *rate;


	//Core Methods
	void (*addBuffer)( struct IEEE80211RadioTapForger *, u_char * );
	void (*setTSTF)( struct IEEE80211RadioTapForger *, u_int64_t );
	void (*setRate)( struct IEEE80211RadioTapForger *, u_int8_t );
	u_char * (*getNextHeader)( struct IEEE80211RadioTapForger * );
	void (*free)( struct IEEE80211RadioTapForger * );

}IEEE80211RadioTapForger;



//"IEEE80211RadioTapForger" method prototypes

IEEE80211RadioTapForger* newIEEE80211RadioTapForger();
void IEEE80211RadioTapForger_addBuffer( IEEE80211RadioTapForger *, u_char * );
void IEEE80211RadioTapForger_setTSTF( IEEE80211RadioTapForger *, u_int64_t );
void IEEE80211RadioTapForger_setRate( IEEE80211RadioTapForger *, u_int8_t );
u_char * IEEE80211RadioTapForger_getNextHeader( IEEE80211RadioTapForger * );
void IEEE80211RadioTapForger_free( IEEE80211RadioTapForger * );
void IEEE80211RadioTapForger_updateLength( IEEE80211RadioTapForger * );








/************** "IEEE80211Forger" Object************/

//"IEEE80211Forger" object definition

typedef struct IEEE80211Forger{
	
	//Genaral frame attributes
	u_char       *buffer;
	u_int16_t    *fc;
	u_int16_t    *duration;
	u_int8_t     *bssid;
	u_int8_t     *da;
	u_int8_t     *sa;
	u_int8_t     *ra;
	u_int16_t    *seq;
	u_int8_t     *fcs;
	int 	     length;

	//Authentication frame attributes
	u_int16_t     *authalg;
	u_int16_t     *authseq;
	u_int16_t     *authstatus;

	//Deauthentication frame attributes
	u_int16_t     *reason;


	//Core Methods
	//Initialisers
	void (*addBuffer)( struct IEEE80211Forger *, u_char * );

	//Setters
	void (*setAck)( struct IEEE80211Forger * );
	void (*setDataIn)( struct IEEE80211Forger * );
	void (*setDataOut)( struct IEEE80211Forger * );
	void (*setAuthReq)( struct IEEE80211Forger * );
	void (*setAuthRes)( struct IEEE80211Forger * );
	void (*setDeAuth)( struct IEEE80211Forger * );
	void (*setReciever)( struct IEEE80211Forger *, u_int8_t * );
	void (*setSender)( struct IEEE80211Forger *, u_int8_t * );
	void (*setBSSID)( struct IEEE80211Forger *, u_int8_t * );
	void (*setDestination)( struct IEEE80211Forger *, u_int8_t * );
	void (*setFCSeq)( struct IEEE80211Forger *, u_int16_t );
	void (*setFCFrag)( struct IEEE80211Forger *, u_int8_t );

	u_char * (*getNextHeader)( struct IEEE80211Forger * );
	void (*free)( struct IEEE80211Forger * );


}IEEE80211Forger;



//"IEEE80211Forger" method prototypes

//Initialisers
IEEE80211Forger* newIEEE80211Forger();
void IEEE80211Forger_addBuffer( IEEE80211Forger *, u_char * );

//Setters
void IEEE80211Forger_setAck( IEEE80211Forger * );
void IEEE80211Forger_setDataIn( IEEE80211Forger * );
void IEEE80211Forger_setDataOut( IEEE80211Forger * );
void IEEE80211Forger_setAuth( IEEE80211Forger * );
void IEEE80211Forger_setAuthReq( IEEE80211Forger * );
void IEEE80211Forger_setAuthRes( IEEE80211Forger * );
void IEEE80211Forger_setDeAuth( IEEE80211Forger * );
void IEEE80211Forger_setReciever( IEEE80211Forger *, u_int8_t * );
void IEEE80211Forger_setSender( IEEE80211Forger *, u_int8_t * );
void IEEE80211Forger_setBSSID( IEEE80211Forger *, u_int8_t * );
void IEEE80211Forger_setDestination( IEEE80211Forger *, u_int8_t * );
void IEEE80211Forger_setFCSeq( IEEE80211Forger *, u_int16_t );
void IEEE80211Forger_setFCFrag( IEEE80211Forger *, u_int8_t );

u_char * IEEE80211Forger_getNextHeader( IEEE80211Forger * );
void IEEE80211Forger_free( IEEE80211Forger * );





/************** "LLCForger" Object ************/


//"LLCForger" object definition

typedef struct LLCForger{
	
	//Attributes
	u_char       *buffer;
	u_int8_t     *dsap;
	u_int8_t     *ssap;
	u_int8_t     *control;
	u_int8_t     *code;//3
	u_int16_t    *type;
	int 	     length;


	//Core Methods
	void (*addBuffer)( struct LLCForger *, u_char * );
	void (*setTypeIP)( struct LLCForger * );
	u_char * (*getNextHeader)( struct LLCForger * );
	void (*free)( struct LLCForger * );

	//Helper Methods
	void (*freer)( void * );

}LLCForger;


//"LLCForger" method prototypes
LLCForger* newLLCForger();
void LLCForger_addBuffer( LLCForger *, u_char * );
void LLCForger_setTypeIP( LLCForger * );
u_char * LLCForger_getNextHeader( LLCForger * );
void LLCForger_free( LLCForger * );
void LLCForger_freer( void * );







/************** "IPForger" Object ************/

//"IPForger" object definition

typedef struct IPForger{
	
	//Attributes
	u_char	     *buffer;
	u_int8_t     *vhl;	// Header length & version
	u_int8_t     *tos;	// Type of service 
	u_int16_t    *tlen;	// Total length 
	u_int16_t    *id;	// Identification 
	u_int16_t    *off;	// Fragment offset field 
	u_int8_t     *ttl;	// Time to live 
	u_int8_t     *prot;	// Protocol 
	u_int16_t    *hc;	// Checksum 
	u_int8_t     *src;	// Source address 
	u_int8_t     *dst;	// Dest address  

	int 	     length;

	//Core Methods
	void (*addBuffer)( struct IPForger *, u_char * );
	void (*setSrc)( struct IPForger *, u_int8_t * );
	void (*setDst)( struct IPForger *, u_int8_t * );
	void (*setUDP)( struct IPForger * );
	void (*setTCP)( struct IPForger * );
	u_char * (*getNextHeader)( struct IPForger * );
	void (*free)( struct IPForger * );

	//Helper Methods
	void (*freer)( void * );

}IPForger;


//"IPForger" method prototyp
IPForger* newIPForger();
void IPForger_addBuffer( IPForger *, u_char * );;
void IPForger_setSrc( IPForger *, u_int8_t * );
void IPForger_setDst( IPForger *, u_int8_t * );
void IPForger_setUDP( IPForger * );
void IPForger_setTCP( IPForger * );
u_char * IPForger_getNextHeader( IPForger * );
void IPForger_free( IPForger * );
void IPForger_freer( void * );






/************** "UDP" Object ************/

//"UDP" object definition

typedef struct UDPForger{
	
	//Attributes
	u_char	     *buffer;
	u_int16_t    *sport;
	u_int16_t    *dport;
	u_int16_t    *len;
	u_int16_t    *check;

	int 	     length;

	//Core Methods
	void (*addBuffer)( struct UDPForger *, u_char * );
	void (*setSrcPort)( struct UDPForger *, u_int16_t );
	void (*setDstPort)( struct UDPForger *, u_int16_t );
	u_char * (*getNextHeader)( struct UDPForger * );
	void (*free)( struct UDPForger * );

	//Helper Methods
	void (*freer)( void * );


}UDPForger;


//"UDPForger" method prototypes

UDPForger* newUDPForger();
void UDPForger_addBuffer( UDPForger *, u_char * );;
void UDPForger_setSrcPort( UDPForger *, u_int16_t );
void UDPForger_setDstPort( UDPForger *, u_int16_t );
u_char * UDPForger_getNextHeader( UDPForger * );
void UDPForger_free( UDPForger * );
void UDPForger_freer( void * );
