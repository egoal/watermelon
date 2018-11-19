Timer = require 'thirdparty/chrono/Timer'
Input = require 'thirdparty/boipushy/Input'
export Vector = require 'thirdparty/hump/vector'
bump = require 'thirdparty/bump/bump'
cargo = require 'thirdparty/cargo/cargo' 
anim8 = require 'thirdparty/anim8/anim8'

--[[GameObject]]
export class GameObject
  id = 0
  new: (pos)=>
    id += 1
    @id = id 

    @pos = pos or Vector 0, 0 
    @angle, @depth = @angle or 0, @depth or 0

    @dead = false 
    @timer = Timer!
    @collider = nil 

  init: =>
  update: (dt)=>
    @timer\update dt 
    @update_collider! if @collider

  draw: =>
    love.graphics.push!
    love.graphics.translate @pos.x, @pos.y
    love.graphics.rotate @angle
    @draw_self!

    love.graphics.pop!

  destroy: =>
    Room.Current\remove_collider self if @collider

  draw_self: =>

  set_collider: (ox, oy, w, h)=>
    Room.Current\add_collider(self, ox, oy, w, h) unless @collider
    @collider = {x: ox, y: oy, w: w, h: h}
    @update_collider!

  update_collider: =>
    Room.Current.world\update self, @pos.x+ @collider.x, @pos.y+ @collider.y, @collider.w, @collider.h   

--[[Room]]
export class Room
  new: =>
    @game_objects = {}
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
  
  exit: =>

  add_game_object: (go)=>
    table.insert @game_objects, #@game_objects+1, go 
    go\init!
  create_game_object: (object_type, ...)=>
    go = object_type ...
    @add_game_object go 
    return go 

  -- static
  @Current: nil
  @Goto: (room)=>
    @Current\exit! if @Current
    @Current = room
    @Current\enter!

export class BumpRoom extends Room
  new: (grid_size=64)=>
    super!
    @world = bump.newWorld grid_size
    @draw_world = true -- debubg 

  draw: =>
    super! 
    if @draw_world then 
      love.graphics.setColor 0.2, 1, 1, 0.3
      items, len = @world\getItems!
      for i in *items
        x, y, w, h = @world\getRect i
        love.graphics.rectangle 'fill', x, y, w, h 
        love.graphics.rectangle 'line', x, y, w, h 

      love.graphics.setColor 1, 1, 1, 1

  add_collider: (game_object, x, y, w, h)=>
    @world\add game_object, x, y, w, h 
  remove_collider: (game_object)=>
    @world\remove game_object

--[[functions]]
export ll = {
  random: (low, high)-> love.math.random! *(high-low)+ low 
  random_color: -> { ll.random(0, 1), ll.random_color(0, 1), ll.random_color(0, 1) }
  
  color_rgb: (r, g, b) -> { r/255, g/255, b/255 }

  deg2rad: (degree)-> math.pi* degree/ 180 
}

initialize = ->
  --[[global variables]]
  export gInput = Input!
  export gWidth, gHeight = love.graphics.getWidth!, love.graphics.getHeight!
  export gAssets = cargo.init('assets')

initialize!
