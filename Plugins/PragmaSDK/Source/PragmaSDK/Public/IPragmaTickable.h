#pragma once

class IPragmaTickable
{
public:
	virtual ~IPragmaTickable() = default;
	
	virtual void Tick(float DeltaTime) = 0;
};
