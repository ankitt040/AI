#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <set>
#include <stdio.h>
#include <map>

using namespace std;
typedef vector<int> vi;
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define present(c,x) ((c).find(x) != (c).end())
#define debug false

int *list_goal_state_input,*map_goal_state_input;
map<string,string> parent;
int h_type = 2;
int num_rows=3,num_cols=3;
int map_nilson_htype4_curState[9],map_nilson_htype4[]={0,1,2,7,8,3,6,5,4};

class State {
	int rows,cols;
    vi list;
    int h,g;
  public:
  	State() {}
    State (int m,int n,const int *arr)
    {
      	rows=m;cols=n;
        int num=m*n;
      	list= vi(arr,arr+ num);
      	parent[this->toString()] = "";
      	g=h=0;
    }
    State (int m,int n,const int *arr,int g_)
    {
        rows=m;cols=n;
        int num=m*n;
        list= vi(arr,arr+ num);
        g=g_;
        set_h_val();
    }
	int* getList()const{
		int num=rows*cols;
		int *tmp_list= (int *) malloc(num * sizeof(int) );
		for(int i=0;i<num;i++)
      		tmp_list[i]=list[i];
      	return tmp_list;
	}
	int getRows()const{ return rows;}
	int getCols()const{ return cols;}
    string getParent()const{ 
        return parent[this->toString()];
    }
    void setParent( string p ){
    	parent[this->toString()] = p;
    }
    int  get_g_val() const{
    	return g;
    }
    void set_g_val(int g_val){
    	g=g_val;
    }
    int  get_h_val() const{
    	return h;
    }
    void set_h_val(int h_val){
    	h=h_val;
    }
    void set_h_val(){
    	h=cal_h();
    }
    void print() const{
    	int num=rows*cols;
    	for(int i=0;i<num;i++){
    		cout<<list[i]<<"_";
    	}
        cout<<" g="<<g<<" h="<<h<<endl;
    }
    void printPath()const{
    }
    vector< State > getChildren() const{
    	vector< State > children;
    	int list_size=rows*cols;
    	State s;
    	int *newlist= (int *) malloc(list_size * sizeof(int) );
        //cout<<"getChildren::";
    	for(int i=0;i<list_size;i++){
      		newlist[i]=list[i];
            //cout<<newlist[i]<<" ";
        }
        //cout<<endl;

    	int pos_blank;
    	for(pos_blank=0;pos_blank<list_size;pos_blank++)
    		if( list[pos_blank]==0 )
    			break;
    	
    	if( pos_blank - cols >= 0  ){ // move blank  up
    		newlist[pos_blank] = newlist[pos_blank - cols];
    		newlist[pos_blank  - cols] = 0;
    		s= State(rows,cols,newlist,g+1);
    		children.push_back(s);
    		newlist[pos_blank  - cols] = newlist[pos_blank]; // restore newlist for next child creation
    		newlist[pos_blank] = 0;
    	}
    	if( pos_blank + cols < list_size  ){ // move blank  down
    		newlist[pos_blank] = newlist[pos_blank + cols];
    		newlist[pos_blank  + cols] = 0;
    		s= State(rows,cols,newlist,g+1);
    		children.push_back(s);
    		newlist[pos_blank  + cols] = newlist[pos_blank]; // restore newlist for next child creation
    		newlist[pos_blank] = 0;
    	}
    	if( ( (pos_blank + 1)%cols!=0 ) && (pos_blank + 1 < list_size)  ){ // move blank  right
    		newlist[pos_blank] = newlist[pos_blank + 1];
    		newlist[pos_blank  + 1] = 0;
    		s= State(rows,cols,newlist,g+1);
    		children.push_back(s);
    		newlist[pos_blank  + 1] = newlist[pos_blank]; // restore newlist for next child creation
    		newlist[pos_blank] = 0;
    	}
    	if( (pos_blank - 1 >= 0) && ( (pos_blank)%cols !=0 )  ){ // move blank  left
    		newlist[pos_blank] = newlist[pos_blank - 1];
    		newlist[pos_blank  - 1] = 0;
    		s= State(rows,cols,newlist,g+1);
    		children.push_back(s);
    	}

    	return children;
    }
    string toString() const{
    	stringstream ss;
    	int list_size=rows*cols;
    	for(int i=0;i<list_size;i++)
    		ss<< list[i]<<"_";
    	return ss.str();
    }
    bool equals(const State& s ) const{
    	int list_size=rows*cols;
	
		for(int i=0;i<list_size;i++)
			if( list[i] != s.list[i] )
				return false;
    	return true;
    }
    int get_f_val()const{
    	return g+h;
    }
    bool operator< (const State& s ) const {
    	int list_size=rows*cols;
    	for(int i=0;i<list_size;i++)
    		if( list[i] == s.list[i] ){}
    		else
                return list[i]< s.list[i];
    	return false; // this == s
    }
    int cal_h( ) const {
		int list_size=rows*cols,r,c,manhattan,s;
    	int ans=0;
    	if(h_type==1)
    		ans= 0;
    	else if(h_type==2){
    		for(int i=0;i<list_size;i++)
    			if( (list[i]!=0) && list[i] != list_goal_state_input[i] )
    				ans++;
    	}
    	else if(h_type==3){
			manhattan=0;
    		for(int i=0;i<list_size;i++)
    			if( (list[i]!=0) &&  list[i] != list_goal_state_input[i] ){
    				r= (list[i])/cols -  map_goal_state_input[ list_goal_state_input[i] ] /cols ;
    				c= (list[i])%cols - map_goal_state_input[ list_goal_state_input[i] ]%cols;
    				r= (r<0? -r : r);
    				c= (c<0? -c : c);
    				manhattan += (r+c);
    			}
			ans = manhattan;
    	}
    	else if(h_type==4 && rows==3 && cols==3){
			manhattan=0,s=0;
    		for(int i=0;i<list_size;i++)
    			if( (list[i]!=0) &&  list[i] != list_goal_state_input[i] ){
    				r= (list[i])/cols -  map_goal_state_input[ list_goal_state_input[i] ] /cols ;
    				c= (list[i])%cols - map_goal_state_input[ list_goal_state_input[i] ]%cols;
    				r= (r<0? -r : r);
    				c= (c<0? -c : c);
    				manhattan += (r+c);
					map_nilson_htype4_curState[ list[i] ]=i;
    			}
			s=0;
			for(int i=0;i<list_size;i++){
				if( map_nilson_htype4_curState[ list[i] ] == 8 )
					s+= 1;
    			else if( map_nilson_htype4_curState[ (list[i] + 1)%9 ] != ( map_nilson_htype4_curState[ list[i] ] + 1 )%8 ){
    				s+=2;
    			}
			}
			ans = manhattan + 3*s;
    	}

    	return ans;
    } 

};

