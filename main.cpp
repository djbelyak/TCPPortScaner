#include "scaner.h"

int main()
{
    int socket;
     struct sockaddr_in servaddr;


     socket = Socket(PF_INET, SOCK_STREAM, 0);

     /* Инициализировать структуру адреса сокета. */
     memset(&servaddr, 0, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(PORT);
     Inet_aton("127.0.0.1", &servaddr.sin_addr);

     try
     {
         Connect(socket, (SA *) &servaddr, sizeof(servaddr));
         Close(socket);
         printf("Port %d is opened.\n", PORT);
     }
     catch (char const* error)
     {
        printf("Port %d is closed.\n", PORT);
     }



    return 0;
}
