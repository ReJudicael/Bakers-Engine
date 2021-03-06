--Move object with WASD inputs

speed = 200;
angularSpeed = 200;

function Start()
	
end

function HandleRotation(deltaTime)
	rotation = Vec3.new();
	
	if (Input:IsKeyDown(Key.A)) then
		rotation = rotation + Vec3.new(0, 1, 0);
	end
	if (Input:IsKeyDown(Key.D)) then
		rotation = rotation - Vec3.new(0, 1, 0);
	end
	
	if (rotation:SquaredLength() > 0.0) then
		Body:SetRotationLock(1, false);
	else
		Body:SetRotationLock(1, true);
	end
	
	Body:SetAngularVelocity(rotation * angularSpeed * deltaTime);
end

function HandleMovement(deltaTime)
	move = Vec3.new();
	inputSpeed = 1;
	
	if (Input:IsKeyDown(Key.Shift)) then
		inputSpeed = 2;
	end
	
	if (Input:IsKeyDown(Key.W)) then
		move = move + object.Transform:Forward();
	end
	
	if (Input:IsKeyDown(Key.S)) then
		move = move - object.Transform:Forward();
	end
	
	if (move:SquaredLength() > 0.0) then
		move:Normalize();
		move = move * speed * inputSpeed * deltaTime;
	end
	
	velocity = Body:getVelocity();
	move.y = velocity.y;
	Body:setVelocity(move);
		
end

function Update(deltaTime)
	if (Health <= 0) then 
		noMove = Vec3.new();
		Body:setVelocity(noMove);
		do return end
	end
	
	HandleRotation(deltaTime);
	HandleMovement(deltaTime);
end
