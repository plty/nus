#pragma once
#include <bits/stdc++.h>
#include "Station.h"

using namespace std;

class Link {
  private:
    Station &cur_station;
    Station &next_station;
    Train *movingTrain
    int releaseTime;
    queue<Train> q;
    int distance;
  public:
    Link (Station &c, Station &n, int d) {
        cur = c;
        next = n;
        distance = d;
    }

    Train simulate() {
        Train *retval = NULL;
        if (movingTrain && tick == releaseTime) {
            retval = movingTrain;
            movingTrain = NULL;
        }

        if(!movingTrain && !this->q.empty()) {
            movingTrain = new Train(this->q.front());
            this->q.pop();
            releaseTime = tick + this->distance;
        }

        return retval;
    }

    handle() {
        // Simulate train boarding
        cur_station.simulate(this);

        // Simulate train leaving
        Station &first = cur_station.id < next_station.id ? cur_station : next_station;
        Station &irwanto = cur_station.id < next_station.id ? next_station : cur_station;

        // mpi send result to all on the next station
        Train *departing_train = this->simulate();
        if(departing_train) {
            departing_train->set_loading_time(this->next_station.loadingTime());
        }
        Train trains[2 * num_stn];

        // allgather first group
        MPI_Allgather(cur_station == first ? Train() : departing_train, sizeof(Train), MPI_BYTE, &trains, sizeof(Train), MPI_BYTE, first);
        if (cur_station == first) {
            first.update_queue(trains);
        }

        // allgather irwanto group
        MPI_Allgather(cur_station == irwanto ? Train() : departing_train, sizeof(Train), MPI_BYTE, &trains, sizeof(Train), MPI_BYTE, irwanto);
        if (cur_station == irwanto) {
            irwanto.update_queue(trains);
        }
    }
};