#pragma once

#include <memory>
#include <string>
#include <list>
#include <stack>

#include "UnitState.h"
#include "UnitManager.h"
#include "UnitTemplate.h"
#include "define.h"

namespace LibLogic
{
	class Match;

	class Unit : public std::enable_shared_from_this<Unit>
	{
	public:
		Unit(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager);
		virtual ~Unit() = default;

		virtual void			Update(float dt);

		template<typename T_STATE>
		void							ChangeState();
		virtual Unit*					GetTargetObject();
		virtual void					MoveTowardsTarget(float dt);
		virtual void					MoveTowardsWay(float dt);
		virtual int						ScanTargetNum();
		virtual std::list<int>			GetEnemyUnitsUnderSight();
		virtual void					NotifyMoveToTarget();
		virtual void					NotifyMoveToWaypoint(const DEF::Vec2& wayPoint);
		virtual void					NotifyUnitState();
		virtual bool					AttackOnce(float dt);
		virtual void					ReserveToHit(int damage, float delay);
		virtual void					EndAttack() {}
		virtual void					EndSummon() {}
		int								GetWayId();
		std::stack<int>&				GetWayIdStack() { return _wayIdStack; }

	public:
		GS_SYNTHESIZE_READONLY(float, _moveSpeed, moveSpeed);
		GS_SYNTHESIZE_READONLY(float, _timeToSummon, TimeToSummon);
		GS_SYNTHESIZE_READONLY(Match*, _match, Match);
		GS_SYNTHESIZE_READONLY(DEF::PlayerSide, _ownerPlayer, OwnerPlayer);
		GS_SYNTHESIZE_READONLY(std::string, _unitName, UnitName);
		GS_SYNTHESIZE_READONLY(int, _unitNum, UnitNum);
		GS_SYNTHESIZE_READONLY(bool, _isMagic, IsMagic);
		GS_SYNTHESIZE(DEF::Vec2, _moveValue, MoveValue);
		GS_SYNTHESIZE(DEF::Vec2, _position, Position);
		GS_SYNTHESIZE(UnitState*, _state, State);
		GS_SYNTHESIZE(int, _targetNum, TargetNum);
		GS_SYNTHESIZE(float, _sightRange, SightRange);
		GS_SYNTHESIZE(float, _attackFreq, AttackFreq);
		GS_SYNTHESIZE(float, _attackRange, AttackRange);
		GS_SYNTHESIZE(int, _hp, Hp);
		GS_SYNTHESIZE(bool, _isDead, IsDead);

	protected:
		void ProcessDamage(float dt);
		void ProcessDead();

	protected:
		UnitManager*			_unitManager = nullptr;
		DEF::Vec2				_currentDirection;
		std::stack<int>			 _wayIdStack;
		float					_attackDelay = 0.f;
		int						_attackPower = 0;
		std::list<std::pair<float, int>> _damageList;
	};

	template<typename T_STATE>
	void Unit::ChangeState()
	{
		if (_state != nullptr)
		{
			_state->EndState(shared_from_this());
			delete _state;
			_state = nullptr;
		}

		_state = new T_STATE();
		_state->StartState(shared_from_this());
	}
}