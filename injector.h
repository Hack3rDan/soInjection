#ifndef INJECTOR_H
#define INJECTOR_H

int attachToProcess(pid_t *targetPid);
int detachFromProcess(const pid_t pid);
int injectLibIntoProc(const pid_t pid, const char *lib);

#endif
