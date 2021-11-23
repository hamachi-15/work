#pragma once

class Enemy;

class JudgmentBase
{
public:
	JudgmentBase(Enemy* enemy) : owner(enemy) {}
	virtual bool Judgment() = 0;
protected:
	Enemy* owner;
};