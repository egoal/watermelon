#include <cmath>
#include <iostream>
#include <memory>

using namespace std;
typedef unsigned int uint;

static int multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};


class Map {
    public:
        Map() {
            for(int r=0; r<get_height(); ++r)
                for(int c=0; c<get_width(); ++c)
                    data[r][c]  =   0;
            data[10][10]    =   1;
            data[9][10] =   1;
            data[8][10] =   1;

            data[6][5]  =   1;
            data[6][6]  =   1;
            for(int r=0; r<get_height(); ++r)
                for(int c=0; c<get_width(); ++c)
                    light[r][c]  =   0;
        };

        void set_visible(uint x, uint y, bool visible) {
            // Set the visibility of the cell at the given position.
            light[y][x] =   visible?1:0;
        }

        uint get_width() const {
            // Return the width of the map.
            return 30;
        }

        uint get_height() const {
            // Return the height of the map.
            return 20;
        }

        bool is_opaque(uint x, uint y) const {
            // Return whether the given position holds an opaque cell.
            return data[y][x]==1;
        }

        void print(ostream& os=std::cout){
            for(int r=0; r<get_height(); ++r){
                for(int c=0; c<get_width(); ++c){
                    if(data[r][c]==1) os<<'#';
                    else if(light[r][c]==1) os<<'o';
                    else os<<' ';
                }  
                os<<"\n";
            }
        }

        int data[20][30];
        int light[20][30];
};

void cast_light(Map& map, uint x, uint y, uint radius, uint row,
        float start_slope, float end_slope, uint xx, uint xy, uint yx,
        uint yy) {
    if (start_slope < end_slope) {
        return;
    }
    float next_start_slope = start_slope;
    for (uint i = row; i <= radius; i++) {
        bool blocked = false;
        for (int dx = -i, dy = -i; dx <= 0; dx++) {
            float l_slope = (dx - 0.5) / (dy + 0.5);
            float r_slope = (dx + 0.5) / (dy - 0.5);
            if (start_slope < r_slope) {
                continue;
            } else if (end_slope > l_slope) {
                break;
            }

            int sax = dx * xx + dy * xy;
            int say = dx * yx + dy * yy;
            if ((sax < 0 && (uint)std::abs(sax) > x) ||
                    (say < 0 && (uint)std::abs(say) > y)) {
                continue;
            }
            uint ax = x + sax;
            uint ay = y + say;
            if (ax >= map.get_width() || ay >= map.get_height()) {
                continue;
            }

            uint radius2 = radius * radius;
            if ((uint)(dx * dx + dy * dy) < radius2) {
                map.set_visible(ax, ay, true);
            }

            if (blocked) {
                if (map.is_opaque(ax, ay)) {
                    next_start_slope = r_slope;
                    continue;
                } else {
                    blocked = false;
                    start_slope = next_start_slope;
                }
            } else if (map.is_opaque(ax, ay)) {
                blocked = true;
                next_start_slope = r_slope;
                cast_light(map, x, y, radius, i + 1, start_slope, l_slope, xx,
                        xy, yx, yy);
            }
        }
        if (blocked) {
            break;
        }
    }
}

void do_fov(Map& map, uint x, uint y, uint radius) {
    for (uint i = 0; i < 8; i++) {
        cast_light(map, x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
                multipliers[1][i], multipliers[2][i], multipliers[3][i]);
    }
}

int main(){
    Map m;
    // m.print(cout);

    do_fov(m, 9, 9, 10);
    // cout<<"----------------------------------------\n";
    m.print(cout);

    return 0;
}