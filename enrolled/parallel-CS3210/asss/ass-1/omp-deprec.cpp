#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

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

class Link {
    private:
        int opened;
        int w;
    public:
        Link () { w = 0; }
        Link (int _w) { w = _w; }

        int acquire() {
            int retval = 0;
            #pragma omp critical    
            {
                retval = opened += w;
            }
            return retval;
        }
};

class Station {
    private:
        string name = "";
        int shortest = INF;
        int longest = -1;
        int total = 0;
        int count;

        int opened = 0;
        double popularity;
    public:
        Station() { popularity = 0.0; }

        Station(String n, double p) { 
            name  = n;
            popularity = p; 
        }

        int acquire() {
            int retval = 0;
            #pragma omp critical    
            {
                if (opened < duration) {
                    int delta = max(time - opened, 0);
                    shortest = min(shortest, delta);
                    longest = max(longest, delta);
                    total += delta;
                    count++;
                }

                int loadingTime = (rand() % 10 + 1) * popularity + 0.5;
                opened = opened = max(opened, time) + loadingTime;
            }
            return retval;
        }
};


class Line {
    public:
        string name;
        Line* next;
        Link* link;
        Station* station;
        Line (Station* _station) {
            station = _station;
        }
};

class Train {
    private:
        String name;
        int id;
        Line* line;
        int timeLoaded = -1;
        int timeMoved = -1;
        int timeSpawned = 0;
    public:
        Train() {
            line = NULL;
            timeSpawned = 0;
        }

        Train(String name, int id, Line *_line, int _time) {
            line = _line;
            timeSpawned = _time;
        }

        void moveToNext() {
            moveToNext();
            timeLoaded = -1;
            timeMoved = -1;
            line = line->next;
        }

        void handle() {
            if (timeSpawned > curTime) { return; }
            if (timeMoved == curTime) moveToNext()
;
            status[id] = name + "-" line->name;
            if (timeLoaded == -1) {
                timeLoaded = line->station->acquire();
            }
            if (timeLoaded > curTime) { return; }

            if (timeMoved == -1) {
                timeMoved = line->link->acquire();
            }  

            int timeStartMoving = timeMoved - line->link->w;
            if (curTime >= timeStartMoving)
                status[id] += "->" + line->next->name;
        }
};

Station stations[MAX_N];
Link links[MAX_N][MAX_N];
vector<Train> trains;
vector<Line> lines[MAX_LINES];

void load_data() {
    scanf("%d", &num_stn);
    for (int i = 0; i < num_stn; i++) {
        char name[100], tmp; scanf(" %[^\n^,]%c", name, &tmp);
        stn_id[name] = i;
        stn_name[i] = name;
    }

    for (int i = 0; i < num_stn; i++) {
        for (int j = 0; j < num_stn; j++) {
            int w; scanf("%d", &w);
            links[i][j] = Link(w);
        }
    }

    for (int i = 0; i < num_stn; i++) {
        double p; 
        char tmp;
        scanf("%lf%c", &p, &tmp);
        stations[i] = Station(p);
    }

    for (int i = 0; i < num_line; i++) {
        char name[100], tmp;
        do {
            scanf(" %[^\n^,]%c", name, &tmp);
            lines[i].push_back(Line(&stations[stn_id[name]]));
        } while (tmp != '\n');
        int len = lines[i].size();

        // create cycle
        for (int j = len - 2; j >= 1; j--)
            lines[i].push_back(Line(lines[i][j].station));

        int cycLength = lines[i].size();
        for (int j = 0; j < cycLength; j++) {
            lines[i][j].next = &lines[i][(j + 1) % cycLength];
            lines[i][j].link = &links[i][j];
        }
    }

    scanf("%d", &duration);

    string prefix[] = {"g", "y" , "b"};
    for (int i = 0; i < num_line; i++) {
        int nTrain; scanf("%d", &nTrain);
        for (int j = 0; j < nTrain; j++)
            trains.push_back(Train(prefix[i] + to_string(j), &lines[i][0], j));
    }
}

int main() {
    load_data();
    srand(time(NULL));
    omp_set_num_threads(trains.size());
    #pragma omp parallel
    {
        Train &train = trains[omp_get_thread_num()];
        for (int i = 0; i < duration; i++ ){
            train.handle();
            #pragma omp barrier
        }
    }
    for (int i = 0; i < duration; i++) {
        printf("%d: ", i);
        for (j = 0; j < trains.size(); j++) {
        }
    }
}
