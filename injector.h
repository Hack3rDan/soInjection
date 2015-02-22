#ifndef INJECTOR_H
#define INJECTOR_H
#define SO_INJECT_SUCCESS 0

int attachToProcess(pid_t *targetPid);
int detachFromProcess(const pid_t pid);
int injectLibIntoProc(const pid_t pid, const char *lib);



#endif
