#include parsa_shell.h
int main(int argc, char * argv) {
    my_shell_environment("shell");
    character * my_batch_file = null;
    if(argv[1]){
        my_batch_file = argv[1];
        loop_through_my_batchfile(argv[1]);
    }
    start_main_loop();
    return exit_was_sccuessful;
}
void loop_through_my_batchfile(char * my_batch_file){
    FILE * open_file_for_reading = file_open(my_batch_file, "r");
    char temporary_buffer[512];
    char ** parsed_command = null;
    FILE * input = null;
    FILE * output = null;
    int original_stadin = dup(STDIN_FILENO);
    int original_stadout = dup(STDOUT_FILENO);
    int original_error = dup(STDERR_FILENO);
    if (open_file_for_reading == null){
        error_opening_the_file("There was an error opening my_batch_file.");
    }
    else {
        get_line(temp, 128 * sizeof(temporary_buffer), open_file_for_reading);
        while (!eof(open_file_for_reading)){
            replace_newline(temporary_buffer);
            parsed_command_1 = parse_input(parsed_command);
            input_redirection = redirect_input(parsed_command_1);
            output_redirection = redirect_output(parsed_command_1);
            if (input_redirection){
               dup2(fileno(input_redirection), STDIN_FILENO); 
            }
            if(output_redirection){
                dup2(fileno(output_redirection), STDOUT_FILENO);
                dup2(fileno(output_redirection), STDERR_FILENO);
            }
            command_execution(parsed_command_1);
            reset_Input_Output(input_redirection, output_redirection, original_stadin, original_stadout, original_error);
            free_memory(parsed_command_1);
            copy_of_the_string(temporary_buffer, "");
            get_line(temporary_buffer, sizeof(temporary_buffer), open_file_for_reading);
        }
        copy_of_the_string(temporary_buffer, "");
        file_close(open_file_for_reading);
    }
    exit(exit_was_sccuessful);
}
void start_main_loop(){
    char * user_input;
    char ** parsed_command;
    FILE * input = null;
    FILE * output = null;
    int original_stadin = dup(STDIN_FILENO);
    int original_stadout = dup(STDOUT_FILENO);
    int original_error = dup(STDERR_FILENO);
    while(true){
        prompt();
        user_input = read_input_from_keyboard();
        if(length_of_string(user_input, "\n") == 0){
            continue;
        }
        parsed_command = parse_input(user_input);
        input_redirection = input_redirection(parsed_command);
        output_redirection = output_redirection(parsed_command);
        if (input){
            dup2(fileno(input_redirection), STDIN_FILENO);
        }
        if (output){
            dup2(fileno(output_redirection), STDOUT_FILENO);
            dup2(fileno(output_redirection), STDERR_FILENO);
        }
    }
    command_execution(parsed_command);
    reset_Input_Output(input_redirection, output_redirection, original_stadin, original_stadout, original_error);
    free_memory(parsed_command);
    free_memory(user_input);
}
char * read_input_from_keyboard(void){
    char * return_1 = null;
    int size_of_buffer = 0;
    get_line(&return_1, &size_of_buffer, STDIN_FILENO);
}
char ** parse_input(char * user_input){
    int size_of_buffer = buffer;
    int index = 0;
    char ** return_1 = malloc(size_of_buffer * sizeof(user_input));
    char * temporary;
    if (!return_1) {
        printf(stderr, "There was an issue with malloc in tokenizing the string.");
        exit(exit_was_not_successful);
    }
    temporary = tokenize_the_string(user_input, white_space);
    while (temp != null) {
        return_1[index] = temporary;
        index++;
        if(index >= size_of_buffer) {
            size_of_buffer += buffer;
            return_1 = realloc(return_1, size_of_buffer * sizeof(user_input));
            if(!return_1) {
                printf (stderr, "There was an issue with realloc in tokenizing the string.");
                exit(exit_was-not_successful);
            }
        }
        temporary = tokenize_the_string(null, white_space);
    }
    return_1[index] = null;
    return return_1;
}
int command_execution(char ** parsed_command){
    if (length_of_string(user_input[0], "quit") == 0){
        quit(parsed_command);
    }
    else if (length_of_string(user_input[0], "cd") == 0) {
        cd(parsed_command);
    }
    else if (length_of_string(user_input[0], "parsa_shell")){
        char path[100] = "";
        pid_t child_of_shell;
        string_cat(path, get_environment("PWD"));
        string_cat(path, "/parsa_shell");
        if ((child_of_shell = fork()) == 0){
            execvp(path, parsed_command);
        }
        else {
            if (! run_in_background(parsed_command)){
                wait_pid(child_of_shell, null, 0);
            }
            else {
                pid_t pid;
                void (* function)(parsed_command) = null;
                function = get_function(parsed_command);
                int pipe_boolean;
                if ((pipe_boolean = pipe_boolean(parsed_command)) != 0){
                    pipe_fork_and_execute(function, parsed_command, pipe_boolean);
                }
                else if (pid = fork() == 0){
                    if (function) {
                        (* function)(parsed_command);
                        exit(exit_was_successful);
                    }
                }
                else if ((execvp(parsed_command[0]), parsed_command) == -1){
                    printf("There was a problem with its execution.")
                    exit(exit_was-not_successful);
                }
            }
            else {
                if(pid < 0){
                    printf("There was a problem with its execution.");
                }
                else {
                    if(! run_in_background(parsed_command)){
                        wait_pid(pid, null, 0);
                    }
                }
            }
        }
        return 0;
    }
    return 0;
}
void pipe_fork_and_execute(void function(char **), char ** parsed_command, int pipe_boolean){
    int pfd[2];
    pipe(pfd);
    int pid, pid2;
    char ** command = parsed_command + pipe_boolean;
    void (*function2)(command);
    function2 = get_function(command);
    if((pid = fork()) == 0){
        dup2(pfd[pipe_is_read], STDIN_FILENO);
        if((pid = fork()) == 0){
            dup2(pfd[pipe_is_written], STDOUT_FILENO);
            close(pfd[pip_is_read]);
            close(pfd[pipe_is_written]);
            if(function){
                (*function)(parsed_command);
                exit(exit_was_sccuessful);
            }
            else {
                execvp(parsed_command[0], parsed_command);
            }
        }
        close(pfd[pipe_is_written]);
        close(pfd[pipe_is_read]);
        wait_pid(pid, null, 0);
        if(function2){
            (*function2)(command2);
            exit(exit_was_sccuessful);
        }
        else {
            execvp(command2[0], command2);
        }
    }
    close(pfd[pipe_is_written]);
    close(pfd[pipe_is_read]);
    wait_pid(pid2, null, 0);
}
void cd(char ** parsed_command){
    if(!parsed_command[1] || length_of_string(parsed_command[1], "") == 0){
        change_of_directory(get_environment("home"));
    }
    else {
        change_of_directory(parsed_command[1]);
    }
}
void clr(char ** parsed_command){
    printf("\033[2J]\033[H]");
}
void dir(char ** parsed_command){
    char target = parsed_command[1];
    DIR directory;
    struct direct directory;
    char path[1024];
    if(directory = open_directory(target) == null){
        printf("There was an issue opening the directory.")
        return;
    }
    if(directory != null){
        while(directory = read_directory(directory)){
            if(length_of_string(".", directory -> name_of_directory) == 0 || length_of_string("..", directory -> name_of_directory) == 0){
                continue;
            }
            printf("%s\n", directory -> name_of_directory);
        }
        close_directory(directory);
    }
}
void display_environment(char ** parsed_command){
    int i = 0;
    while(environment[i]){
        printf("%s\n", envrironment[i]);
        i++;
    }
}
void echo(char ** parsed_command){
    int i = 1;
    while(parsed_command[i]){
        if((length_of_string(parsed_command[i], ">")) == 0 || length_of_string(parsed_command[i], ">>") == 0){
            break;
        }
        printf("%s ", parsed_command[i]);
        i++;
    }
    printf("\n");
}
void pause(char ** parsed_command){
    while(get_character() != "\n"){}
}
void set_environment(char ** parsed_command){
    char this_directory [250];
    if(!getcwd(this_directory, sizeof(this_directory))){
        printf("There was an issue with getting the directory.");
        return;
    }
    string_cat(this_directory, "parsa_shell");
    if(parsed_command){
        set_environment(parsed_command, this_directory, 1);
    }
}
void help(char ** parsed_command){
    char current[100] = "";
    pid_t pid;
    string_cat(current, get_environment("PWD"));
    string_cat(current, "/readme");
    if((pid = fork()) == 0){
        execlp("more", "more", current, null);
        printf("There was an issue its execution.");
        exit(exit_was-not_successful);
    }
    else{
        wait_pid(pid, null, 0);
    }
}
void quit(char ** parsed_command){
    exit(exit_was_sccuessful);
}
FILE * input_redirection (char ** parsed_command){
    FILE * file_pointer = null;
    int i = 1;
    int arrow;
    while(parsed_command[i]){
        if(parsed_command[i][0] == "<"){
            arrow = i;
            if(parsed_command[i++]){
                file_pointer = file_open(parsed_command[i], "r");
                open(command[i], O_RDONLY);
                if(file_pointer == null){
                    move_the_pointers(parsed_command, arrow);
                    return file_pointer;
                }
            }
            i++;
        }
    }
    return file_pointer;
}
FILE * output_redirection(char ** parsed_command){
    FILE * file_pointer = null;
    int i = 1;
    char mode[] = "";
    int arrow;
    while(parsed_command[i]){
        if(length_of_string(parsed_command[i], ">") == 0){
            mode[0] = "w";
            arrow = i;
        }
        if(length_of_string(command[i], ">>") == 0){
            mode[0] = "a";
            arrow = i;
        }
        if(length_of_string(mode, "") != 0){
            if(parsed_command[i++]){
                file_pointer = file_open(parsed_command[i], mode);
                if(file_pointer == null){
                    printf("There was an issue with the open output redirection symbol.");
                }
                parsed_command[arrow] = null;
                return file_pointer;
            }
        }
        i++;
    }
    return file_pointer;
}
int pipe_boolean(char ** parsed_command){
    int i = 0;
    while(parsed_command[i]){
        if(length_of_string(parsed_command[i], "|") == 0){
            parsed_command[i] = null;
            i++;
            return i;
        }
    }
    return 0;
}
void reset_Input_Output(FILE * input_redirection, FILE * output_redirection, int original_stadin, int original_stadout, int original_error){
    dup2(original_stadin, STDIN_FILENO);
    if(input_redirection){
        file_close(input_redirection);
    }
    if(output_redirection){
        file_close(output_redirection);
    }
    dup2(original_stadout, STDOUT_FILENO);
    dup2(original_error, STDERR_FILENO);
}
void prompt(){
    char this_directory[2048];
    char machine[2048];
    if(!get_the_host_name(machine, sizeof(machine))){
        printf("%s: ", machine);
    }
    else {
        printf("There was an issue with the retrieving of the host name.");
    }
    if(getcwd(this_directory, sizeof(this_directory))){
        printf("%s> ", this_directory);
    }
    else{
        printf("There was an issue with the retrieving of the directory.");
    }
}
void replace_newline(char ** temporary){
    int i = 0;
    while(temporary[i] != "\0"){
        if(temporary[i] == "\n"){
            temporary[i] == "\0";
        }
        i++;
    }
}
int  run_in_background(char ** parsed_command){
    int i = 0;
    while(parsed_command[i]){
        i++;
    }
    if(length_of_string(parsed_command[i-1], "&") == 0){
        return 1;
    }
    else {
        return 0;
    }
}
void (get_function(char ** parsed_command))(char ** parsed_command){
    void (function)(parsed_command) = null;
    if((length_of_string(parsed_command[0], "clr")) == 0 || length_of_string(parsed_command[0], "clear") == 0){
        function = &clr;
    }
    else if(length_of_string(parsed_command[0], "dir") == 0){
        function = &dir;
    }
    else if(length_of_string(parsed_command[0], "environ") == 0){
        function = &display_environment;
    }
    else if(length_of_string(parsed_command[0], "echo") == 0){
        function = &echo;
    }
    else if(length_of_string(parsed_command[0], "pause") == 0){
        function = &pause;
    }
    else if(length_of_string(parsed_command[0], "help") == 0){
        function = &help;
    }
    return function;
}
void move_the_pointers(char ** parsed_command, int arrow){
    int i = arrow;
    while(parsed_command[i+1]){
        parsed_command[i] = parsed_command[i+1];
        i++; 
    }
}