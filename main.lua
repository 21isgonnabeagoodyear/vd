framenum = 0.0

function testent(id)
	--hooks.log("ent ".. id .." signing in", 0)
	if id % 2 == 0 then
		hooks.move(math.sin((framenum + id*3)*0.1)*50+100, math.cos((framenum+id*3)*0.1)*50+100)
	else
		hooks.move(math.sin((framenum + id*3)*0.1)*50+300, math.cos((framenum+id*3)*0.1)*50+100)
	end
	if id == 0 then
		framenum = framenum + 1.0
	end
end
hooks.log("loaded main.lua", 0)
