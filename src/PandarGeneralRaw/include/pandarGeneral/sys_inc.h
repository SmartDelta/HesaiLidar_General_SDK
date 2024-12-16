#ifndef HGUARD_SMD_HESAI_SYSINC
#define HGUARD_SMD_HESAI_SYSINC

#ifdef _WIN32

//Low, never mind the ChatGPT comments

#include <winsock2.h> // Windows Sockets API
#include <ws2tcpip.h> // Voor aanvullende functies zoals getaddrinfo
#pragma comment(lib, "ws2_32.lib") // Zorg ervoor dat de WinSock-bibliotheek gelinkt wordt

#include <basetsd.h>

typedef SSIZE_T ssize_t;

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // Voor close()
#include <netdb.h> // Voor getaddrinfo()
#include <poll.h>

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#endif

#endif