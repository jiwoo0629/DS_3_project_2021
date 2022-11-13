#include "Vertex.h"

Vertex::Vertex()
{
	m_key = 0; m_size = 0; m_pEHead = NULL; m_pNext = NULL;
}

Vertex::Vertex(int key)
{
	m_key = key; m_size = 0; m_pEHead = NULL; m_pNext = NULL;
}

Vertex::~Vertex() 
{
	Clear();
}

void Vertex::SetNext(Vertex* pNext)
{
	m_pNext = pNext;
}

int Vertex::GetKey() const
{
	return m_key;
}

Vertex* Vertex::GetNext() const
{
	return m_pNext;
}

int Vertex::Size() const
{
	return m_size;
}

void Vertex::AddEdge(int edgeKey, int weight)
{
	Edge* newEdge = new Edge(edgeKey, weight); 
	if (!m_pEHead) //if there is no edge for the vertex, add it
	{
		m_pEHead = newEdge;
		m_size++;
		return;
	}
	Edge* cur = m_pEHead;
	while (cur->GetNext())
		cur = cur->GetNext();
	cur->SetNext(newEdge); //Add the edge at the end of the linked list
	m_size++;
}

Edge* Vertex::GetHeadOfEdge() const
{
	return m_pEHead;
}

void Vertex::Clear() //Remove every edge
{
	Edge* cur = this->GetHeadOfEdge();
	while (cur)
	{
		Edge* delEdge = cur;
		cur = cur->GetNext();
		delete delEdge;
		m_size--;
	}
	m_pEHead = NULL;
}
