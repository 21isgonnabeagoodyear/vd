framenum = 0
--playerdamagepoints = {}
function framestart(notused)
	if spawnnewplayer then
		playerid = hooks.spawn("player", pos[1],pos[2])
		hooks.log("spawn new player"..pos[1].." "..pos[2], 0)
		spawnnewplayer = false
		crittertable = {}--this should not be necessary but just in case we have duplicate ids
	end
	if playerdamagepoints ~= nil then
		playerdamagepoints.ttl = playerdamagepoints.ttl -1
		if playerdamagepoints.ttl <= 0 then
			playerdamagepoints = nil
		end
	end
	framenum = framenum +1

end

function dieifnec(x,y)
	--[[for i, dmgpos in ipairs(playerdamagepoints) do
		if math.abs(x - dmgpos.x) + math.abs(y-dmgpos.y) < 20 then
			hooks.die()
		end
	end]]
	if playerdamagepoints == nil then
		return
	end
	if x > playerdamagepoints.x and x < playerdamagepoints.x+playerdamagepoints.w and y >playerdamagepoints.y and y < playerdamagepoints.y+playerdamagepoints.h then
		hooks.die()
		smoketimers[hooks.spawn("smoke", x, y)] = 15--math.abs(newpos[1]-pos[1])
	end
end

function testent(id)
	--hooks.log("ent ".. id .." signing in", 0)
	if id == 0 then
		framenum = framenum + 0.1--1.0
	end
	if id < 2 then
		hooks.frame(framenum % 10)
		return
	end
	if id % 2 == 0 then
		hooks.move(math.sin((framenum + id*3)*0.1)*50+100, math.cos((framenum+id*3)*0.1)*50+100)
	else
		hooks.move(math.cos((framenum*(1+id*0.06)+id*3)*0.1)*200+500, math.sin((framenum*(1+id*0.05) + id*3)*0.2)*100+100 )
	end
end

function default(id)

	local px,py = hooks.position()

	local keyu = {hooks.keys()}
	if keyu[5] >= 1 and hooks.terrainat(px+1, py) ==0 then
		hooks.move(px+1,py)--px = px + 1
		px = px +1
	end
	if keyu[4] >= 1 and hooks.terrainat(px-1, py) ==0 then
		hooks.move(px-1,py)--px = px + 1
		px = px -1
	end
	if keyu[2] >= 1 and hooks.terrainat(px, py-2) ==0 then
		hooks.move(px,py-2)--px = px + 1
		py = py -2
	end

	if hooks.terrainat(px, py+1) ==0 then
		hooks.move(px, py+1)
	end
	hooks.frame(framenum % 10)
end

function shrubbery(id)
	--hooks.frame(framenum %1)
	hooks.frame((framenum+id) %2 + 1024/32)
end






vx = 0
vy = 0
jumpedintheair = false
attackedintheair = false
playersize = 12
pos = {0,0}
playerpause = 0

function player(id)
	if playerid ~= nil and playerid ~= id then
		hooks.die()
	end
	--hooks.log(""..pos[1].." "..pos[2], 0)
	local keys = {hooks.keys()}
	pos = {hooks.position()}

	if playerpause >0 then
		playerpause = playerpause -1
		return
	end

	if keys[6] == 1 and keys[5]-keys[4] ~= 0 and not attackedintheair then--attack
		local newpos
		if keys[5]-keys[4] > 0 then
			newpos = {hooks.nudge(pos[1], pos[2], playersize, playersize, 200, 0)}
		elseif keys[5]-keys[4] < 0 then
			newpos = {hooks.nudge(pos[1], pos[2], playersize, playersize, -200, 0)}
		end
		hooks.move(newpos[1],newpos[2])
		local lctr =0
		for i=pos[1], newpos[1], 17*math.abs(newpos[1]- pos[1])/(newpos[1]- pos[1]) do
			lctr = lctr + 1
			smoketimers[hooks.spawn("sparkles", i, newpos[2])] = lctr--math.abs(newpos[1]-pos[1])
			--diecounter = diecounter + 40
			--table.insert(playerdamagepoints, {x=i, y=newpos[2], life=lctr})
		end
		playerdamagepoints = {x= math.min(pos[1], newpos[1]), y=newpos[2], w=math.abs(pos[1]-newpos[1])+playersize, h=25, ttl=10}
		vy = math.min(vy,0)
		--hooks.spawn("smoke", pos[1],pos[2])
		diecounter = 10
		playerpause = 10
		attackedintheair = true
		return
	end


	if hooks.terrainat(pos[1], pos[2]+playersize+1) ~= 0 or hooks.terrainat(pos[1]+playersize, pos[2]+playersize+1) ~= 0 then--jump
		if vy > 0 then
			vy = 0
		end
		jumpedintheair = false
		attackedintheair = false
		if keys[1] ==1 then
			vy = -7
			--diecounter = 10
		end
		vx = vx + keys[5]-keys[4]
		if keys[5]-keys[4] ==0 then
			vx = vx/2
		end
		--vx = vx * 0.5
	else
		vx = vx + 0.2*(keys[5]-keys[4])
		if keys[1] == 1 and not jumpedintheair then
			vy = -7
			--pos[1] = pos[1]+20*(keys[5]-keys[4])
			jumpedintheair = true
			--attackedintheair = true
			smoketimers[hooks.spawn("smoke", pos[1],pos[2])] = 10
		elseif keys[1] >0 then
			vy = vy - 0.2
		end
		vy = vy +0.6
	end
	if hooks.terrainat(pos[1], pos[2]-1) ~= 0 or hooks.terrainat(pos[1]+playersize, pos[2]-1) ~= 0 then
		vy = 1
	end

	--hooks.log("vy "..vy, 0)
	hooks.move(hooks.nudge(pos[1], pos[2], playersize, playersize, 0, math.floor(vy)))
	local posa = {hooks.position()}
	if posa[1] ~= pos[1] then--this goes below
		vx = 0
	end
	pos = posa


	--hooks.move(pos[1]+keys[5]-keys[4], pos[2]+keys[3]-keys[2])
	--hooks.move(hooks.nudge(pos[1], pos[2], playersize, playersize, keys[5]-keys[4], keys[3]-keys[2]))
	vx = math.min(math.max(vx,-5), 5)
	hooks.move(hooks.nudge(pos[1], pos[2], playersize, playersize, vx, 0))
	if math.abs(vx) < math.abs(vy) and math.abs(vy) > 1 then
		hooks.frame(3)
	elseif vx > 1 then
		hooks.frame(4)
	elseif vx < -1 then
		hooks.frame(5)
	else
		hooks.frame(2)
	end
	--posx = posx +keys[5]=keys[4]
	--posy =  posy+keys[3]-keys[2]



	if pos[1] < 0 then
		hooks.warp(-1,0)
		pos[1] = 800-playersize
		spawnnewplayer = true
	elseif pos[1] > 800-playersize then
		hooks.warp(1,0)
		pos[1] = 0
		spawnnewplayer = true
	elseif pos[2] > 320-playersize-5 then
		hooks.warp(0,1)
		pos[2] = 0
		spawnnewplayer = true
	elseif pos[2] < 0 then
		hooks.warp(0,-1)
		pos[2] = 320-playersize-5
		spawnnewplayer = true
	end


	vy = math.min(math.max(-20, vy), 20)
