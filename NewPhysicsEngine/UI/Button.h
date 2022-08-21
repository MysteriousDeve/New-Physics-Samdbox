#pragma once

#include "../Structs/Vector2.h"

struct Button
{
private:
	void (*onHover)();
	void (*onClick)();
	void (*onRelease)();
	void (*onIdle)();
	void (*update)();
public:
	Transform2D transform;
	Color color;

	Button(Vector2 position, Vector2 size, void(*onHover)(), void(*onClick)(), Color color = Color(0.2, 0.2, 0.2))
	{
		this->transform.position = position;
		this->transform.scale = size;
		this->onHover = onHover;
		this->onClick = onClick;
		this->color = color;
	}

	bool isInside(Vector2 pos)
	{
		Vector2 position = transform.position, size = transform.scale;
		return !(
			pos.x < position.x ||
			pos.y < position.y ||
			pos.x > position.x + size.x ||
			pos.y > position.y + size.y
		);
	}

	void Update()
	{

	}

	void OnIdle()
	{
		color = Color(0.2, 0.2, 0.2);
	}

	void OnHover()
	{
		color = Color(0.5, 0.5, 0.5);
		(*onHover)();
	}

	void OnClick()
	{
		(*onClick)();
	}

	void OnRelease()
	{
		(*onRelease)();
	}
};