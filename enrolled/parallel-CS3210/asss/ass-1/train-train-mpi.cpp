#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;
#define UNDEFINED -1
#define MAX_N 200
#define MAX_STATION 200
#define MAX_TRAIN 200
#define MAX_LINES 3
#define MAX_DURATION 1000

class Train;
class Station;
class Link;

Station *stations[MAX_N];
Link *links[MAX_N][MAX_N];
vector<Train> trains;
int num_stn, num_line = 3;
int duration, tick;
map<string, int> stn_id;
map<int, string> stn_name;

class Train {
  private:
    int color;
    int loading_time;
    int destination;
  public:
    Train() {
        color = UNDEFINED;
    }

    Train(int color) {
        this->color = color;
    };

    ~Train() = default;

    int get_color() {
        return this->color;
    }
    int get_loading_time() {
        return this->loading_time;
    }

    void set_loading_time(int x) {
        this->loading_time = x;
    }

    bool valid() {
        return this->color != UNDEFINED;
    }

    int get_destination() {
        return this->destination;
    }

    void set_destination(int x) {
        this->destination = x;
    }
};

Train invalidTrain;

class Station {
  private:
    string name;
    double popularity;
    queue<Train> q;
    Train loading_train;
    int departure_time;
  public:
    MPI_Comm comm;
    Station(string name, double popularity) {
        this->name = name;
        this->popularity = popularity;
    }

    ~Station() = default;

    Train simulate() {
        Train retval;
        if (loading_train.valid() && tick == departure_time) {
            retval = loading_train;
            loading_train = Train();
        }

        if (!loading_train.valid() && !q.empty()) {
            loading_train = q.front();
            q.pop();
            departure_time = tick + loading_train.get_loading_time();
        }
        return retval;
    }

    int get_id() {
        return stn_id[this->name];
    }

    void update_queue(Train trains[]) {
        // this might be wrong
        for (int i = 0; i < 2 * num_stn - 2; i++) {
            Train t = trains[i];
            if (t.valid() && t.get_destination() == this->get_id()) {
                q.push(t);
            }
        }
    }
};

class Link {
  private:
    queue<Train> q;
    Station *current_station;
    Station *next_station;
    Link *next[MAX_LINES];
    int distance;
    int arrival_time;
    Train moving_train;
  public:
    Link(Station *current_station, Station *next_station, int distance) {
        this->current_station = current_station;
        this->next_station = next_station;
        this->distance = distance;
    }

    ~Link() = default;

    void set_next(int color, Link *next) {
        this->next[color] = next;
    }

    Train simulate() {
        Train retval;
        if (moving_train.valid() && tick == arrival_time) {
            retval = moving_train;
            moving_train = Train();
        }

        if (!moving_train.valid() && !q.empty()) {
            moving_train = q.front();
            q.pop();
            arrival_time = tick + distance;
        }
        return retval;
    }

    void handle() {
        // stationnya di simulate
        Train train = this->current_station->simulate();
        if (train.valid()) {
            q.push(train);
        }

        // gua nya disimulate
        Train arrived_train = this->simulate();
        train.set_destination(this->next[train.get_color()]->current_station->get_id());

        // pilih yang duluan
        Station *first = current_station->get_id() < next_station->get_id() ? current_station : next_station;
        Station *second = current_station->get_id() < next_station->get_id() ? next_station : current_station; 
	Train trains[2 * num_stn];

        MPI_Allgather(current_station == first ? &invalidTrain : &arrived_train, sizeof(Train), MPI_BYTE, &trains, sizeof(Train), MPI_BYTE, first->comm);
        if (current_station == first) {
            first->update_queue(trains);
        }

        // allgather second group
        MPI_Allgather(current_station == second ? &invalidTrain : &arrived_train, sizeof(Train), MPI_BYTE, &trains, sizeof(Train), MPI_BYTE, second->comm);
        if (current_station == second) {
            second->update_queue(trains);
        }
    }
};

int id_of(int x, int y) {
    return x * num_stn + y;
}

void load_data() {
    // Number of stations
    scanf("%d", &num_stn);
    char name[100], tmp;

    // Station name
    for (int i = 0; i < num_stn; i++) {
        scanf(" %[^\n^,]%c", name, &tmp);
        stn_id[name] = i;
        stn_name[i] = name;
    }

    // Inter station distance
    int dist[num_stn][num_stn];
    for (int i = 0; i < num_stn; i++) {
        for (int j = 0; j < num_stn; j++) {
            scanf("%d", &dist[i][j]);
        }
    }

    // Station popularity
    double popularity[num_stn];
    for (int i = 0; i < num_stn; i++) {
        scanf("%lf%c", &popularity[i], &tmp);
    }

    // Line route
    vector<int> routes[MAX_LINES];
    for (int i = 0; i < num_line; i++) {
        do {
            scanf(" %[^\n^,]%c", name, &tmp);
            routes[i].emplace_back(stn_id[name]);
        } while (tmp != '\n');
    }

    for (int i = 0; i < num_line; i++) {
        int len = routes[i].size();
        for (int j = (len - 1) - 1; j >= 1; j--) {
            routes[i].emplace_back(routes[i][j]);
        }
    }

    // Train count
    int train_count[MAX_LINES];
    for (int i = 0; i < num_line; i++) {
        scanf("%d", &train_count[i]);
    }

    // Duration
    scanf("%d", &duration);

    // Station
    for (int i = 0; i < num_stn; i++) {
        stations[i] = new Station(stn_name[i], popularity[i]);
    }

    // Links
    for (int i = 0; i < num_stn; i++) {
        for (int j = 0; j < num_stn; j++) {
            links[i][j] = new Link(stations[i], stations[j], dist[i][j]);
        }
    }

    for (int i = 0; i < num_line; i++) {
        int len = routes[i].size();
        for (int j = 0; j < len; j++) {
            int cur = routes[i][j];
            int nxt = routes[i][(j + 1) % len];
            int nxtnxt = routes[i][(j + 2) % len];
            links[cur][nxt]->set_next(i, links[nxt][nxtnxt]);
        }
    }
}

int main(int argc, char **argv) {
freopen("test.in", "r", stdin);
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Process %d Ready\n", rank);
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);

    load_data();
    srand(time(NULL));
    printf("Process %d Ready\n", rank);
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < num_stn; i++) {
        int arr[2 * num_stn];
        int ctr = 0;
        for (int j = 0; j < num_stn; j++) if (i != j) {
            arr[ctr++] = id_of(i, j);
        }

        for (int j = 0; j < num_stn; j++) if (i != j) {
            arr[ctr++] = id_of(j, i);
        }
        MPI_Group world_group;
        MPI_Comm_group(MPI_COMM_WORLD, &world_group);
        MPI_Group new_group;
        MPI_Group_incl(world_group, ctr, arr, &new_group);
        MPI_Comm_create(MPI_COMM_WORLD, new_group, &stations[i]->comm);
    }

    printf("Process %d Ready\n", rank);
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Simulation per tick
    for(tick = 0; tick < duration; tick ++) {
        links[rank / num_stn][rank % num_stn]->handle();
        // MPI receive new train arriving at cur
        if (rank == 0) {
            printf("%d:", tick);
            fflush(stdout);
        }

    return 0;
    }
}
