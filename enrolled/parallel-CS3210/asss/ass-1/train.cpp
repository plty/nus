#include<bits/stdc++.h>
using namespace std;

#define MAX_N 200
#define MAX_LINES 3
#define fi first
#define se second

map<string, int> stn_id;
map<int, string> stn_name;
vector<int> lines[MAX_LINES];
int num_train[MAX_LINES];
int dist[MAX_N][MAX_N];
double popularity[MAX_N];
int num_stn, num_line = 3, duration;

void load_data() {
    scanf("%d", &num_stn);
    for (int i = 0; i < num_stn; i++) {
        char name[100], tmp; scanf(" %[^\n^,]%c", name, &tmp);
        stn_id[name] = i;
        stn_name[i] = name;
    }

    for (int i = 0; i < num_stn; i++)
        for (int j = 0; j < num_stn; j++)
            scanf("%d", &dist[i][j]);

    for (int i = 0; i < num_stn; i++) {
        char tmp; scanf("%lf%c", &popularity[i], &tmp);
    }

    for (int i = 0; i < num_line; i++) {
        char name[100], tmp; 
        do {
            scanf(" %[^\n^,]%c", name, &tmp);
            lines[i].push_back(stn_id[name]);
        } while (tmp != '\n');
    }

    scanf("%d", &duration);
    for (int i = 0; i < num_line; i++)
        scanf("%d", &num_train[i]);
}

void debug_data() {
    for (auto &stn: stn_id)
        printf("%s: %d\n", stn.fi.c_str(), stn.se);
    printf("\n");

    for (int i = 0; i < num_stn; i++) {
        for (int j = 0; j < num_stn; j++) {
            printf("%2d ", dist[i][j]);
        } printf("\n");
    } printf("\n"); 

    for (int i = 0; i < num_stn; i++) {
        printf("%lf " , popularity[i]);
    } printf("\n\n");

    for (int i = 0; i < num_line; i++) {
        printf("lines %d: ", i);
        for (int stn: lines[i]) printf("%d ", stn);
        printf("\n");
    } printf("\n");
}

int main() {
    load_data();
}
