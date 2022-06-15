#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void type_prompt(void);
void read_command(void);
/**
 * main - executes the commands
 * Return: 0 success
 */
int main(void)
{
	char cmd[100], command[100], *parameters[20];
	/*environment variables*/
	char *envp[] = { (char *) "PATH=/bin", 0};

	while (1)
	{
		type_prompt();
		read_command(command, parameters);
		if (fork() != 0)
		{
			int wstatus;

			wait(&wstatus);
			if (WIFEXITED(wstatus))
			{
				int statusCode = WEXITSTATUS(wstatus);

				if (statusCode == 0)
					printf("success\n");
				else
					printf("FAILURE\n");
			}
		}
		else
		{
			strcpy(cmd, "/bin/");
			strcat(cmd, command);
			if (execve(cmd, parameters, envp) == -1)
				perror("Error:");
		}
		if (strcmp(command, "exit") == 0)
			break;
	}
	return (0);
}
/**
 * type_prompt - avail the prompt
 */

void type_prompt(void)
{
	static int first_time = 1;

	if (first_time)
	{
		/*clear screen for the first time*/
		const char *CLEAR_SCREEN_ANSI = "\033";

		write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
		first_time = 0;
	}
	/*display prompt*/
	printf("#");
}

/**
 * read_command - Reads the command from the commandline
 * @cmd: returns command to main
 * @par: returns the parameters to main
 */
void read_command(char cmd[], char *par[])
{
	char line[1024];
	int count = 0, i = 0, j = 0;
	char *array[100], *pch;

	/*read one line*/
	for ( ;; )
	{
		int c = fgetc(stdin);

		line[count++] = (char) c;
		if (c == '\n')
			break;
	}
	if (count == 1)
		return;
	pch = strtok(line, "\n");
	/*perse the line into words*/
	while (pch != NULL)
	{
		array[i++] = strdup(pch);
		pch = strtok(NULL, "\n");
	}
	/*first word is the command*/
	strcpy(cmd, array[0]);
	/*others are perameters*/
	for (j = 0; j < i; j++)
	{
		par[j] = array[j];
		par[i] = NULL; /*NULL terminates the perameter list*/
	}
}
