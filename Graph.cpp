#include "Graph.h"

#define BFS_FIRST_PATH

Graph::Graph()
{
    m_pVHead = NULL; m_vSize = 0;
}
Graph::~Graph()
{
    Clear();
}

Vertex* Graph::GetHead()
{
    return m_pVHead;
}

void Graph::AddVertex(int vertexKey)
{
    Vertex* newVertex = new Vertex(vertexKey);
    if (!m_pVHead) //if there is no vertex for the graph, add it
    {
        m_pVHead = newVertex;
        m_vSize++;
        return;
    }
    Vertex* cur = m_pVHead;
    while (cur->GetNext())
        cur = cur->GetNext();
    cur->SetNext(newVertex); //Add vertex at the end of the linked list
    m_vSize++;
}

void Graph::AddEdge(int startVertexKey, int endVertexKey, int weight)
{
    Vertex* Vcur = this->FindVertex(startVertexKey);
    if (Vcur) //if there is a vertex with startVertexKey, add edge with endVertexKey
        Vcur->AddEdge(endVertexKey, weight);
    else //add vertex with startVertexKey
        this->AddVertex(startVertexKey); 
}

Vertex* Graph::FindVertex(int key) //Find a vertex with key
{
    Vertex* Vcur = m_pVHead;
    while (Vcur)
    {
        if (key == Vcur->GetKey())
            return Vcur;
        Vcur = Vcur->GetNext();
    }
    return NULL;
}

int Graph::Size() const
{
    return m_vSize;
}

void Graph::Clear() //Remove every vertex
{
    Vertex* cur = this->GetHead();
    while (cur)
    {
        Vertex* delVertex = cur;
        cur = cur->GetNext();
        delete delVertex;
        m_vSize--;
    }
    m_pVHead = NULL;
}

void Graph::Print(std::ofstream& fout)
{
    
}

bool Graph::IsNegativeEdge() //Check if there is a negative edge in the graph
{
    Vertex* Vcur = m_pVHead;
    while (Vcur)
    {
        Edge* Ecur = Vcur->GetHeadOfEdge();
        while (Ecur) //Check every edge in the graph
        {
            if (Ecur->GetWeight() < 0) //if there is an edge with a weight which value is less than 0
                return true;
            Ecur = Ecur->GetNext();
        }
        Vcur = Vcur->GetNext();
    }
    return false; //if there is no negative edge
}

