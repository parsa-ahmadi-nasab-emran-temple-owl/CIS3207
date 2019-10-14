#ifndef parsa_shell
#define parsa_shell
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#define buffer 64
#define white_space " \t\r\n\a"
constant int pipe_is_read = 0;
constant int pipe_was_written = 1;
external char **environment;
void loop_through_my_batchfile(char * my_batch_file);
void start_main_loop();
char * read_input_from_keyboard(void);
char ** parse_input(char * input);
int command_execution(char ** parsed_command);
void pipe_fork_and_execute(void function(char **), char ** parsed_command, int pipe_boolean);
void cd(char ** parsed_command);
void clr(char ** parsed_command);
void dir(char ** parsed_command);
void display_environment(char ** parsed_command);
void set_environment(char * parsed_command);
void echo(char ** parsed_command);
void pause(char ** parsed_command);
void help(char ** parsed_command);
void quit(char ** parsed_command); 
FILE * input_redirection(char ** parsed_command);
FILE * output_redirection(char ** parsed_command);
int pipe_boolean(char ** parsed_command);
void reset_Input_Output(FILE * input_redirection, FILE * output_redirection, int original_stadin, int original_stadout, int original_error);
void prompt();
int run_in_background(char ** parsed_command);
void (*get_function(char ** parsed_command))(char ** parsed_command);
void replace_newline(char * temporary);
void move_the_pointers(char ** parsed_command, int arrow);