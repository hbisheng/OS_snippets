#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<cstdlib>
#include<iomanip>

using namespace std;

const int N = 4;
const int COUT_INTERVAL = 20;
const int MAX_LOOP = 10;

int round = 0;
bool time_to_exit = false;

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
        
        ++round;
        if(round == MAX_LOOP) {
            time_to_exit = true;
            sem_post(&agency_tobacco);
            sem_post(&agency_paper);
            sem_post(&agency_match);
            break;
        }
        
        int soybean = rand() % 3;
        cout << setw(COUT_INTERVAL) << "Wakeup!" << endl;
        sleep(2);
        if(soybean == 0) {
            cout << setw(COUT_INTERVAL) << "Place paper&match" << endl;
            cout << setw(COUT_INTERVAL) << "Wakeup tobacco owner" << endl;
            sem_post(&agency_tobacco);
        } else if(soybean == 1) {
            cout << setw(COUT_INTERVAL) << "Place tobacco&match" << endl;
            cout << setw(COUT_INTERVAL) << "Wakeup paper owner" << endl;
            sem_post(&agency_paper);
        } else if(soybean == 2) {
            cout << setw(COUT_INTERVAL) << "Place tobacco&paper" << endl;
            cout << setw(COUT_INTERVAL) << "Wakeup match owner" << endl;
            sem_post(&agency_match);
        }
        
    }
    
    return NULL;
}

void* tobacco_owner_func(void* arg) {
    while(true) {
        sem_wait(&agency_tobacco);
        if(time_to_exit) break;
        
        cout << setw(2*COUT_INTERVAL) << "Grab paper&match" << endl;
        cout << setw(2*COUT_INTERVAL) << "Smoke..." << endl;
        sleep(2);
        cout << setw(2*COUT_INTERVAL) << "Wakeup agent" << endl;
        sem_post(&table_empty);
    }
}

void* paper_owner_func(void* arg) {
    while(true) {
        sem_wait(&agency_paper);
        if(time_to_exit) break;
        
        cout << setw(3*COUT_INTERVAL) << "Grab tobacco&match" << endl;
        cout << setw(3*COUT_INTERVAL) << "Smoke..." << endl;
        sleep(2);
        cout << setw(3*COUT_INTERVAL) << "Wakeup agent" << endl;
        sem_post(&table_empty);
    }
}

void* match_owner_func(void* arg) {
    while(true) {
        sem_wait(&agency_match);
        if(time_to_exit) break;
        
        cout << setw(4*COUT_INTERVAL) << "Grab tobacco&paper" << endl;
        cout << setw(4*COUT_INTERVAL) << "Smoke..." << endl;
        sleep(2);
        cout << setw(4*COUT_INTERVAL) << "Wakeup agent" << endl;
        sem_post(&table_empty);
    }
}

int main(int argc, char const *argv[])
{
    cout << setw(COUT_INTERVAL) << "Agency" 
        << setw(COUT_INTERVAL) << "Tabacco_owner" 
        << setw(COUT_INTERVAL) << "Paper_owner" 
        << setw(COUT_INTERVAL) << "Match_owener" << endl;
    
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
