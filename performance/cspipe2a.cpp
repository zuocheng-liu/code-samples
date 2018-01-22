#ifdef _WIN32
#	include <windows.h>
#	define Errno		GetLastError()
#	define FILED		HANDLE
#else
#	define FILED		int
#	define WINAPI
#	include <unistd.h>
#	include <string.h>
#	include <errno.h>
#	include <sys/types.h>
#	include <sys/wait.h>
#	include <sys/time.h>
#	include <sys/ipc.h>
#	include <sys/sem.h>
#	include <fcntl.h>
#	include <pthread.h>

	int sema;

#endif

int Put(FILED);
int Get(FILED);

#include <stdio.h>
#include <ctype.h>

#define equal	!strcmp
#define MAXCOUNT	100000

void tstart(void);
void tend(void);
double tval(void);

void tstart2(void);
void tend2(void);
double tval2(void);

//
// cspipe [maxcount]
//
// This program does pipes and threads.
//
// We create two execution environments and pass a token back and forth
// between them as fast as we can. We count the number and time and
// produce a context switches per second number.
//


void USAGE();
int do_threads();
size_t atoik(char *s);

unsigned long maxcount = MAXCOUNT;

unsigned long completedA, completedC;

unsigned long counter = 0;

char *applname;
char applnamebuf[256];
char *pipeAdult = "\\\\.\\pipe\\cspipe";

#ifndef _WIN32
int pipeAfds[2], pipeBfds[2];
#endif

unsigned long adultId;		// Thread ID's
unsigned long childId;

int main(int ac, char *av[])
{
	int ret = 0;

	strcpy(applnamebuf,av[0]);
#ifdef _WIN32
	{
		char *q;
		
		if((q=strrchr(applnamebuf, '.')))
			if(!equal(q+1,"exe"))
				strcat(applnamebuf,".exe");
	}
#endif
	applname = applnamebuf;

	if(ac > 1 && isdigit(av[1][0])) {
		maxcount = atoik(av[1]);
		ac--;
		av++;
		if(maxcount == 0)
			maxcount = 1;
	}
	ret = do_threads();
	return ret;
}

void USAGE()
{
	printf("%s [maximum count]\n",applname);
	return;
}