struct compCloseList
{
	bool operator() (const State& s1, const State& s2 ){
		return (s1 < s2);
	}
};
struct compOpenList
{
	bool operator() (const State& s1, const State& s2 ){
		if(s1.get_f_val() == s2.get_f_val())
			return (s1 < s2);
		return (s1.get_f_val() < s2.get_f_val() ) ;
	}
};
struct compOpenList_check
{
	bool operator() (const pair<string,int>& s1, const pair<string,int>& s2 ){
        int res=  (s1.first).compare(s2.first);
		//return ( res==0? (s1.second < s2.second): res );
        return res<0;
	}
};

pair<string,int> getPair(const State& s){
	return pair<string,int>(s.toString(),s.get_g_val());
}
bool a_star( const State& , const State& );
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}
void printPath(string node){

        //int g_ = node->get_g_val() ;
        //while(g_-- ){
            if(node == ""){
                return;
            }
            else{
                printPath(parent[node]);
                vector<string> v_sS=split(node,'_');
                int k=0;
                for (k = 0; k<num_cols; ++k)
                {
                    cout<<"__";
                }
                cout<<"_\n";
                tr(v_sS,i){
                    cout<<"|"<<*i;
                    if(++k%num_cols == 0)
                        cout<<"|\n";
                }
                cout<<"\n";
            }
            
        //}
        //cout<<"path done"<<endl;
}

class OpenList{
    set<State,compOpenList > open_list ;
    set<pair<string,int> > open_list_check ;
public:
    bool empty(){
        return open_list.empty();
    }
    State pop_min(){
        State curState,child,tmp;
        typeof(open_list.begin()) open_list_it,curState_open_list_it;
        typeof(open_list_check.begin()) open_list_check_it;

        curState_open_list_it = open_list.begin();
        curState = (State) *( curState_open_list_it );
        open_list_check.erase( getPair( *curState_open_list_it ) ); // 
        open_list.erase(curState_open_list_it);
        return curState;
    }
    void insert(const State& s){
        open_list.insert(s);
        open_list_check.insert( getPair(s) );
    }
    bool exists(const State& s){
        if(present(open_list_check, getPair(s) ) )
            return true;
        return false;
    }
    int getGval(const State& s){
        return open_list_check.find( getPair(s) ) ->second  ;
    }
    bool eraseCopy(const State& s){
        State tmp = State(s.getRows(),s.getCols(),s.getList());
        typeof(open_list_check.begin()) open_list_check_it;
        open_list_check_it = open_list_check.find( getPair(s) ) ;
        tmp.set_g_val( open_list_check_it->second );
        tmp.set_h_val( s.get_h_val() ) ;

        open_list.erase(tmp);
        open_list_check.erase( open_list_check_it );
        return true;
    }
    void print()const{
        cout<<"open_list:\n";
        tr(open_list,i){
            cout<<"       "; i->print();//<<endl;
        }
        cout<<endl;
    }

};

