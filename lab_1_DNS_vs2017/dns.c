
/*************************************************************************
   LAB 1                                                                

    Edit this file ONLY!

*************************************************************************/



#include "dns.h"
#include "hash_table.h"




DNSHandle InitDNS( )
{
    return (DNSHandle)InitHashTable();
}




void LoadHostsFile( DNSHandle hDNS, const char* hostsFilePath ) {
	FILE*  hosts;
	fopen_s(&hosts, hostsFilePath, "r");
	char domain[256];
	unsigned dotIP[4];
	while (fscanf_s(hosts, "%u.%u.%u.%u\t%s\n", dotIP + 3, dotIP + 2, dotIP + 1, dotIP, domain, _countof(domain)) == 5) {
		IPADDRESS IP = 0;
		for (int i = 0; i < 4; i++) IP |= dotIP[i] << 8 * i;
		InsertToHashTable((HashTable*)hDNS, domain, IP);
	}
	fclose(hosts);
}



void ShutdownDNS( DNSHandle hDNS )
{ 
	DestroyHashTable((HashTable*)hDNS);
} 




IPADDRESS DnsLookUp( DNSHandle hDNS, const char* hostName )
{
	return FindInHashTable((HashTable*)hDNS, hostName);
}



