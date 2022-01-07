/* $begin shellmain */
#include "csapp.h"
//#include <signal.h>
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
int help();
int id();
int pid();
int exitp();
void sighandle(int num);
int cd(char *argv);

int main(int argc, char *argv[]) 
{
    char cmdline[MAXLINE]; /* Command line */
    while (1) {
    signal(SIGINT, sighandle);
	/* Read */
    if(argc >1){
        if(!strcmp(argv[1],"-p")){
            printf("%s> ",argv[2]); 
        }
    }
        else{
	printf("my257sh> ");   
        }          
	Fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);

	/* Evaluate */
	eval(cmdline);
    }
}
/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { 
        if ((pid = Fork()) == 0) {   /* Child runs user job */
            if (execvp(argv[0], argv) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0)
        unix_error("waitfg: waitpid error");
        else{
        printf("Child exit status: %d\n", status);
        }
	}
	else
	    printf("%d %s", pid, cmdline);
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "exit")) /* quit command */
	    return exitp();  
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	return 1;
    if(!strcmp(argv[0], "pid"))
    return id();
    if(!strcmp(argv[0], "ppid"))
    return pid();
    if(!strcmp(argv[0], "help"))
    return help();
    if(!strcmp(argv[0], "cd")){
        if(argv[1]!=NULL)
    return cd(argv[1]);
    return cd("");
    }
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */
int help(){
    printf("Developer: Adam Harms\nTo change shell prompt start program with ./my257sh -p <prompt>\nrefer to man pages for non-built-in commands.\n");
    printf("built-in commands:\n");
    printf("help : use to see details about the program and get info about built-in commands.\n");
    printf("cd : cd <no args> will display current directory, cd <args> will cd to that directory.\n");
    printf("pid : prints proccess id of the shell.\n");
    printf("ppid : prints the parent process id of the shell.\n");
    printf("exit : Exits the shell.\n");
    return 1;
}
int id(){
    printf("%d\n",getpid());
    return 1;
}
int pid(){
    printf("%d\n",getppid());
    return 1;
}
int exitp(){
    raise(SIGTERM);
    return 1;
}
void sighandle(int sig) {
    return;
}
int cd(char *argv){
    //printf(argv);
    char direct[100];
    if(!strcmp(argv, "..")){
        chdir("..");
    }
    else if(!strcmp(argv, "")){
    printf("%s\n", getcwd(direct, 100));
    }
    else{
        chdir(argv);
    }
return 1;
}