-- modulas
export Anim8 = require("lib.anim8")
Input = require("lib.Input")
Cargo = require("lib.cargo")
export Timer = require("lib.Timer")
M = require("lib.Moses")

-- utilities
export ll = {
  is: (obj, cls)-> 
    o = obj.__class
    while o
      return true if o==cls 
      o = o.__parent 
    return false 

  random: (low, high)-> math.random! *(high- low)+ low
  random_color: -> { math.random!, math.random!, math.random! }

  color_rgb: (r, g, b)=> { r/255, g/255, b/255 }

  deg2rad: math.rad
  rad2deg: math.deg 

  -- table functions
  size: M.size 
  count: M.count 
  count_if: M.countf 
  any: M.include
  find_if: (t, f)->
    for k, v in pairs t
      return k if f v, k 
    nil 
  any_if: find_if
  for_each: M.each 
  max: M.max 
  min: M.min  
  all: M.all 
  sort: M.sort 
  map: M.map
  reduce: M.reduce 
  filter: M.select  

  -- array functions 
  shuffle: M.shuffle
  fill: M.fill 
  zeros: M.zeros 
  ones: M.ones 
  push_front: M.addTop 
  push_back: M.push 
  reverse: M.reverse 
  extend: M.append 
  range: M.range 
}

export class vec2
  new: (x=0, y=0)=>
    @x, @y = x, y 

  copy: => vec2 @x, @y 
  unpack: => @x, @y 
  __tostring: => "(#{@x}, #{@y})"
  
  len2: => @x* @x+ @y* @y 
  len: => math.sqrt @len2!

  normalize: => 
    l = @len!
    if l>0
      @x, @y = @x/ l, @y/ l 
  normalized: => 
    v = @copy!
    v\normalize!
    v

  -- operators
  __unm: => vec2 -@x, -@y 
  __add: (other)=> 
    if type(other)=='number' vec2 @x+ other, @y+ other 
    else vec2 @x+ other.x, @y+ other.y
  __sub: (other)=>
    if type(other)=='number' vec2 @x- other, @y- other 
    else vec2 @x- other.x, @y- other.y
  __mul: (num)=> vec2 @x* num, @y* num 
  __div: (num)=> vec2 @x/ num, @y/ num 
  __eq: (other)=> @x==other.x and @y==other.y 

  -- statics 
  @Dot: (this, other)=> this.x* other.x+ this.y* other.y 
  @Cross: (this, other)=> this.x* other.y- this.y* other.x 
  @FromPolar: (angle, radius=1)=> vec2 radius* math.cos(angle), radius* math.sin angle
  @RandomDirection: (len_min=1, len_max=len_min)=> @FromPolar ll.random(0, 2*math.pi), ll.random(len_min, len_max)

export class trans2d
  new: (pos=vec2!, angle=0, scale=vec2(1, 1))=>
    @pos, @angle, @scale = pos, angle, scale

-- engine
export class Node
  new: =>
    @transform = trans2d!
    @color = {1, 1, 1, 1}

    @children = {} -- array
    @parent = nil 
  
  attach: (node)=>
    @children[#@children+ 1] = node
    node.parent = @
  
  detach: (node)=>
    for i, v in ipairs(@children)
      if v==node
        v.parent = nil 
        table.remove(@children, i)
        break 

  detach_from_parent: =>
    @parent\detach(@) if @parent

  detach_all: =>
    for c in *@children
      c.parent = nil 
      c\detach_all!

  update: (dt)=>
    @update_self(dt)
    child\update(dt) for child in *@children

  draw: =>
    love.graphics.push("all")
    love.graphics.translate(@transform.pos\unpack!)
    love.graphics.rotate(@transform.angle)
    love.graphics.scale(@transform.scale\unpack!)
    love.graphics.setColor(@color)
    @draw_self!
    child\draw! for child in *@children
    love.graphics.pop!

  draw_self: =>
  update_self: (dt)=>

-- initialization
initialize = ()->
  export gInput = Input()
  export gAssets = Cargo.init('assets')
  
  export gWidth, gHeight = love.graphics.getWidth!, love.graphics.getHeight!

  export gScene = { 
    update: (dt)=>
    draw: => love.graphics.print("no scene loaded.", 100, 100)
  }

initialize!
