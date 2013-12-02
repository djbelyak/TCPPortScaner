#ifndef SCANER_H
#define SCANER_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <limits.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define PORT 80
#define MAXLINE 256

#define SA struct sockaddr

/*
 * Обработчик фатальных ошибок.
 */
void error(const char *s);

/*
 * Функция - обертка инициализации сокета
 */
int Socket(int domain, int type, int protocol);

/*
 * Функция - обертка инициализации соединения
 */
void Connect (int socket, const struct sockaddr *addr, socklen_t addrlen);

/*
 * Функция - обертка закрытия соединения
 */
void Close (int fd);

/*
 * Функция - обертка параметризации IP адреса
 */
void Inet_aton (const char *str, struct in_addr *addr);


#endif // SCANER_H
