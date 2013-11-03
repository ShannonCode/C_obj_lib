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
#include "PacketHandlers.h"


//Declare packet handler objects
IEEE80211RadioTap* radiotap;
IEEE80211* ieee80211;
LLC* llc;
IP* ip;
UDP* udp;
NTP* ntp;


//Printd PCAP Details
void printPcap( const struct pcap_pkthdr* pkthdr )
{
	printf("\nPCAP HEADER ->\n\n");
	printf("\tTimestamp:\t%ld.%06ld(s)\n", pkthdr->ts.tv_sec, pkthdr->ts.tv_usec);
	printf("\tLength:\t\t%d (Bytes)\n", pkthdr->len);
	printf("\n");
}



//Handles Packets - Called everytime a packet is transmitted or recieved
void packet_handler( u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet )
{
	static int n = 1;
	
	printf("\n\n\n\n****************** PACKET %d ******************\n", n);

	//Print PCAP details
	printPcap( pkthdr );

	//Process Radiotap header and print
	radiotap->addPacket( radiotap, packet );
	radiotap->print( radiotap );

	//Process 802.11 header and print
	ieee80211->addPacket( ieee80211, radiotap->getNextHeader(radiotap) );
	ieee80211->print( ieee80211 );

		
	//Check if Packet is an 802.11 Data Frame
	if ( ieee80211->isDataIn( ieee80211 ) || ieee80211->isDataOut( ieee80211 ) )
	{
		//Process LLC header and print
		llc->addPacket( llc, ieee80211->getNextHeader(ieee80211) );
		llc->print( llc );

		//Check if Packet is an IP packet
		if( llc->isIP( llc ) )
		{
			//Process IP header and print
			ip->addPacket( ip, llc->getNextHeader(llc) );
			ip->print( ip );

			//Check if Packet is a UDP packet
			if( ip->isUDP( ip ) )
			{
				//Process UDP header and print
				udp->addPacket( udp, ip->getNextHeader(ip) );
				udp->print( udp );

				//Check if Packet is an NTP packet
				if( udp->isDstPort( udp, 123 ) || udp->isSrcPort( udp, 123 ) )
				{
					//Process NTP header and print
					ntp->addPacket( ntp, udp->getNextHeader(udp) );			
					ntp->print( ntp );
				}
			}
		}
	}
	
	
	
	//Increment packet count
	n++;
}




int main(int argc,char **argv)
{ 
	char *dev; 
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* handle;					//Session handle
	struct bpf_program fp;      	//Holds compiled program 
	bpf_u_int32 maskp;          	//Subnet mask
	bpf_u_int32 netp;           	//ip
	u_char* args = NULL;

	//Options passed as a string
	if(argc < 3)
	{ 
		fprintf(stdout,"\nUsage: %s <interface> <numpackets> \"filter string\"\n\n",argv[0]);
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
	printf("\n\nDevice name:\t%s",argv[1]);
	printf("\nLink Type:\t%s (%d) (%s)",pcap_datalink_val_to_name( pcap_datalink(handle) ), pcap_datalink(handle), pcap_datalink_val_to_description( pcap_datalink(handle) )  );

	if(pcap_datalink(handle) != 127)
	{
		printf("\n\nLink not supported!");
		return 0;
	}


	if(argc > 3)
	{
		//Compile filter specified by user
		if( pcap_compile( handle, &fp, argv[3], 0, netp ) == -1 )
		{ 
			fprintf(stderr,"Error calling pcap_compile\n"); 
			exit(1); 
		}

		//Register the filter
		if( pcap_setfilter( handle, &fp ) == -1 )
		{ 
			fprintf(stderr,"Error setting filter\n"); 
			exit(1); 
		}

		fprintf(stdout,"\n\nCapturing %s packet/s on interface %s using filter \"%s\"\n\n", argv[2], argv[1], argv[3]);
	}
	else
		fprintf(stdout,"\n\nCapturing %s packet/s on interface %s \n\n", argv[2], argv[1]);



	//Initialise Packet Handler Objects
	radiotap = newIEEE80211RadioTap();
	ieee80211 = newIEEE80211();
	llc = newLLC();
	ip = newIP();
	udp = newUDP();
	ntp = newNTP();


	//Sniff
	pcap_loop( handle, atoi(argv[2]), packet_handler, args );


	//Free Packet handler Objects
	radiotap->free( radiotap );;
	ieee80211->free( ieee80211 );
	llc->free( llc );
	ip->free( ip );
	udp->free( udp );
	ntp->free( ntp );

	printf("\nCOMPLETE\n\n");

	return 0;
	
}
