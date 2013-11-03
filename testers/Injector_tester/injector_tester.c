#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h> 
#include <net/ethernet.h>
#include <netinet/ether.h> 
#include <netinet/ip.h> 
#include <netinet/udp.h>
#include <time.h>
#include <unistd.h>
#include "PacketForgers.h"


//Declare Buffer to hold forged packet
u_char buffer[65536];


//Initialise MAC Addresses
u_int8_t my_mac[6] = { 0x00, 0x27, 0x19, 0xEF, 0xAF, 0x5C }; 
u_int8_t ap_mac[6] = { 0x00, 0x1D, 0xA1, 0x75, 0xB1, 0x64 }; 
u_int8_t spoof_mac[6] = { 0x00, 0x1E, 0xD0, 0x42, 0x42, 0x42 }; 


//Declare packet forger objects
IEEE80211RadioTapForger* radiotap_f;
IEEE80211Forger* ieee80211_f;



//Inject forgeded packets
void inject( pcap_t* handle )
{
	int packetsize = 0; 

	//************ INJECT 802.11 ACK ***************/
	/*
	//Create RadioTap Header
	radiotap_f->addBuffer( radiotap_f, buffer );
	packetsize = packetsize + radiotap_f->length;
	
	//Create 802.11 ACK Frame
	ieee80211_f->addBuffer( ieee80211_f, radiotap_f->getNextHeader( radiotap_f ) );
	ieee80211_f->setAck( ieee80211_f );
	ieee80211_f->setReciever( ieee80211_f, ap_mac ); //Set reciever field of ACK - who the ACK is for
	packetsize = packetsize + ieee80211_f->length;

	//Atempt injection
	printf("\nAttempting to send \"ACK\" ...\n");
	if( pcap_sendpacket(handle, buffer, packetsize ) == -1 )
			printf("Failed to inject ACK\n\n");
	else
			printf("ACK successfully injected\n\n");
	
	*/


	//************ INJECT 802.11 AUTHENTICATION REQUEST FRAME ***************/
	
	//Create RadioTap Header
	radiotap_f->addBuffer( radiotap_f, buffer );
	packetsize = packetsize + radiotap_f->length;

	//Create 802.11 Authentication Request Frame
	ieee80211_f->addBuffer( ieee80211_f, radiotap_f->getNextHeader( radiotap_f ) );
	ieee80211_f->setAuthReq( ieee80211_f );
	ieee80211_f->setDestination( ieee80211_f, ap_mac );
	ieee80211_f->setSender( ieee80211_f, spoof_mac );
	ieee80211_f->setBSSID( ieee80211_f, ap_mac );
	packetsize = packetsize + ieee80211_f->length;
	
	//Atempt injection
	printf("\nAttempting to send \"Authentication Request\" frame ...\n");
	if( pcap_sendpacket(handle, buffer, packetsize ) == -1)
			printf("Failed to inject \"Authentication Request\"\n\n");
	else
			printf("\"Authentication Request\" successfully injected\n\n");
			
	
}






int main(int argc,char **argv)
{ 
	char *dev; 
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* handle;	
	struct bpf_program fp;      	//Holds compiled program 
	bpf_u_int32 maskp;          	//Subnet mask
	bpf_u_int32 netp;           	//ip
	u_char* args = NULL;

	//Options passed as a string
	if(argc < 2)
	{ 
		fprintf(stdout,"\nUsage: %s <interface>\n\n",argv[0]);
		return 0;
	}


	//Get network address and mask of the device
	pcap_lookupnet( argv[1], &netp, &maskp, errbuf );

	//pen device for reading
	handle = pcap_open_live( argv[1], BUFSIZ, 1, -1, errbuf );
	if(handle == NULL)
	{ 
		printf("pcap_open_live(): %s\n",errbuf); 
		exit(1); 
	}
	printf("\nDevice name:\t%s",argv[1]);
	printf("\nLink Type:\t%s (%d) (%s)",pcap_datalink_val_to_name( pcap_datalink(handle) ), pcap_datalink(handle), pcap_datalink_val_to_description( pcap_datalink(handle) )  );

	if(pcap_datalink(handle) != 127)
	{
		printf("\n\nSorry, Link not supported!\n\n");
		return 0;
	}


	//Initialise Packet Forger Objects
	radiotap_f = newIEEE80211RadioTapForger();
	ieee80211_f = newIEEE80211Forger();
	
	fprintf(stdout,"\n\nInjecting packet on interface %s \n\n", argv[1]);
	
	//Inject
	inject( handle );
	

	//Free Packet Forger Objects
	radiotap_f->free( radiotap_f );;
	ieee80211_f->free( ieee80211_f );


	return 0;
}
