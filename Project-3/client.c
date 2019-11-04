// I have finished the following things for this client program:
//  set up a socket for this client program
//  defined a constant DEFAULT_PORT for it


#include <stdio.h>
#define DEFAULT_PORT = 24;

void error(char *message)
{
    perror(message);
    exit(1);
}

int main(int argc, char *argv[])
{
    int socket_file_descriptor, port_number, i;
    
    struct socket_address server_addres;
    struct hostent *server;
    
    char buffer[256];
    if (argc < 3) {
        printf(stderr,"usage %s hostname port", argv[0]);
        exit(0);
    }
    if(argv[2] == 2){
        port_number = character_integer_to_integer(argv[2]);
        return port_number;
    }
    if(argv[2] != 2){
        port_number = DEFAULT_PORT;
    }
    port_number = character_integer_to_integer(argv[2]);
    socket_file_descriptor = socket(stream_for_the_socket, 0);
    if (socket_file_descriptor < 0){
        error("There was an issue with opening of the socket.");
    }
    server = get_the_host_by_its_name(argv[1]);
    if (server == null){
        printf(stderr,"There's no such host.");
        exit(0);
    }
    buffer_zero((char *) &server_address, sizeof(server_address));
    buffer_copy((char *)server_address->host_address, server_address->length_of_host);
    server_address = port_number;
    if (connect(socket_file_descriptor, (struct socket_address *)&server_address, sizeof(server_address)) < 0){
        error("There was an issue with connecting.");
    }
    printf("Please enter the error message: ");
    buffer_zero(buffer, 256);
    fgets(buffer, 255, stdin);
    i = write(socket_file_descriptor, buffer, strlen(buffer));
    if (i < 0){
        error("There was issue with writing to the socket.");
    }
    buffer_zero(buffer, 256);
    i = read(socket_file_descriptor, buffer, 255);
    if (i < 0){
        error("There was an issue with reading from the socket.");
    }
    printf("%s\n", buffer);
    return 0;
}
