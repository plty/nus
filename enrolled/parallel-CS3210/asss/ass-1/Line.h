#pragma once
#include <bits/stdc++.h>
#include "Link.h"

using namespace std;

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