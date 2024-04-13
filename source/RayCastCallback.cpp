#include "RayCastCallback.h"
#include "Rigidbody.h"

float RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{

	Rigidbody* Rb = reinterpret_cast<Rigidbody*>(fixture->GetUserData().pointer);

	if (Rb->actor == nullptr) return -1.0f;
	if (!Rb->has_collider && !Rb->has_trigger) return -1.0f;

	if (m_fraction < fraction) {
		m_fixture = fixture;
		m_point = point;
		m_normal = normal;
	}

	return fraction;
}

float RayCastCallbackAll::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
	Rigidbody* Rb = reinterpret_cast<Rigidbody*>(fixture->GetUserData().pointer);
	if (Rb->actor == nullptr) return -1.0f;
	if (Rb->has_collider || Rb->has_trigger) {
		HitResult hitresult(Rb->actor, point, normal, fixture->GetFilterData().maskBits == category_trigger);
		hitresults.insert({ fraction, hitresult });
	}

	return 1.0f;
}


luabridge::LuaRef RayCastResult::RaycastFirst(b2Vec2 pos, b2Vec2 dir, float dist)
{
	if (dist <= 0.0f) return luabridge::LuaRef(ComponentDB::lua_state);

	RayCastCallback raycastOne;

	dir.Normalize();

	b2Vec2 end = pos + dir.operator_mul(dist);

	PhysicsWorld::b2world->RayCast(&raycastOne, pos, end);

	if(!raycastOne.m_fixture) return luabridge::LuaRef(ComponentDB::lua_state);

	Rigidbody* Rb = reinterpret_cast<Rigidbody*>(raycastOne.m_fixture->GetUserData().pointer);

	HitResult hitresult(Rb->actor, raycastOne.m_point, raycastOne.m_normal, raycastOne.m_fixture->GetFilterData().maskBits == category_trigger);

	luabridge::push(ComponentDB::lua_state, hitresult);
	luabridge::LuaRef result = luabridge::LuaRef::fromStack(ComponentDB::lua_state, -1);
	lua_pop(ComponentDB::lua_state, 1);

	return result;
}

luabridge::LuaRef RayCastResult::RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist)
{
	if (dist <= 0.0f) return luabridge::LuaRef(ComponentDB::lua_state);

	RayCastCallbackAll raycastAll;

	b2Vec2 end = pos + dir.operator_mul(dist);

	PhysicsWorld::b2world->RayCast(&raycastAll, pos, end);

	luabridge::LuaRef table = luabridge::newTable(ComponentDB::lua_state);
	int i = 1;
	for (auto it = (raycastAll.hitresults).begin(); it != (raycastAll.hitresults).end(); it++) {
		table[i] = it->second;
		i++;
	}

	return table;
}
