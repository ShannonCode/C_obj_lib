


/************** "IEEE80211RadioTap" Object ************/

//"IEEE80211RadioTap" object definition

typedef struct IEEE80211RadioTap{
	
	//Attributes
	const u_char *packet;
	u_int16_t    *len;
	u_int32_t    *present;

	int 	     length;
	char fieldnames[31][18];


	//Core Methods
	void (*addPacket)( struct IEEE80211RadioTap *, const u_char * );
	int (*hasTSTF)( struct IEEE80211RadioTap * );
	u_int64_t (*getTSTF)( struct IEEE80211RadioTap * );
	int (*hasRate)( struct IEEE80211RadioTap * );
	u_int8_t (*getRate)( struct IEEE80211RadioTap * );
	const u_char * (*getNextHeader)( struct IEEE80211RadioTap * );
	void (*print)( struct IEEE80211RadioTap * );
	void (*printFile)( struct IEEE80211RadioTap *, FILE * );
	void (*free)( struct IEEE80211RadioTap * );

	//Helper Methods
	void (*freer)( void * );

}IEEE80211RadioTap;


//"IEEE80211RadioTap" method prototypes

IEEE80211RadioTap* newIEEE80211RadioTap();
void IEEE80211RadioTap_addPacket( IEEE80211RadioTap *, const u_char * );
int IEEE80211RadioTap_hasTSTF( IEEE80211RadioTap * );
u_int64_t IEEE80211RadioTap_getTSTF( IEEE80211RadioTap * );
int IEEE80211RadioTap_hasRate( IEEE80211RadioTap * );
u_int8_t IEEE80211RadioTap_getRate( IEEE80211RadioTap * );
const u_char * IEEE80211RadioTap_getNextHeader( IEEE80211RadioTap * );
void IEEE80211RadioTap_print( IEEE80211RadioTap * );
void IEEE80211RadioTap_printFile( IEEE80211RadioTap *, FILE * );
void IEEE80211RadioTap_free( IEEE80211RadioTap * );
void IEEE80211RadioTap_freer( void * );







/************** "IEEE80211" Object************/

//"IEEE80211" object definition

typedef struct IEEE80211{
	
	//Genaral frame attributes
	const u_char *packet;
	u_int16_t    *fc;
	u_int16_t    *duration;
	u_int8_t     *bssid;
	u_int8_t     *da;
	u_int8_t     *sa;
	u_int8_t     *ra;
	u_int16_t    *seq;
	u_int8_t     *fcs;
	int 	     length;
	char types[4][17];
	char subtypes[4][16][25];

	//Authentication frame attributes
	u_int16_t     *authalg;
	u_int16_t     *authseq;
	u_int16_t     *authstatus;

	//Deauthentication frame attributes
	u_int16_t     *reason;
	char 	       reasoncodes[10][100];

	//Core Methods
	//Initialisers
	void (*addPacket)( struct IEEE80211 *, const u_char * );
	void (*configure)( struct IEEE80211 * );
	//checkers
	int (*isAck)( struct IEEE80211 * );
	int (*isData)( struct IEEE80211 * );
	int (*isDataIn)( struct IEEE80211 * );
	int (*isDataOut)( struct IEEE80211 * );
	int (*isAuth)( struct IEEE80211 * );
	int (*isAuthReq)( struct IEEE80211 * );
	int (*isAuthRes)( struct IEEE80211 * );
	int (*isDeAuth)( struct IEEE80211 * );
	int (*isReciever)( struct IEEE80211 *, u_int8_t * );
	int (*isSender)( struct IEEE80211 *, u_int8_t * );
	int (*isBSSID)( struct IEEE80211 *, u_int8_t * );
	int (*isDestination)( struct IEEE80211 *, u_int8_t * );
	//Printers
	void (*print)( struct IEEE80211 * );
	void (*printBody)( struct IEEE80211 * );
	void (*printFile)( struct IEEE80211 *, FILE * );
	void (*printBodyFile)( struct IEEE80211 *, FILE * );

	const u_char * (*getNextHeader)( struct IEEE80211 * );
	void (*free)( struct IEEE80211 * );

	//Helper Methods
	void (*freer)( void * );

}IEEE80211;



