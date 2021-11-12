#include<bits/stdc++.h>
using namespace std;

int L,B,n;
vector<pair<int,int>> components;
vector<pair<int,int>> domain_value;
int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

void displayPath(vector<vector<char>> pcb){
    for(int i=0;i<pcb.size();i++){
        for(int j=0;j<pcb[0].size();j++){
            if(pcb[i][j]==1) cout << '1' << " ";
            else if(pcb[i][j]==0) cout << '-' << " ";
            else cout << pcb[i][j] << " ";
        }
        cout << endl;
    }
    cout << "* represents the pin position" << endl;
    cout << "1 represents the route formed" << endl;
    cout << "- represents the blank space" << endl;
}

void display2d(vector<vector<char>> pcb){
    for(int i=0;i<pcb.size();i++){
        for(int j=0;j<pcb[0].size();j++){
            cout << pcb[i][j] << " ";
        }
        cout << endl;
    }
}

void pcbDisplay(vector<pair<int,int>> location){
    location.push_back({0,0});
    components.push_back({L,B});
    vector<vector<char>> pcb(L+1,vector<char>(B+1,' ')); 
    for(int i=0;i<=n;i++){
        for(int x=location[i].first;x<=location[i].first+components[i].first;x++){
            pcb[x][location[i].second]='.';
        }    
        for(int x=location[i].first;x<=location[i].first+components[i].first;x++){
            pcb[x][location[i].second+components[i].second]='.';
        }    
        for(int x=location[i].second;x<=location[i].second+components[i].second;x++){
            pcb[location[i].first][x]='.';
        }    
        for(int x=location[i].second;x<=location[i].second+components[i].second;x++){
            pcb[location[i].first+components[i].first][x]='.';
        }    
    }
    for(int i=0;i<n;i++){
        pcb[location[i].first][location[i].second]= '>';
        pcb[location[i].first+components[i].first][location[i].second+components[i].second]= '<';
    }
    display2d(pcb);
    cout << "> represents top-left corner of the componennt" << endl;
    cout << "< represents bottom-right corner of the componennt" << endl;
    cout << "Note: As the dimensions has been considered along with the required padding, overlap will not happen in actual placement" << endl;
    components.pop_back();
}

bool iscomplete(vector<pair<int,int>> assignment){
    for(int i=0;i<assignment.size();i++){
        if(assignment[i].first==-1 || assignment[i].second==-1){
            return false;
        }
    }
    return true;
}

int select_unassigned_variable(vector<pair<int,int>> assignment){
    for(int i=0;i<assignment.size();i++){
        if(assignment[i].first==-1 && assignment[i].second==-1){
            return i;
        }
    }
}

void display(vector<pair<int,int>> outcome){
    for(int i=0;i<outcome.size();i++){
            cout <<"("<<outcome[i].first << "," << outcome[i].second << ")";
            if(i<outcome.size()-1)
                cout << ",";
        }
        cout << endl;
}

bool isconsistent(pair<int,int> value, int var,vector<pair<int,int>> assignment){
    if(!(value.first+components[var].first<=L) or !(value.second+components[var].second<=B)){
        return false;
    }
    int i=0;
    for(auto v:assignment){
        if(v==make_pair(-1,-1)){
            break;
        } 
        if(value.first+components[var].first<=v.first or value.first>=v.first+components[i].first){
            i++;
            continue;
        }
        else if(value.second+components[var].second<=v.second or value.second>=v.second+components[i].second){
            i++;
            continue;
        }
        else
            return false; 
    }
    return true;
}

bool bfs(pair<int,int> p1, pair<int,int> p2,vector<vector<char>>& pcb){
    vector<vector<bool>> visited(L,vector<bool>(B,0));
    queue<pair<int,int>> q;
    q.push(p1);
    visited[p1.first][p1.second]=true;
    map<pair<int,int>,pair<int,int>> parent;
    while(!q.empty()){
        pair<int,int> p=q.front();
        q.pop();
        if(p==p2){
            while(p1!=p){
                pcb[p.first][p.second]=1;
                p = parent[p];
            }
            pcb[p.first][p.second]=1;

            break;
        }
        for(int i=0;i<4;i++){
            int x=p.first+dx[i];
            int y=p.second+dy[i];
            if(x<0 || y<0 || x >= L || y >=B ||visited[x][y]==true || pcb[x][y]==1){
                continue;
            }
            visited[x][y]=true;
            q.push({x,y});
            parent[{x,y}]=p;
        }
    }
    if(visited[p2.first][p2.second]){
        return true;
    }else{
        return false;
    }

}

vector<pair<int,int>> recursive_backtracking(vector<pair<int,int>> assignment){

    if(iscomplete(assignment)){
        return assignment;
    }
    int var = select_unassigned_variable(assignment);
    for(int i=0;i<L;i++){
        for (int j=0;j<B;j++){
            pair<int,int> value={i,j};
            if(isconsistent(value,var,assignment)){
                assignment[var] = value;
                vector<pair<int,int>> result = recursive_backtracking(assignment);
                if (iscomplete(result)){
                    return result;
                }
                assignment[var]= {-1,-1};
            }
        }
    }
    return assignment;

}

vector<pair<int,int>> backtracking_search(){
    vector<pair<int,int>> assignment(components.size(),{-1,-1});
    return recursive_backtracking(assignment);
}


int main(){
    cout << "Please enter the Length and Breadth of PCB:";
    cin >> L >> B;
    cout << "Please enter the number of components:";
    cin >> n;
    cout << "Enter the components length and breadth in next N lines:\n";
    for(int i=0;i<n;i++){
        int l,b;
        cin >> l >> b;
        components.push_back({l,b});
    }
    vector<pair<int,int>> outcome = backtracking_search();
    if(iscomplete(outcome)){
        cout << "The possible assignment is (top-left corner):\n";
        display(outcome);
        pcbDisplay(outcome);
        //===============End of component placememnt=================

        //***************Route generation****************************
        sort(outcome.begin(),outcome.end());
        while(1){
            int flag=1;
            vector<vector<char>> pcb1(L,vector<char>(B,0));
            for(int i=0;i<outcome.size()-1;i++){
                if(!bfs(outcome[i],outcome[i+1],pcb1)){
                    flag=0;
                    break;
                }
            }
            if(flag==1){
                for(auto v:outcome){
                    pcb1[v.first][v.second]='*';
                }
                cout << "The order in which bus selects the points is:\n";
                display(outcome);
                displayPath(pcb1);
                break;
            }
            if(next_permutation(outcome.begin(),outcome.end())){
                continue;
            }else{
                cout << "It is not possible to have a route" << endl;
                return 0;
            }
        }
        cout << "Note: We have considered a BUS going from each node and have checked for the possibility of the same\n";
    }
    else{
        cout << "Mapping failed" << endl;
    }
}