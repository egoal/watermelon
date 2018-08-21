#!/usr/bin/python3

import sys, os
import random

MAP_WIDTH, MAP_HEIGHT = 64, 64

UP, DOWN, LEFT, RIGHT = 1, -1, 2, -2
DIRECTIONS = [UP, DOWN, LEFT, RIGHT]
HORIZONTAL, VERTICAL = 'horizontal', 'vertical'
CORRIDOR_DIRECTIONS = [HORIZONTAL, VERTICAL]

ROOM_WIDTH_RANGE, ROOM_HEIGHT_RANGE = [6, 12], [6, 12]
CORRIDOR_LENGTH_RANGE = [4, 12]

ROOM_NUMBER_RANGE = [16, 24]
PROBABILITY_SPAWN_ROOM = .4

def clamp(x, min, max):
    if x<min:
        return x
    elif x>max:
        return max
    return x

# [x1->x2], [y1->y2]
class Rect:
    def __init__(self, x1, x2, y1, y2):
        assert(x1<=x2 and y1<=y2)
        self.x1, self.x2, self.y1, self.y2 = x1, x2, y1, y2

    def random(self, inner=0):
        return (random.randint(self.x1+inner, self.x2-inner), 
            random.randint(self.y1+inner, self.y2-inner))

    def w(self):
        return self.x2-self.x1+1
    def h(self):
        return self.y2-self.y1+1

    def inner_rect(self, inner):
        return Rect(self.x1+inner, self.x2-inner, self.y1+inner, self.y2-inner)

    def center(self):
        return ((self.x1+self.x2)//2, (self.y1+self.y2)//2)

    def __str__(self):
        return '({}, {})->({}, {})'.format(self.x1, self.y1, self.x2, self.y2)

class Exit(Rect):
    # direct: direction to expand
    def __init__(self, x1, x2, y1, y2, direct):
        super().__init__(x1, x2, y1, y2)
        self.direction = direct

    def __str__(self):
        return super().__str__()+': {}'.format(self.direction)

# map tiles
tiles = None

def init_map():
    global tiles
    tiles = [[' ' for i in range(MAP_WIDTH)] for j in range(MAP_HEIGHT)]

def print_map():
    print('..|', ''.join([str(i%10) for i in range(MAP_WIDTH)]))
    print('-'*(MAP_WIDTH+3))
    i = 0
    for row in tiles:
        print('%02d|' % i, ''.join(row), )
        i += 1

def set_tile(x, y, tile):
    tiles[y][x] = tile
def set_rect(r, tile):
    for x in range(r.x1, r.x2+1):
        for y in range(r.y1, r.y2+1):
            set_tile(x, y, tile)
def set_room(r):
    set_rect(r, '#')
    set_rect(r.inner_rect(1), '.')

# now generate
rooms, exits = [], []

def make_rect(x, y, w, h):
    return Rect(x, x+w-1, y, y+h-1)

def can_place_rect(rect):
    if rect.x1<1 or rect.x2>=MAP_WIDTH-1 or rect.y1<1 or rect.y2>=MAP_HEIGHT-1:
        return False

    # no overlap
    for x in range(rect.x1, rect.x2+1):
        for y in range(rect.y1, rect.y2+1):
            if tiles[y][x]!=' ':
                return False

    return True

def expand(rect, direct):
    if not direct==UP and rect.w()>1:
        e = Exit(rect.x1, rect.x2, rect.y2+1, rect.y2+1, DOWN)
        if can_place_rect(e):
            exits.append(e)
    if not direct==DOWN and rect.w()>1:
        e = Exit(rect.x1, rect.x2, rect.y1-1, rect.y1-1, UP)
        if can_place_rect(e):
            exits.append(e)
    if not direct==LEFT and rect.h()>1:
        e = Exit(rect.x2+1, rect.x2+1, rect.y1, rect.y2, RIGHT)
        if can_place_rect(e):
            exits.append(e)
    if not direct==RIGHT and rect.h()>1:
        e = Exit(rect.x1-1, rect.x1-1, rect.y1, rect.y2, LEFT)
        if can_place_rect(e):
            exits.append(e)

def place_first_room():
    w = random.randint(ROOM_WIDTH_RANGE[0], ROOM_WIDTH_RANGE[1])
    h = random.randint(ROOM_HEIGHT_RANGE[0], ROOM_HEIGHT_RANGE[1])
    inner = 10
    x1 = random.randint(0+inner, MAP_WIDTH-inner-w)
    y1 = random.randint(0+inner, MAP_HEIGHT-inner-h)

    r = Rect(x1, x1+w-1, y1, y1+h-1)
    c = r.center()
    set_tile(c[0], c[1], '0')
    rooms.append(r)

    expand(r.inner_rect(1), None)
    set_room(r)

def place_a_room():
    eid = random.randint(0, len(exits)-1)
    e = exits[eid]
    pos = e.random()

    room = place_a_room_at(pos[0], pos[1], e.direction)

    # update exits
    if room:
        # set door
        dx, dy = pos
        # if e.direction==UP:
        #     dy += 1
        # elif e.direction==DOWN:
        #     dy -= 1
        # elif e.direction==LEFT:
        #     dx += 1
        # elif e.direction==RIGHT:
        #     dx -= 1
        # if tiles[dy][dx]=='.':
        #     set_tile(pos[0], pos[1], 'D')

        del exits[eid]
        expand(room.inner_rect(1), e.direction)
        set_room(room)
        set_tile(dx, dy, 'D')        

    return room
        

def place_a_room_at(x, y, direct):
    w = random.randint(ROOM_WIDTH_RANGE[0], ROOM_WIDTH_RANGE[1])
    h = random.randint(ROOM_HEIGHT_RANGE[0], ROOM_HEIGHT_RANGE[1])
    x1, y1 = -1, -1
    if direct==UP:
        x1 = random.randint(x+2-w, x-1)
        y1 = y-h+1
    elif direct==DOWN:
        x1 = random.randint(x+2-w, x-1)
        y1 = y
    elif direct==LEFT:
        x1 = x-w+1
        y1 = random.randint(y+2-h, y-1)
    elif direct==RIGHT:
        x1 = x
        y1 = random.randint(y+2-h, y-1)

    r = make_rect(x1, y1, w, h)
    if can_place_rect(r.inner_rect(1)):
        # set_rect(r, '.')
        rooms.append(r)
        return r

    return None

def place_a_corridor():
    eid = random.randint(0, len(exits)-1)
    e = exits[eid]
    pos = e.random()
    orient = HORIZONTAL if random.randint(0, 1) else VERTICAL

    c = place_a_corridor_at(pos[0], pos[1], orient, e.direction)
    if c:
        # set door
        if tiles[pos[1]][pos[0]]=='#':
            set_tile(pos[0], pos[1], 'D')
        # dx, dy = pos
        # if e.direction==UP:
        #     dy += 1
        # elif e.direction==DOWN:
        #     dy -= 1
        # elif e.direction==LEFT:
        #     dx += 1
        # elif e.direction==RIGHT:
        #     dx -= 1
        # if tiles[dy][dx]=='#':
        #     set_tile(dx, dy, 'D')

        del exits[eid]
        expand(c, e.direction)

    return c

def place_a_corridor_at(x, y, orient, direct):
    l = random.randint(CORRIDOR_LENGTH_RANGE[0], CORRIDOR_LENGTH_RANGE[1])
    w = l if orient==HORIZONTAL else 1
    h = 1 if orient==HORIZONTAL else l
    x1, y1 = -1, -1

    should_offset = tiles[y][x]=='#'

    if direct==UP:
        if should_offset:
            y -= 1
        x1 = random.randint(x-w+1, x)
        y1 = y-h+1
    elif direct==DOWN:
        if should_offset:
            y += 1
        x1 = random.randint(x-w+1, x)
        y1 = y
    elif direct==LEFT:
        if should_offset:
            x -= 1
        x1 = x-w+1
        y1 = random.randint(y-h+1, y)
    elif direct==RIGHT:
        if should_offset:
            x += 1
        x1 = x
        y1 = random.randint(y-h+1, y)

    c = make_rect(x1, y1, w, h)
    if can_place_rect(c):
        set_rect(c, '.')

        return c

    return None

def show_exits():
    for e in exits:
        set_rect(e, '^')

# process
def place_rooms():
    max_rooms = random.randint(ROOM_NUMBER_RANGE[0], ROOM_NUMBER_RANGE[1])
    
    place_first_room()

    for i in range(1000):
        if len(exits)==0:
            break
        if random.random()<PROBABILITY_SPAWN_ROOM:
            for j in range(100):
                if place_a_room():
                    break
            if len(rooms)==max_rooms:
                break
        else:
            for j in range(100):
                if place_a_corridor():
                    break

    print('{} rooms spawned.'.format(len(rooms)))
    
def is_near_floor(x, y):
    def is_floor(_x, _y):
        if _x<0 or _x>=MAP_WIDTH or _y<0 or _y>=MAP_HEIGHT:
            return False
        return tiles[_y][_x]=='.'
        
    for i in [-1, 0, 1]:
        for j in [-1, 0, 1]:
            if i==0 and j==0:
                continue
            if is_floor(x+i, y+j):
                return True
    return False

def build_walls():
    for y in range(0, MAP_HEIGHT):
        for x in range(0, MAP_WIDTH):
            if tiles[y][x]==' ' and is_near_floor(x, y):
                set_tile(x, y, '#')

def show_picture():
    import matplotlib.pyplot as plt
    import numpy as np

    img = np.zeros((MAP_HEIGHT, MAP_WIDTH), dtype=np.uint8)
    dct = { ' ': 0, '#': 1, '.': 2, 'D': 3 }
    for y in range(MAP_HEIGHT):
        for x in range(MAP_WIDTH):
            img[y, x] = dct[tiles[y][x]]
    
    plt.imshow(img)
    plt.show()

# process
init_map()

place_rooms()
build_walls()

# print_map()
show_picture()
