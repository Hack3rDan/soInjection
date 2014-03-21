#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include "injector.h"

int main(int argc, char *argv[])
{
    pid_t targetPd = 0;
    int retVal = EXIT_SUCCESS;
    char *procName = NULL;

    retVal = getProcName(&procName);
    if(retVal < 0)
    {
        printf("Failed to get process name from user\n");
        goto MAIN_CLEAN_AND_EXIT;
    }
    
    retVal = getPidOfProcName(&targetPid);
    if(reVal < 0)
    {
        printf("Failed to getPidOfProcName\n");
        goto MAIN_CLEAN_AND_EXIT;
    }
    //Attach to process
    retVal = attachToProcess(&targetPid);
    if(retVal < 0)
    {
        printf("Failed to attach to process\n");
        goto MAIN_CLEAN_AND_EXIT;
    }
    // do some injecting
    // detach    
    retVal = detachFromProcess(targetPid);
    if(retVal < 0)
    {
        printf("Error occured while trying to detach...ignoring\n");
    }

    // Clean and exit
MAIN_CLEAN_AND_EXIT:
    exit(retVal);
}


int getProcName(char *procName, int size)
{
    //TODO add sanitization
    printf("What is the name of the process that you want to inject into?(please be nice): ");
    fgets(procName, size, STDIN);
    return EXIT_SUCCESS;
}

int getPidOfProcName(const char *procName, pid_t *pid)
{
    int retVal = EXIT_SUCCESS;
    char command[MAX_COMMAND_LENGTH] = {0};
    FILE *commandStream = NULL;


    sprintf(command, "pidof %s", procName);
    commandStream = popen(command, "r");
    if(NULL == commandStream)
    {
        perror(popen);
        retVal = EXIT_FAILURE;
        goto GET_PID_CLEAN_RETURN;
    }
    
GET_PID_CLEAN_RETURN:
    if(commandStream)
    {
        pclose(commandStream);
    }
    return retVal;
}