end
smoketimers = {}
function smoke(id)
	smoketimers[id] = smoketimers[id]-1
	if smoketimers[id]<0 then
		hooks.die()
		return
	end
	if smoketimers[id] > 5 then
		hooks.frame(0)
	else
		hooks.frame(1)
	end
end

function sparkles(id)
	if smoketimers[id] == nil or smoketimers[id]<0 then
		hooks.die()
		return
	end
	smoketimers[id] = smoketimers[id]-1
	if smoketimers[id] > 5 then
		hooks.frame(16)
	else
		hooks.frame(17)
	end
end

--crittertime = 0
--critterdir = 1
crittertable={}
function critterai(id, decisiveness, laziness, mintime,size, baseframe, animframes, facingframes, animspeed)
	--hooks.log(id, 0)
	if crittertable[id] == nil then
		crittertable[id] = {crittertime=0, critterdir=1}
	end
	if crittertable[id].crittertime <= 0 then
		crittertable[id].critterdir = (hooks.rand()/50)-1
		crittertable[id].crittertime = hooks.rand()*decisiveness  +mintime
	end
	crittertable[id].crittertime = crittertable[id].crittertime-1
	local pos = {hooks.position()}
	hooks.frame(baseframe + math.floor(facingframes*(crittertable[id].critterdir+1)*0.499))
	--hooks.log(facingframes*(crittertable[id].critterdir+1)*0.499, 0)
	if crittertable[id].critterdir >= laziness and hooks.terrainat(pos[1]+1+size, pos[2] ) ==0 and hooks.terrainat(pos[1]+1+size, pos[2]+32) ~= 0 then
		--if crittertable[id].critterdir >= laziness then
			hooks.move(pos[1]+1,pos[2])
			hooks.frame(baseframe+facingframes+math.floor(crittertable[id].crittertime/animspeed)%animframes + animframes)
		--end
	elseif crittertable[id].critterdir <= -laziness and hooks.terrainat(pos[1]-1, pos[2]) ==0 and hooks.terrainat(pos[1]-1, pos[2]+32) ~= 0  then
		--if crittertable[id].critterdir <= -laziness then
			hooks.move(pos[1]-1,pos[2])
			hooks.frame(baseframe+facingframes+math.floor(crittertable[id].crittertime/animspeed)%animframes)
		--end
	elseif math.abs(crittertable[id].critterdir) >= laziness then
		--crittertable[id].crittertime =0
		crittertable[id].critterdir = -crittertable[id].critterdir
	end


end
function critter(id)
	critterai(id, 0.5, 0.9, 10,16, 6, 2,1, 8)
	critterai(id, 0.5, 0.8, 10,16, 6, 2,1, 8)

	local x,y = hooks.position()
	dieifnec(x+5,y+20)
end
function critter2(id)
	if framenum %2 ==0 then
		critterai(id, 0.5, 0.5, 10,32, 11, 2,1, 8)
	end
	local x,y = hooks.position()
	dieifnec(x+5,y+20)
end
function gnome(id)
--function critterai(id, decisiveness, laziness, mintime,size, baseframe, animframes, animspeed)
	critterai(id, 0.2, 0.6, 20, 16, 18, 2,1, 4)
end
function deer(id)
--function critterai(id, decisiveness, laziness, mintime,size, baseframe, animframes, animspeed)
	if math.floor(framenum+id) % 2 <= 0.1 then
		critterai(id, 0.8, 0.5, 40, 32, 23, 2,2, 4)
	end
end

function snowspawn(id)
	hooks.frame(36)
	if math.floor(framenum+id) % 80 <= 0.1 then
		local x,y = hooks.position()
		hooks.spawn("snow", x+hooks.rand()-50, y-50)
	end

end
function snow(id)
	hooks.frame(35)
	local x,y = hooks.position()
	hooks.move(x-math.sin(id+framenum*0.01), y+1+id%3)
	if y > 320 then
		hooks.die()
	end
	dieifnec(x+5,y+5)
end

hooks.log("loaded main.lua", 0)
