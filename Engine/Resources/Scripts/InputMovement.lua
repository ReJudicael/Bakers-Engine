--Move object with WASD inputs

speed = 100;
angularSpeed = 0.1; --value used for quaternion slerp. Must be between 0 and 1

function Start()
	
end

function HandleRotation()
	rotation = Vec3.new();

	if (Input:IsKeyDown(Key.A)) then
		rotation = rotation + object.Transform:Right();
	end
	if (Input:IsKeyDown(Key.D)) then
		rotation = rotation - object.Transform:Right();
	end
	
	if (rotation:SquaredLength() > 0.0) then
		rotation:Normalize();
		object:RotateTowards(rotation, angularSpeed) 
	end
end

function HandleMovement(deltaTime)
	move = Vec3.new();
	
	if (Input:IsKeyDown(Key.W)) then
		move = move + object.Transform:Forward();
	end
	
	if (Input:IsKeyDown(Key.S)) then
		move = move - object.Transform:Forward();
	end
	
	if (move:SquaredLength() > 0.0) then
		move:Normalize();
		move = move * speed * deltaTime;
		velocity = Body:getVelocity();
		move.y = velocity.y;
		Body:setVelocity(move);
		
	end
end

function Update(deltaTime)
	HandleRotation();
	HandleMovement(deltaTime);
end
