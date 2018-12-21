export Timer = require 'thirdparty/chrono/Timer'
Input = require 'thirdparty/boipushy/Input'
bump = require 'thirdparty/bump/bump'
cargo = require 'thirdparty/cargo/cargo' 
anim8 = require 'thirdparty/anim8/anim8'

--[[functions]]
export ll = {
  is: (obj, cls)->
    o = obj.__class 
    while o 
      return true if o==cls
      o = o.__parent
    return false 

  random: (low, high)-> love.math.random! *(high-low)+ low 
  random_color: -> { ll.random(0, 1), ll.random(0, 1), ll.random(0, 1) }
  
  color_rgb: (r, g, b) -> { r/255, g/255, b/255 }

  deg2rad: (degree)-> math.pi* degree/ 180 

  any: (tab, value)->
    for k, v in pairs tab do 
      return true if v==value
    return false 

  any_if: (tab, func)-> 
    for k, v in pairs tab do 
      return true if func v 
    return false 

  find_if: (tab, func)->
    for k, v in pairs tab do return v if func v 
}

--[[Vector, 2d]]
export class Vector 
  new: (x=0, y=0)=> @x, @y = x, y 
  
  copy: => Vector @x, @y 

  unpack: => @x, @y 

  -- operators
  __tostring: => "("..tonumber(@x)..","..tonumber(@y)..")"

  __unm: => Vector -@x, -@y

  __add: (other)=> 
    return Vector @x+ other, @y+ other if type(other)=='number' 
    Vector @x+ other.x, @y+ other.y 

  __sub: (other)=> Vector @x- other.x, @y- other.y  

  __mul: (number)=> Vector @x* number, @y* number

  __div: (number)=> Vector @x/number, @y/number

  __eq: (other)=> @x==other.x and @y==other.y

  __dot: (other)=> @x* other.x + @y* other.y 

  len2: => @x* @x+ @y* @y 

  len: => math.sqrt @x*@x+ @y*@y 

  angle: => math.tan2 @y, @x 

  normalize: => 
    l = @len!
    if l>0
      @x /= l 
      @y /= l 

  normalized: => 
    r = @copy!
    r\normalize!
    r 

  -- statics
  @Zero: => Vector 0, 0 

  @FromPolar: (angle, radius=1)=>
    Vector math.cos(angle)* radius, math.sin(angle)* radius
  
  @RandomDirection: (len_min=1, len_max)=>
    len_max = len_max or len_min
    @FromPolar ll.random(0, 2*math.pi), ll.random(len_min, len_max)

-- [[GameObject]]
export class GameObject
  __inherited: (child)=>
    assert false, 'forbidden: '.. @__name.. ' was inherited by '..child.__name

  new: (x=0, y=0, angle=0, depth=0)=>
    @pos = Vector x, y 
    @angle, @depth = angle, depth 

    @components = {}
    @dead = false 
  
  init: =>
    @sort_component!
    for com in *@components do com\init! if com.init

  update: (dt)=>
    for com in *@components do com\update dt if com.update
  
  draw: =>
    love.graphics.push 'all'
    love.graphics.translate @pos.x, @pos.y
    love.graphics.rotate @angle
    for com in *@components do com\draw! if com.draw

    love.graphics.pop!

  destroy: =>
    for v in *@components do v\detach!
    @components = {}

  add_component: (com)=>
    assert (com and ll.is com, Component), com.__class.__name.. ' is not a Component'
    com\attach @ 
    table.insert @components, #@components+1, com 
    com 

  get_component: (com_type)=>
    for v in *@components
      return v if ll.is v, com_type 

  find_component: (func)=>
    ll.find_if @components, func 

  remove_component: (com_or_com_type)=>
    for i=#@components, 1, -1 do 
      com = @components[i]
      if com==com_or_com_type or ll.is com, com_or_com_type 
        com\detach!
        table.remove @components, i 

  -- sort by priority
  sort_component: =>
    table.sort @components, (a, b)-> a.priority> b.priority