//"IEEE80211" method prototypes

//Initialisers
IEEE80211* newIEEE80211();
void IEEE80211_addPacket( IEEE80211 *, const u_char * );
void IEEE80211_configureAck( IEEE80211 * );
void IEEE80211_configureDataIn( IEEE80211 * );
void IEEE80211_configureDataOut( IEEE80211 * );
void IEEE80211_configureAuth( IEEE80211 * );
void IEEE80211_configureDeAuth( IEEE80211 * );

//Checkers
int IEEE80211_isAck( IEEE80211 * );
int IEEE80211_isData( IEEE80211 * );
int IEEE80211_isDataIn( IEEE80211 * );
int IEEE80211_isDataOut( IEEE80211 * );
int IEEE80211_isAuth( IEEE80211 * );
int IEEE80211_isAuthReq( IEEE80211 * );
int IEEE80211_isAuthRes( IEEE80211 * );
int IEEE80211_isDeAuth( IEEE80211 * );
int IEEE80211_isReciever( IEEE80211 *, u_int8_t * );
int IEEE80211_isSender( IEEE80211 *, u_int8_t * );
int IEEE80211_isBSSID( IEEE80211 *, u_int8_t * );
int IEEE80211_isDestination( IEEE80211 *, u_int8_t * );

//Printers
void IEEE80211_print( IEEE80211 * );
void IEEE80211_printFile( IEEE80211 *, FILE * );
void IEEE80211_printAck( IEEE80211 * );
void IEEE80211_printAckFile( IEEE80211 *, FILE * );
void IEEE80211_printDataIn( IEEE80211 * );
void IEEE80211_printDataInFile( IEEE80211 *, FILE * );
void IEEE80211_printDataOut( IEEE80211 * );
void IEEE80211_printDataOutFile( IEEE80211 *, FILE * );
void IEEE80211_printAuth( IEEE80211 * );
void IEEE80211_printAuthFile( IEEE80211 *, FILE * );

const u_char * IEEE80211_getNextHeader( IEEE80211 * );
void IEEE80211_free( IEEE80211 * );

//Helpers
void IEEE80211_freer( void * );






/************** "LLC" Object ************/


//"LLC" object definition

typedef struct LLC{
	
	//Attributes
	const u_char *packet;
	u_int8_t     *dsap;
	u_int8_t     *ssap;
	u_int8_t     *control;
	u_int8_t     *code;//3
	u_int16_t    *type;
	int 	     length;


	//Core Methods
	void (*addPacket)( struct LLC *, const u_char * );
	int (*isIP)( struct LLC * );
	const u_char * (*getNextHeader)( struct LLC * );
	void (*print)( struct LLC * );
	void (*free)( struct LLC * );

	//Helper Methods
	void (*freer)( void * );

}LLC;


//"LLC" method prototypes

LLC* newLLC();
void LLC_addPacket( LLC *, const u_char * );
int isIP( LLC * );
const u_char * LLC_getNextHeader( LLC * );
void LLC_print( LLC * );
void LLC_free( LLC * );
void LLC_freer( void * );







/************** "IP" Object ************/

//"IP" object definition

typedef struct IP{
	
	//Attributes
	const u_char *packet;
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
	void (*addPacket)( struct IP *, const u_char * );
	int (*isSource)( struct IP *, u_int8_t * );
	u_int8_t * (*getSrc)( struct IP * );
	int (*isDestination)( struct IP *, u_int8_t * );
	u_int8_t * (*getDst)( struct IP * );
	int (*isUDP)( struct IP * );
	int (*isTCP)( struct IP * );
	const u_char * (*getNextHeader)( struct IP * );
	void (*print)( struct IP * );
	void (*free)( struct IP * );

	//Helper Methods
	void (*freer)( void * );

}IP;


