#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

//Sends a message by tracking the number of send charactors and returning that number
int send_all_message(char* mess_to_send, int length_of_message, int connectionSocket){
  int garbage_value = send(connectionSocket, &length_of_message, sizeof(length_of_message), 0);
  int just_wrote = 0;
  int written_so_far = 0;
  //if the whole message wasnt sent...
  while(written_so_far < length_of_message){
    //printf("\nSending message: %d, %d\n", written_so_far, length_of_message);
    //send from the last bytes not sent 
    just_wrote = send(connectionSocket, mess_to_send + written_so_far, length_of_message - written_so_far, 0);
    written_so_far = just_wrote + written_so_far;
  }
  return written_so_far;
}

// Error function used for reporting issues
void error(const char *msg) { 
  perror(msg); 
  exit(0); 
} 

//Checks to validate whether the key and message have any charactors that are invalid
//Returns 1 if valid    0 if invalid
int invalid_txt(char * text, int length_text, char * key, int length_key){
  int i = 0;
  for(i = 0; i < length_text; i++){
    if((text[i] < 'A' || text[i] > 'Z')){
      if(text[i] != ' ' && text[i] != '\n'){
        //fprintf(stderr, "Invalid Charactor in message: %c\n", text[i]);
        return 0;
      }
    }
  }
  for(i = 0; i < length_key; i++){
    if((key[i] < 'A' || key[i] > 'Z')){
      if(key[i] != ' ' && key[i] != '\n'){
        //fprintf(stderr, "Invalid Character in key: %c\n", key[i]);
        return 0;
      }
    }
  }
  return 1;
}

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber, 
                        char* hostname){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);

  // Get the DNS entry for this host name
  struct hostent* hostInfo = gethostbyname(hostname); 
  if (hostInfo == NULL) { 
    fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
    exit(0); 
  }
  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char*) &address->sin_addr.s_addr, 
        hostInfo->h_addr_list[0],
        hostInfo->h_length);
}

int main(int argc, char *argv[]) {
  int socketFD, portNumber, garbage_int, ensure, charsRead = 0;
  struct sockaddr_in serverAddress;
  char buffer[256];
  // Check usage & args
  if (argc < 3) { 
    fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); 
    exit(0); 
  } 

  // Create a socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0); 
  if (socketFD < 0){
    error("CLIENT: ERROR opening socket");
  }

   // Set up the server address struct
  setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
    error("Decription CLIENT: ERROR connecting");
  }
  //Setup Handshake like protocol
  int good_num = -101;
  garbage_int = send(socketFD, &good_num, sizeof(good_num), 0);

  ensure = 0;
  charsRead = recv(socketFD, &good_num, sizeof(good_num), 0);
  //Throw Tantrum if couldnt connect
  if(good_num != -101){
    error("CLIENT: ERROR opening socket");
    close(socketFD);
    exit(EXIT_FAILURE);
  }
  //printf("Client:: Connected\n");
  //Get key from the specified file
  FILE* file_key = fopen(argv[2], "r");
  fseek(file_key, 0L, SEEK_END);
  long file_key_length = ftell(file_key);
  fseek(file_key ,0L, SEEK_SET);
  //This is the location of the key
  char * key = calloc(file_key_length+1, sizeof(char));
  memset(key, '\0', file_key_length);
  fgets(key,file_key_length, file_key);
  fclose(file_key);
  
  //get text from specified file
  FILE* file = fopen(argv[1], "r");
  fseek(file, 0L, SEEK_END);
  long file_length = ftell(file);
  fseek(file ,0L, SEEK_SET);
  char * txt = calloc(file_length + 1, sizeof(char));
  memset(txt, '\0', file_length + 1);
  fgets(txt, file_length + 1, file);
  fclose(file);
  //Validate whther or not the key is too short or not
  if(file_key_length < file_length){
    error("Encription Error: Key length too short");
    exit(0);
  }
  //Validate all charactors in file and key are valid
  if(invalid_txt(txt, file_length - 1, key, file_key_length - 1 ) == 0){
    error("Client: Plaintext or Key has Bad Characters");
    exit(0);
  }
  //printf("Client Key: %s\n", key);
  //printf("%d\n",strlen(txt));
  //Send key 
  send_all_message(key, file_key_length, socketFD);
  sleep(2);
  //Send message to encription server side
  send_all_message(txt, file_length, socketFD);
  sleep(1);
  //get message
  int mess_len = 0;
  int i = 0;
       
  garbage_int = recv(socketFD, &mess_len, 4, 0);
  char * spacer = calloc(1000, sizeof(char));
  char * mess = calloc(mess_len + 1, sizeof(char));         
  while(i < mess_len){
    memset(spacer, '\0', 1000);
    int num_chars_rec = recv(socketFD, spacer, 1000, 0);
    //printf("Recieved: %s\n", spacer);
    memcpy(mess+i, spacer, num_chars_rec);
    i += num_chars_rec;
  }
  //memset(mess,'\0', mess_len+1);
  //Print Message
  printf("%s\n", mess);
  // Close the socket
  close(socketFD); 
  return 0;
}