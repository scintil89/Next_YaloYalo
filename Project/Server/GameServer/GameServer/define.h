#pragma once
#include <cmath>
#include <memory>

namespace LibLogic
{
	namespace DEF
	{
		const int MAX_UNIT_COUNT = 100;
		enum PlayerSide : bool
		{
			BOTTOM,
			TOP,
		};

		struct Vec2
		{
			Vec2() {}
			Vec2(float x, float y) { _x = x, _y = y; }
#pragma region operators
			Vec2 operator/(const Vec2& rhs)
			{
				return Vec2(this->_x / rhs._x, this->_y / rhs._y);
			}
			Vec2 operator/(float rhs)
			{
				return Vec2(this->_x / rhs, this->_y / rhs);
			}
			Vec2 operator*(float rhs)
			{
				return Vec2(this->_x * rhs, this->_y * rhs);
			}
			Vec2 operator+(const Vec2& rhs)
			{
				return Vec2(this->_x + rhs._x, this->_y + rhs._y);
			}
			Vec2 operator-(const Vec2& rhs)
			{
				return Vec2(this->_x - rhs._x, this->_y - rhs._y);
			}
#pragma endregion
			float _x = 0.f;
			float _y = 0.f;
		};

		//Anchor point is middle
		struct RECT
		{
		public:
			RECT() {}
			RECT(float x, float y, float w, float h) :
				_x(x), _y(y), _w(w), _h(h)
			{
				_xMin = _x - (_w / 2);
				_xMax = _x + (_w / 2);
				_yMin = _y - (_h / 2);
				_yMax = _y + (_h / 2);
			}

			bool Contains(const Vec2& point) const
			{
				if (_xMin < point._x &&
					_yMin < point._y &&
					point._x < _xMax &&
					point._y < _yMax)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			float _x = 0.f;
			float _y = 0.f;
			float _w = 0.f;
			float _h = 0.f;
			float _xMin = 0.f;
			float _yMin = 0.f;
			float _xMax = 0.f;
			float _yMax = 0.f;
		};

		static float GetDistance(Vec2 a, Vec2 b)
		{
			Vec2 delta = a - b;
			return sqrt(delta._x * delta._x + delta._y * delta._y);
		}

#define GS_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }

#define GS_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }\
public: virtual void Set##funName(varType var){ varName = var; }
	}
}