unsigned long WINAPI threadrun(void * var)
{
	unsigned i;
	char *p = (char *)var;

	if(equal(p,"A")) {		// Adult
#ifdef _WIN32
		HANDLE pipeA, pipeB;
#else
		struct sembuf sop;
		int pipeA, pipeB;
		int i;
#endif

#ifdef _WIN32
		pipeA = CreateNamedPipe(pipeAdult,
								PIPE_ACCESS_DUPLEX,
								PIPE_TYPE_BYTE,
								2,
								128,
								128,
								INFINITE,
								NULL);
		if(pipeA == INVALID_HANDLE_VALUE) {
			printf("CreateNamedPipe <%s> failed ERROR=%d\n", pipeAdult,Errno);
			ExitThread(1);
		}
		pipeB = pipeA;
		//
		// ConnectNamedPipe()
		//
		if(!ConnectNamedPipe(pipeA, NULL)) {
			printf("ConnectNamePipe ERROR: err=%d\n",Errno);
			ExitThread(1);
		}
#else
		pipeA = pipeAfds[1];
		pipeB = pipeBfds[0];
#endif
		tstart();
		//
		// ADULT: Writes the first byte.
		//
		for(i = 0; i < maxcount; i++) {
			counter++;
			if(!Put(pipeA))
					break;
			if(!Get(pipeB))
					break;
		}
		tend();
		double t = tval();

		printf("%d pipe/thread Context switches in %7.3f sec ",
			maxcount, t);

		printf("%7.3f usec/cswitch",
			(t*1e6)/maxcount);
		printf("\n");
#ifdef _WIN32
		ExitThread(0);
#else
		sop.sem_num = 0;
		sop.sem_op  = -1;
		sop.sem_flg = 0;
		if(semop(sema, &sop, 1) == -1) {
			printf("semop failed (waiting for threads): err=%d\n", Errno);
			return 1;
		}
#endif
	}
	else {
#ifdef _WIN32
		HANDLE pipeA, pipeB, pipeC;
#else
		struct sembuf sop;
		int pipeA, pipeB;
		int i;
#endif
#ifdef _WIN32
		//
		//
		// Here we call OpenNamedPipe or CreateFile
		//
        Sleep(1);
		pipeC = CreateFile(pipeAdult,
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
		if(pipeC == INVALID_HANDLE_VALUE) {
			printf("CreateFile for pipe failed: err=%d\n",
					Errno);
			ExitThread(1);
		}
		pipeA = pipeB = pipeC;
#else
		pipeA = pipeAfds[0];
		pipeB = pipeBfds[1];
#endif

		//
		// CHILD:
		// The ALREADY_EXISTS detector will wait for the
		// thing to be incremented. The other guy will start
		// this off.
		//
		// Release pipeC and waitfor pipeA
		//
		tstart2();
		for(i = 0; i < maxcount; i++) {
			if(!Get(pipeA))
					break;
			if(!Put(pipeB))
					break;
		}
		tend2();

		double t = tval2();


		printf("%d pipe/thread Context switches in %7.3f sec ",
			maxcount, t);

		printf("%7.3f usec/cswitch",
			(t*1e6)/maxcount);
		printf("\n");
#ifdef _WIN32
		ExitThread(0);
#else
		sop.sem_num = 0;
		sop.sem_op  = -1;
		sop.sem_flg = 0;
		if(semop(sema, &sop, 1) == -1) {
			printf("semop failed (waiting for threads): err=%d\n", Errno);
			return 1;
		}
#endif
	}
	return 0;
}
int Put(FILED fd)
{
#ifdef _WIN32
	if(WriteFile(fd, &counter, sizeof(counter),
					&completedA,NULL) == 0) {
		printf("WriteFile in Adult Failed after %d writes err=%d\n",
				counter, Errno);
		return 0;
	}
#else
	if(write(fd,&counter,sizeof(counter)) != sizeof(counter)) {
		printf("Write in A Failed after %d writes err=%d\n",
				counter, Errno);
		return 0;
	}
#endif
	return 1;
}
int Get(FILED fd)
{
#ifdef _WIN32
	if(ReadFile(fd, &counter, sizeof(counter),
					&completedA,NULL) == 0) {
		printf("ReadFile in Adult Failed after %d Reads err=%d\n",
				counter, Errno);
		return 0;
	}
#else
	if(read(fd,&counter,sizeof(counter)) != sizeof(counter)) {
		printf("Read in A Failed after %d Reads err=%d\n",
				counter, Errno);
		return 0;
	}
#endif
	return 1;
}

int do_threads()
{
#ifdef _WIN32
	//
	// creates two semaphores for use by the threads.
	// creates two threads and pass a token back and forth.
	//
	//printf("THREAD CONTEXT SWITCHING\n");
	//printf("PIPE SYNCHRONIZATION\n");

	HANDLE t1, t2; // two threads will swap the pipe token hot potato.

	t1 = CreateThread(NULL, 4096, threadrun, "A", NULL, &adultId);
	if(t1 == NULL) {
		printf("Creation of first thread failed err=%d\n", Errno);
		return 1;
	}
	t2 = CreateThread(NULL, 4096, threadrun, "C", NULL, &childId);
	if(t2 == NULL) {
		printf("Creation of second thread failed err=%d\n", Errno);
		return 1;
	}
	HANDLE waithandles[4];

	waithandles[0] = t1;
	waithandles[1] = t2;

	(void)WaitForMultipleObjects(2,waithandles,TRUE,INFINITE);
#else
	//
	// Have to make a semaphore to allow the parent
	// to behave responsibly toward its to thread children.
	//


	if((sema = semget(IPC_PRIVATE, 1, IPC_CREAT|0777)) == -1) {
		printf("semget failed: err=%d\n", Errno);
		return 0;
	}

	struct sembuf sop;

	//
	// Create and initialize semaphore to 2 so each "threadrun()"
	// can decrement it once.
	//
	sop.sem_num = 0;
	sop.sem_op  = 2;
	sop.sem_flg = 0;
	if(semop(sema, &sop, 1) == -1) {
		printf("semop filed: err=%d\n",Errno);
		return 0;
	}

	if(pipe(pipeAfds)) {
		printf("Pipe() syscall failed, errno=%d\n", Errno);
		return 0;
	}
	if(pipe(pipeBfds)) {
		printf("Pipe() syscall failed, errno=%d\n", Errno);
		return 0;
	}
	pthread_t tA, tB;

#	define DEC	( void *(*)(void*) )
	if(pthread_create(&tA, NULL, DEC  threadrun, (void *)"A")) {
		printf("pthread_create A failed: err=%d\n", Errno);
		return 0;
	}
	if(pthread_create(&tB, NULL, DEC threadrun, (void *)"B")) {
		printf("pthread_create B failed: err=%d\n", Errno);
		return 0;
	}
	//
	// Wait for semaphore to go to zero.
	//
	sop.sem_num = 0;
	sop.sem_op  = 0;
	sop.sem_flg = 0;
	if(semop(sema, &sop, 1) == -1) {
		printf("semop failed (waiting for threads): err=%d\n", Errno);
		return 1;
	}
#endif
	return 0;
}

#include <ctype.h>

size_t atoik(char *s)
{
	size_t ret = 0;
	size_t base;

	if(*s == '0') {
		base = 8;
		if(*++s == 'x' || *s == 'X') {
			base = 16;
			s++;
		}
	}
	else
		base = 10;

	for(; isxdigit(*s); s++) {
		if(base == 16)
			if(isalpha(*s))
				ret = base*ret + (toupper(*s) - 'A');
			else
				ret = base*ret + (*s - '0');
		else if(isdigit(*s))
				ret = base*ret + (*s - '0');
		else
			break;
	}
	for(; isalpha(*s); s++) {
		switch(toupper(*s)) {
		case 'K': ret *= 1024; break;
		case 'M': ret *= 1024*1024; break;
		default:
			return ret;
		}
	}
	return ret;
}

#ifdef _WIN32
static LARGE_INTEGER _tstart, _tend;
static LARGE_INTEGER _tstart2, _tend2;
static LARGE_INTEGER freq;
static int tfirst = 1;

void tstart(void)
{
	if(tfirst) {
		QueryPerformanceFrequency(&freq);
		tfirst = 0;
	}
	QueryPerformanceCounter(&_tstart);
}
void tstart2(void)
{
	if(tfirst) {
		QueryPerformanceFrequency(&freq);
		tfirst = 0;
	}
	QueryPerformanceCounter(&_tstart2);
}
void tend(void)
{
	QueryPerformanceCounter(&_tend);
}
void tend2(void)
{
	QueryPerformanceCounter(&_tend2);
}

double tval()
{
	return ((double)_tend.QuadPart -
				(double)_tstart.QuadPart)/((double)freq.QuadPart);
}
double tval2()
{
	return ((double)_tend2.QuadPart -
				(double)_tstart2.QuadPart)/((double)freq.QuadPart);
}
#else
static struct timeval _tstart, _tend;
static struct timeval _tstart2, _tend2;

void tstart(void)
{
	gettimeofday(&_tstart, NULL);
}
void tstart2(void)
{
	gettimeofday(&_tstart2, NULL);
}
void tend(void)
{
	gettimeofday(&_tend,NULL);
}
void tend2(void)
{
	gettimeofday(&_tend2,NULL);
}

double tval()
{
	double t1, t2;

	t1 =  (double)_tstart.tv_sec + (double)_tstart.tv_usec/(1000*1000);
	t2 =  (double)_tend.tv_sec + (double)_tend.tv_usec/(1000*1000);
	return t2-t1;
}
double tval2()
{
	double t1, t2;

	t1 =  (double)_tstart2.tv_sec + (double)_tstart2.tv_usec/(1000*1000);
	t2 =  (double)_tend2.tv_sec + (double)_tend2.tv_usec/(1000*1000);
	return t2-t1;
}
#endif
