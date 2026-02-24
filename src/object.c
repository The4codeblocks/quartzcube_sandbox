
#include <stdlib.h>

#include "object.h"

int curid = 0;

Object* createObject(pos3 pos, cstr serial) { // TODO: serialization
	ObjectNode* objectNode = calloc(1, sizeof(ObjectNode));
	objectNode->object.pos = pos;
	Object* object = &(objectNode->object);
	object->node = objectNode;
	object->orientation = (orientation){ 0,0,1, 0,1,0 };
	objectNode->next = mainEnv.root;
	if (mainEnv.root) mainEnv.root->prev = objectNode;
	mainEnv.root = objectNode;
	writeMap_ip(refbackmap, ++curid, object);
	writeMap_pi(refmap, object, curid);
	return object;
}

void eliminateObject(ObjectRef ref) {
	Object* obj = delMap_ip(refbackmap, ref.id);
	if (!obj) return;
	ObjectNode* node = obj->node;
	Component* comp = obj->components;
	while (comp) {
		Component* next = comp->next;
		eliminateComponent(comp);
		comp = next;
	}
	delMap_pi(refmap, obj);
	if (node) {
		ObjectNode* prev = node->prev;
		ObjectNode* next = node->next;
		if (prev) prev->next = next;
		else mainEnv.root = next;
		if (next) next->prev = prev;
		free(node);
	} else free(obj);
}

void eliminateObjectRaw(Object* obj) {
	if (!obj) return;
	ObjectNode* node = obj->node;
	Component* comp = obj->components;
	while (comp) {
		Component* next = comp->next;
		eliminateComponent(comp);
		comp = next;
	}
	delMap_ip(refbackmap, delMap_pi(refmap, obj));
	if (node) {
		ObjectNode* prev = node->prev;
		ObjectNode* next = node->next;
		if (prev) prev->next = next;
		else mainEnv.root = next;
		if (next) next->prev = prev;
		free(node);
	}
	else free(obj);
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

Object* fromRef(ObjectRef ref) {
	if (ref.id == 0) return NULL;
	return readMap_ip(refbackmap, ref.id);
}

ObjectRef toRef(Object* obj) {
	return (ObjectRef) { readMap_pi(refmap, obj) };
}

bool exists(ObjectRef ref) {
	if (ref.id == 0) return false;
	return readMap_ip(refbackmap, ref.id) && true;
}
