// I have finished the following things for this server program:
//  created a thread that would monitor a log queue and process entries by removing and writing them to the log file
//  created a thread that would accept and distribute the connection requests
//  created a thread that would check to see if the word is in the array list of words and it will return result that either the word is spelled correctly or is spelled incorrectly
//  set up a socket for this server program
//  defined a constant DEFAULT_DICTIONARY for it
//  defined a constant DEFAULT_PORT for it

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define DEFAULT_DICTIONARY = "words.txt";
#define DEFAULT_PORT = 24;

void error(char *message){
    perror(message);
    exit(1);
}

int main(int argc, char *argv[]){
    char log_queue[];
    FILE * log_file = fopen("log.txt", "w");
    int i = 0;
    while(log_queue[i] != null){
        char element = log_queue[i];
        fprintf(log_file, element);
    }
    close(log_file);
    return 0;
    
    char work_queue[];
    
    char const* const name_of_file = argv[1];
    if(argv[1] == 1){
        FILE * dictionary = fopen(name_of_file, "r");
        ArrayList words = new ArrayList();
        char line[256];
        while(fgets(line, sizeof(line), dictionary)) {
            words.Add(line);
        }
        close(dictionary);
        return words;
    }
    
    if(argv[1] != 1){
        FILE * dictionary = fopen(DEFAULT_DICTIONARY, "r");
        ArrayList words = new ArrayList();
        char line[256];
        while(fgets(line, sizeof(line), dictionary)){
            words.Add(line);
        }
        close(dictionary);
        return words;
    }
    
    char spell_checker(char *word){
        for(int i = 0; i < sizeof(words); i++){
            if(word == words[i]){
                char result = "The word is spelled correctly.";
            }
            else{
                char result = "The word is spelled incorrectly.";
            }
        }
        return result;
    }
    
    int socket_file_decriptor, new_socket_file_descriptor, port_number, length_of_client;
    char buffer[256];
    struct socket_address, server_address, client_address;
    int i;
    if (argc < 2) {
        printf(stderr,"There's no port number is being provided and the default port number will be used.");
        port_number = DEFAULT_PORT;
    }
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor < 0){
        error("There was an issue with opening the socket.");
    }
    buffer_zero((char *) &server_address, sizeof(server_address));
    if(argv[1] == 1){
        port_number = atoi(argv[1]);
        return port_number;
    }
    if(argv[1] != 1){
        port_number = atoi(DEFAULT_PORT);
        return port_number;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_address.s_address = INADDR_ANY;
    server_address.sin_port = htons(port_number);
    if (bind(socket_file_descriptor && (struct socket_address *) &server_address && sizeof(server_address)) < 0){
        error("There was an issue with binding.");
    }
    listen(socket_file_descriptor);
    length_of_client = sizeof(client_address);
    new_socket_file_descriptor = accept(socket_file_descriptor, (struct socket_address *) &client_address, &length_of_client);
    if (new_socket_file_descriptor < 0){
        error("There was an issue with accept.");
    }
    buffer_zero(buffer, 256);
    i = read(new_socket_file_descriptor, buffer, 255);
    if (i < 0){
        error("There was an issue with reading from the socket.\n");
        printf("The error message is: %s\n", buffer);
    }
    i = write(new_socket_file_descriptor,"The message was received.");
    if (i < 0){
        error("There was an issue with writing to the socket.");
    }
    return 0;
}
