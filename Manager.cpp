#include "Manager.h"
#include <cstring>

Manager::~Manager()
{
    if (fout.is_open()) //if ofstream is open, close it
        fout.close();
    m_graph.~Graph(); //Remove the graph
}

void Manager::Run(const char* filepath)
{
    ifstream fin;
    fin.open(filepath);
    fout.open(RESULT_LOG_PATH, ios::out);
    if (!fin) //if 'filepath' does not exist, file open error
    {
        fout << "===== SYSTEM =====" << endl;
        fout << "CommandFileNotExist" << endl;
        fout << "==================" << endl;
        return;
    }

    while (!fin.eof())
    {
        string cmd;
        getline(fin, cmd); //read from the file line by line
        char* cmd_buf = new char[cmd.length() + 1];
        strcpy(cmd_buf, cmd.c_str());

        char* tmp = strtok(cmd_buf, " ");
        if (tmp == NULL)
            return;
        string str;
        if (tmp != NULL)
            str = string(tmp);
        if (str == "LOAD") //if the command is 'LOAD'
        {
            char* cmd = strtok(NULL, " ");
            ifstream fin2;
            fin2.open(cmd);
            if ((strcmp(cmd, "mapdata.txt") == 0) && !fin2) //if 'mapdata.txt' does not exist
                PrintError(LoadFileNotExist);
            else
                Load(cmd);
            fin2.close();
        }
        else if (str == "LOADREPORT") //if the command is 'LOADREPORT'
        {
            char* cmd = strtok(NULL, " ");
            ifstream fin2;
            fin2.open(cmd);
            if ((strcmp(cmd, "reportdata.txt") == 0) && !fin2) //if 'reportdata.txt' does not exist
                PrintError(FailedtoUpdatePath);
            else
                Load(cmd);
            fin2.close();
        }
        else if (str == "PRINT") //if the command is 'PRINT'
        {
            ifstream fin2;
            fin2.open("mapdata.txt");
            Print();
            fin2.close();
        }
        else if (str == "BFS") //if the command is 'BFS'
        {
            ifstream fin2;
            fin2.open("mapdata.txt");
            char* cmd = strtok(NULL, " ");
            if (cmd != NULL) //if there is any argument
            {
		fout << "======InvalidVertexKey======" << endl;
		fout << "Error: " << InvalidVertexKey << endl;
		fout << "============================" << endl << endl;
	    }
            else
                FindPathBfs(0, 6); //startVertexKey = 0, endVertexKey = 6
            fin2.close();
        }
        else if (str == "DIJKSTRA") //if the command is 'DIJKSTRA'
        {
            ifstream fin2;
            fin2.open("mapdata.txt");
            char* cmd = strtok(NULL, " ");
            if (cmd != NULL) //if there is any argument
            {
		fout << "======InvalidVertexKey======" << endl;
		fout << "Error: " << InvalidVertexKey << endl;
		fout << "============================" << endl << endl;
	    }
	    else
                FindShortestPathDijkstraUsingSet(0, 6); //startVertexKey = 0, endVertexKey = 6
            fin2.close();
        }
        else if (str == "BELLMANFORD") //if the command is 'BELLMANFORD'
        {
            ifstream fin2;
            fin2.open("mapdata.txt");
            char* cmd = strtok(NULL, " ");
            if (cmd != NULL) //if there is any argument
            {
		fout << "======InvalidVertexKey======" << endl;
		fout << "Error: " << InvalidVertexKey << endl;
		fout << "============================" << endl << endl;
	    }
            else
                FindShortestPathBellmanFord(0, 6); //startVertexKey = 0, endVertexKey = 6
            fin2.close();
        }
        else if (str == "FLOYD") //if the command is 'FLOYD'
        {
            ifstream fin2;
            fin2.open("mapdata.txt");
            FindShortestPathFloyd();
            fin2.close();
        }
        else if (str == "RABINKARP") //if the command is 'RABINKARP'
        {
            char* cmd = strtok(NULL, "/");
            string word;
            if (cmd != NULL)
                word = string(cmd);
            if (word.length() >= 10 || word.length() == 0) //if word does not exist or length of word >= 10
                PrintError(InvalidOptionNumber);
            else
            {
                RabinkarpCount = 0; 
                fout << "======RABINKARP======" << endl;
                list<string>::iterator iter;
                for (iter = reportdata.begin(); iter != reportdata.end(); iter++)
                {
                    const char* report = iter->c_str();
                    RabinKarpCompare(cmd, report);
                }
                if (RabinkarpCount == 0)
                    fout << "NO DUPLICATE TITLE EXISTS" << endl;
                fout << "===================" << endl << endl;  
            }
        }
        else //if the command is something else
        {
            fout << "======" << str << "======" << endl;
            fout << "NonDefinedCommand" << endl;
            fout << "=====================" << endl << endl;
        }
    }
    fin.close();
    fout.close();
}

