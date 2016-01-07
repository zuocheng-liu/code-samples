/**
 * How to write a daemon ?
 * - fork clild process
 * - close parent process
 * - become session leader
 * - change working directory
 * - clear file mode creation mask
 * - close stdin, stdout and stderr
 * - masking terminal signal 
 * - use system log
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>

int daemon_init(void) { 
    pid_t pid;
    if ((pid = fork()) < 0) { 
        return(-1);
    } else if (pid != 0) {
        exit(0); /* parent exits */
    }
    /* child continues */
    setsid(); /* become session leader*/
    chdir("/"); /* changeworking directory */
    umask(0); /* clear file modecreation mask */
    close(STDIN_FILENO); /* close stdin */
    close(STDOUT_FILENO); /* close stdout */
    close(STDERR_FILENO); /* close stderr */
    return 0; 
}

void sig_term(int signo) { 
    if(signo == SIGTERM) {
        /* catched signal sent by kill(1)command */
        syslog(LOG_INFO, "program terminated.");
        closelog(); 
        exit(0); 
    }
}

int main(void) { 
    if(daemon_init() == -1) { 
        printf("can't fork daemon\n"); 
        exit(0); 
    }
    openlog("daemontest", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "program started.");
    signal(SIGTERM, sig_term); /*arrange to catch the signal */
    signal(SIGTTOU, SIG_IGN);  
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    while(1) { 
        /* put yourmain program here */ 
        sleep(1); 
    }
    return 0; 
}
