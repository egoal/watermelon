**lua**

```lua
math.mod
-- 重载，需要结合meta table
__add, __sub, __mul, __div, __unm -- 相反数
__mod, __pow, __concat -- 字符串连接符 ..
__eq, _lt, _le -- ==, <, <=
__tostring
```



**shader**

```lua
local shader_str = [[
extern number time = 0.;
const number T = 4.;
const number B = 0.01;

number plot(number x, number y, number width){
	return smoothstep(y- width, y, x)- smoothstep(y, y+ width, x);
}

vec4 color_lerp(vec4 c1, vec4 c2, number a){
	return (1- a)* c1+ a* c2;
}

vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords){
	vec4 pixel = Texel(texture, texture_coords);
	number margin = 0.;
	if(time< 2.) margin = min(0.1* time, 0.2);
	else if(time< 3.) margin = 0.2;
	else if(time< 5.) margin = 0.2- (time- 3.)* 0.1;
	
	if(texture_coords.y< margin || texture_coords.y> (1- margin)) return vec4(4.);

	pixel *= 1.- 2.* margin;
	return pixel;
}
]]

shader = love.graphics.newShader(shader_str)
shader:send("time", passed_time)

love.graphics.setShader(shader)
...
love.graphics.setShader()
```



**thirdparty**

object-orient	 `git clone https://github.com/rxi/classic`

input	`git clone https://github.com/adnzzzzZ/boipushy`

timer/tween	`git clone https://github.com/adnzzzzZ/chrono`

camera	`git clone https://github.com/adnzzzzZ/STALKER-X`

aabb collider	`git clone https://github.com/kikito/bump.lua`

assets manager	`git clone https://github.com/bjornbytes/cargo`

frame animation	`git clone https://github.com/kikito/anim8`

path finding	`git clone https://github.com/Yonaba/Jumper`

functional	`git clone https://github.com/Yonaba/Moses`

gui	`git clone https://github.com/Ulydev/Uare`	`git clone https://github.com/vrld/suit`

serialization	`git clone https://github.com/gvx/bitser/`

ecs	`git clone https://github.com/bakpakin/tiny-ecs`