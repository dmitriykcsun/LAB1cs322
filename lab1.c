#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(){

	time_t starttime,endtime;
	pid_t childID;
	pid_t waiter;
	int waitstatus;
	struct tms st_cpu;
	struct tms en_cpu;
	clock_t st_t;
	clock_t en_t;


	starttime = time(NULL);

	printf("START: %ld \n",starttime);

	st_t = times(&st_cpu);

	childID = fork();
	
	if(childID < 0){
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if(childID==0){
	printf("PPID: %d, PID: %d  \n",getppid(),getpid());
	}

	else{
		sleep(3);
		waiter=waitpid(childID, &waitstatus, WUNTRACED);
		printf("PPID: %d, PID: %d, CPID: %d, RETVAL: %d \n",getppid(),getpid(),childID,WEXITSTATUS(waitstatus));

		if(waiter < 0){
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		en_t = times(&en_cpu);

		printf("USER: %ju, SYS: %ju \n", (uintmax_t)en_cpu.tms_utime - st_cpu.tms_utime, (uintmax_t)en_cpu.tms_stime - st_cpu.tms_stime);
		printf("CUSER: %ju, CSYS: %ju\n",(uintmax_t)en_cpu.tms_cutime - st_cpu.tms_cutime,(uintmax_t)en_cpu.tms_cstime - st_cpu.tms_cstime);
		endtime = time(NULL);
		printf("STOP: %ld \n", endtime);
		exit(EXIT_SUCCESS);
			
	}
	return 0;
}