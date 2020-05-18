--Move object with WASD inputs

speed = 100;

function Start()
	
end

function Update(deltaTime)
	move = Vec3.new();

	if (Input:IsKeyDown(Key.W)) then
		move.z = 1;
	end
	if (Input:IsKeyDown(Key.A)) then
		move.x = 1
	end
	if (Input:IsKeyDown(Key.S)) then
		move.z = -1
	end
	if (Input:IsKeyDown(Key.D)) then
		move.x = -1
	end
	
	
	if (move:SquaredLength() > 0.0) then
		move:Normalize();
		move = move * speed * deltaTime;
		object:Translate(move);
	end;
end
