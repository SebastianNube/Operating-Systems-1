#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


// Error function used for reporting issues
void error(const char *msg) {
  perror(msg);
  exit(1);
} 

char * encription(char* key, char * mess, int message_len){
  char*  alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  char * finished_message = calloc(message_len, sizeof(char));
  char * temp = calloc(message_len, sizeof(char));
  int message_len_new = strlen(mess);
  int l = 0;
  //printf("Before Encryption: %d\n", message_len);
  while(l < message_len_new - 1 ){
    char key_index = key[l] - 65;
    char message_index = mess[l] - 65;

    if(mess[l] == ' '){
      message_index = 26;
    }
    if(key[l] == ' '){
      key_index = 26;
    }
    int final = message_index + key_index;

    finished_message[l] = alpha[final % 27];
    temp[l] = mess[l];
    l += 1;
  }
  finished_message[message_len_new] = '\0';
  //printf("After Encryption: %d\t temp: %s\n", strlen(temp), temp);
  //printf("After Encryption: %d\t finished: %s", strlen(finished_message), finished_message);
  return finished_message;
}

int send_all_message(char* mess_to_send, int length_of_message, int connectionSocket){
  int garbage_value = send(connectionSocket, &length_of_message, sizeof(length_of_message), 0);
  int just_wrote = 0;
  int written_so_far = 0;
  while(written_so_far < length_of_message){
    just_wrote = send(connectionSocket, mess_to_send + written_so_far, length_of_message-written_so_far, 0);
    written_so_far = just_wrote + written_so_far;
  }
  return written_so_far;
}


// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}

int main(int argc, char *argv[]){
  int connectionSocket, charsRead, child_pid = 0;
  int client_count = 0;
  char buffer[256];
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);
  // Check usage & args
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    exit(1);
  } 
  
  // Create the socket that will listen for connections
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    error("ERROR opening socket");
  }

  // Set up the address struct for the server socket
  setupAddressStruct(&serverAddress, atoi(argv[1]));

  // Associate the socket to the port
  if (bind(listenSocket, 
          (struct sockaddr *)&serverAddress, 
          sizeof(serverAddress)) < 0){
    error("ERROR on binding");
  }

  // Start listening for connetions. Allow up to 5 connections to queue up
  listen(listenSocket, 5); 
  // Accept a connection, blocking if one is not available until one connects
  while(1){
    // Accept the connection request which creates a connection socket
    connectionSocket = accept(listenSocket, 
                (struct sockaddr *)&clientAddress, 
                &sizeOfClientInfo); 
    if (connectionSocket < 0){
      error("ERROR on accept");
    }

    if(client_count < 5){
      //printf("Server:: Connected\n");
      //Specify the client, ensure correct client
      int ensure = 0;
      int garbage_int = recv(connectionSocket, &ensure, sizeof(int),0);
      int good_num = -101;
      garbage_int = send(connectionSocket, &good_num, sizeof(good_num), 0);
      if(ensure == -101){

        child_pid = fork();
        switch(child_pid){
          //Fire on all cyclinders 
          case(0):
            //get the key
            sleep(1);
            int key_len = 0;
            int i = 0;          
            int garbage_int = recv(connectionSocket, &key_len, 4, 0);
            //printf("Server got key length: %d\n", key_len);
            char * spacer = calloc(1000, sizeof(char));
            char * key = calloc(key_len+ 1, sizeof(char));
            while(i < key_len){
              
              memset(spacer, '\0', 1000);
              int num_chars_rec = recv(connectionSocket, spacer, 1000, 0);
              //printf("Copied: %s\tKeyLen: %d\n", spacer, key_len);
              memcpy(key+i, spacer, num_chars_rec);
              i += num_chars_rec;
            }
            //printf("Server Key: %s", key);
            //get the message
            sleep(1);
            int message_len = 0;
            i = 0;          
            garbage_int = recv(connectionSocket, &message_len, 4, 0);
            spacer = calloc(100000, sizeof(char));
            char * mess = calloc(100000, sizeof(char));
            //printf("got message Length: %d\n", message_len);   
            while(i < message_len){
              memset(spacer, '\0', 100000);
              int num_chars_rec = recv(connectionSocket, spacer, 100000, 0);
              memcpy(mess+i, spacer, num_chars_rec);
              i += num_chars_rec;
            }
            //encrypt message
            //printf("HERE\n");   
            //printf("MessLen: %d\n", message_len);
            char * dec_mess = encription(key, mess, message_len); 
            //printf("MessLen: %d\n", strlen(dec_mess));
            //send decoded message
            garbage_int = send_all_message(dec_mess, strlen(dec_mess), connectionSocket);
            //Exit
            close(connectionSocket);
            exit(EXIT_FAILURE);
            break;
          case(-1):
            printf("Something is very very wrong\n");
            return 0;
            break;
          default:
            break;
        }
      }
    }
    else if(client_count < 0){
      printf("Something is very very wrong\n");
      return 0;
    }

    else{
      printf("Client count is greater than 5 (Max reached)");
      close(connectionSocket);
      return 0;
    }
  }
  // Close the listening socket
  close(listenSocket); 
  return 0;
}
