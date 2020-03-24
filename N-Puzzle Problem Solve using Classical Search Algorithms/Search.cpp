#include <bits/stdc++.h>
#define UP 1
#define DOWN 2
#define Left 3
#define Right 4
using namespace std;


ofstream BFS_file;
ofstream UCS_file;
ofstream DLS_file;
ofstream IDS_file;
ofstream GBFS_file;
ofstream A_STAR_file;



struct Node
{
    int state[25][25];
    string path_parent = "";
    int pos_0_i, pos_0_j;
    int level;
    int number_of_misplaced_tiles;
    int evaluation_function_for_A_search;
};

struct level_compare
{
    bool operator()(Node const& a, Node const& b)
    {
        return b.level < a.level;
    }
};

struct gbfs_comapre_function
{
    bool operator()(Node const& a, Node const& b)
    {
        return b.number_of_misplaced_tiles < a.number_of_misplaced_tiles;
    }
};

struct A_search_comapre_function
{
    bool operator()(Node const& a, Node const& b)
    {
        return b.evaluation_function_for_A_search < a.evaluation_function_for_A_search;
    }
};

map<string, int> Visted_checking;
map<pair<string, int>, int> Visit_for_DLS;

int n;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void print_node(Node cur)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << cur.state[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

void Path_Print(Node node, string path)  /// parent node sent, and the path of the goal node
{
    int len = path.size();
    int x_action, y_action;
    for (int i = 0; i < len; i++)
    {
        int x_0 = node.pos_0_i;
        int y_0 = node.pos_0_j;
        if (path[i] == '1')
        {
            x_action = x_0 - 1;  /// up dx = -1 , dy =0;
            y_action = y_0;
            cout << "Action is UP" << endl;
        }
        if (path[i] == '2')  /// down dx=1, dy=0
        {
            x_action = x_0 + 1;
            y_action = y_0;

            cout << "Action is down" << endl;
        }
        if (path[i] == '3')  /// left dx=0 dy=-1
        {
            x_action = x_0;
            y_action = y_0 - 1;
            cout << "Action is left" << endl;
        }

        if (path[i] == '4')  /// right dx=0 dy =1
        {
            x_action = x_0;
            y_action = y_0 + 1;
            cout << "Action is right" << endl;
        }

        swap(node.state[x_0][y_0], node.state[x_action][y_action]);
        node.pos_0_i = x_action;
        node.pos_0_j = y_action;
        print_node(node);
    }
}

int visit(Node cur)
{
    string s = "";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            s += cur.state[i][j] + '0';

    if (Visted_checking[s] == 1)
        return 1;
    else
    {
        Visted_checking[s] = 1;  /// since it is not visited, so set map 1 and return 0 - means it was not explored previous
        return 0;
    }
}

int DLS_VISIT(Node node, int Limit)
{
    string s = "";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            s += node.state[i][j] + '0';

    if( Visit_for_DLS[make_pair(s, Limit)] ==0 )
    {

        Visit_for_DLS[make_pair(s, Limit)] = 1;

        return 0;

    }
    return 1;
}

int valid(Node cur)
{
    // string Accepted = "123456780";
    string Accepted = "";
    for (int i = 1; i <= (n * n) - 1; i++)
        Accepted += i + '0';
    Accepted += '0';
    //  cout<<Accepted<<endl;
    string s = "";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            s += cur.state[i][j] + '0';
    if (Accepted == s) return 1;
    return 0;
}

int misplaced_tiles(Node node, int n)
{
    string goal = "";
    string cur="";
    int misplaced=0;
    for (int i = 1; i <= (n * n) - 1; i++)
        goal += i + '0';
    goal += '0';

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cur += node.state[i][j] + '0';

    for(int i=0; i<cur.length(); i++)
    {
        if(goal[i]!=cur[i])
            misplaced++;

    }

    //cout<<misplaced<<endl;
    return misplaced;

}

int bfs(int n, Node node)
{
    int i = 0;
    int total_nodes_generated = 0;
    string parent_node;
    int validation_check;
    //  print_node(node);
    node.level = 0;
    queue<Node> q;
    q.push(node);
    Node FRONT;
    while (!q.empty())
    {
        FRONT = q.front();
        q.pop();
        validation_check = valid(FRONT);
        if (validation_check == 1)
        {
            // print_node(FRONT);
              printf("BFS : Accepted in level %d \n",FRONT.level);
              BFS_file<<FRONT.level<<"    ,    ";
              Path_Print(FRONT,FRONT.path_parent);

            return total_nodes_generated;
        }
        visit(FRONT);

        for (int j = 0; j < 4; j++)
        {
            int x = FRONT.pos_0_i + dx[j];
            int y = FRONT.pos_0_j + dy[j];
            int x_0 = FRONT.pos_0_i;
            int y_0 = FRONT.pos_0_j;

            if (!(x < 0 || x > n - 1 || y < 0 || y > n - 1))
            {

                Node cur = FRONT;
                swap(cur.state[x_0][y_0], cur.state[x][y]);
                if (visit(cur) == 0)
                {
                    total_nodes_generated++;
                    if (dx[j] == -1 && dy[j] == 0)  ///UP Move =1
                        cur.path_parent += '1' ;
                    if (dx[j] == 1 && dy[j] == 0)  /// Down move =2
                        cur.path_parent += '2';

                    if (dx[j] == 0 && dy[j] == -1)  ///Left move = 3
                        cur.path_parent += '3';
                    if (dx[j] == 0 && dy[j] == 1)  /// Right move =4
                        cur.path_parent += '4';

                    cur.pos_0_i = x;
                    cur.pos_0_j = y;
                    cur.level = FRONT.level + 1;
                    validation_check = valid(cur);
                    if (validation_check == 1)
                    {
                        // cout<<cur.path_parent<<endl;
                        printf("BFS : Accepted in level %d\n",cur.level);
                        BFS_file<<cur.level<<"    ,    ";
                        Path_Print(node,cur.path_parent);

                        //cout<<cur.level<<" , ";
                        return total_nodes_generated;
                    }
                    q.push(cur);
                }
            }
        }
    }

    BFS_file<<INT_MAX<<"    ,     ";

    printf("BSF couldn't reach to the goal state\n");
    return total_nodes_generated;
}

int UCS(Node node)
{
    priority_queue<Node, vector<Node>, level_compare> pq;
    node.level = 0;
    int total_nodes_generated = 0;

    pq.push(node);
    int validation_check;
    Node FRONT;
    while (!pq.empty())
    {
        FRONT = pq.top();
        total_nodes_generated++;
        pq.pop();
        validation_check = valid(FRONT);
        if (validation_check == 1)
        {
            printf("UCS: Accepted in level %d \n", FRONT.level);
            UCS_file<<FRONT.level<<"    ,    ";

            Path_Print(node, FRONT.path_parent);


            return total_nodes_generated;
        }
        visit(FRONT);

        for (int j = 0; j < 4; j++)
        {
            int x = FRONT.pos_0_i + dx[j];
            int y = FRONT.pos_0_j + dy[j];
            int x_0 = FRONT.pos_0_i;
            int y_0 = FRONT.pos_0_j;
            // printf("x= %d , y= %d , x_0=%d , y_0 = %d\n\n",x,y,x_0,y_0);
            if (!(x < 0 || x > n - 1 || y < 0 || y > n - 1))
            {
                Node cur = FRONT;
                swap(cur.state[x_0][y_0], cur.state[x][y]);

                if (visit(cur) == 0)
                {
                    if (dx[j] == -1 && dy[j] == 0)  ///UP Move =1
                        cur.path_parent += '1';
                    if (dx[j] == 1 && dy[j] == 0)  /// Down move =2
                        cur.path_parent += '2';

                    if (dx[j] == 0 && dy[j] == -1)  ///Left move = 3
                        cur.path_parent += '3';
                    if (dx[j] == 0 && dy[j] == 1)  /// Right move =4
                        cur.path_parent += '4';

                    cur.pos_0_i = x;
                    cur.pos_0_j = y;
                    cur.level = FRONT.level + 1;
                    validation_check = valid(cur);
                    if (validation_check == 1)
                    {
                        //cout<<cur.path_parent<<endl;
                        printf("UCS : Accepted in level %d \n",cur.level);
                        UCS_file<<cur.level<<"    ,    ";
                        Path_Print(node,cur.path_parent);
                        //  cout<<cur.level<<",";
                        return total_nodes_generated;
                    }
                    pq.push(cur);
                }
            }
        }
    }
    UCS_file<<INT_MAX<<"    ,    ";
    printf("UCS couldn't reach to the goal state\n");
    return total_nodes_generated;
}

int flag = 0;
int IDS_ACCEPTED_LEVEL = 0;
int DLS_generated_node = 0;
int IDS_CALL=0;

void global_varrible(Node node)
{
    flag = 0;
    IDS_CALL=0;
    IDS_ACCEPTED_LEVEL = 0;
    DLS_generated_node = 0;
    node.level = 0;
    Visit_for_DLS.clear();
    Visted_checking.clear();
}

void DLS(Node node, int Limit)
{
    if (DLS_VISIT(node, node.level) or Limit<0 or flag) return;

    if (valid(node) == 1)
    {


        flag = 1;
        if(flag==1 && IDS_CALL==0 )
        {
            cout << "DLS accpeted in level " << node.level << endl;
            DLS_file<<node.level<<"   ,   ";
            Path_Print(node,node.path_parent);

        }

        if(IDS_CALL==1)
        {
            cout << "IDS accpeted in level " << node.level << endl;
            IDS_file<<node.level<<"    ,   ";
            Path_Print(node,node.path_parent);

        }
        return;
    }

    if (node.level == Limit)
    {
        //  cout << "Reached in level " << node.level << endl;
        return;
    }

    DLS_VISIT(node, node.level);
    for (int j = 0; j < 4; j++)
    {
        int x = node.pos_0_i + dx[j];
        int y = node.pos_0_j + dy[j];
        int x_0 = node.pos_0_i;
        int y_0 = node.pos_0_j;

        if (!(x < 0 || x > n - 1 || y < 0 || y > n - 1))
        {
            if (dx[j] == -1 && dy[j] == 0)  ///UP Move =1
                node.path_parent += '1';
            if (dx[j] == 1 && dy[j] == 0)  /// Down move =2
                node.path_parent += '2';

            if (dx[j] == 0 && dy[j] == -1)  ///Left move = 3
                node.path_parent += '3';
            if (dx[j] == 0 && dy[j] == 1)  /// Right move =4
                node.path_parent += '4';


            Node cur = node;
            swap(cur.state[x_0][y_0], cur.state[x][y]);
            cur.pos_0_i = x;
            cur.pos_0_j = y;
            cur.level = node.level+1;
            DLS_generated_node++;
            DLS(cur, Limit-1);
        }
    }
}

int IDS(Node node)
{
    IDS_CALL=1;
    flag = 0;
    int IDS_generated_node = 0;
    for (int i = 1;; i++)
    {
        Visit_for_DLS.clear();
        flag = 0;
        DLS_generated_node = 0;
        DLS(node, i);

        IDS_generated_node+=DLS_generated_node;

        if (flag == 1)
        {
            //cout<<IDS_ACCEPTED_LEVEL<<",";
//        printf("Number of nodes generated in IDS is :  %d\n", IDS_generated_node);

            return IDS_generated_node;
        }
    }
//    if(flag==0)
//    printf("IDS End\n");
    return IDS_generated_node;
}

int GBFS(Node node)
{
    priority_queue<Node, vector<Node>, gbfs_comapre_function> pq;
    node.number_of_misplaced_tiles = misplaced_tiles(node, n);
    int total_nodes_generated = 0;
    node.level = 0;
    pq.push(node);
    int validation_check;
    Node FRONT;
    while (!pq.empty())
    {
        FRONT = pq.top();
        pq.pop();
        validation_check = valid(FRONT);
        if (validation_check == 1)
        {
            printf("GBFS: Accepted in level %d \n",FRONT.level);
            GBFS_file<<FRONT.level<<"   ,     ";
            Path_Print(node,FRONT.path_parent);
            // cout<<FRONT.level<<",";

            return total_nodes_generated;
        }
        visit(FRONT);

        for (int j = 0; j < 4; j++)
        {
            int x = FRONT.pos_0_i + dx[j];
            int y = FRONT.pos_0_j + dy[j];
            int x_0 = FRONT.pos_0_i;
            int y_0 = FRONT.pos_0_j;
            // printf("x= %d , y= %d , x_0=%d , y_0 = %d\n\n",x,y,x_0,y_0);
            if (!(x < 0 || x > n - 1 || y < 0 || y > n - 1))
            {
                Node cur = FRONT;
                swap(cur.state[x_0][y_0], cur.state[x][y]);

                if (visit(cur) == 0)
                {
                    if (dx[j] == -1 && dy[j] == 0)  ///UP Move =1
                        cur.path_parent += '1';
                    if (dx[j] == 1 && dy[j] == 0)  /// Down move =2
                        cur.path_parent += '2';

                    if (dx[j] == 0 && dy[j] == -1)  ///Left move = 3
                        cur.path_parent += '3';
                    if (dx[j] == 0 && dy[j] == 1)  /// Right move =4
                        cur.path_parent += '4';

                    cur.pos_0_i = x;
                    cur.pos_0_j = y;
                    cur.level = FRONT.level + 1;
                    cur.number_of_misplaced_tiles = misplaced_tiles(cur, n);
                    total_nodes_generated++;

                    validation_check = valid(cur);
                    if (validation_check == 1)
                    {
                        printf("GBFS : Accepted in level %d\n",cur.level);
                        GBFS_file<<cur.level<<"   ,     ";

                        //cout<<cur.level<<",";
                        Path_Print(node,cur.path_parent);
                        return total_nodes_generated;

                    }


                    pq.push(cur);
                }
            }
        }
    }
    GBFS_file<<INT_MAX<<"   ,     ";

    printf("GBFS couldn't reach the goal node\n");
    return total_nodes_generated;
}

int A_search_algorithm(Node node)
{
    priority_queue<Node, vector<Node>, A_search_comapre_function> pq;
    node.level = 0;
    int total_nodes_generated = 0;

    node.number_of_misplaced_tiles = misplaced_tiles(node, n);
    node.evaluation_function_for_A_search = node.level + node.number_of_misplaced_tiles;

    pq.push(node);
    int validation_check;
    Node FRONT;
    while (!pq.empty())
    {
        FRONT = pq.top();
        pq.pop();
        validation_check = valid(FRONT);
        if (validation_check == 1)
        {
            //cout << FRONT.level << ",";
            printf("A*: Accepted in level \n",FRONT.level);
            A_STAR_file<<FRONT.level<<"    ,    ";
            Path_Print(node,FRONT.path_parent);
            return total_nodes_generated;
        }
        visit(FRONT);

        for (int j = 0; j < 4; j++)
        {
            int x = FRONT.pos_0_i + dx[j];
            int y = FRONT.pos_0_j + dy[j];
            int x_0 = FRONT.pos_0_i;
            int y_0 = FRONT.pos_0_j;
            //printf("x= %d , y= %d , x_0=%d , y_0 = %d\n\n",x,y,x_0,y_0);
            if (!(x < 0 || x > n - 1 || y < 0 || y > n - 1))
            {
                Node cur = FRONT;
                swap(cur.state[x_0][y_0], cur.state[x][y]);

                if (visit(cur) == 0)
                {
                    if (dx[j] == -1 && dy[j] == 0)  ///UP Move =1
                        cur.path_parent += '1';
                    if (dx[j] == 1 && dy[j] == 0)  /// Down move =2
                        cur.path_parent += '2';

                    if (dx[j] == 0 && dy[j] == -1)  ///Left move = 3
                        cur.path_parent += '3';
                    if (dx[j] == 0 && dy[j] == 1)  /// Right move =4
                        cur.path_parent += '4';

                    cur.pos_0_i = x;
                    cur.pos_0_j = y;
                    cur.level = FRONT.level + 1;
                    cur.number_of_misplaced_tiles = misplaced_tiles(cur, n);
                    cur.evaluation_function_for_A_search = cur.level + cur.number_of_misplaced_tiles;
                    validation_check = valid(cur);
                    total_nodes_generated++;

                    if (validation_check == 1)
                    {
                        //cout<<cur.path_parent<<endl;
                        printf("A* : Accepted in level %d \n",cur.level);
                        A_STAR_file<<cur.level<<"    ,    ";

                        Path_Print(node,cur.path_parent);
                        //cout<<cur.level<<",";
                        return total_nodes_generated;
                    }

                    pq.push(cur);
                }
            }
        }
    }
    A_STAR_file<<INT_MAX<<"    ,    ";

    printf("A* couldn't reach the goal node\n");
    return total_nodes_generated;
}

int main()
{
    Node node;
    BFS_file.open("BFS.txt",ios::out);
    UCS_file.open("UCS.txt",ios::out);
    DLS_file.open("DLS.txt",ios::out);
    IDS_file.open("IDS.txt",ios::out);
    GBFS_file.open("GBFS.txt",ios::out);
    A_STAR_file.open("A_STAR.txt",ios::out);





    while(1)
    {
        cout<<"Enter the number of matrix size of the puzzle : "<<endl;

        cin>>n;
        Node node;
        cout<<"Enter the matrix: "<<endl;;
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
            {
                cin>>node.state[i][j];
                if(node.state[i][j]==0)
                    node.pos_0_i=i, node.pos_0_j=j;
            }

        int test_case;
//         freopen("out.txt", "w", stdout);

        cout<<"Now choose the search you want to perform on the given input"<<endl;
        cout<<"1.BFS"<<endl;
        cout<<"2.UCS"<<endl;
        cout<<"3.DLS"<<endl;
        cout<<"4.IDS"<<endl;
        cout<<"5.GBFS"<<endl;
        cout<<"6.A*"<<endl;
        cout<<"7.All"<<endl;

        cin>>test_case;

        global_varrible(node);

        if(test_case==1 or test_case==7)
        {
            global_varrible(node);

            clock_t start = clock();
            int number_of_generated_nodes =  bfs(n,node);
            cout<<"The total number of nodes generated in BFS "<<number_of_generated_nodes<<endl;
            BFS_file<<number_of_generated_nodes<<"    ,    ";
            float end_time = (float) (clock()-start )/CLOCKS_PER_SEC;
            cout<<"BFS took time for searching goal is : "<< end_time<<endl;

            BFS_file<<end_time<<endl;
            cout<<endl;
            cout<<endl;
            cout<<"--------------------------------------"<<endl;
            cout<<endl;
            cout<<endl;

        }
        node.level=0;
        int Limit=25;

        if(test_case==2 or test_case==7)
        {
            global_varrible(node);

            clock_t start = clock();

            int number_node = UCS(node);

            float end_time = (float) (clock()- start) / CLOCKS_PER_SEC ;
            UCS_file<<number_node<<"    ,    ";
            cout<<"Number of nodes generated in UCS is : " << number_node<<endl;
            cout<<"UCS took time for searching goal is : "<< end_time<<endl;
            UCS_file<<end_time<<endl;

            cout<<endl;
            cout<<endl;
            cout<<"--------------------------------------"<<endl;
            cout<<endl;
            cout<<endl;

        }



        if(test_case==3 or test_case==7)
        {
            global_varrible(node);

            clock_t start= clock();
            DLS(node,Limit);
            if(flag==0)
            {
                cout<<"DLS couldn't reach to goal node"<<endl;
                DLS_file<<INT_MAX<<"    ,   ";

            }
            float end_time = (float) (clock()- start) / CLOCKS_PER_SEC;


            cout<<"The number of nodes generated in DLS is  : "<<DLS_generated_node<<endl;
            DLS_file<<DLS_generated_node<<"     ,     ";
            cout<<"DLS took time for searching goal is : "<< end_time<<endl;
            DLS_file<<end_time<<endl;
            cout<<endl;
            cout<<endl;
            cout<<"--------------------------------------"<<endl;
            cout<<endl;
            cout<<endl;

        }


        if(test_case==4 or test_case==7)
        {
            global_varrible(node);

            Visit_for_DLS.clear();
            clock_t start= clock();
            int number_of_nodes = IDS(node);
            if(flag==0)
            {
                cout<<"IDS couldn't reach to the goal state "<<endl;
                IDS_file<<INT_MAX<<"    ,    ";

            }
            cout<<"The number of nodes generated in IDS is: "<<number_of_nodes<<endl;
            IDS_file<<number_of_nodes<<"     ,        ";
            float end_time = (float) ( clock() - start) / CLOCKS_PER_SEC;
            cout<<"IDS took time for searching goal is : "<< end_time<<endl;
            IDS_file<<end_time<<endl;
            cout<<endl;
            cout<<endl;
            cout<<"--------------------------------------"<<endl;
            cout<<endl;
            cout<<endl;

        }

        if(test_case==5 or test_case==7)
        {
            global_varrible(node);

            clock_t start = clock();
            int number_node = GBFS(node);
            float end_time = (float) ( clock() - start) / CLOCKS_PER_SEC;
            cout<<"Numebr of nodes generated in GBFS is  :  "<<number_node<<endl;
            GBFS_file<<number_node<<"    ,    ";
                        cout<<"GBFS took time for searching goal is : "<< end_time<<endl;
                        GBFS_file<<end_time<<endl;

            cout<<endl;
            cout<<endl;
            cout<<"--------------------------------------"<<endl;
            cout<<endl;
            cout<<endl;
        }


        if(test_case==6 or test_case==7)
        {
            global_varrible(node);

            clock_t start = clock();
            int number_of_nodes = A_search_algorithm(node);
            cout<<"Total number of nodes generated by A* is : "<<number_of_nodes<<endl;
            A_STAR_file<<number_of_nodes<<"    ,    ";

            float end_time = (float) ( clock() - start) / CLOCKS_PER_SEC;
            cout<<"A* took time for searching goal is : "<< end_time<<endl;
            A_STAR_file<<end_time<<endl;
            cout<<endl;
            cout<<endl;
            cout<<"--------------------------------------"<<endl;
            cout<<endl;
            cout<<endl;

        }

        cout<<"--------------END------------"<<endl;


    }

}

