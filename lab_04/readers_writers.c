#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define ACTIVE_READERS  0
#define WRITE_QUEUE     1
#define READ_QUEUE      2
#define ACTIVE_WRITER   3
#define BIN_WRITER      4

#define P -1
#define V  1

struct sembuf start_read[] = {
  {READ_QUEUE, V, 0},
  {ACTIVE_WRITER, 0, 0},
  {WRITE_QUEUE, 0, 0},
  {ACTIVE_READERS, V, 0},
  {READ_QUEUE, P, 0},
};

struct sembuf stop_read[] = {
  {ACTIVE_READERS, P, 0},
};

struct sembuf start_write[] = {  
  {WRITE_QUEUE, V, 0},
  {ACTIVE_READERS, 0, 0},
  {BIN_WRITER, P, 0},
  {ACTIVE_WRITER, 0, 0},
  {ACTIVE_WRITER, V, 0},
  {WRITE_QUEUE, P, 0},
};

struct sembuf stop_write[] = {
  {ACTIVE_WRITER, P, 0},
  {BIN_WRITER, V, 0},
};

char *shared_char;
int shmid;
int semid;

int init_monitor()
{
  if (semctl(semid, ACTIVE_READERS, SETVAL, 0) == -1)
      return EXIT_FAILURE;
  if (semctl(semid, WRITE_QUEUE, SETVAL, 0) == -1)
      return EXIT_FAILURE;
  if (semctl(semid, READ_QUEUE, SETVAL, 0) == -1)
      return EXIT_FAILURE;
  if (semctl(semid, ACTIVE_WRITER, SETVAL, 0) == -1)
      return EXIT_FAILURE;
  if (semctl(semid, BIN_WRITER, SETVAL, 1) == -1)
      return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

void log_exit(const char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

void writer()
{
  srand(getpid());
  while(1)
  {
    sleep(rand() % 5);

    if (semop(semid, start_write, 6) == -1)
    {
      perror("cant start_write");
      exit(EXIT_FAILURE);
    }
    
    if (*shared_char == 'z') {
        *shared_char = 'a';
    } else {
        (*shared_char)++;
    }
    printf("Writer %d write '%c'\n", getpid(), (*shared_char));

    if (semop(semid, stop_write, 2) == -1)
    {
      perror("cant stop_write");
      exit(EXIT_FAILURE);
    }
  }
  exit(EXIT_SUCCESS);
}

void reader()
{
  srand(getpid());
  while(1)
  {
    sleep(rand() % 2);

    if (semop(semid, start_read, 5) == -1)
    {
      perror("cant start_read");
      exit(EXIT_FAILURE);
    }
    
    printf("Reader %d read '%c'\n", getpid(), (*shared_char));
    
    if (semop(semid, stop_read, 1) == -1)
    {
      perror("cant stop_read");
      exit(EXIT_FAILURE);
    }
  }
  exit(EXIT_SUCCESS);
}

int main(void) 
{
  const int wr_count = 3;
  const int rd_count = 5;
  const int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  if ((semid = semget(IPC_PRIVATE, 5, IPC_CREAT | perms)) == -1)
    return EXIT_FAILURE;

  shmid = shmget(IPC_PRIVATE, sizeof(char), IPC_CREAT | perms);
  if (shmid == -1)
    return EXIT_FAILURE;

  shared_char = shmat(shmid, 0, 0);
  if (shared_char == (char *)-1)
    return EXIT_FAILURE;
  *shared_char = 'a';

  if (init_monitor())
    log_exit("cant init_monitor");

  pid_t chpid[wr_count + rd_count];
  
  for (int i = 0; i < wr_count; i++) 
  {
    if ((chpid[i] = fork()) == -1)
      log_exit("cant fork");
    else if (chpid[i] == 0)
      writer();
  }

  for (int i = wr_count; i < rd_count + wr_count; i++) 
  {
    if ((chpid[i] = fork()) == -1)
      log_exit("cant fork");
    else if (chpid[i] == 0)
      reader();
  }

  for (int i = 0; i < rd_count + wr_count; i++)
  {
    int status;
    if (waitpid(chpid[i], &status, WUNTRACED) == -1)
      log_exit("cant wait");

    if (WIFEXITED(status)) 
        printf("child with pid %d has finished, code: %d\n", chpid[i], WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("child with pid %d has finished by unhandlable signal, signum: %d\n", chpid[i], WTERMSIG(status));
    else if (WIFSTOPPED(status))
        printf("child with pid %d has finished by signal, signum: %d\n", chpid[i], WSTOPSIG(status));
  }

  if (semctl(semid, 0, IPC_RMID, NULL) == -1)
    log_exit("Can't semctl\n");

  if (shmdt(shared_char) == -1)
    log_exit("Can't shmdt\n");
  
  if (shmctl(shmid, IPC_RMID, NULL) == -1)
    log_exit("Can't shmctl\n");

  return EXIT_SUCCESS;
}