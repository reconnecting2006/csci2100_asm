#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
typedef enum {vipBanking, ordinaryBanking} accountType;

typedef struct customerDataT {
    int timeOfArrival;
    int ServiceTimeStillNeeded;
    accountType A;
} customerDataT;

typedef struct counterT {
    int numberOfCustomersServed;
    int totalServiceTime;
} counterT;
int getBestQueue(accountType type, queueADT queues[], customerDataT *serving[]) {
    int startIdx = (type == vipBanking) ? 0 : 2; 
    int bestQ = startIdx;
    int minLen = QueueLength(queues[startIdx]) + (serving[startIdx] != NULL ? 1 : 0);

    for (int i = startIdx + 1; i < 5; i++) {
        int currentLen = QueueLength(queues[i]) + (serving[i] != NULL ? 1 : 0);
        if (currentLen < minLen) {
            minLen = currentLen;
            bestQ = i;
        }
    }
    return bestQ;
}

int main() {
    FILE *fin = fopen("bankcustomers.dat", "r");
    FILE *fout = fopen("bank.out", "w");
    counterT counter[5];
    queueADT customerQueue[5];
    customerDataT *serving[5]; 
    for (int i = 0; i < 5; i++) {
        customerQueue[i] = EmptyQueue(); 
        serving[i] = NULL;
        counter[i].numberOfCustomersServed = 0;
        counter[i].totalServiceTime = 0;
    }
    double totalWaitVIP = 0, totalWaitOrd = 0;
    int countVIP = 0, countOrd = 0;
    double waitOrdByQ[5] = {0};
    int servedOrdByQ[5] = {0};
    char line[81];
    int time = 1; 
    int simulationActive = 1;
    while (simulationActive) {
        char *input = fgets(line, 80, fin);
        if (input != NULL) {
            if (line[0] == 'V' || line[0] == 'O') {
                customerDataT *newCust = malloc(sizeof(customerDataT));
                newCust->timeOfArrival = time;
                newCust->A = (line[0] == 'V') ? vipBanking : ordinaryBanking;
                sscanf(line + 1, "%d", &newCust->ServiceTimeStillNeeded);

                int qIdx = getBestQueue(newCust->A, customerQueue, serving);
                Enqueue(customerQueue[qIdx], newCust);
            }
        }
        int systemBusy = 0;
        for (int i = 0; i < 5; i++) {
            if (serving[i] == NULL && !QueueIsEmpty(customerQueue[i])) {
                serving[i] = Dequeue(customerQueue[i]);
                int wait = time - serving[i]->timeOfArrival;
                if (serving[i]->A == vipBanking) {
                    totalWaitVIP += wait;
                    countVIP++;
                } else {
                    totalWaitOrd += wait;
                    countOrd++;
                    waitOrdByQ[i] += wait;
                    servedOrdByQ[i]++;
                }
            }
            if (serving[i] != NULL) {
                systemBusy = 1;
                serving[i]->ServiceTimeStillNeeded--;
                if (serving[i]->ServiceTimeStillNeeded == 0) {
                    counter[i].numberOfCustomersServed++;
                    free(serving[i]);
                    serving[i] = NULL;
                }
            }
        }
        if (input == NULL && !systemBusy) {
            simulationActive = 0;
        } else {
            time++;
        }
    }
    fprintf(fout, "On average a VIP account holder needs to wait for %.2f minutes after he joins a queue until he leaves the queue and starts to be served.\n", 
            countVIP ? totalWaitVIP / countVIP : 0.0);
    fprintf(fout, "On average an ordinary account holder needs to wait for %.2f minutes after he joins a queue until he leaves the queue and starts to be served.\n", 
            countOrd ? totalWaitOrd / countOrd : 0.0);
    fprintf(fout, "The average time an ordinary account holder needs to wait after he joins queue 2 until he leaves queue 2 and starts to be served is %.2f minutes.\n", 
            servedOrdByQ[2] ? waitOrdByQ[2] / servedOrdByQ[2] : 0.0);
    fprintf(fout, "The average time an ordinary account holder needs to wait after he joins queue 3 until he leaves queue 3 and starts to be served is %.2f minutes.\n", 
            servedOrdByQ[3] ? waitOrdByQ[3] / servedOrdByQ[3] : 0.0);
    fprintf(fout, "The average time an ordinary account holder needs to wait after he joins queue 4 until he leaves queue 4 and starts to be served is %.2f minutes.\n", 
            servedOrdByQ[4] ? waitOrdByQ[4] / servedOrdByQ[4] : 0.0);
    for (int i = 0; i < 5; i++) {
        fprintf(fout, "The total number of customers counter %d serves is %d.\n", i, counter[i].numberOfCustomersServed);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}