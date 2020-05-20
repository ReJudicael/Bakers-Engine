--Move forward script

speed = 10

function Start()

end

function Update(deltaTime)
	move = object.Transform:Forward()
	move:Normalize()
	move = move * deltaTime * speed
	object:Translate(move)
end
