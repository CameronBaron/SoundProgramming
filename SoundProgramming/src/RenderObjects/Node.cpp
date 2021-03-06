#include "Node.h"



Node::Node(glm::vec3 a_pos, glm::quat a_rot, Node* a_parent) : m_position(a_pos), m_rotation(a_rot)
{
	if (a_parent == nullptr)
		GetRoot().AddChild(this);
	else
	{
		a_parent->AddChild(this);
		SetParent(a_parent);
	}
}

Node::~Node()
{
	if (m_parent != nullptr)
		m_parent->RemoveChild(this);

	for (Node* child : m_children)
		child->m_parent = nullptr;
}

void Node::Update()
{
	//m_localMatrix = glm::translate(m_localMatrix, m_position);
	//m_localMatrix = glm::scale(m_localMatrix, m_scale);
	//m_localMatrix = glm::rotate(m_localMatrix, 10.0f, glm::vec3(1,0.3f, 0.5f));
	m_localMatrix = glm::translate(m_position) * glm::scale(m_scale);

	if (m_parent != nullptr)
		m_worldMatrix = m_parent->m_worldMatrix * m_localMatrix;
	else
		m_worldMatrix = m_localMatrix;

	for (Node* child : m_children)
		child->Update();
}

//---------------------PARENT/CHILD-----------------//
Node& Node::GetRoot()
{
	static Node root(true);
	return root;
}

void Node::SetParent(Node* parent)
{
	m_parent = parent;
}

void Node::AddChild(Node* childToAdd)
{
	auto iter = std::find(m_children.begin(), m_children.end(), childToAdd);
	if (iter == m_children.end())
	{
		childToAdd->SetParent(this);
		m_children.push_back(childToAdd);
		return;
	}
}

void Node::RemoveChild(Node* childToRemove)
{
	auto iter = std::find(m_children.begin(), m_children.end(), childToRemove);
	if (iter != m_children.end())
	{
		childToRemove->m_parent = nullptr;
		m_children.erase(iter);
		return;
	}
}

//----------------------RETURN ROOT----------------//
Node::Node(bool root)
{
}
