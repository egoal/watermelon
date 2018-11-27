#!/usr/bin/python3 
import random

T_VOID, T_WALL, T_FLOOR, T_DOOR, T_TUNEL, T_TEST = 0, 1, 2, 3, 4, 100
TERRAINS = { T_VOID: ' ', T_WALL: '#', T_FLOOR: '.', T_DOOR: 'D', T_TEST: '+', T_TUNEL: '='}

LEFT, RIGHT, BOTTOM, TOP = -1, 1, -2, 2
DIRECTIONS = [LEFT, RIGHT, BOTTOM, TOP]

def clamp(x, min, max):
  if x<min:
    return x
  elif x>max:
    return max
  return x

def highrandom(low, high):
  return max(random.randint(low, high), random.randint(low, high))
def lowrandom(low, high):
  return min(random.randint(low, high), random.randint(low, high))
def meanrandom(low, high):
  return ((random.randint(low, high)+ random.randint(low, high)))//2

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

class Room(Rect):
  def __init__(self, x1, x2, y1, y2):
    super().__init__(x1, x2, y1, y2)
    self.doors = []
  
  def wall(self, direction):
    if direction==LEFT:
      return Wall(self.x1-1, self.x1-1, self.y1, self.y2, direction, True)
    elif direction==RIGHT:
      return Wall(self.x2+1, self.x2+1, self.y1, self.y2, direction, True)
    elif direction==TOP:
      return Wall(self.x1, self.x2, self.y1-1, self.y1-1, direction, True)
    elif direction==BOTTOM:
      return Wall(self.x1, self.x2, self.y2+1, self.y2+1, direction, True)
    else:
      raise 'bad direction'

  @classmethod
  def create(cls, x, y, w, h):
    return Room(x, x+w-1, y, y+h-1)

class Wall(Rect):
  def __init__(self, x1, x2, y1, y2, d, link_room=False):
    super().__init__(x1, x2, y1, y2)
    self.direction = d 
    self.link_room = link_room

class RoomDigger:
  def __init__(self, room, doorpos, enter_direction):
    self.direction = enter_direction
    self.door = doorpos
    self.room = room
  def dig(self, map):
    pass 
  def expand(self):
    []

class RoomPainter:
  def __init__(self, room, doorpos, enter_direction):
    self.direction = enter_direction
    self.door = doorpos
    self.room = room

  def paint(self, map):
    map.paint_room(self.room, T_FLOOR)

  def expand(self):
    return [self.room.wall(d) for d in DIRECTIONS if not d==-self.direction]

# class CircleRoomPainter(RoomPainter):
#   def paint(self, map):
#     cx, cy = self.room.center()
#     r = min(self.room.w(), self.room.h())//2
#     for x in range(self.room.x1, self.room.x2+1):
#       for y in range(self.room.y1, self.room.y2+1):
#         if (x-cx)**2+ (y-cy)**2<= r**2:
#           map.paint_at((x, y), T_FLOOR)

#   def expand(self):
#     return []
class TrapRoomDigger(RoomDigger):
  def dig(self, map):
    pass 

