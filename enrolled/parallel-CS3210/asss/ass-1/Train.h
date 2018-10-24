#pragma once
#include <bits/stdc++.h>
#include "Link.h"

using namespace std;

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

    Link* link() {
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