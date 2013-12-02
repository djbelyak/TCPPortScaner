#include "scaner.h"

/*
 * Обработчик фатальных ошибок.
 */
void error (const char *s)
{
    perror (s);
    throw s;
    exit (-1);
}

/*
 * Функция - обертка инициализации сокета
 */
int Socket (int domain, int type, int protocol)
{
    int rc;

    rc = socket (domain, type, protocol);
    if (rc == -1)
        error ("socket()");

    return rc;
}

/*
 * Функция - обертка инициализации соединения
 */
void Connect (int socket, const struct sockaddr *addr, socklen_t addrlen)
{
    int rc;

    rc = connect (socket, addr, addrlen);
    if (rc == -1)
        error ("connect()");
}

/*
 * Функция - обертка закрытия соединения
 */
void Close (int fd)
{
    int rc;

    for(;;)
    {
        rc = close (fd);
        if (!rc)
            break;
        if (errno == EINTR)
            continue;
        error ("close()");
    }
}

/*
 * Функция - обертка параметризации IP адреса
 */
void Inet_aton (const char *str, struct in_addr *addr)
{
    int rc;

    rc = inet_aton (str, addr);
    if(!rc)
    {
        /* Функция inet_aton() не меняет errno в случае ошибки. Чтобы
           сообщение, выводимое error(), было более осмысленным,
           присваиваем errno значение EINVAL. */

        errno = EINVAL;
        error ("inet_aton()");
    }
}

