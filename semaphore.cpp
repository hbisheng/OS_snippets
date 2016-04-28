#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<cstdlib>

using namespace std;

const int N = 4;
const int MAX_LOOP = 100;
sem_t table_empty;
sem_t agency_tobacco;
sem_t agency_paper;
sem_t agency_match;




void init(){
    sem_init(&table_empty, 0, 1);
    sem_init(&agency_tobacco, 0, 0);
    sem_init(&agency_paper, 0, 0);
    sem_init(&agency_match, 0, 0);
}

void* agency_func(void* arg){
    
    while(true) {
        sem_wait(&table_empty);
        
        int soybean = rand() % 3;
        if(soybean == 0) {
            sem_post(&agency_tobacco);
        } else if(soybean == 1) {
            sem_post(&agency_paper);
        } else if(soybean == 2) {
            sem_post(&agency_match);
        }
        
    }
    
    return NULL;
}

void* tobacco_owner_func(void* arg) {
    while(true) {
        sem_wait(&agency_tobacco);
        cout << "tobacco_owner smoke" << endl;
        sleep(2);
        cout << "tobacco_owner smoke done" << endl;
        sem_post(&table_empty);
    }
}

void* paper_owner_func(void* arg) {
    while(true) {
        sem_wait(&agency_paper);
        cout << "paper_owner smoke" << endl;
        sleep(2);
        cout << "paper_owner smoke done" << endl;
        sem_post(&table_empty);
    }
}

void* match_owner_func(void* arg) {
    while(true) {
        sem_wait(&agency_match);
        cout << "match_owner smoke" << endl;
        sleep(2);
        cout << "match_owner smoke done" << endl;
        sem_post(&table_empty);
    }
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    init();    
    
    pthread_t thread[N];
	pthread_create(&thread[0], 0, agency_func, NULL);
    pthread_create(&thread[1], 0, tobacco_owner_func, NULL);
    pthread_create(&thread[2], 0, paper_owner_func, NULL);
    pthread_create(&thread[3], 0, match_owner_func, NULL);
	 
    for (int i = 0; i < N; ++i)
        pthread_join(thread[i], 0);

    return 0;
}
