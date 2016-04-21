#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#define FCGI_LISTENSOCK_FILENO 0

# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <sys/un.h>
# include <arpa/inet.h>

# include <netdb.h>
static int bind_socket(unsigned short port) {
    int fcgi_fd, socket_type, val;

    struct sockaddr_in fcgi_addr_in;
    struct sockaddr *fcgi_addr;

    socklen_t servlen;

    memset(&fcgi_addr_in, 0, sizeof(fcgi_addr_in));
    fcgi_addr_in.sin_family = AF_INET;
    fcgi_addr_in.sin_port = htons(port);

    servlen = sizeof(fcgi_addr_in);
    socket_type = AF_INET;
    fcgi_addr = (struct sockaddr *) &fcgi_addr_in;
    fcgi_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == (fcgi_fd = socket(socket_type, SOCK_STREAM, 0))) {
        fprintf(stderr, "spawn-fcgi: couldn't create socket: %s\n", strerror(errno));
        return -1;
    }

    val = 1;
    if (setsockopt(fcgi_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        fprintf(stderr, "spawn-fcgi: couldn't set SO_REUSEADDR: %s\n", strerror(errno));
        return -1;
    }

    if (-1 == bind(fcgi_fd, fcgi_addr, servlen)) {
        fprintf(stderr, "spawn-fcgi: bind failed: %s\n", strerror(errno));
        return -1;
    }

    if (-1 == listen(fcgi_fd, 1024)) {
        fprintf(stderr, "spawn-fcgi: listen failed: %s\n", strerror(errno));
        return -1;
    }

    return fcgi_fd;
}

static int fcgi_spawn_connection(char *appPath, int fcgi_fd, int fork_count) {
    int status, rc = 0;
    struct timeval tv = { 0, 100 * 1000 };
    pid_t child;

    while (fork_count-- > 0) {

        child = fork();
        switch (child) {
        case 0: {
            /* child */
            int max_fd = 0;
            int i = 0;

            if(fcgi_fd != FCGI_LISTENSOCK_FILENO) {
                close(FCGI_LISTENSOCK_FILENO);
                dup2(fcgi_fd, FCGI_LISTENSOCK_FILENO);
                close(fcgi_fd);
            }

            /* loose control terminal */
            setsid();

            max_fd = open("/dev/null", O_RDWR);
            if (-1 != max_fd) {
                if (max_fd != STDOUT_FILENO) dup2(max_fd, STDOUT_FILENO);
                if (max_fd != STDERR_FILENO) dup2(max_fd, STDERR_FILENO);
                if (max_fd != STDOUT_FILENO && max_fd != STDERR_FILENO) close(max_fd);
            } else {
                fprintf(stderr, "spawn-fcgi: couldn't open and redirect stdout/stderr to '/dev/null': %s\n", strerror(errno));
            }

            /* we don't need the client socket */
            for (i = 3; i < max_fd; i++) {
                if (i != FCGI_LISTENSOCK_FILENO) close(i);
            }

            /* fork and replace shell */

            char *b = malloc((sizeof("exec ") - 1) + strlen(appPath) + 1);
            strcpy(b, "exec ");
            strcat(b, appPath);

            /* exec the cgi */
            execl("/bin/sh", "sh", "-c", b, (char *)NULL);
            /* in nofork mode stderr is still open */
            fprintf(stderr, "spawn-fcgi: exec failed: %s\n", strerror(errno));
            exit(errno);

            break;
        }
        case -1:
            /* error */
            fprintf(stderr, "spawn-fcgi: fork failed: %s\n", strerror(errno));
            break;
        default:
            /* father */
            /* wait */
            select(0, NULL, NULL, NULL, &tv);

            switch (waitpid(child, &status, WNOHANG)) {
            case 0:
                fprintf(stdout, "spawn-fcgi: child spawned successfully: PID: %d\n", child);
                break;
            case -1:
                break;
            default:
                if (WIFEXITED(status)) {
                    fprintf(stderr, "spawn-fcgi: child exited with: %d\n",
                        WEXITSTATUS(status));
                    rc = WEXITSTATUS(status);
                } else if (WIFSIGNALED(status)) {
                    fprintf(stderr, "spawn-fcgi: child signaled: %d\n",
                        WTERMSIG(status));
                    rc = 1;
                } else {
                    fprintf(stderr, "spawn-fcgi: child died somehow: exit status = %d\n",
                        status);
                    rc = status;
                }
            }

            break;
        }
    }

    close(fcgi_fd);

    return rc;
}


int main(int argc, char **argv) {
    char *fcgi_app = NULL;
    char *endptr = NULL;
    unsigned short port = 0;
    int fork_count = 1;
    int o;
    int fcgi_fd = -1;

    if (argc < 2) { /* no arguments given */
        return -1;
    }

    while (-1 != (o = getopt(argc, argv, "f:p:F:"))) {
        switch(o) {
        case 'f': fcgi_app = optarg; break;
        case 'p': port = strtol(optarg, &endptr, 10);/* port */
            if (*endptr) {
                fprintf(stderr, "spawn-fcgi: invalid port: %u\n", (unsigned int) port);
                return -1;
            }
            break;
        case 'F': fork_count = strtol(optarg, NULL, 10);/*  */ break;
        default:
            return -1;
        }
    }

    if (NULL == fcgi_app ) {
        fprintf(stderr, "spawn-fcgi: no FastCGI application given\n");
        return -1;
    }

    if (0 == port) {
        fprintf(stderr, "spawn-fcgi: no socket given (use either -p or -s)\n");
        return -1;
    }  

    if (-1 == (fcgi_fd = bind_socket(port))) {
        return -1;
    }

    return fcgi_spawn_connection(fcgi_app, fcgi_fd, fork_count);
}
