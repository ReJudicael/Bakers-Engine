--Move object with WASD inputs

speed = 50;

function Start()
	
end

function Update(deltaTime)
	move = Vec3.new();

	if (Input:IsKeyDown(Key.W)) then
		move = move + object.Transform:Forward();
	end
	if (Input:IsKeyDown(Key.A)) then
		move = move + object.Transform:Right();
	end
	if (Input:IsKeyDown(Key.S)) then
		move = move - object.Transform:Forward();
	end
	if (Input:IsKeyDown(Key.D)) then
		move = move - object.Transform:Right();
	end
	
	
	if (move:SquaredLength() > 0.0) then
		move:Normalize();
		move = move * speed * deltaTime;
		Body:AddVelocity(move);
	end;
end
