#include<bits/stdc++.h>
#include <unistd.h>
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
char color_short_name[] = {'g', 'y', 'b'};

class Train {
  private:
    int id;
    int color;
    int loading_time;

    int destination;
    int next_destination;

  public:
    Train() {
        color = UNDEFINED;
    }

    Train(int color, int id) {
        this->color = color;
        this->id = id;
        this->loading_time = 0;
        this->destination = -1;
        this->next_destination = -1;
    };

    ~Train() = default;

    int get_color() {
        return this->color;
    }

    int get_id() {
        return this->id;
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

    int get_next_destination() {
        return this->next_destination;
    }

    void set_next_destination(int x) {
        this->next_destination = x;
    }
};

Train invalid_train = Train();

class Station {
  private:
    string name;
    double popularity;
    Train loading_train;
    int departure_time;
    int stat_min;
    int stat_max;
    int stat_ave;
    int stat_count;

  public:
    MPI_Comm comm;
    deque<Train> q;
    Station(string name, double popularity) {
        this->name = name;
        this->popularity = popularity;
        this->departure_time = 0;

        this->stat_count = 0;
        this->stat_sum = 0;

        this->stat_max = 1e9;
        this->stat_min = -1e9;
    }

    ~Station() = default;

    void update_statistics(int delta) {
        this->stat_sum += delta;
        this->stat_count++;
        this->stat_min = min(this->stat_min, delta);
        this->stat_max = max(this->stat_max, delta);
    }
    Train simulate() {
        Train retval;
        if (loading_train.valid() && tick >= departure_time) {
            retval = loading_train;
            loading_train = invalid_train;
        }

        if (!loading_train.valid() && !q.empty()) {
            this->update_statistics(tick - departure_time);
            loading_train = q.front();
            q.pop_front();
            departure_time = tick + loading_train.get_loading_time();
        }
        return retval;
    }

    int get_id() {
        return stn_id[this->name];
    }

    int get_popularity() {
        return popularity;
    }

    void update_queue(Train trains[]) {
        for (int i = 0; i < 2 * num_stn - 2; i++) {
            Train t = trains[i];
            if (t.valid() && t.get_destination() == this->get_id()) {
                q.push_back(t);
            }
        }
    }

    void print() {
        for (Train t : q) if (tick >= t.get_id()) {
                printf(" %c%d-s%d,", color_short_name[t.get_color()], t.get_id(), this->get_id());
            }

        if (loading_train.valid()) {
            printf(" %c%d-s%d,", color_short_name[loading_train.get_color()], loading_train.get_id(), this->get_id());
        }
    }
};

class Link {
  private:
    Station *current_station;
    Station *next_station;
    Link *next[MAX_LINES];
    int distance;
    int arrival_time;
    Train moving_train;

  public:
    deque<Train> q;
    Link(Station *current_station, Station *next_station, int distance) {
        this->current_station = current_station;
        this->next_station = next_station;
        this->distance = distance;
        this->arrival_time = 0;
    }

    ~Link() = default;

    void print() {
        int rank;
        MPI_Comm_rank(current_station->comm, &rank);
        if (rank == 0) {
            current_station->print();
        }

        for (Train t : this->q) {
            printf(" %c%d-s%d,", color_short_name[t.get_color()], t.get_id(), current_station->get_id());
        }

        if (moving_train.valid()) {
            printf(" %c%d-s%d->s%d,", color_short_name[moving_train.get_color()], moving_train.get_id(), current_station->get_id(), next_station->get_id());
        }
    }

    void set_next(int color, Link *next) {
        this->next[color] = next;
    }

    Train simulate() {
        Train retval;
        if (moving_train.valid() && tick >= arrival_time) {
            retval = moving_train;
            moving_train = invalid_train;
        }

        if (!moving_train.valid() && !q.empty()) {
            moving_train = q.front();
            q.pop_front();
            arrival_time = tick + distance;
        }
        return retval;
    }

    void handle() {
        // Simulating Station
        Train train = this->current_station->simulate();
        if (train.valid() && train.get_next_destination() == this->next_station->get_id()) {
            q.push_back(train);
        }

        // Simulating Link
        Train arrived_train = this->simulate();
        if (arrived_train.valid()) {
            arrived_train.set_destination(this->next[arrived_train.get_color()]->current_station->get_id());
            arrived_train.set_next_destination(this->next[arrived_train.get_color()]->next_station->get_id());
            int popularity = this->next[arrived_train.get_color()]->current_station->get_popularity();
            arrived_train.set_loading_time((rand() % 10 + 1) * popularity + 0.5);
        }

        Station *first = current_station->get_id() < next_station->get_id() ? current_station : next_station;
        Station *second = current_station->get_id() < next_station->get_id() ? next_station : current_station;
        Train trains[2 * num_stn - 2];

        MPI_Allgather(current_station == first ? &invalid_train : &arrived_train, sizeof(Train), MPI_BYTE, &trains, sizeof(Train), MPI_BYTE, first->comm);
        if (current_station == first) {
            first->update_queue(trains);
        }

        MPI_Allgather(current_station == second ? &invalid_train : &arrived_train, sizeof(Train), MPI_BYTE, &trains, sizeof(Train), MPI_BYTE, second->comm);
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

    // Duration
    scanf("%d", &duration);

    // Train count
    int train_count[MAX_LINES];
    for (int i = 0; i < num_line; i++) {
        scanf("%d%c", &train_count[i], &tmp);
    }

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

    for (int i = 0; i < num_line; i++) {
        int len = routes[i].size();
        Station *home_station = stations[routes[i][0]];
        for (int j = 0; j < train_count[i]; j++) {
            home_station->q.push_back(Train(i, j));
            home_station->q.back().set_next_destination(routes[i][1]);
        }
    }
}

int main(int argc, char **argv) {
    freopen(argv[1], "r", stdin);
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    load_data();
    srand(time(NULL));
    printf("Process %d Ready\n", rank);
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < num_stn; i++) {
        int arr[2 * num_stn];
        int ctr = 0;
        for (int j = 0; j < num_stn; j++) {
            if (i != j) {
                arr[ctr++] = id_of(i, j);
            }
        }


        for (int j = 0; j < num_stn; j++) {
            if (i != j) {
                arr[ctr++] = id_of(j, i);
            }
        }

        MPI_Group world_group;
        MPI_Comm_group(MPI_COMM_WORLD, &world_group);
        MPI_Group new_group;
        MPI_Group_incl(world_group, ctr, arr, &new_group);
        MPI_Comm_create_group(MPI_COMM_WORLD, new_group, i, &stations[i]->comm);
    }

    // Simulation per tick
    for(tick = 0; tick < duration; tick ++) {
        if(rank / num_stn != rank % num_stn) {
            links[rank / num_stn][rank % num_stn]->handle();
        }

        // MPI receive new train arriving at cur
        if (rank == 0) {
            printf(tick == 0 ? "%d:" : "\n%d:", tick);
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank / num_stn != rank % num_stn) {
            links[rank / num_stn][rank % num_stn]->print();
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == 0) {
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
