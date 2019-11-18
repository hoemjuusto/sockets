//
// Created by juuso on 14.11.2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <values.h>

int main(){

    // create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);  // high number just to make sure that the port is open
    // IF CONNECTING BACK TO OWN SYSTEM
    server_address.sin_addr.s_addr = INADDR_ANY;

    //OR IF CONNECTING TO REMOTE IP ADDRESS
    /*struct in_addr addr;
    //Convert the IP dotted quad into struct in_addr
    inet_aton("88.195.222.155", &(addr));
    server_address.sin_addr.s_addr = addr.s_addr;*/

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if(connection_status == -1){
        perror("Error connecting to remote socket! \n\n");
    }
    // receive connection message from the server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);

    printf("Client got server response: %s\n", server_response);

    int keepRunning = 1;
    char user_input[256];

    while(keepRunning){
        printf("\nGive serve-request (submit with enter, e to exit):\n");
        fgets(user_input, 256, stdin);
        char delim[] = "\n";
        strcpy(user_input, strtok(user_input, delim));
        printf("Sending the request: %s to server\n", user_input);
        if(strcmp(user_input,"e") == 0)
        {
            printf("\n\nExiting the bank!\n\n");
            send(network_socket, user_input, sizeof(user_input), 0);
            keepRunning = 0;
        }else{
            send(network_socket, user_input, sizeof(user_input), 0);
            recv(network_socket, &server_response, sizeof(server_response), 0);
            printf("Server sent response: %s\n", server_response);
        }
    }

    close(network_socket);

    return 0;
}