-- [[Component]]
export class Component
  new: => @priority = 0
  attach: (go)=> @belonger = go 
  detach: => @belonger = nil 

export class Drawable extends Component
  new: (x, y, color)=>
    super!
    @x, @y, @color = x or 0, y or 0, color or {1, 1, 1, 1} 
  
  draw: => 
    love.graphics.translate @x, @y 
    love.graphics.setColor @color
    @draw_self!

  draw_self: => 

export class TextLabel extends Drawable
  new: (text, x, y, color)=>
    super x, y, color  
    @text = text

  draw_self: =>
    love.graphics.print @text, @x, @y 

export class Collider extends Component
  -- box collider
  new: (world_name, x, y, w, h, tag)=>
    super!
    @x, @y, @w, @h = x, y, w, h 
    @world = world_name
    @tag = tag 

  attach: (who)=>
    super who 
    Room.Current\bump_add_box @world, @, @belonger.pos.x+ @x, @belonger.pos.y+ @y, @w, @h 

  detach: =>
    super!
    Room.Current\bump_remove_box @world, @

  update: (dt)=>
    Room.Current\bump_update_box @world, @, @belonger.pos.x+ @x, @belonger.pos.y+ @y, @w, @h 

  check: (dx, dy, filter)=>
    x, y, items, len = Room.Current\bump_check_box @world, @, @belonger.pos.x+ @x+ dx, @belonger.pos.y+ @y+ dy, filter
    x- @x, y- @y, items, len 

--[[Room]]
export class Room
  new: =>
    @game_objects = {}
    @bump_worlds = {} 
    @draw_world = true  
    @timer = Timer!

  enter: =>

  update: (dt)=>
    @timer\update dt 
    for i=#@game_objects, 1, -1
      go = @game_objects[i]
      if go.dead then 
        go\destroy!
        table.remove @game_objects, i
      else 
        go\update dt 
  
  draw: =>
    for v in *@game_objects do v\draw!

    if @draw_world 
      love.graphics.push 'all'
      for k, v in pairs @bump_worlds do
        color = v.color 
        world = v.world
        love.graphics.setColor v.color
        items, len = world\getItems!
        for i in *items do 
          x, y, w, h = world\getRect i 
          love.graphics.rectangle 'line', x, y, w, h 
          love.graphics.rectangle 'fill', x, y, w, h 

      love.graphics.pop! 

  exit: =>
    go\destroy! for go in *@game_objects
    @game_objects = {}

  add_game_object: (go)=>
    table.insert @game_objects, #@game_objects+1, go 
    go\init!

  -- create from compoenents
  create_game_object: (x, y, ...)=>
    go = GameObject x, y 
    go\add_component com for com in *{...}
    @add_game_object go 
    go 

  -- simple bump warper 
  bump_add_world: (name, size, color)=>
    assert not @bump_worlds[name], 'cannot add world '..name..' twice!'
    @bump_worlds[name] = { color: color, world: bump.newWorld size }

  bump_add_box: (name, who, x, y, w, h)=>
    @bump_worlds[name].world\add who, x, y, w, h 
  bump_remove_box: (name, who)=>
    @bump_worlds[name].world\remove who 
  
  bump_check_box: (name, who, x, y, filter)=>
    @bump_worlds[name].world\check who, x, y, filter 
  bump_update_box: (name, who, x, y, w, h)=>
    @bump_worlds[name].world\update who, x, y, w, h 
  bump_move_box: (name, who, x, y, filter)=>
    @bump_worlds[name].world\move who, x, y, filter 
  
  -- static
  @Current: nil
  @Goto: (room)=>
    @Current\exit! if @Current
    @Current = room
    @Current\enter!

initialize = ->
  --[[global variables]]
  export gInput = Input!
  export gWidth, gHeight = love.graphics.getWidth!, love.graphics.getHeight!
  export gAssets = cargo.init('assets')

initialize!