//"IP" method prototypes

IP* newIP();
void IP_addPacket( IP *, const u_char * );
int isSource( IP *, u_int8_t );
u_int8_t * getSrc( IP * );
int isDestination( IP *, u_int8_t );
u_int8_t * getDst( IP * );
int isUDP( IP * );
int isTCP( IP * );
const u_char * IP_getNextHeader( IP * );
void IP_print( IP * );
void IP_free( IP * );
void IP_freer( void * );






/************** "UDP" Object ************/


//"UDP" object definition

typedef struct UDP{
	
	//Attributes
	const u_char *packet;
	u_int16_t    *sport;
	u_int16_t    *dport;
	u_int16_t    *len;
	u_int16_t    *check;

	int 	     length;

	//Core Methods
	void (*addPacket)( struct UDP *, const u_char * );
	int (*isSrcPort)( struct UDP *, u_int16_t  );
	int (*isDstPort)( struct UDP *, u_int16_t  );
	const u_char * (*getNextHeader)( struct UDP * );
	void (*print)( struct UDP * );
	void (*free)( struct UDP * );

	//Helper Methods
	void (*freer)( void * );

}UDP;


//"UDP" method prototypes

UDP* newUDP();
void UDP_addPacket( UDP *, const u_char * );
int UDP_isSrcPort( UDP *, u_int16_t  );
int UDP_isDstPort( UDP *, u_int16_t  );
const u_char * UDP_getNextHeader( UDP * );
void UDP_print( UDP * );
void UDP_free( UDP * );
void UDP_freer( void * );






/************** "NTP" Object ************/

//"NTP" object definition

typedef struct NTP{
	
	//Attributes
	const u_char *packet;
	u_int8_t     *flags;
	u_int8_t     *stratum;
	u_int8_t     *poll;
	int8_t       *precision;
	int16_t      *delay_sec;
	u_int16_t    *delay_frac;
	u_int16_t    *disp_sec;
	u_int16_t    *disp_frac;
	u_int8_t     *clockid;//4
	u_int32_t    *ref_sec;
	u_int32_t    *ref_frac;
	u_int32_t    *org_sec;
	u_int32_t    *org_frac;
	u_int32_t    *rec_sec;
	u_int32_t    *rec_frac;
	u_int32_t    *trans_sec;
	u_int32_t    *trans_frac;

	
	char leaps[4][31];
	char modes[8][27];
	char stratums[3][12];

	//Core Methods
	void (*addPacket)( struct NTP *, const u_char * );
	double (*getPoll)( struct NTP * );
	double (*getPrecision)( struct NTP * );
	double (*getRefTime)( struct NTP * );
	double (*getOrgTime)( struct NTP * );
	double (*getRecTime)( struct NTP * );
	double (*getTransTime)( struct NTP * );
	u_int32_t (*getOrgS)( struct NTP * );
	u_int32_t (*getOrgF)( struct NTP * );
	u_int32_t (*getTransS)( struct NTP * );
	u_int32_t (*getTransF)( struct NTP * );
	double (*getTransFrac)( struct NTP * );
	void (*print)( struct NTP * );
	void (*printFile)( struct NTP *, FILE * );
	void (*free)( struct NTP * );

	//Helper Methods
	void (*freer)( void * );

}NTP;


//"NTP" method prototypes

NTP* newNTP();
void NTP_addPacket( NTP *, const u_char * );
double NTP_getPoll( NTP * );
double NTP_getPrecision( NTP * );
double NTP_getRefTime( NTP * );
double NTP_getOrgTime( NTP * );
double NTP_getRecTime( NTP * );
double NTP_getTransTime( NTP * );
u_int32_t getOrgS( NTP * );
u_int32_t getOrgF( NTP * );
u_int32_t getTransS( NTP * );
u_int32_t getTransF( NTP * );
void NTP_print( NTP * );
void NTP_printFile( NTP *, FILE * );
void NTP_free( NTP * );
void NTP_freer( void * );



