/*
Questions to answer at top of client.c:
(You should not need to change the code in client.c)

1. What is the address of the server it is trying to connect to (IP address and
port number).
   - IP Address: 127.0.0.1
   - Port Number: 8000


2. Is it UDP or TCP? How do you know?
   - It is TCP. The socket() call uses SOCK_STREAM which is used for TCP. If it
was UDP, SOCK_DGRAM would have been used.

3. The client is going to send some data to the server. Where does it get this
data from? How can you tell in the code?
   - It gets data from user input. This is shown by the read() call in the while
loop: while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) STDIN_FILENO is
the file descriptor for standard user input.

4. How does the client program end? How can you tell that in the code?
   - The program ends when the while loop's condition is no longer met,
     which happens when the read() call returns 1 or less.
   - This occurs when I trigger an End-of-File by pressing
     Ctrl+D, or if I just press Enter (which sends a single newline
     character, so num_read would be 1).
   - After the loop finishes, the code calls close(sfd); and
exit(EXIT_SUCCESS);.
*/

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main_client() { // Renamed main to main_client
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}

/*******************************************************************************
 * Server code begins here
 ******************************************************************************/

#define LISTEN_BACKLOG 32

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;
  int cfd = client->cfd;
  int client_id = client->client_id;
  free(client);

  char buf[BUF_SIZE];
  ssize_t num_read;

  printf("New client created! ID %d on socket FD %d\n", client_id, cfd);

  while ((num_read = read(cfd, buf, BUF_SIZE - 1)) > 0) {
    buf[num_read] = '\0';
    if (num_read > 0 && buf[num_read - 1] == '\n') {
      buf[num_read - 1] = '\0';
    }

    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    int current_msg_count = total_message_count;
    pthread_mutex_unlock(&count_mutex);

    printf("Msg # %d; Client ID %d: %s\n", current_msg_count, client_id, buf);
  }

  printf("Ending thread for client %d\n", client_id);
  close(cfd);
  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    int client_fd = accept(sfd, NULL, NULL);
    if (client_fd == -1) {
      perror("accept");
      continue;
    }

    struct client_info *client = malloc(sizeof(struct client_info));
    if (client == NULL) {
      perror("malloc for client_info failed");
      close(client_fd);
      continue;
    }

    pthread_mutex_lock(&client_id_mutex);
    int new_id = client_id_counter;
    client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    client->cfd = client_fd;
    client->client_id = new_id;

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_client, client) != 0) {
      perror("pthread_create failed");
      free(client);
      close(client_fd);
      continue;
    }

    pthread_detach(thread_id);
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
