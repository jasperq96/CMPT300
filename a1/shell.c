// Shell starter file
// You may make any changes to any part of this file.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/types.h>
#include <pwd.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10

struct History{
	char historyList[HISTORY_DEPTH][COMMAND_LENGTH];
	int size;
	int firstCommand;
	int totalCommands;
};

struct History history;

/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);

	//add current command to history list
	if(buff[0] != '!' && buff[0] != '\0'){
		if(history.size == HISTORY_DEPTH){
			for(int i = 0; i < HISTORY_DEPTH-1; i++){
				strcpy(history.historyList[i], history.historyList[i+1]);
			}
			strcpy(history.historyList[HISTORY_DEPTH-1], buff);
			history.firstCommand++;
		}else{
			strcpy(history.historyList[history.size], buff);
			history.size++;
		}
		history.totalCommands++;
	}

	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;
		case '~':


		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}

/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */
int read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);
	int errno;

	if ((length < 0) && (errno != EINTR)) {
		perror("Unable to read command from keyboard. Terminating.\n");
		exit(-1);
	}

	if ((length < 0) && (errno == EINTR)) {
		return 1;
	}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return 0;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
	return 0;
}

void run_history(){
	char command_num[1000];
	for(int i = history.size-1; i >= 0; i--){
		sprintf(command_num, "%d", history.totalCommands - (history.size-i));
		strcat(command_num,"\t");
		write(STDOUT_FILENO, command_num, strlen(command_num));
		write(STDOUT_FILENO, history.historyList[i], strlen(history.historyList[i]));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}
}

void reset(char *tokens[]){
	for(int i = 0; tokens[i] != NULL; i++){
		tokens[i] = NULL;
	}
}

void display_help(){
	const char *internal[5][2] = {
		{"exit","terminating your program"},
		{"pwd","displaying the current working directory"},
		{"cd","changing directories"},
		{"help","displaying help information on internal commands"},
		{"history","displaying the past 10 recently entered commands"}
	};
	for(int i = 0; i < 5; i++){
		write(STDOUT_FILENO, "'", strlen("'"));
		write(STDOUT_FILENO, internal[i][0], strlen(internal[i][0]));
		write(STDOUT_FILENO, "' is a builtin command for ", strlen("' is a builtin command for "));
		write(STDOUT_FILENO, internal[i][1], strlen(internal[i][1]));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}
}

int internalCommands(char *tokens[], const char *internal[][2], char *previous_dir){
	char cwd[256];
	char recent_dir[256];
	if(strcmp(tokens[0],"exit") == 0){
		if(tokens[1] == NULL){
			write(STDOUT_FILENO, "Exiting program.....\n", strlen("Exiting program.....\n"));
			exit(0);
		}else{
			write(STDOUT_FILENO, "ERROR: Too many arguments\n", strlen("ERROR: Too many arguments\n"));
			return 1;
		}
	}else if(strcmp(tokens[0],"pwd") == 0){
		if(tokens[1] == NULL){
			write(STDOUT_FILENO, getcwd(cwd, sizeof(cwd)), strlen(getcwd(cwd, sizeof(cwd))));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}else{
			write(STDOUT_FILENO, "ERROR: Too many arguments\n", strlen("ERROR: Too many arguments\n"));
		}
		return 1;
	}else if(strcmp(tokens[0],"cd") == 0){
		if(tokens[1] != NULL && tokens[2] == NULL){
			int success = 0;
			if(strcmp(tokens[1],"-") == 0){
				success = chdir(previous_dir);
			}else{
				getcwd(recent_dir, sizeof(recent_dir));
				strcpy(previous_dir,recent_dir);
				success = chdir(tokens[1]);
			}
			if(success != 0){
				write(STDOUT_FILENO, "ERROR: Command 'cd' failed\n", strlen("ERROR: Command 'cd' failed\n"));
			}
		}else{
			if(tokens[1] == NULL){ //change to home directory
				getcwd(recent_dir, sizeof(recent_dir));
				strcpy(previous_dir,recent_dir);
				struct passwd *home = getpwuid(getuid());
				int success = chdir(home->pw_dir);
				if(success != 0){
					write(STDOUT_FILENO, "ERROR: Command 'cd' failed\n", strlen("ERROR: Command 'cd' failed\n"));
				}
			}else{
				write(STDOUT_FILENO, "ERROR: Too many arguments\n", strlen("ERROR: Too many arguments\n"));
			}
		}
		return 1;
	}else if(strcmp(tokens[0],"help") == 0){
		if(tokens[1] != NULL && tokens[2] == NULL){
			int count = 0;
			for(int i = 0; i < 5; i++){
				if(strcmp(tokens[1],internal[i][0]) == 0){
					write(STDOUT_FILENO, "'", strlen("'"));
					write(STDOUT_FILENO, internal[i][0], strlen(internal[i][0]));
					write(STDOUT_FILENO, "' is a builtin command for ", strlen("' is a builtin command for "));
					write(STDOUT_FILENO, internal[i][1], strlen(internal[i][1]));
					write(STDOUT_FILENO, "\n", strlen("\n"));
					break;
				}else{
					count++;
				}
			}
			if(count == 5){
				write(STDOUT_FILENO, "'", strlen("'"));
				write(STDOUT_FILENO, tokens[1], strlen(tokens[1]));
				write(STDOUT_FILENO, "' is an external command or application\n", strlen("' is an external command or application\n"));
			}
			return 1;
		}else{
			if(tokens[1] == NULL){
				display_help();
				return 1;
			}else{
				write(STDOUT_FILENO, "ERROR: Too many arguments\n", strlen("ERROR: Too many arguments\n"));
				return 1;
			}
		}
	}else if(strcmp(tokens[0],"history") == 0){
		if(tokens[1] == NULL){
			run_history(history);
		}else{
			write(STDOUT_FILENO, "ERROR: Too many arguments\n", strlen("ERROR: Too many arguments\n"));
		}
		return 1;
	}
	return 0;
}

