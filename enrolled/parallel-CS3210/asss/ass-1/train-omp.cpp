#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <mutex>
#include <queue>
#include <omp.h>

using namespace std;

#define NONE -1
#define WAITING 1
#define LOADING 2
#define TRAVELING 4

#define is_waiting(x) ((x & WAITING) == WAITING)
#define is_loading(x) ((x & LOADING) == LOADING)
#define is_traveling(x) ((x & TRAVELING) == TRAVELING)

const int line_num = 3;
const string line_code[] = {"g", "y", "b"};
const string line_name[] = {"green", "yellow", "blue"};

int station_num;
int total_train;
vector<int> train_num;
vector<string> station_name;
vector<vector<int>> dist;
vector<double> popularity;
vector<vector<int>> lines;
map<string, int> station_idx;
int ticks;

vector<string> split(const string& str, const string& delim) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

vector<string> read_csv_line() {
	string buff; cin >> buff;
	return split(buff, ",");
}

class train {
public:
	int line_id;
	int line_train_id;
	int pos;
	int time_cp;
	int status;
	int start_time;

	train(int line_id, int line_train_id) : line_id(line_id), time_cp(0), line_train_id(line_train_id) {}

	int advance() {
		return pos = (pos + 1) % lines[line_id].size();
	}

	int bef() {
		return (lines[line_id].size() + pos - 1) % lines[line_id].size();
	}

	int loading() {
		int r = (rand() % 10) + 1;
		return max(1, (int)(popularity[pos] * r));
	}
};

vector<vector<vector<int>>> loading_timing;

class train_queue {
private:
	mutex q_mutex;
	int last_tick;
public:
	train_queue() : last_tick(-100) {}
	train_queue(const train_queue& o) : last_tick(-100) {}

	void push(int cur_tick, int waiting_time, train& t, bool flag = false) {
		q_mutex.lock();
		t.start_time = (last_tick = max(last_tick, cur_tick));
		t.time_cp = (last_tick += waiting_time);
		if (flag) {
			auto cur_station = lines[t.line_id][t.pos];
			loading_timing[cur_station][t.line_id].push_back(t.start_time);
			loading_timing[cur_station][t.line_id].push_back(t.time_cp);
		}
		q_mutex.unlock();
	}
};

vector<train_queue> loading_queue;
vector<vector<train_queue>> link_queue;
vector<train> trains;

