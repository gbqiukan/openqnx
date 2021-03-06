/*
 * $QNXLicenseC:
 * Copyright 2007, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to QNX 
 * Software Systems before you may reproduce, modify or distribute this software, 
 * or any work that includes all or part of this software.   Free development 
 * licenses are available for evaluation and non-commercial purposes.  For more 
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 * for other information.
 * $
 */





#ifdef __USAGE		/* uname.c */
%C - return system name (POSIX)

%C	[-amnprsv] [-S name]
Options:
 -a       All display options (as though the options -snrvmp were specified).
 -m       Machine name (write to standard output).
 -n       Node name (write to standard output).
 -p       Processor Name.
 -r       Release level (write to standard output).
 -s       System (OS) name (write to standard output).
 -v       Version (write to standard output).
 -S name  Set host name
Note:
 If no options are specified, the uname utility writes the System name,
 as if the -s option had been specified.
#endif
	
/*
 * Include declarations:
 */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Define declarations:
 */
#define TRUE	1
#define FALSE	0

#define MACHINE 1
#define NODE    2
#define RELEASE 4
#define OSFLAG  8
#define VERSION 16
#define PROCESSOR 32
#define ALL (MACHINE|NODE|RELEASE|OSFLAG|VERSION|PROCESSOR)

/*
 * Global Declarations
 */
int create_mask;

void setname(char *name) {
	if(confstr(_CS_SET_HOSTNAME, name, 0) == 0) {
		fprintf(stderr, "Error setting hostname\n");
	}		
}

/*	Uname writes the current system name to standard output.  When options
 *	are specified, symbols representing one or more system characteristics are 
 *	written to the stdandard output.  
 */
int
main( int argc, char *argv[] )
{
	static struct utsname name;
	int  result, i, error;
	int flags=0;
	int	set = 0;

	result=error= FALSE;

	while(( i= getopt( argc, argv, "apmnrsvS:")) != -1)
		switch ( i ){
			case 'a':	flags |= ALL;
						break;
			case 'm':	flags |= MACHINE;   /* hardware machine name */
						break;
			case 'n':	flags |= NODE;      /* system node name */
						break;
			case 'r':	flags |= RELEASE;   /* OS release level */
						break;
			case 'p':	flags |= PROCESSOR;   /* OS release level */
						break;
			case 's':	flags |= OSFLAG;    /* OS name - also default */
						break;            
			case 'v':	flags |= VERSION;   /* OS version */
						break;
			case 'S':	setname(optarg);
						set = 1;
						break;
			case '?':	error = TRUE;       /* error */
						break;
			}

	if (flags == 0 && !set) 
		flags |= OSFLAG;

	if ( error )
		exit(EXIT_FAILURE);
	
	if(flags) {
		if (uname(&name)==-1) {
			perror("uname");
			exit(EXIT_FAILURE);
		}
	}

#define OUT(flag, var) if(flags & flag) { printf("%s", var);\
										  if(flags &= ~flag) printf(" "); }
	OUT(OSFLAG, name.sysname);
	OUT(NODE, name.nodename);
	OUT(RELEASE, name.release);
	OUT(VERSION, name.version);
	OUT(MACHINE, name.machine);
	if(flags & PROCESSOR) {
		char buff[257];

		if(confstr(_CS_ARCHITECTURE, buff, sizeof buff) == 0) {
			fprintf(stderr, "%s: error reading processor\n", argv[0]);
		} else {
			printf("%s", buff);
		}
	}
	printf("\n");

	return(EXIT_SUCCESS);
}
