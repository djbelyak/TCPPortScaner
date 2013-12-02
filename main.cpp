#include "scaner.h"

int main (int argc, char** argv)
{
    int socket;
    struct sockaddr_in servaddr;
    char IPAddress [17] = "127.0.0.1";
    int StartPort = 23;
    int EndPort = 1024;

    for (int iPort = StartPort; iPort <= EndPort; iPort++)
    {
        socket = Socket (PF_INET, SOCK_STREAM, 0);

        memset (&servaddr, 0, sizeof (servaddr));servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons (iPort);
        Inet_aton (IPAddress, &servaddr.sin_addr);

        try
        {
            Connect (socket, (SA *) &servaddr, sizeof (servaddr));

            printf ("Port %d is opened.\n", iPort);
            Close (socket);
        }
        catch (char const* error)
        {
            printf ("Port %d is closed.\n", iPort);
        }

    }

    return 0;
}
