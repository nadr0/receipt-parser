#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "parsePDF.h"

int main(int argc, char ** argv){

	/* Make sure test strings have \n at the end */


	/* Get the current directory */
	DIR * dir;
	struct dirent * ent;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));

	dir = opendir(cwd);
	ent = readdir(dir);


	/* Count the number of PDFs to convert */
	int numberOfPDFs = 0;

	/* Read the current directory to count the number of pdfs*/
	while((ent = readdir(dir)) != NULL){
		char * checker = strstr(ent->d_name,".pdf");
		if(checker){
			numberOfPDFs++;
		}
	}

	/* 
		Create new ARGV for exec 
		numberOfPDFs + 1 since "./tet" goes first
		+1 for NULL
	*/
	char ** tet_ARGV = calloc(numberOfPDFs + 2,sizeof(char*));

	int tet_ARGC = numberOfPDFs + 2;

	/* "./tet" file name */
	char * tet_fileName = calloc(strlen("./tet")+1,sizeof(char));
	strcpy(tet_fileName,"./tet");
	tet_ARGV[0] = tet_fileName;


	/* Get all file names from current directory*/
	dir = opendir(cwd);
	ent = readdir(dir);

	int j = 1;
	while((ent = readdir(dir)) != NULL){
		char * checker = strstr(ent->d_name,".pdf");
		if(checker){
			/* 
				Current string to store the PDF name 
				argv[i+1], since argv[0] is name of exe	
			*/
			char * current_PDF = calloc(1,strlen(ent->d_name) + 1);
			/* Copy the string*/
			strcpy(current_PDF, ent->d_name);
			/* Store the string */
			tet_ARGV[j] = current_PDF;
			j++;
		}

	}

	/* FORK */
	pid_t child = fork();

	if(!child){
		/* I am the child! e*/

		/* EXEC tet
			with the pdfs
		*/
		int exec_returnValue = execvp(tet_fileName, tet_ARGV);

		if(exec_returnValue == -1){
			fprintf(stderr, "Exec call failed\n");
			exit(1);
		}

	}else{
		/* I am the parent! */

		int val;

		/* Wait on the child process */
		pid_t wait_returnValue = waitpid(child,&val,0);
		for (int i = 1; i < tet_ARGC - 1; ++i)
		{
			/* read the pdf file names and convert them into the text format*/
			char * fileBegin = strtok(tet_ARGV[i],".");
			int fileNameLength = strlen(fileBegin) + strlen(".txt");

			char * fileName = calloc(1,fileNameLength+1);

			strcat(fileName, fileBegin);
			strcat(fileName, ".txt");	

			char str[15];
			sprintf(str,"%d", i);

			/* Parse each txt file into a csv file */
			parseTXTtoCSV(fileName,str);

			free(fileName);
		}

		/* Memory clean up */
		for(int i = 0; i < numberOfPDFs + 1; i++){
			free(tet_ARGV[i]);
		}
		free(tet_ARGV);

	}


	return 0;
}