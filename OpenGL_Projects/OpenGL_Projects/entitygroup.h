#ifndef ENTITYGROUP_H
#define ENTITYGROUP_H

#include "Entity.h"


class EntityGroup {
	std::vector<Entity*> en;

public:
	EntityGroup();

	void Add(Entity &e);
	void Update(GLint type, GLint length, glm::vec3 mag);
};

#endif /* ENTITYGROUP_H */