/// <summary>
/// make a graph
/// </summary>
///
/// <param name="filepath">
/// the filepath to read to make the graph
/// </param>
///
/// <returns>
/// Result::Success if load is successful.
/// Result::LoadFileNotExist if file is not exist.
/// </returns>
Result Manager::Load(const char* filepath)
{
    ifstream fin2;
    fin2.open(filepath);

    if (strcmp(filepath, "mapdata.txt") == 0) //Update the graph
    {
        int countV = 0; //Count of vertex
        while (!fin2.eof())
        {
            string cmd;
            getline(fin2, cmd); //read from the file line by line
            char* cmd_buf = new char[cmd.length() + 1];
            strcpy(cmd_buf, cmd.c_str());
            char* com = strtok(cmd_buf, "/");
            string company; //string for name of company
            if (com != NULL)
                company = string(com);
            if (company != "")
                Company[countV] = company;

            m_graph.AddVertex(countV); //Add vertex which key is countV
            int countE = 0; //Count of Edge
            for (int i = 0; i < 7; i++)
            {
                int w = 0; //int for weight
                char* tmp = strtok(NULL, " ");
                if (tmp != NULL)
                    w = atoi(tmp);
                if (w != 0)
                {
                    countE = i;
                    m_graph.AddEdge(countV, countE, w);
                }
            }
            countV++;
        }
        PrintSuccess("LOAD");
    }
    if (strcmp(filepath, "reportdata.txt") == 0) //Update the string for reportdata
    {
        string firstdata;
        getline(fin2, firstdata);
        reportdata.push_back(firstdata);
        while (!fin2.eof())
        {
            string blank;
            string data;
            //Any two sentences are divided by a line
            //add two string and divide with '/'
            getline(fin2, blank);
            getline(fin2, data);
            reportdata.push_back(data);
        }
        PrintSuccess("LOADREPORT");
    }
    fin2.close();
    PrintError(Success);
    return Success;
}

/// <summary>
/// print out the graph as matrix form
/// </summary>
///
/// <returns>
/// Result::Success if the printing is successful
/// Result::GraphNotExist if there is no graph
/// </returns>
Result Manager::Print() //Print the graph as a matrix form
{
    if (m_graph.GetHead() == NULL) //if mapdata does not exist
    {
        PrintError(GraphNotExist);
        return GraphNotExist;
    }
    fout << "======" << "PRINT" << "======" << endl;
    Vertex* Vcur = m_graph.GetHead();
    while (Vcur)
    {
        Edge* Ecur = Vcur->GetHeadOfEdge();
        for (int i = 0; i < 7; i++)
        {
            if (Ecur)
            {
                if (i == Ecur->GetKey())
                {
                    fout << Ecur->GetWeight() << " ";
                    if (Ecur->GetNext())
                        Ecur = Ecur->GetNext();
                }
                else
                    fout << "0 ";
            }
        }
        if (Vcur->GetNext())
            fout << endl;
        Vcur = Vcur->GetNext();
    }
    fout << "===================" << endl << endl;
    return Success;
}

