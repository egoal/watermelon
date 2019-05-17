
import numpy as np 
import matplotlib.pyplot as plt 

width, height = 36, 36
data = np.zeros((height, width), dtype=np.int32)

rects = []

def is_rect_overlapped(r1, r2):
    return not (r1[0]> r2[1] or r1[1]< r2[0] or r1[2]> r2[3] or r1[3]< r2[2])

def random_place_rects():
    while len(rects)<14:
        w = np.random.randint(4, 9)
        h = np.random.randint(4, 9)
        x = np.random.randint(0, width- w)
        y = np.random.randint(0, height- h)
        newr = (x, x+ w- 1, y, y+ h- 1)
        if any(map(lambda x: is_rect_overlapped(x, newr), rects)):
            continue

        rects.append(newr)

cen = (width// 2, height// 2)

cen = (np.random.randint(5, width-5), np.random.randint(5, height- 5))

def move_rect(rect, dx, dy):
    x1, x2, y1, y2 = rect
    return (x1+ dx, x2+ dx, y1+ dy, y2+ dy)    

def move_rect_toward_center_once(i):
    x1, x2, y1, y2 = rects[i]
    x, y = cen
    dx, dy = x- (x1+ x2)// 2, y- (y1+ y2)// 2
    if dx==0 and dy==0:
        return rects[i], False

    def try_move(rect, dx, dy):
        dst_rect = move_rect(rect, dx, dy)

        cango = True
        for j, other in enumerate(rects):
            if j==i:
                continue
            elif is_rect_overlapped(other, dst_rect):
                cango = False
                break
            
        return cango, dst_rect
    
    rect = rects[i]
    if abs(dx)> abs(dy):
        b, dst = try_move(rect, dx// abs(dx), 0)
        if b:
            return dst, True
        if not dy==0:
            b, dst = try_move(rect, 0, dy// abs(dy))
            if b: 
                return dst, True
    else: 
        b, dst = try_move(rect, 0, dy// abs(dy))
        if b: 
            return dst, True
        if not dx==0:
            b, dst = try_move(rect, dx// abs(dx), 0)
            if b:
                return dst, True

    return rect, False

random_place_rects()

plt.ion()

iter, any_moved = 0, True
while any_moved:
    data[:, :] = 0
    for x1, x2, y1, y2 in rects:
        data[y1: y2, x1: x2] = 1
    data[cen[1], cen[0]] = 2

    plt.imshow(data, cmap='gray')
    plt.pause(0.1)

    any_moved = False
    for i, rect in enumerate(rects):
        rects[i], moved = move_rect_toward_center_once(i)
        if moved:
            any_moved = True
    
    iter += 1

print(f"stop after {iter} iterations.")

plt.ioff()
plt.show()
