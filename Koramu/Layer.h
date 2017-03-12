#pragma once

class Layer
{
public:
	virtual void update() = 0;
	virtual void render() = 0;
};
