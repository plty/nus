#pragma once
#include <bits/stdc++.h>
#include <mpi.h>
#include "Train.h"

using namespace std;

// Basically station is maintained by every link independently
class Station {
  private:
    MPI_Comm comm;
    string name;
    double popularity;
    queue<Train> q;
    Train *loadingTrain;
    int releaseTime;
  public:
    MPI_Comm comm;
    Station(String n, double p) {
        name  = n;
        popularity = p;
    }

    int loadingTime() {
        return (rand() % 10 + 1) * popularity + 0.5;
    }

    void simulate(Link *link) {
        if (loadingTrain && tick == releaseTime) {
            if (train.link == link) {
                link.push(loadingTrain);
            }

            loadingTrain = NULL;
        }

        if (!loadingTrain && !q.empty()) {
            loadingTrain = new Train(q.front());
            q.pop();
            releaseTime = tick + loadingTrain.get_loading_time();
        }
    }

    void update_queue(Train trains[]) {
        for(int i = 0 ; i < 2 * num_stn ; i ++) {
            if(trains[i].get_color() == UNDEFINED) {
                continue;
            }

            q.push(Train(train));
        }
    }

    bool operator==(Train &o) {
        return this == &o;
    }
};