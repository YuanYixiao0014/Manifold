#include "Rigidbody.h"

b2Vec2 Rigidbody::GetPosition()
{
    if (body) {
        return body->GetPosition();
    }
    else
    {
        return b2Vec2(x, y);
    }
}

float Rigidbody::GetRotation()
{
    return body->GetAngle() * (180.0f / b2_pi);
}

b2Vec2 Rigidbody::GetVelocity()
{
    return body->GetLinearVelocity();
}

float Rigidbody::GetAngularVelocity()
{
    return body->GetAngularVelocity() * (180.0f / b2_pi);
}

float Rigidbody::GetGravityScale()
{
    return body->GetGravityScale();
}

b2Vec2 Rigidbody::GetUpDirection()
{
    float angle = body->GetAngle();

    b2Vec2 direction(std::sin(angle), -std::cos(angle));

    return direction;
}

b2Vec2 Rigidbody::GetRightDirection()
{
    float angle = body->GetAngle();

    b2Vec2 direction(std::cos(angle), std::sin(angle));

    return direction;
}

void Rigidbody::OnStart()
{
    //std::cout << "Rigidbody Onstart" << std::endl;

    //create body
    b2BodyDef bodyDef;
    if (body_type == "dynamic") {
        bodyDef.type = b2_dynamicBody;
    }
    else if (body_type == "kinematic") {
        bodyDef.type = b2_kinematicBody;
    }
    else if (body_type == "static") {
        bodyDef.type = b2_staticBody;
    }

    bodyDef.position = b2Vec2(x, y);
    bodyDef.bullet = precise;
    bodyDef.angularDamping = angular_friction;
    bodyDef.gravityScale = gravity_scale;

    body = PhysicsWorld::b2world->CreateBody(&bodyDef);

    body->SetTransform(body->GetPosition(), rotation * (b2_pi / 180.0f));
    
    this->actor->hasRigidbody = true;

    create_fixture();

}

void Rigidbody::OnUpdate()
{
    //std::cout << GetPosition().x << "," << GetPosition().y << std::endl;
}

void Rigidbody::OnDestroy()
{
   PhysicsWorld::b2world->DestroyBody(body);
}

void Rigidbody::AddForce(b2Vec2 vec2)
{
    /*if (vec2.x != 0 || vec2.y != 0) {
        std::cout << Helper::GetFrameNumber() << ": add force: " << vec2.x << ", " << vec2.y << std::endl;
    }*/
    body->ApplyForceToCenter(vec2, true);
}

void Rigidbody::SetVelocity(b2Vec2 vec2)
{
    body->SetLinearVelocity(vec2);
}

void Rigidbody::SetPosition(b2Vec2 vec2)
{
    if (body == nullptr) {
        x = vec2.x;
        y = vec2.y;
    }
    else
    {
        body->SetTransform(vec2, body->GetAngle());

    }

}

void Rigidbody::SetRotation(float degrees_clockwise)
{
    body->SetTransform(body->GetPosition(), degrees_clockwise * (b2_pi / 180.0f));
}

void Rigidbody::SetAngularVelocity(float degrees_clockwise)
{
    body->SetAngularVelocity(degrees_clockwise * (b2_pi / 180.0f));
}

void Rigidbody::SetGravityScale(float gravityScale)
{
    body->SetGravityScale(gravityScale);
}

void Rigidbody::SetUpDirection(b2Vec2 vec2)
{
    vec2.Normalize();
    float angle = std::atan2(vec2.x, -vec2.y);

    body->SetTransform(body->GetPosition(), angle);

}

void Rigidbody::SetRightDirection(b2Vec2 vec2)
{
    vec2.Normalize();
    float angle = std::atan2(vec2.x, -vec2.y) - (b2_pi / 2.0f);

    body->SetTransform(body->GetPosition(), angle);

}

void Rigidbody::create_fixture()
{
    //no trigger or collider
    if (!has_collider && !has_trigger) {
        b2FixtureDef fixtureDef;
        b2PolygonShape* polyshape = new b2PolygonShape();
        polyshape->SetAsBox(0.5f * width, 0.5f * height);
        fixtureDef.shape = polyshape;
        fixtureDef.density = density;
        fixtureDef.isSensor = true;

        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

        body->CreateFixture(&fixtureDef);
        return;
    }
    if (has_collider) {
        //create collider before trigger
        create_collider();
    }
    if (has_trigger) {
        //create trigger if has
        create_trigger();
    }

}

void Rigidbody::create_collider()
{
    b2FixtureDef fixtureDef;
    if (collider_type == "box") {
        b2PolygonShape* polyshape = new b2PolygonShape();
        polyshape->SetAsBox(0.5f * width, 0.5f * height);
        fixtureDef.shape = polyshape;
    }
    else if (collider_type == "circle")
    {
        b2CircleShape* circleshape = new b2CircleShape();
        circleshape->m_radius = radius;
        fixtureDef.shape = circleshape;
    }

    fixtureDef.density = density;
    fixtureDef.isSensor = false;
    
    fixtureDef.friction = friction;
    fixtureDef.restitution = bounciness;

    fixtureDef.filter.categoryBits = category_collider;
    fixtureDef.filter.maskBits = category_collider;

    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

    body->CreateFixture(&fixtureDef);
}

void Rigidbody::create_trigger()
{
    b2FixtureDef fixtureDef;

    if (trigger_type == "box") {
        b2PolygonShape* polyshape = new b2PolygonShape();
        polyshape->SetAsBox(0.5f * trigger_width, 0.5f * trigger_height);
        fixtureDef.shape = polyshape;
    }
    else if (trigger_type == "circle")
    {
        b2CircleShape* circleshape = new b2CircleShape();
        circleshape->m_radius = trigger_radius;
        fixtureDef.shape = circleshape;
    }

    fixtureDef.density = density;
    fixtureDef.isSensor = true;

    fixtureDef.friction = friction;
    fixtureDef.restitution = bounciness;

    fixtureDef.filter.categoryBits = category_trigger;
    fixtureDef.filter.maskBits = category_trigger;

    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

    body->CreateFixture(&fixtureDef);

}
