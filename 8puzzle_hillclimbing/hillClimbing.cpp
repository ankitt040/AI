#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <map>

using namespace std;
typedef vector<string> string_vector;
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define loop2(i,n) for(int i=0;i<n;i++)
#define debug false
#define SIZE 9
#define COLS 3
#define ROW  3
string goal_state = "123804765";
map<string,string> parent;

int cal_h(const string&);
string_vector getNextState(const string&);
bool hill_climb(const string&, const string&);
void printPath(string);

int main(){
    std::ios::sync_with_stdio(false);
    //freopen("inp.txt","r",stdin);

	int temp;
	string start_state= string(9,'x'),stemp;
    cout<<"Start_State Matrix of size="<<ROW<<"*"<<COLS<<endl;
	loop2(i,9){
		cin>>temp;
		start_state[i]= (char) ('0'+ temp);
	}

    if( hill_climb(start_state,goal_state) ){
    	cout<<"SUCCESS"<<endl;
    }else{
    	cout<<"FAIL!"<<endl;
    }

	return 0;
}

bool hill_climb(const string& start_state, const string& goal_state ){
    cout<<"____________________\t HILL CLIMB  \t__________________________"<<endl;
    cout<<"start_state: "<<start_state<<endl;
    cout<<"goal_state:  "<<goal_state<<endl;

    bool bool_temp;
	string curState = start_state, next_state,temp_state;
	long int states_discovered=1,goal_depth=-1;
	int h_new, h_cur, h_temp;
	string_vector children;
    parent[curState]="";
	while(true){
		goal_depth++;
		if(curState == goal_state ){
			cout<<"SUCCESS"<<endl;
            cout<<"Solution Steps::"<<endl;
			printPath(curState);
			cout<<"States Discovered = "<<states_discovered <<endl;
            cout<<"Solution at depth = "<<goal_depth<<endl;
			return true;
		}

		if(!children.empty())
			children.clear();
		children = getNextState(curState); // get next states generated after moving blank up,down,right,left
		h_cur= cal_h(curState);
		next_state="";

		if(debug) cout<<goal_depth<<"::"<< curState<<" => ";

		// TRAVERSE EACH NEXT STATE
		tr(children,i){
			temp_state = string( (*i).begin(),(*i).end() );
			h_temp = cal_h(temp_state);
			next_state= (h_temp>h_cur)? temp_state : next_state ;
			states_discovered++;
			if(debug) cout<<h_temp <<":"<< temp_state <<"  ";
		}
		if(next_state!=""){
			parent[next_state]=curState;
			curState=next_state;
			if(debug) cout<<"[next:"<< cal_h(next_state) <<": "<<next_state<<"]\n";
		}else{
			return false;
		}
		if(debug && ( goal_depth>200000) ){
			cout<<"goal_state not found after states_discovered:"<<states_discovered<<" depth:"<<goal_depth<<endl;
			return false;
		}
	}
}

int cal_h(const string& s){
	int h=0;
	for(int i=0;i< SIZE ;i++)
		if( (s[i]!='0') && s[i] != goal_state[i] )
			h--;
	return h;
}

string_vector getNextState(const string& s ){
	string s1,s2,s3,s4;
	string_vector children;
	int pos_blank;
	for(pos_blank=0; pos_blank<SIZE; pos_blank++)
		if( s[pos_blank]== '0' )
			break;
	if( pos_blank - COLS >= 0  ){ // move blank  up
		s1=string(s.begin(),s.end());
		s1[pos_blank] = s[pos_blank - COLS];
		s1[pos_blank  - COLS] = s[pos_blank];
		children.push_back(s1);
	}
	if( pos_blank + COLS < SIZE  ){ // move blank  down
		s2=string(s.begin(),s.end());
		s2[pos_blank] = s[pos_blank + COLS];
		s2[pos_blank  + COLS] = s[pos_blank];
		children.push_back(s2);
	}
	if( ( (pos_blank + 1)%COLS!=0 ) && (pos_blank + 1 < SIZE)  ){ // move blank  right
		s3=string(s.begin(),s.end());
		s3[pos_blank] = s[pos_blank + 1];
		s3[pos_blank  + 1] = s[pos_blank];
		children.push_back(s3);
	}
	if( (pos_blank - 1 >= 0) && ( (pos_blank)%COLS !=0 )  ){ // move blank  left
		s4=string(s.begin(),s.end());
		s4[pos_blank] = s[pos_blank - 1];
		s4[pos_blank  - 1] = s[pos_blank];
		children.push_back(s4);
	}
	return children;
}


void printPath(string node){

    if(node == ""){
        return;
    }
    else{
        printPath(parent[node]);
        int k;
        for (k = 0; k<COLS; ++k)
            cout<<"__";
        cout<<"_\n";
        k=0;
        loop2(i,9) {
            cout<<"|"<< node[i];
            if(++k % COLS == 0)
                cout<<"|\n";
        }
        cout<<"\n";
    }
}
