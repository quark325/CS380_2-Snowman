#include <Animation.hpp>

Animation::Animation()
{
    _objects = std::vector<Engine::RenderObject*>();
}

Animation::~Animation()
{
    _objects.clear();
}

void Animation::AddObject(Engine::RenderObject* object)
{
    _objects.push_back(object);
}

void Animation::Animate(Engine::Camera* cam, float deltaTime)
{
    // Apply constant animation
    // Translation: moving toward (-1, -1, 0) per second
    // Rotation: 10-degree per second
    for (int i = 0; i < _objects.size(); i++)
    {
        _objects[i]->SetOrientation(glm::rotate(_objects[i]->GetOrientation(), glm::radians(deltaTime * 10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        _objects[i]->SetPosition(_objects[i]->GetPosition() + glm::vec3(deltaTime * -1.0f, deltaTime * -1.0f, 0.0));
        _objects[i]->Render(cam);

        // Reposition object after its out of sight.
        if (_objects[i]->GetPosition().y < -5.0f)
        {
            _objects[i]->SetPosition(glm::vec3(-5.0f + 10.0f * ((rand() % 255) / 255.0f), 5.0f * ((rand() % 255) / 255.0f), 0.0f));
        }
    }
}