class Map: 
  def __init__(self):
    self.width, self.height = 42, 42 
    self.tiles = [[T_WALL for y in range(self.height)] for x in range(self.width)]

  def generate(self):
    self.walls = [] # digable walls
    self.rooms = [] # rooms 

    TOTOAL_ROOM = 20
    self.place_init_room()
    # for i in range(20):
    while len(self.rooms)<TOTOAL_ROOM:
      if random.random()<0.3:
        success, room, doorpos, wallidx = self.dig_a_room()
        if success:
          self.paint_at(doorpos, T_DOOR)
          painter = RoomPainter(room, doorpos, self.walls[wallidx].direction) # if random.random()<0.6 else CircleRoomPainter(room, doorpos, self.walls[wallidx].direction)
          painter.paint(self)
          self.rooms.append(room)
          del self.walls[wallidx]
          self.walls.extend(painter.expand())

        else:
          print('no where to dig room')
          break
      else: 
        if not self.dig_a_tunel():
          print('no where to dig tunel')
          break

    self.clean_walls()
    # self.paint_walls()

  def place_init_room(self):
    w, h = highrandom(2, 6), highrandom(2, 6)
    x = random.randint(self.width//4, self.width//4* 3)
    y = random.randint(self.height//4, self.height//4* 3)

    room = Room.create(x, y, w, h)
    self.rooms.append(room)
    self.paint_room(room)
    
    for d in DIRECTIONS:
      self.walls.append(room.wall(d))

  def dig_a_room(self):
    for k in range(100):
      # select a wall
      i = random.randint(0, len(self.walls)-1)
      wall = self.walls[i]
      
      digpos = wall.random()
      w, h = highrandom(2, 6), highrandom(2, 6) # random.randint(2, 6), random.randint(2, 6)
      if wall.direction==LEFT:
        x = wall.x1-w
        y = random.randint(digpos[1]-h+1, digpos[1])
      elif wall.direction==RIGHT:
        x = wall.x2+1
        y = random.randint(digpos[1]-h+1, digpos[1])
      elif wall.direction==TOP:
        x = random.randint(digpos[0]-w+1, digpos[0])
        y = wall.y1-h
      elif wall.direction==BOTTOM:
        x = random.randint(digpos[0]-w+1, digpos[0])
        y = wall.y2+1

      new_room = Room.create(x, y, w, h)

      success = self.can_dig_a_room(new_room)

      if success:
        return True, new_room, digpos, i
    return False, None, None, None 

  def dig_a_tunel(self):
    # select a wall
    for i in range(100):
      i = random.randint(0, len(self.walls)-1)
      wall = self.walls[i]
      
      digpos = wall.random()
      if random.randint(0, 1)==0:
        w, h = random.randint(2, 5), 1
      else:
        w, h = 1, random.randint(2, 5)

      if wall.direction==LEFT:
        x = wall.x1-w
        y = random.randint(digpos[1]-h+1, digpos[1])
      elif wall.direction==RIGHT:
        x = wall.x2+1
        y = random.randint(digpos[1]-h+1, digpos[1])
      elif wall.direction==TOP:
        x = random.randint(digpos[0]-w+1, digpos[0])
        y = wall.y1-h
      elif wall.direction==BOTTOM:
        x = random.randint(digpos[0]-w+1, digpos[0])
        y = wall.y2+1

      new_room = Room.create(x, y, w, h)
      success = self.can_dig_a_room(new_room, [T_WALL, T_TUNEL])
      if success:
        new_walls = [new_room.wall(d) for d in DIRECTIONS if d!=-wall.direction]
        for w in new_walls:
          w.link_room = False
        self.walls.extend(new_walls)
        self.paint_room(new_room, T_TUNEL)
        self.paint_at(digpos, T_DOOR if wall.link_room else T_TUNEL)
        return True

    return False

  def paint_at(self, pos, tile):
    self.tiles[pos[0]][pos[1]] = tile

  def paint_room(self, room, tile=T_FLOOR):
    for x in range(room.x1, room.x2+1):
      for y in range(room.y1, room.y2+1):
        self.tiles[x][y] = tile

  def paint_walls(self):
    for w in self.walls:
      self.paint_room(w, T_TEST)

  def can_dig_a_room(self, room, mask=[T_WALL]):
    if room.x1>=1 and room.x2<self.width-1 and room.y1>=1 and room.y2< self.height-1:
      for x in range(room.x1-1, room.x2+2):
        for y in range(room.y1-1, room.y2+2):
          if not self.tiles[x][y] in mask:
            return False
      return True

    return False

  def clean_walls(self):
    def NEIGHBOR8(x, y):
      return [(x-1, y-1), (x, y-1), (x+1, y-1), (x-1, y), (x+1, y), 
        (x-1, y+1), (x, y+1), (x+1, y+1)]

    for x in range(self.width):
      for y in range(self.height):
        need_clear = True
        for n in NEIGHBOR8(x, y):
          if self.in_map(n[0], n[1]) and self.tiles[n[0]][n[1]] not in [T_WALL, T_VOID]:
            need_clear = False
            break
        if need_clear:
          self.tiles[x][y] = T_VOID

  def in_map(self, x, y):
    return x>=0 and x<self.width and y>=0 and y<self.height

  def print(self):
    for y in range(self.height):
      print(''.join(TERRAINS[self.tiles[x][y]] for x in range(self.width)))

  def show(self):
    import numpy as np 
    import matplotlib.pyplot as plt 

    img = np.zeros((self.height, self.width), np.int)
    for x in range(self.width):
      for y in range(self.height):
        img[y, x] = self.tiles[x][y]

    plt.imshow(img)
    plt.show()

m = Map()
m.generate()
# m.print()
m.show()
