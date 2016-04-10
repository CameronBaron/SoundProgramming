#pragma once
#include "../../deps/glm/glm/glm.hpp"
#include "../../deps/glm/glm/ext.hpp"
#include <vector>

class Node
{
public:
	Node(glm::vec3 a_pos, glm::quat a_rot, Node* a_parent);
	~Node();

	virtual void Update();

	static Node& GetRoot();
	void SetParent(Node* parent);
	void AddChild(Node* childToAdd);
	void RemoveChild(Node* childToRemove);

	glm::vec3 m_position;
	glm::vec3 m_origin;
	glm::vec3 m_scale = glm::vec3(1);
	glm::quat m_rotation;

	glm::mat4 m_worldMatrix;
	glm::mat4 m_localMatrix;

	Node(bool root);

	Node* m_parent;
	std::vector<Node*> m_children;
};

