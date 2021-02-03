#include "Component/ScriptComponent.hpp"

class Player : public ScriptComponent
{
private:
	int life = 0;
public:

	Player(class GameObject* _gameobject);

	void Begin() final;
	void Update() final;
};