int main(int argc, char** argv) {
    bool print = false;
#if DEBUG
    print = true; 
#endif
	
	// input
	cin >> station_num;
	station_name = read_csv_line();
	for(int i = 0 ; i < station_num ; i ++) {
		station_idx[station_name[i]] = i;
	}
	dist = vector<vector<int>>(station_num, vector<int>(station_num, 0));
	for(int i = 0 ; i < station_num ; i ++) {
		for(int j = 0 ; j < station_num ; j ++) {
			cin >> dist[i][j];
		}
	}
	auto pop_string = read_csv_line();
	popularity.resize(station_num);
	for(int i = 0 ; i < station_num ; i ++) {
		popularity[i] = stod(pop_string[i]);
	}
	lines.resize(line_num);
	for(int i = 0 ; i < line_num ; i ++) {
		auto line = read_csv_line();
		auto line_length = line.size();
		for(int j = 0 ; j + 2 < line_length ; j ++) {
			line.push_back(line[line_length - 2 - j]);
		}
		line_length = line.size();
		lines[i].resize(line_length);
		for(int j = 0 ; j < line_length ; j ++) {
			lines[i][j] = station_idx[line[j]];
		}
	}
	cin >> ticks;
	total_train = 0;
	auto train_num_string = read_csv_line();
	train_num.resize(line_num);
	for(int i = 0 ; i < line_num ; i ++) {
		train_num[i] = stoi(train_num_string[i]);
		total_train += train_num[i];
	}

	// init
	loading_queue.resize(station_num);
	link_queue = vector<vector<train_queue>>(station_num, vector<train_queue>(station_num, train_queue()));
	for(int i = 0 ; i < line_num ; i ++) {
		for(int j = 0 ; j < train_num[i] ; j ++) {
			trains.push_back({i, j});
		}
	}
	loading_timing.resize(station_num);
	for(int i = 0 ; i < station_num ; i ++) {
		loading_timing[i].resize(line_num);
		for(int j = 0 ; j < line_num ; j ++) {
			loading_timing[i][j] = vector<int>();
		}
	}

	// simulation
	#pragma omp parallel num_threads(total_train)
	for(int tick = 0; tick < ticks ; tick ++) {
		#pragma omp barrier
		#pragma omp for
		for(int train_id = 0 ; train_id < total_train ; train_id ++) {
			auto& cur = trains[train_id];
			if (cur.line_train_id == tick) {
				cur.status = LOADING | WAITING;
				int line_id = cur.line_id;
				loading_queue[lines[line_id][0]].push(0, cur.loading(), cur, true);
			}
			if (cur.time_cp <= tick) {
				if (is_traveling(cur.status)) {
					// arrived
					auto pos = cur.pos;
					auto nxt_station = lines[cur.line_id][pos];
					loading_queue[nxt_station].push(tick, cur.loading(), cur, true);
					cur.status = LOADING | WAITING;
				} else if(is_loading(cur.status)) {
					// departing
					auto cur_station = lines[cur.line_id][cur.pos];
					auto pos = cur.advance();
					auto nxt_station = lines[cur.line_id][pos];
					link_queue[cur_station][nxt_station].push(tick, dist[cur_station][nxt_station], cur);
					cur.status = TRAVELING | WAITING;
				}
			} 

			if (cur.start_time == tick) {
				cur.status ^= WAITING;
			}
		}

		if (print && !omp_get_thread_num()) {
			cout << tick << ":";
			for(int train_id = 0 ; train_id < total_train ; train_id ++) {
				auto& cur = trains[train_id];
				if (cur.line_train_id > tick) continue;
				if(train_id != 0) cout << ",";
				cout << " ";
				cout <<	line_code[cur.line_id] << cur.line_train_id << "-";
				if(is_loading(cur.status)) {
					cout << "s" << lines[cur.line_id][cur.pos];
				} else if(is_traveling(cur.status) && tick < cur.start_time) {
					cout << "s" << lines[cur.line_id][cur.bef()];
				} else {
					cout << "s" << lines[cur.line_id][cur.bef()];
					cout << "->";
					cout << "s" << lines[cur.line_id][cur.pos];
				}
			}
			cout << endl;
		}
	}
	
	cout << "Average waiting times:" << endl;
	for(int i = 0 ; i < line_num ; i ++) {
		cout << line_name[i] << ": " << train_num[i] << " trains -> ";
		double avg = 0;
		double cntr = 0;
		for(int j = 0 ; j < station_num ; j ++) {
			for(int k = 1 ; k + 1 < loading_timing[j][i].size() ; k += 2) {
				avg += loading_timing[j][i][k + 1] - loading_timing[j][i][k];
				cntr += 1;
			}
		}
		avg /= cntr;
		cout << avg;

		avg = 0;
		cntr = 0;
		for(int j = 0 ; j < station_num ; j ++) {
			int mx = -1;
			for(int k = 1 ; k + 1 < loading_timing[j][i].size() ; k += 2) {
				mx = max(mx, loading_timing[j][i][k + 1] - loading_timing[j][i][k]);
			}
			if(mx != -1) {
				cntr ++;
				avg += mx;
			}
		}
		avg /= cntr;
		cout << ", " << avg;

		avg = 0;
		cntr = 0;
		for(int j = 0 ; j < station_num ; j ++) {
			int mn = ticks * 2;
			for(int k = 1 ; k + 1 < loading_timing[j][i].size() ; k += 2) {
				mn = min(mn, loading_timing[j][i][k + 1] - loading_timing[j][i][k]);
			}
			if(mn != ticks * 2) {
				cntr ++;
				avg += mn;
			}
		}
		avg /= cntr;
		cout << ", " << avg;

		cout << endl;
	}
}