/// <summary>
/// find the path from startVertexKey to endVertexKey with BFS
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindPathBfs(int startVertexKey, int endVertexKey)
{
    if (m_graph.GetHead() == NULL) //if mapdata does not exist
    {
        PrintError(GraphNotExist);
        return GraphNotExist;
    }

    if (m_graph.IsNegativeEdge() == true) //if there is a negative edge
        fout << "===== InvalidAlgorithm =====" << endl;
    else
        fout << "===== BFS =====" << endl;

    //initialize arry
    bool visit[7] = { unvisited, unvisited, unvisited, unvisited, unvisited, unvisited, unvisited }; //array that checks if I visited the node
    int past[7] = { -1, -1, -1, -1, -1, -1, -1 }; //array that stores its past node

    //Its algorithm is similar to level-order
    queue<int> q; //using queue
    q.push(startVertexKey);
    visit[startVertexKey] = visited;

    while (!q.empty())
    {
        int x = q.front();
        q.pop();
        Vertex* Vcur = m_graph.GetHead();
        for (int i = 0; i < x; i++)
        {
            Vcur = Vcur->GetNext();
        }
        Edge* Ecur = Vcur->GetHeadOfEdge();
        for (int i = 0; i < 7; i++)
        {
            if (Ecur->GetKey() == i)
            {
                if (visit[Ecur->GetKey()] != visited)
                {
                    if (Ecur->GetWeight() > 0) //BFS does not think of negative edge
                    {
                        q.push(Ecur->GetKey());
                        visit[Ecur->GetKey()] = visited;
                        past[Ecur->GetKey()] = x;
                    }
                }
                if (Ecur->GetNext())
                    Ecur = Ecur->GetNext();
            }
        }
    }

    int reversepath[7]; //follow the shortest path from endVertex to startVertex
    int pathsize = 0; //the number of edges in the path
    int e = endVertexKey;
    while (e != startVertexKey) //repeat until e becomes startVertexKey
    {
        reversepath[pathsize] = e;
        e = past[e];
        pathsize++;
    }
    reversepath[pathsize] = e;
    fout << "shoftest path: ";
    for (int i = pathsize; i > -1; i--)
        fout << reversepath[i] << " ";
    fout << endl;

    int pathlength = 0; //length of the shortest path
    for (int i = pathsize; i > 0; i--)
    {
        Vertex* Vcur = m_graph.GetHead();
        for (int j = 0; j < reversepath[i]; j++)
        {
            Vcur = Vcur->GetNext();
        }
        Edge* Ecur = Vcur->GetHeadOfEdge();
        while (Ecur->GetKey() != reversepath[i - 1])
        {
            Ecur = Ecur->GetNext();
        }
        pathlength += Ecur->GetWeight();
    }
    fout << "path length: " << pathlength << endl;
    fout << "Course : ";
    for (int i = pathsize; i > -1; i--)
    {
        fout << Company[reversepath[i]] << " "; //Print company's name for each node in the path
    }
    fout << endl;
    fout << "==================" << endl;

    if (m_graph.IsNegativeEdge())
    {
        PrintError(InvalidAlgorithm);
        return InvalidAlgorithm;
    }
    else
        return Success;
}

