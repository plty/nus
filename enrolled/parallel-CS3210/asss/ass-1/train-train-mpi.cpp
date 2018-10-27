#include<bits/stdc++.h>
#include<unistd.h>
#include<mpi.h>
using namespace std;

#define UNDEFINED -1
#define MAX_N 200
#define MAX_LINK 40000
#define MAX_STATION 200
#define MAX_TRAIN 200
#define MAX_LINES 3
#define MAX_DURATION 1000

class Train;
class Stats;
class Station;
class Link;

Station *stations[MAX_N];
Link *links[MAX_N][MAX_N];
vector<Train> trains;
vector<int> routes[MAX_LINES];
int row_link[MAX_LINK];
int col_link[MAX_LINK];
int train_count[MAX_LINES];
int num_stn, num_link, num_line = 3;
int duration, tick;
map<string, int> stn_id;
map<int, string> stn_name;
char color_short_name[] = {'g', 'y', 'b'};
const char* color_long_name[] = {"green", "yellow", "blue"};

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

class Stats {
  public:
    int min_time;
    int max_time;
    int sum_time;
    int count;

    Stats(){
        this->min_time = 1 << 30;
        this->max_time = - 1 << 30;
        this->sum_time = 0;
        this->count = 0;
    }

    ~Stats() = default;

    int update(int t) {
        this->count++;
        this->sum_time += t;

        this->min_time = min(this->min_time, t);
        this->max_time = max(this->max_time, t);
    }
};

class Station {
  private:
    string name;
    double popularity;
    Train loading_train;
    int departure_time;
    Stats stat;
    int stat_min;
    int stat_max;
    int stat_sum;
    int stat_count;
  public:
    int group_size;
    MPI_Comm comm;
    deque<Train> q;
    Station(string name, double popularity) {
        this->name = name;
        this->popularity = popularity;
        this->departure_time = 0;
    }

    ~Station() = default;

    Stats get_stat() {
        return stat;
    }

    Train simulate() {
        Train retval;
        if (loading_train.valid() && tick >= departure_time) {
            retval = loading_train;
            loading_train = invalid_train;
        }

        if (!loading_train.valid() && !q.empty()) {
            stat.update(tick - departure_time);
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
        for (int i = 0; i < this->group_size; i++) {
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
    int id;
  public:
    deque<Train> q;
    Link(Station *current_station, Station *next_station, int distance) {
        this->current_station = current_station;
        this->next_station = next_station;
        this->distance = distance;
        this->arrival_time = 0;
        this->id = -1;
    }

    ~Link() = default;

    void set_id(int x) {
        this->id = x;
    }

    int get_id() {
        return this->id;
    }

    Stats get_stat() {
        return this->current_station->get_stat();
    }

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
        if (moving_train.valid() && tick == arrival_time) {
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

        MPI_Allgather(current_station == first ? &invalid_train : &arrived_train, sizeof(Train), MPI_BYTE, trains, sizeof(Train), MPI_BYTE, first->comm);
        if (current_station == first) {
            first->update_queue(trains);
        }

        MPI_Allgather(current_station == second ? &invalid_train : &arrived_train, sizeof(Train), MPI_BYTE, trains, sizeof(Train), MPI_BYTE, second->comm);
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

    num_link = 0;
    for (int i = 0; i < num_line; i++) {
        int len = routes[i].size();
        for (int j = 0; j < len; j++) {
            int cur = routes[i][j];
            int nxt = routes[i][(j + 1) % len];
            int nxtnxt = routes[i][(j + 2) % len];
            links[cur][nxt]->set_next(i, links[nxt][nxtnxt]);
            if (links[cur][nxt]->get_id() == -1) {
                links[cur][nxt]->set_id(num_link);
                row_link[num_link] = cur;
                col_link[num_link] = nxt;
                num_link++;
            }
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

    for (int i = 0; i < num_stn; i++) {
        int arr[2 * num_stn];
        int ctr = 0;
        for (int j = 0; j < num_stn; j++) {
            if (i != j && links[i][j]->get_id() != -1) {
                arr[ctr++] = links[i][j]->get_id();
            }
        }

        for (int j = 0; j < num_stn; j++) {
            if (j != i && links[j][i]->get_id() != -1) {
                arr[ctr++] = links[j][i]->get_id();
            }
        }

        stations[i]->group_size = ctr;

        MPI_Group world_group;
        MPI_Comm_group(MPI_COMM_WORLD, &world_group);
        MPI_Group new_group;
        MPI_Group_incl(world_group, ctr, arr, &new_group);
        MPI_Comm_create_group(MPI_COMM_WORLD, new_group, i, &stations[i]->comm);
    }
}

int main(int argc, char **argv) {
    freopen(argv[1], "r", stdin);
    MPI_Init(&argc, &argv);
    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    load_data();
    if (nprocs != num_link) {
        printf("num process supossed to be %d\n", num_link);
        return 0;
    }

    srand(time(NULL));
    // printf("Process %d Ready\n", rank);
    // fflush(stdout);
    // MPI_Barrier(MPI_COMM_WORLD);


    // Simulation per tick
    int row = row_link[rank], col = col_link[rank];
    for(tick = 0; tick < duration; tick ++) {
        links[row][col]->handle();

        // MPI receive new train arriving at cur
        if (rank == 0) {
            printf(tick == 0 ? "%d:" : "\n%d:", tick);
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        links[row][col]->print();
        fflush(stdout);
        MPI_Barrier(MPI_COMM_WORLD);
    }
    Stats stat = links[row][col]->get_stat();
    Stats stats[num_link];
    MPI_Gather(&stat, sizeof(Stats), MPI_BYTE, stats, sizeof(Stats), MPI_BYTE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nAverage Waiting Time:\n");
        for (int i = 0; i < num_line; i++) {
            double ave_ave = 0;
            double max_ave = 0;
            double min_ave = 0;
            int route_length = routes[i].size();
            int route_considered = 0;
            for (int j = 0; j < route_length; j++) {
                int cur = routes[i][j];
                int nxt = routes[i][(j + 1) % route_length];
                int id = links[cur][nxt]->get_id();
                if (!stats[id].count) continue;
                route_considered++;
                ave_ave += 1.0 * stats[id].sum_time / stats[id].count;
                max_ave += stats[id].max_time;
                min_ave += stats[id].min_time;
            }
            ave_ave /= route_considered;
            max_ave /= route_considered;
            min_ave /= route_considered;
            printf("%s: %d trains -> %.1lf, %.1lf, %1.lf\n", color_long_name[i], train_count[i], ave_ave, max_ave, min_ave);
        }
    }

    MPI_Finalize();
    return 0;
}

