/*
002  * Шаблон TCP клиента.
003  *
004  * Компиляция:
005  *      cc -Wall -O2 -o client client.c
006  *
007  * Завершение работы клиента: Ctrl+D.
008  */
009 
010 #include <arpa/inet.h>
011 #include <errno.h>
012 #include <netinet/in.h>
013 #include <stdio.h>
014 #include <stdlib.h>
015 #include <string.h>
016 #include <sys/types.h>
017 #include <sys/select.h>
018 #include <sys/socket.h>
019 #include <unistd.h>
020 
021 #include <limits.h>
022 
023 #define MAX(a, b) ((a) > (b) ? (a) : (b))
024 
025 #define PORT 1027
026 #define MAXLINE 256
027 
028 #define SA struct sockaddr 
029 
030 /*
031  * Обработчик фатальных ошибок.
032  */
033 void error(const char *s)
034 {
035         perror(s);
036         exit(-1);
037 }   
038 
039 /*
040  * Функции-обертки.
041  */
042 int Socket(int domain, int type, int protocol)
043 {
044         int rc;
045         
046         rc = socket(domain, type, protocol);
047         if(rc == -1) error("socket()");
048 
049         return rc;
050 }
051 
052 void Connect(int socket, const struct sockaddr *addr, socklen_t addrlen)
053 {
054         int rc;
055         
056         rc = connect(socket, addr, addrlen);
057         if(rc == -1) error("connect()");
058 }
059 
060 void Close(int fd)
061 {
062         int rc;
063         
064         for(;;) {
065                 rc = close(fd);
066                 if(!rc) break;
067                 if(errno == EINTR) continue;
068                 error("close()");
069         }
070 }
071 
072 void Inet_aton(const char *str, struct in_addr *addr)
073 {
074         int rc;
075         
076         rc = inet_aton(str, addr);
077         if(!rc) {
078                 /* Функция inet_aton() не меняет errno в случае ошибки. Чтобы
079                 сообщение, выводимое error(), было более осмысленным,
080                 присваиваем errno значение EINVAL. */
081                                 
082                 errno = EINVAL;
083                 error("inet_aton()");
084         }
085 }
086 
087 int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
088         struct timeval *timeout)
089 {
090         int rc;
091         
092         for(;;) {
093                 rc = select(n, readfds, writefds, exceptfds, timeout);
094                 if(rc != -1) break;
095                 if(errno == EINTR) continue;
096                 error("select()");
097         }
098         
099         return rc;
100 }
101 
102 size_t Read(int fd, void *buf, size_t count)
103 {
104         ssize_t rc;
105         
106         for(;;) {
107                 rc = read(fd, buf, count);
108                 if(rc != -1) break;
109                 if(errno == EINTR) continue;
110                 error("read()");
111         }
112         
113         return rc;
114 }
115 
116 size_t Write(int fd, const void *buf, size_t count)
117 {
118         ssize_t rc;
119         
120         for(;;) {
121                 rc = write(fd, buf, count);
122                 if(rc != -1) break;
123                 if(errno == EINTR) continue;
124                 error("write()");
125         }
126         
127         return rc;
128 }
129 
130 /*
131  * Запись count байтов в сокет.
132  */
133 size_t writen(int socket, const char *buf, size_t count)
134 {
135         const char *p;
136         size_t n, rc;
137 
138         /* Проверить корректность переданных аргументов. */
139         if(buf == NULL) {
140                 errno = EFAULT;
141                 error("writen()");
142         }
143         
144         p = buf;
145         n = count;
146         while(n) {
147                 rc = Write(socket, p, n);
148                 n -= rc;
149                 p += rc;
150         }
151 
152         return count;
153 }
154 
155 void show_usage()
156 {
157         puts("Usage: client ip_address");       
158         exit(-1);
159 }
160 
161 void do_work(int socket)
162 {
163         int n;
164         fd_set readfds;
165         char s[MAXLINE];
166         ssize_t rc;
167 
168         n = MAX(STDIN_FILENO, socket) + 1;      
169                         
170         for(;;) {
171                 /* Инициализировать набор дескрипторов. */
172                 FD_ZERO(&readfds);
173                 FD_SET(STDIN_FILENO, &readfds);
174                 FD_SET(socket, &readfds);
175 
176                 Select(n, &readfds, NULL, NULL, NULL);
177                 if(FD_ISSET(STDIN_FILENO, &readfds)) {
178                         rc = Read(STDIN_FILENO, s, MAXLINE);
179                         if(!rc) break;
180                         writen(socket, s, rc);
181                 }
182                 if(FD_ISSET(socket, &readfds)) {
183                         rc = Read(socket, s, MAXLINE);
184                         if(!rc) break;
185                         Write(STDOUT_FILENO, s, rc);
186                 }
187         }
188 }
189 
190 int main(int argc, char **argv)
191 {
192         int socket;
193         struct sockaddr_in servaddr;
194         
195         if(argc != 2) show_usage();
196 
197         socket = Socket(PF_INET, SOCK_STREAM, 0);
198         
199         /* Инициализировать структуру адреса сокета. */
200         memset(&servaddr, 0, sizeof(servaddr));
201         servaddr.sin_family = AF_INET;
202         servaddr.sin_port = htons(PORT);
203         Inet_aton(argv[1], &servaddr.sin_addr);
204 
205         Connect(socket, (SA *) &servaddr, sizeof(servaddr));
206         do_work(socket);                
207         Close(socket);
208         
209         return 0;
210 }