/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    if (m_graph.GetHead() == NULL) //if mapdata does nt exist
    {
        PrintError(GraphNotExist);
        return GraphNotExist;
    }

    if (m_graph.IsNegativeEdge()) //if there is a negative edge
        fout << "===== InvalidAlgorithm =====" << endl;
    else
        fout << "===== DIJKSTRA =====" << endl;

    int dist[7] = { IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY }; //array for updating the shortest distance
    int past[7] = { -1, -1, -1, -1, -1, -1, -1 }; //array that stores the past node
    vector<pair<int, int> > a[7]; //array that represents a connection within nodes using vector

    Vertex* Vcur = m_graph.GetHead();
    for (int i = 0; i < 7; i++)
    {
        Edge* Ecur = Vcur->GetHeadOfEdge();
        for (int j = 0; j < 7; j++)
        {
            if (i == j) //if startVertexKey == endVertexKey
                a[i].push_back(make_pair(j, 0));
            else if (Ecur->GetKey() == j) //if two nodes are connected
            {
                a[i].push_back(make_pair(j, Ecur->GetWeight()));
                if (Ecur->GetNext())
                    Ecur = Ecur->GetNext();
            }
            else
                a[i].push_back(make_pair(j, IN_FINITY));
        }
        Vcur = Vcur->GetNext();
    }

    //Its algorithm is similar to level-order
    dist[startVertexKey] = 0;
    set<pair<int, int> > s; //Use set to check the shortest path
    s.insert(make_pair(startVertexKey, 0));

    while (!s.empty())
    {
        int cur = s.begin()->first;
        int distance = s.begin()->second;
        s.erase(make_pair(cur, distance));

        if (dist[cur] < distance)
            continue;

        for (int i = 0; i < 7; i++)
        {
            int next = a[cur][i].first; //first : endVertexKey
            int nextDistance = distance + a[cur][i].second; //second : weight

            if (nextDistance < dist[next] && nextDistance > 0)
            {
                dist[next] = nextDistance;
                past[next] = cur;
                s.insert(make_pair(next, nextDistance));
            }
        }
    }
    //결과 출력
    int reversepath[7]; //follow the path from endVertex to startVertex
    int pathsize = 0; //number of edges in the path
    int e = endVertexKey;
    while (e != startVertexKey) //repeat until e becomes startVertexKey
    {
        reversepath[pathsize] = e;
        e = past[e];
        pathsize++;
    }
    reversepath[pathsize] = e;
    fout << "shoftest path: ";
    for (int i = pathsize; i > -1; i--)
        fout << reversepath[i] << " ";
    fout << endl;
    fout << "path length: " << dist[endVertexKey] << endl; //path length is a value of dist[endVertexKey]
    fout << "Course : ";
    for (int i = pathsize; i > -1; i--) //
    {
        fout << Company[reversepath[i]] << " "; //Print company's name for each node in the path
    }
    fout << endl;
    fout << "==================" << endl;

    if (m_graph.IsNegativeEdge())
    {
        PrintError(InvalidAlgorithm);
        return InvalidAlgorithm;
    }
    else
        return Success;
}

/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::NegativeCycleDetected if exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    fout << "===== BELLMANFORD =====" << endl;
    
    if (m_graph.GetHead() == NULL) //if mapdata does not exist
    {
        PrintError(GraphNotExist);
        return GraphNotExist;
    }

    int dist[7] = { IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY, IN_FINITY }; //array for updating the shortest distance
    int past[7] = { -1, -1, -1, -1, -1, -1, -1 }; //array that stores the past node

    vector <pair<pair<int, int>, int> > edge[7]; //array that represents a connection within nodes using vector
    //vector <pair <pair<startVertexKey, endVertexKey>, weight> > edge[7];
    dist[startVertexKey] = 0;

    Vertex* Vcur = m_graph.GetHead();
    for (int i = 0; i < 7; i++)
    {
        Edge* Ecur = Vcur->GetHeadOfEdge();
        for (int j = 0; j < 7; j++)
        {
            if (i == j) //if startVertexKey == endVertexKey
                edge[i].push_back(make_pair(make_pair(i, j), 0)); //weight = 0
            else if (Ecur->GetKey() == j) //if two nodes are connected
            {
                edge[i].push_back(make_pair(make_pair(i, j), Ecur->GetWeight()));
                if (Ecur->GetNext())
                    Ecur = Ecur->GetNext();
            }
            else
                edge[i].push_back(make_pair(make_pair(i, j), IN_FINITY));
        }
        Vcur = Vcur->GetNext();
    }

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            int from = edge[i][j].first.first; //startVertexKey
            int to = edge[i][j].first.second; //endVertexKey
            int cost = edge[i][j].second; //weight

            if (dist[from] == IN_FINITY)
                continue;
            if (dist[to] > dist[from] + cost) //dist[to] = min(dist[to], dist[from] + cost)
            {
                dist[to] = dist[from] + cost;
                past[to] = from;
            }
        }
    }
    int negcycle = 0; //Count that checks if there is a negative cycle
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            int from = edge[i][j].first.first;
            int to = edge[i][j].first.second;
            int cost = edge[i][j].second;

            if (dist[from] == IN_FINITY)
                continue;
            if (dist[to] > dist[from] + cost) //if there is still an edge that has negative value
                negcycle = 1; //
        }
    }

    //결과 출력
    int reversepath[7]; //follow the path from endVertex to startVertex
    int pathsize = 0; //number of edges in the path
    int e = endVertexKey;
    while (e != startVertexKey) //repeat until e becomes startVertexKey
    {
        reversepath[pathsize] = e;
        e = past[e];
        pathsize++;
    }
    reversepath[pathsize] = e;
    fout << "shoftest path: ";
    for (int i = pathsize; i > -1; i--)
        fout << reversepath[i] << " ";
    fout << endl;

    fout << "path length: " << dist[endVertexKey] << endl;

    fout << "Course : ";
    for (int i = pathsize; i > -1; i--)
    {
        fout << Company[reversepath[i]] << " ";
    }
    fout << endl;
    fout << "==================" << endl;
    if (negcycle == 1) //if there is a negative cycle
        PrintError(NegativeCycleDetected);

    return Success;
}

