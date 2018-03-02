#include "entitygroup.h"

EntityGroup::EntityGroup() {
	en = std::vector<Entity*>();
}

void EntityGroup::Add(Entity &e) {
	en.push_back(&e);
}

void EntityGroup::Update(GLint type, GLint length, glm::vec3 mag) {
	for (int i = 0; i < en.size(); i++) {
		en.at(i)->AddAnimation(type, length, mag);
	}
}