#pragma once

class Enemy;

class JudgmentBase
{
public:
	// コンストラクタ
	JudgmentBase(Enemy* enemy) : owner(enemy) {}
	
	// 判定処理
	virtual bool Judgment() = 0;
protected:
	Enemy* owner;
};