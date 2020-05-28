--Move object with WASD inputs

speed = 10;
maxVelocity = 50;
angularSpeed = 0.1; --value used for quaternion slerp. Must be between 0 and 1

function Start()
	
end

function Update(deltaTime)
	move = Vec3.new();
	canRotate = false;
	
	target = Vec3.new(-1.0, 0, 0);
	
	if (Input:IsKeyDown(Key.W)) then
		move = move + object.Transform:Forward();
	end
	if (Input:IsKeyDown(Key.A)) then
		move = move + object.Transform:Right();
		canRotate = true;
	end
	if (Input:IsKeyDown(Key.S)) then
		move = move - object.Transform:Forward();
	end
	if (Input:IsKeyDown(Key.D)) then
		move = move - object.Transform:Right();
		canRotate = true;
	end
	
	velocity = Body:getVelocity();
	if (velocity:SquaredLength() > maxVelocity) then
		do return end
	end

	
	if (move:SquaredLength() > 0.0) then
		move:Normalize();
		if (canRotate) then object:RotateTowards(move, angularSpeed) end;
		move = move * speed * deltaTime;
		Body:AddVelocity(move);
	end;
end
