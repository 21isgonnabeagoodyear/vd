framenum = 0.0

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
		hooks.move(math.cos((framenum*(1+id*0.05)+id*3)*0.1)*200+500, math.sin((framenum*(1+id*0.05) + id*3)*0.2)*100+100 )
	end
end

function default(id)
	hooks.frame(framenum % 10)
end
hooks.log("loaded main.lua", 0)
