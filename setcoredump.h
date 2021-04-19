#ifndef SETCOREDUMP_H_
#define SETCOREDUMP_H_

#include <list>
#include <string>

typedef struct ProcessInfo {
    int pid;
    std::string name;
} ProcessInfo;

void ShowProcessInformation(std::list<ProcessInfo> &process_list);
int KillProcess(int pid);

#endif /* SETCOREDUMP_H_ */

