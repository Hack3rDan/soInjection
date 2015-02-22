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

int getDataFromProc(pid_t child, long addr, char *str, int len)
{
    char *laddr = str;
    int i = 0, j = len/long_size;
    union u
    {
        long val;
        char chars[long_size];
    } data;

    while(i < j)
    {
        data.val = ptrace(  PTRACE_PEEKDATA,    // request
                            child,              // pid
                            addr + i * 4,       // addr
                            NULL);              // data
        memcpy( laddr,
                data.chars,
                sizeof(long));
        i++;
        laddr += sizeof(long);
    }

    j = len % sizeof(long);

    if(j != 0)
    {

        data.val = ptrace(  PTRACE_PEEKDATA,    //request
                            child,              // pid
                            addr + i * 4,       // addr
                            NULL);              // data
        memcpy( laddr,
                data.chars,
                j);
    }
    str[len] = '\0';
    return SO_INJECT_SUCCESS;
}


int putDataIntoProc(pid_t child, long addr, char *str, int len)
{
    char *laddr = str;
    int i = 0, j = len / sizeof(long);
    union u
    {
        long val;
        char chars[sizeof(long)];
    } data;

    while(i < j)
    {
        memcpy( data.chars,
                laddr,
                sizeof(long));
        ptrace( PTRACE_POKEDATA,    // request
                child,              // pid
                addr + i * 4,       // addr
                data.val);          // data
        i++;
        laddr += sizeof(long);
    }
    
    j = len % sizeof(long);
    if(j != 0)
    {
        memcpy( data.chars,
                laddr,
                j);
        ptrace( PTRACE_POKETEXT,    // request
                child,              // pid
                addr + i * 4,       // addr
                data.val);          // data
    }
    
    return SO_INJECT_SUCCESS;

}

int attachToProc(int pid, struct user_regs_struct *regs)
{
    ptrace( PTRACE_ATTACH,  // request
            pid,            // pid
            NULL,           // addr
            NULL);          // data
    wait(NULL);
    ptrace( PTRACE_GETREGS, // request
            pid,            // pid
            NULL,           // addr
            regs);          data
    return SO_INJECT_SUCCESS;
}

int detachFromProc(int pid, struct user_regs_struct *regs)
{
    ptrace( PTRACE_SETREGS, // request
            pid,            // pid
            NULL,           // addr
            regs);          // data
    ptrace( PTRACE_DETACH,  // request
            pid,            // pid
            NULL,           // addr
            NULL);          // NULL
    return SO_INJECT_SUCCESS;
}


int injectCode(int pid, struct user_regs_struct *regs, void *backup, void *code, int lenOfCode, void *addrOfInject)
{
    if(NULL == regs || NULL == backup || NULL == code)
    {
        return SO_INJECT_NULL_PARAMETER;
    }
    if(NULL == addrOfInject)
    {
        addrOfInject = regs.eip;
    }
    // copy instruction to backukp variable
    getDataFromProc(    pid,        // child
                        addrOfInject,   // addr
                        backup,     // str
                        lenOfCode); // len
    // insert breakpoint
    // TODO: insert shellcode
    putDataToProc(  pid,        // child
                    addrOfInject,   // addr
                    code,       // data
                    lenOfCode); // lenOfData
    // restore original
    putDataToProc(  pid,
                    addrOfInject,
                    backup,
                    lenOfCode);
                    
}