Result Manager::FindShortestPathFloyd()
{
    if (m_graph.GetHead() == NULL) //if mapdata does not exist
    {
        PrintError(GraphNotExist);
        return GraphNotExist;
    }

    int A[7][7]; //A[k][i][j] : the shortest distance from i to j whose value is not bigger than k

    Vertex* Vcur = m_graph.GetHead();
    for (int i = 0; i < 7; i++)
    {
        Edge* Ecur = Vcur->GetHeadOfEdge();
        for (int j = 0; j < 7; j++)
        {
            if (i == j) //if startVertexKey == endVertexKey
                A[i][j] = 0;
            else if (Ecur->GetKey() == j) //if two nodes are connected
            {
                A[i][j] = Ecur->GetWeight();
                if (Ecur->GetNext())
                    Ecur = Ecur->GetNext();
            }
            else
                A[i][j] = IN_FINITY;
        }
        Vcur = Vcur->GetNext();
    }

    for (int k = 0; k < 7; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                int temp = A[i][k] + A[k][j];
                if (temp < A[i][j]) //A[i][j] = min(A[i][j], A[i][k] + A[k][j])
                    A[i][j] = temp;
            }
        }
    }
    int negcycle = 0; //Count that checks if there is a negative cycle
    for (int k = 0; k < 7; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                if (A[i][k] == IN_FINITY)
                    continue;
                if (A[i][j] > A[i][k] + A[k][j])
                    negcycle = 1;
            }
        }
    }

    fout << "===== FLOYD =====" << endl;
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            fout << A[i][j] << " ";
        }
        fout << endl;
    }
    fout << "==================" << endl;
    fout << endl;
    if (negcycle == 1) //if there is a negative cycle
        PrintError(NegativeCycleDetected);
    return Success;
}

Result Manager::RabinKarpCompare(const char* CompareString, const char* ComparedString)
{
    string com1;
    if (CompareString != NULL)
        com1 = string(CompareString);
    int size1 = com1.length(); //CompareString consists of size1 letter
    double com1hash = 0; //com1hash = com1[size - 1] * 2^0 + com1[size - 2] * 2^1 + ... + com1[0] * 2^(size-1)
    for (int i = size1 - 1; i > -1; i--)
    {
        if (com1[i] >= 'A' && com1[i] <= 'Z') //do not think of upper and lower case
            com1[i] += 32;
        com1hash += (com1[i] * pow(2, (size1 - 1 - i)));
    }

    string com2;
    if (ComparedString != NULL)
       com2 = string(ComparedString);
    int size2 = com2.length();
    string com2_low = com2;
    for (int i = 0; i < size2; i++)
    {
         if (com2_low[i] >= 'A' && com2_low[i] <= 'Z') //do not think of upper and lower case
             com2_low[i] += 32;
    }
    for (int i = size1 - 1; i < size2; i++) //repeat until it reaches the end of ComparedString
    {
        double com2hash = 0; //str1hash = str2[size - 1] * 2^0 + str2[size - 2] * 2^1 + ... + str2[0] * 2^(size-1)
        for (int j = i; j > i - size1; j--)
        {
            com2hash += (com2_low[j] * pow(2, (i - j)));
        }
        if (com1hash == com2hash) //if two value is equal, it means that duplicate title exists
        {
            fout << com2 << endl;
            RabinkarpCount = 1;
        }
    }

    return Success;
}

void Manager::PrintError(Result result)
{
    fout << "===================" << endl;
    fout << "Error code: " << result << endl;
    fout << "===================" << endl << endl;
}
void Manager::PrintSuccess(const char* command)
{
    fout << "======" << command << "======" << endl;
    fout << "Success" << endl;
    fout << "====================" << endl << endl;
}
