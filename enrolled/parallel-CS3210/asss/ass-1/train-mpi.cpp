#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

#define UNDEFINED -1
#define MAX_N 200
#define MAX_STATION 200
#define MAX_TRAIN 200
#define MAX_LINES 3
#define MAX_DURATION 1000

#define fi first
#define se second
const int inf = 1e9;
map<string, int> stn_id;
map<int, string> stn_name;
int num_train[MAX_LINES], tot_train;
string output[MAX_STATION][MAX_DURATION];
int num_stn, num_line = 3, duration;

int tick;

class Train {
  private:
    int color;
    int line_id;
    int loading_time;
  public:
    Train() {
        color = UNDEFINED;
    }

    Train(Train &o) {
        this.color = o.get_color();
        this.link_id = o.get_link_id();
    }

    void next() {
        line_id = line[color][line_id]->get_next()->get_id();
    }

    Link *link() {
        return line[color][line_id]->get_link();
    }

    int get_color() {
        return color;
    }

    int get_link_id() {
        return link_id;
    }

    int get_loading_time() {
        return loading_time;
    }

    void set_loading_time(int o) {
        this->loading_time = o;
    }
};
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


class Line {
  private:
    int id;
    Station *station;
    Link *link;
    Line *next;
  public:
    Line(int id, Station *station, Link *link) {
        this->id = id;
        this->station = station;
        this->link = link;
    }

    Link *get_link() {
        return this->link;
    }

    void set_next(Line *n) {
        this->next = next;
    }

    int get_id() {
        return id;
    }

    Line* get_next() {
        return this->next;
    }
}
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

Station stations[MAX_N];
Link links[MAX_N][MAX_N];
vector<Train> trains;
vector<Line> lines[MAX_LINES];

void load_data() {
    // Number of stations
    scanf("%d", &num_stn;)
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
    int popularity[num_stn];
    for (int i = 0; i < num_stn; i++) {
        scanf("%lf%c", &popularity[i], &tmp);
    }

    // Line route
    vector<int> route[MAX_LINES];
    for (int i = 0; i < num_line; i++) {
        do {
            scanf(" %[^\n^,]%c", name, &tmp);
            route[i].emplace_back(stn_id[name]);
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
        stations.emplace_back(Station(stn_name[i], popularity[i]));
    }

    // Links
    for (int i = 0; i < num_stn; i++) {
        for (int j = 0; j < num_stn; j++) {
            links[i][j] = Link(stations[i], stations[j], dist[i][j], i);
        }
    }

    // Lines
    for (int i = 0; i < num_line; i++) {
        int len = routes[i].size();
        for (int j = 0; j < len; j++) {
            auto cur = routes[i][j];
            auto nxt = routes[i][(j + 1) % len];
            Lines[i].emplace_back(Line(j, &stations[cur], links[cur][nxt]));
        }

        for (int j = 0; j < len; j++) {
            Lines[i][j].set_next(Lines[i][(j + 1) % len]]);
        }
    }
}

int id_of(int x, int y) {
    return x * num_stn + y;
}

int main() {
    load_data();
    srand(time(NULL));
    for (int i = 0; i < num_stn; i++) {
        int arr[2 * num_stn];
        int ctr = 0;
        for (int j = 0; j < num_stn; j++) {
            arr[ctr++] = id_of(i, j);
        }

        for (int j = 0; j < num_stn; j++) {
            arr[ctr++] = id_of(j, i);
        }

        MPI_Group new_group;
        MPI_Group_incl(MPI_COMM_WORLD, 2 * num_stn, arr, &new_group);
        MPI_Comm_create(MPI_COMM_WORLD, new_group, &Station[i].comm);
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Simulation per tick
    for(tick = 0; tick < duration; tick ++) {
        Link[rank / num_stn][rank % num_stn].handle();
        // mpi receive new train arriving at cur
        MPI_Barrier(MPI_COMM_WORLD);
    }
}
