Object = require('thirdparty/classic/classic')
Timer = require('thirdparty/chrono/Timer')
local Input = require('thirdparty/boipushy/Input')
local bump = require('thirdparty/bump/bump')
local cargo = require('thirdparty/cargo/cargo')
local anim8 = require 'thirdparty/anim8/anim8'

-- [[GameObject]]
GameObject = Object:extend()
GameObject.ID = 0

function GameObject:new(x, y, opts)
  self.id = GameObject.ID 
  GameObject.ID = GameObject.ID+ 1

  self.x, self.y = x or 0, y or 0
  if opts then 
    for k, v in pairs(opts) do self[k] = v end 
  end
  self.angle = self.angle or 0 
  self.depth = self.depth or 0 

  self.dead = false 
  self.timer = Timer()
end

function GameObject:init() end 

function GameObject:update(dt)
  self.timer:update(dt)
end

function GameObject:draw() 
  love.graphics.push('all')
  love.graphics.translate(self.x, self.y)
  love.graphics.rotate(self.angle)
  self:draw_self()
  love.graphics.pop()
end 

function GameObject:draw_self() end 

function GameObject:destroy() end 

-- [[Room]]
Room = Object:extend()

function Room:new()
  self.game_objects = {}
  self.timer = Timer()

  self.draw_world = false -- debug
end

function Room:enter() end

function Room:update(dt) 
  for i=#self.game_objects, 1, -1 do 
    local go = self.game_objects[i]
    if go.dead then 
      go:destroy()
      table.remove(self.game_objects, i)
    else go:update(dt)
    end
  end
end

function Room:draw()
  for _, v in ipairs(self.game_objects) do 
    v:draw()
  end  

  -- bump world
  if self.draw_world then 
    love.graphics.push('all')
    love.graphics.setColor(0, 1, 1, 0.3)
    local items, len = self.world:getItems()
    for i=1, len do 
      local x, y, w, h = self.world:getRect(items[i])
      love.graphics.rectangle('fill', x, y, w, h)
      love.graphics.rectangle('line', x, y, w, h)
    end
    love.graphics.pop()
  end
end 

function Room:exit() end 

function Room:add_game_object(go) 
  self.game_objects[#self.game_objects+1] = go 
  go:init()
end 

-- bump world
function Room:create_bump_world(grid_size)
  assert(not self.world, 'world already exists.')
  self.world = bump.newWorld(grid_size or 32)
end

-- statics
Room.Current = nil 
function Room.Goto(room)
  if Room.Current then Room.Current:exit() end 
  Room.Current = room 
  Room.Current:enter()
end

function Room.CreateGameObject(object_type, ...)
  local go = object_type(...)
  Room.Current:add_game_object(go)
  return go 
end

-- [[components]]
Component = Object:extend()

function Component:attach(belonger)
  assert(not self.belonger, 'cannot attach a component twice.')

  self.belonger = belonger
end

function Component:detach()
  self.belonger = nil 
end

-- [[sequence frame animation]]
-- simple warp on anim8
Animation = Component:extend()

function Animation:new(texture, frame_width, frame_height, time_per_frame, left, top)
  self.texture = texture
  self.grids = anim8.newGrid(frame_width, frame_height, texture:getWidth(), texture:getHeight(), left or 0, top or 0)
  self.time_per_frame = time_per_frame
  self.anchor_x, self.anchor_y, self.angle = 0, 0, 0
end

function Animation:set_anchor(x, y, a)
  self.anchor_x, self.anchor_y, self.angle = x, y, a or 0
end

function Animation:set_frames(...)
  self.animation = anim8.newAnimation(self.grids(...), self.time_per_frame)
end

function Animation:update(dt)
  self.animation:update(dt)
end

function Animation:draw()
  love.graphics.push()
  love.graphics.rotate(self.angle)
  love.graphics.translate(-self.anchor_x, -self.anchor_y)
  self.animation:draw(self.texture)
  love.graphics.pop()
end

-- [[collider mask]]
Collider = Component:extend()

function Collider:new(x, y, w, h)
  self.x, self.y, self.w, self.h = x, y, w, h
end

function Collider:attach(belonger)
  Collider.super.attach(self, belonger)
  Room.Current.world:add(self, self.belonger.x+ self.x, self.belonger.y+ self.y, self.w, self.h)
end

function Collider:detach() 
  Collider.super.detach(self)
  Room.Current.world:remove(self)
end

function Collider:update(dt)
  Room.Current.world:update(self, self.belonger.x+ self.x, self.belonger.y+ self.y, self.w, self.h)
end

-- [[utils]]
utils = {}

function utils.random(low, high)
  return love.math.random()*(high-low)+ low 
end 

function utils.random_color()
  return { utils.random(0, 1), utils.random(0, 1), utils.random(0, 1) } 
end

function utils.color_rgb(r, g, b)
  return { r/255, g/255, b/255 }
end

function utils.math_deg2rad(degree) return degree/180* math.pi end

-- [[initialization]]
local function Initialize()
  print('initializing engine...')
  
  gWindowWidth, gWindowHeight = love.graphics.getWidth(), love.graphics.getHeight()
  gInput = Input()
  gAssets = cargo.init('assets')
end

Initialize()