void handle_SIGINT(){
	write(STDOUT_FILENO, "\n", strlen("\n"));
	display_help();
}

/**
 * Main and Execute Commands
 */
char previous_dir[256] = "";

int main(int argc, char* argv[]){
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];
	getcwd(previous_dir, sizeof(previous_dir));
	const char *internal[5][2] = {
		{"exit","terminating your program"},
		{"pwd","displaying the current working directory"},
		{"cd","changing directories"},
		{"help","displaying help information on internal commands"},
		{"history","displaying the past 10 recently entered commands"}
	};
	history.size = 0;
	history.totalCommands = 0;
	history.firstCommand = 0;

	/* Set up the signal handler */
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);

	while (true) {
		char cwd[256];
		_Bool in_background = false;
		
		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().


		write(STDOUT_FILENO, getcwd(cwd, sizeof(cwd)), strlen(getcwd(cwd, sizeof(cwd))));
		write(STDOUT_FILENO, "$ ", strlen("$ "));
		
		int cntrlC = read_command(input_buffer, tokens, &in_background);
		if(cntrlC){
			continue;
		}

		// DEBUG: Dump out arguments:
		// for (int i = 0; tokens[i] != NULL; i++) {
		// 	write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
		// 	write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
		// 	write(STDOUT_FILENO, "\n", strlen("\n"));
		// }


		/**
		 * Steps For Basic Shell:
		 * 1. Fork a child process. Fork() returns negative if fail, 0 if good, positive if returned to parent or caller. 
		 * 2. Child process invokes execvp() using results in token array. Returns negative value if execution fails
		 * 3. If in_background is false, parent waits for
		 *    child to finish. Otherwise, parent loops back to
		 *    read_command() again immediately.
		 */
		if(strcmp(tokens[0], "!!") == 0){
			if(history.totalCommands > 0){
				char command_to_run[COMMAND_LENGTH];
				if(history.totalCommands < 10){
					strcpy(command_to_run,history.historyList[history.totalCommands-1]);
				}else{
					strcpy(command_to_run,history.historyList[HISTORY_DEPTH-1]);
				}
				
				int token_count = tokenize_command(command_to_run, tokens);
				if (token_count == 0) {
					continue;
				}

				// Extract if running in background:
				if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
					in_background = true;
					tokens[token_count - 1] = 0;
				}
				for (int i = 0; tokens[i] != NULL; i++) {
					write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
					write(STDOUT_FILENO, " ", strlen(" "));
				}
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}else{
				write(STDOUT_FILENO, "ERROR: No Previous Commands\n", strlen("ERROR: No Previous Commands\n"));
				continue;
			}
		}else if(tokens[0][0] == '!'){
			float cmdNum = strtof(&tokens[0][1], NULL);
			if(((int)cmdNum == 0) && (strcmp(&tokens[0][1], "0") != 0)){
				write(STDOUT_FILENO, "ERROR: Invalid Command Number\n", strlen("ERROR: Invalid Command Number\n"));
				continue;
			}
			if(cmdNum == (int)cmdNum && cmdNum >= 0 && cmdNum >= history.totalCommands - history.size && cmdNum < history.totalCommands){
				reset(tokens);
				// printf("Before: %f\n", cmdNum);
				if(history.totalCommands >= HISTORY_DEPTH){
					cmdNum = cmdNum - history.firstCommand;
				}
				// printf("Index: %f, First: %d\n",cmdNum, history.firstCommand);
				char command_to_run[COMMAND_LENGTH];
				strcpy(command_to_run,history.historyList[(int)cmdNum]);
				int token_count = tokenize_command(command_to_run, tokens);
				if (token_count == 0) {
					continue;
				}

				// Extract if running in background:
				if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
					in_background = true;
					tokens[token_count - 1] = 0;
				}
				for (int i = 0; tokens[i] != NULL; i++) {
					write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
					write(STDOUT_FILENO, " ", strlen(" "));
				}
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}else{
				write(STDOUT_FILENO, "ERROR: Invalid Command Number\n", strlen("ERROR: Invalid Command Number\n"));
				continue;
			}
		}

		if(!internalCommands(tokens,internal,previous_dir)){
			pid_t child = fork();
			// printf("before: %d\n",child);
			if(child < 0){
				write(STDOUT_FILENO, "Failed to create child process\n", strlen("Failed to create child process\n"));
			}else if(child == 0){
				// write(STDOUT_FILENO, "Child is working.....\n", strlen("Child is working.....\n"));
				if(execvp(tokens[0], tokens) < 0){ //if execvp is successful, child will end once completed. only returns -1 if fail
					write(STDOUT_FILENO, "ERROR: Command failed\n", strlen("ERROR: Command failed\n"));
				}
			}else if(!in_background){
				// write(STDOUT_FILENO, "Parent waiting......\n", strlen("Parent waiting......\n"));
				// printf("after: %d\n",child);
				while(waitpid(-1, NULL, WCONTINUED) > 0); //WNOHANG continues parent if status of child is unknown, meaning it's currently running
				// write(STDOUT_FILENO, "Wait is over, continuing!\n", strlen("Wait is over, continuing!\n"));
			}else{
				write(STDOUT_FILENO, "Parent continuing......\n", strlen("Parent continuing......\n"));
			}
		}		
		reset(tokens);
		while(waitpid(-1, NULL, WNOHANG) > 0); //clean up zombies
		// printf("Previous Dir: %s\n", previous_dir);
	}
	return 0;
}