int main(){
    std::ios::sync_with_stdio(false);
    freopen("inp.txt","r",stdin);

	int i,mat_size;
	int  *inp_start;

    cout<<"rows  cols\n";
	cin>>num_rows >>num_cols; // INPUT
	mat_size= num_rows * num_cols;
    if(mat_size <= 0){
        cout<<"ERROR, rows & cols values must be +ve!!!\n";
        return -1;
    }
    map_goal_state_input = (int*) malloc(mat_size* sizeof(int) );
    list_goal_state_input = (int*) malloc(mat_size* sizeof(int) );
    inp_start = list_goal_state_input;

    cout<<"Start_State Matrix of size="<<num_rows<<"*"<<num_cols<<endl;
	i=0;
	while(i< mat_size)
		cin>> inp_start[i++];
    State start= State(num_rows,num_cols,inp_start,0) ;


    cout<<"Goal_State Matrix of size="<<num_rows<<"*"<<num_cols<<endl;
	i=0;
	while(i< mat_size){
		cin>> list_goal_state_input[i];
        map_goal_state_input[ list_goal_state_input[i] ] = i;
        i++; 
    }
	State goal = State(num_rows,num_cols, list_goal_state_input ) ;

    for(h_type=4;h_type>0;h_type--){
        cout<<"_______________________\t h_type = "<<h_type<<"\t________________________\t"<<endl;
        if( a_star(start,goal) )
            cout<<"path found"<<endl;
        else{
            cout<<"\t**** FAILED TO REACH Goal_State ! ****\n"<<endl;  
		}
    }



	return 0;
}

bool a_star( const State& startState,const State& goalState ){
    cout<<"\n **** A STAR*****\n"<<"startState::"<<startState.toString()<<"\ngoalState::"<<goalState.toString()<<endl<<endl;

	long int iterations=0,States_discovered=1;
    OpenList open_list;
	set<State, compCloseList > close_list;
	
	open_list.insert(startState);
    parent[startState.toString()]="";

	vector<State > children;
	State curState,child,tmp;
	while(! open_list.empty()){

        curState = open_list.pop_min();
		if( present(close_list,curState) ){
			if(debug){ cout<<"**ERROR** "<<curState.toString()<<" in closed_list"<<endl; }
			continue;
		}
		close_list.insert(curState);

		if( curState.equals(goalState) ){
            cout<<"SUCCESS"<<endl;
            //cout<<"iterations="<<iterations<<endl;
			cout<<"States Discovered = "<<States_discovered <<endl;
            cout<<"Solution at depth = "<<curState.get_g_val() <<endl;
            cout<<"Solution Steps::"<<endl;
			printPath(curState.toString());

            cout<<"elements in open_list::"<<endl;
            open_list.print();

            cout<<"elements in close_list::"<<endl;
            tr(close_list,i){
                cout<<"       "; i->print();//<<endl;
            }

			return true;
		}

        // GET NEXT STATES REACHABLE FROM CURRENT STATE
		if(!children.empty())
			children.clear();
		children = curState.getChildren();

        if(debug){  cout<<"children:\n"; }

        // TRAVERSE EACH NEXT STATE
		tr(children,i){

            if(debug){ cout<<"     "; i->print(); }
			child = (State) (*i);

			if( ! present(close_list,child) )  // IF NEXT STATE IS NOT PRESENT IN CLOSE_LIST
            {
                // IF NEXT STATE IS PRESENT IN OPEN_LIST , CHECK IF ITS G_VALUE [=> PARENT ] NEEDS TO BE UPDATED
				if( open_list.exists(child)  ){
                    if ( child.get_g_val() < open_list.getGval(child) )  
                            open_list.eraseCopy(child); 
				}else{
                    States_discovered++;
                }
                open_list.insert(child);
                parent[ child.toString() ]= curState.toString(); // SET CURRENT_STATE AS PARENT OF THIS NEXT_STATE
                
			}
		}

        iterations++;
        if(debug){

            cout<<iterations<<"\n";
            open_list.print();
            cout<<"close_list:\n";
            tr(close_list,i){
                cout<<"       "; i->print();//<<endl;
            }
        }
        if(iterations >= 200000){
            cout<<"exiting after iterations="<<iterations<<endl;
            break;
        }

	}
    return false;
}