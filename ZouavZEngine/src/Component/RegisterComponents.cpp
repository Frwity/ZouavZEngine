#include "GameObject.hpp"
#include <imgui.h>

#include "Component/RegisterComponent.hpp"

#include <vector>

//<REGISTERINCLUDE>
#include "Game/Player.hpp"
#include "Game/Enemy.hpp"
#include "Game/EnemyArcher.hpp"
#include "Game/Arrow.hpp"
//</REGISTERINCLUDE>

void RegisterComponents(std::vector<bool (*)(GameObject* _go)>& _addComponentsFunctions)
{
#define REGISTER(ClassName) _addComponentsFunctions.emplace_back([](GameObject* _go) -> bool {  \
	if (ImGui::Button((std::string("Add ") + #ClassName).c_str()))\
    {\
		_go->AddComponent<ClassName>();\
	    return true;\
    }\
    return false; })\

	//<REGISTER>
	REGISTER(Player);
    REGISTER(Enemy);
    REGISTER(EnemyArcher);
    REGISTER(Arrow);
	//</REGISTER>


#undef REGISTER
} 