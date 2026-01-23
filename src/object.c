
#include <stdlib.h>

#include "object.h"

Object* createObject(pos3 pos, cstr serial) { // TODO: serialization
	ObjectNode* objectNode = calloc(1, sizeof(ObjectNode));
	objectNode->object.pos = pos;
	Object* object = &(objectNode->object);
	object->node = objectNode;
	object->orientation = (orientation){ 0,0,1, 0,1,0 };
	objectNode->next = rootObjectNode;
	if (rootObjectNode) rootObjectNode->prev = objectNode;
	rootObjectNode = objectNode;
	return object;
}

void eliminateObject(Object* obj) {
	ObjectNode* node = obj->node;
	Component* comp = obj->components;
	while (comp) {
		Component* next = comp->next;
		eliminateComponent(comp);
		comp = next;
	}
	if (node) {
		ObjectNode* prev = node->prev;
		ObjectNode* next = node->next;
		if (prev) prev->next = next;
		else rootObjectNode = next;
		if (next) next->prev = prev;
		obj->node->next = (ObjectNode*)1;
	} else free(obj);
}

Component* addComponent(Object* object, ComponentDef* def) {
	Component* components = object->components;
	Component* component = malloc(sizeof(Component));
	component->def = def;
	component->data = NULL;
	component->next = components;
	if (components) components->prev = component;
	component->prev = NULL;
	component->obj = object;
	object->components = component;
	if (def->init) def->init(component);
	return component;
}

void eliminateComponent(Component* component) {
	Component* prev = component->prev;
	if (prev) {
		prev->next = component->next;
	} else {
		component->obj->components = component->next;
	}
	void (*elim)(Component*) = component->def->elim;
	if (elim) elim(component);
	free(component);
}

void copyComponents(Object* target, Object* from) {
	Component* comp = from->components;
	while (comp) {
		ComponentDef* def = comp->def;
		Component* newcomp = addComponent(target, def);
		if (def->serialize) def->deserialize(newcomp, def->serialize(*comp));
		comp = comp->next;
	}
}

void drawFrom(ObjectNode* startingNode) {
	ObjectNode* node = startingNode;
	while (node) {
		Component* comp = node->object.components;
		while (comp) {
			ComponentDef* def = comp->def;
			if (def->draw) def->draw(comp);
			comp = comp->next;
		}
		node = node->next;
	}
}

void UIFrom(ObjectNode* startingNode) {
	ObjectNode* node = startingNode;
	while (node) {
		Component* comp = node->object.components;
		while (comp) {
			ComponentDef* def = comp->def;
			if (def->pushUI) def->pushUI(comp);
			comp = comp->next;
		}
		node = node->next;
	}
}
