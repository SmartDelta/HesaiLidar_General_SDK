#ifndef HGUARD_SMD_HESAI_SYSINC
#define HGUARD_SMD_HESAI_SYSINC

#ifdef _WIN32
#include <winsock2.h> // Windows Sockets API
#include <ws2tcpip.h> // Voor aanvullende functies zoals getaddrinfo
#pragma comment(lib, "ws2_32.lib") // Zorg ervoor dat de WinSock-bibliotheek gelinkt wordt
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // Voor close()
#include <netdb.h> // Voor getaddrinfo()
#endif

#endif