#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>
#include <tuple>
#include <algorithm>

using namespace std;

class contestExtHeap{
    public:
        contestExtHeap(int s);
        void findContestant(int k);
        void insertContestant(int k, int s);
        void eliminateWeakest();
        void earnPoints(int k, int p);
        void losePoints(int k, int p);
        void showContestants();
        void showHandles();
        void showLocation(int k);
        void crownWinner();
    
    private:
        int size;
        int num_elements;
        tuple<int, int> *heap;
        int *handle;
};

contestExtHeap::contestExtHeap(int s){
    this->size = s;
    this->num_elements = 0;
    this->heap = new tuple<int, int>[s];
    for(int i = 0; i < s; i++){
        get<0>(this->heap[i]) = -1;
        get<1>(this->heap[i]) = std::numeric_limits<int>::max();
    }
    this->handle = new int[s];
    for(int i = 0; i < s; i++){
        this->handle[i] = -1;
    }
}

void contestExtHeap::findContestant(int k){
    if(this->handle[k] == -1){
        cout << "Contestant <" << k << "> is not in the extended heap." << endl;
    }
    else{
        cout << "Contestant <" << k << "> is in the extended heap with score <" << get<1>(this->heap[this->handle[k]])<< ">." << endl;
    }
}

void contestExtHeap::insertContestant(int k, int s){
    if(this->handle[k] != -1){
        cout << "Contestant <" << k << "> is already in the extended heap: cannot insert." << endl;
    }
    else if(this->num_elements >= this->size-1){ 
        cout << "Contestant <" << k << "> could not be inserted because the extended heap is full." << endl;
    }
    else{

        get<0>(this->heap[this->num_elements+1]) = k;
        get<1>(this->heap[this->num_elements+1]) = s;
        this->handle[k] = this->num_elements+1;   
        
        this->num_elements++;
        
        int i = this->num_elements;
        
        while((i > 1) && (get<1>(this->heap[int(floor(i / 2))]) > get<1>(this->heap[i]))){
            
            //i info
            tuple<int, int> temp_i = this->heap[i];
            int temp_i_loc = this->handle[get<0>(this->heap[i])];
            
            //parent info
            tuple<int, int> temp_parent = this->heap[int(floor(i / 2))];
            int temp_parent_loc = this->handle[get<0>(this->heap[int(floor(i / 2))])];
            
            //exchange info
            this->heap[int(floor(i / 2))] = temp_i;
            this->handle[get<0>(this->heap[int(floor(i / 2))])] = temp_parent_loc;
            
            this->heap[i] = temp_parent;
            this->handle[get<0>(this->heap[i])] = temp_i_loc;
            
            i = int(floor(i / 2));
        }
        
        cout << "Contestant <" << k << "> inserted with initial score <" << s << ">." << endl;
        
    }
}

void contestExtHeap::eliminateWeakest(){
    if(this->num_elements == 0){
        cout << "No contestant can be eliminated since the extended heap is empty." << endl;
    }
    else{
        
        //return value
        tuple<int, int> min = this->heap[1];
        
        //1 info
        tuple<int, int> temp_1 = this->heap[1];
        int temp_1_loc = this->handle[get<0>(this->heap[1])];
        
        //num_elements info
        tuple<int, int> temp_ne = this->heap[this->num_elements];
        int temp_ne_loc = this->handle[get<0>(this->heap[this->num_elements])];
        
        //exchange info
        this->heap[this->num_elements] = temp_1;
        this->handle[get<0>(this->heap[this->num_elements])] = temp_ne_loc;
        
        this->heap[1] = temp_ne;
        this->handle[get<0>(this->heap[1])] = temp_1_loc;
        
        //remove last node
        this->handle[get<0>(this->heap[this->num_elements])] = -1;
        get<0>(this->heap[this->num_elements]) = -1;
        get<1>(this->heap[this->num_elements]) = std::numeric_limits<int>::max();
        
        this->num_elements--;
        
        /////////////////////////////////////////////////////////////////////////////
        //int i = this->num_elements;
        int i = 1;
        
        //while((i > 1) && (get<1>(this->heap[int(floor(i / 2))]) > get<1>(this->heap[i]))){
        while((i < this->num_elements+1) && ((get<1>(this->heap[2*i])) < get<1>(this->heap[i]) || (get<1>(this->heap[(2*i)+1])) < get<1>(this->heap[i]))){
            
            int left = get<1>(this->heap[2*i]);
            int right = get<1>(this->heap[(2*i) + 1]);
            int child;
            if(left <= right){
                child = 2*i;
            }
            else{
                child = (2*i) + 1;
            }
        
            //i info
            tuple<int, int> temp_i = this->heap[i];
            int temp_i_loc = this->handle[get<0>(this->heap[i])];
            
            //parent info
            //tuple<int, int> temp_parent = this->heap[int(floor(i / 2))];
            //int temp_parent_loc = this->handle[get<0>(this->heap[int(floor(i / 2))])];
            tuple<int, int> temp_parent = this->heap[child];
            int temp_parent_loc = this->handle[get<0>(this->heap[child])];
            
            //exchange info
            //this->heap[int(floor(i / 2))] = temp_i;
            //this->handle[get<0>(this->heap[int(floor(i / 2))])] = temp_i_loc;
            this->heap[child] = temp_i;
            this->handle[get<0>(this->heap[child])] = temp_parent_loc;//temp_i_loc;
            
            this->heap[i] = temp_parent;
            this->handle[get<0>(this->heap[i])] = temp_i_loc;//temp_parent_loc;
            
            //i = int(floor(i / 2));
            i = child;
        }
        
        cout << "Contestant <" << get<0>(min) << "> with current lowest score <" << get<1>(min) << "> eliminated." << endl;
    }
}

void contestExtHeap::earnPoints(int k, int p){
    if(this->handle[k] == -1){
        cout << "Contestant <" << k << "> is not in the extended heap." << endl;
    }
    else{
        
        get<1>(this->heap[this->handle[k]]) = get<1>(this->heap[this->handle[k]]) + p;
        
        int i = 1;
        
        //while((i > 1) && (get<1>(this->heap[int(floor(i / 2))]) > get<1>(this->heap[i]))){
        while((i < this->num_elements) && ((get<1>(this->heap[2*i])) < get<1>(this->heap[i]) || (get<1>(this->heap[(2*i)+1])) < get<1>(this->heap[i]))){
            
            int left = get<1>(this->heap[2*i]);
            int right = get<1>(this->heap[(2*i) + 1]);
            int child;
            if(left <= right){
                child = 2*i;
            }
            else{
                child = (2*i) + 1;
            }
        
            //i info
            tuple<int, int> temp_i = this->heap[i];
            int temp_i_loc = this->handle[get<0>(this->heap[i])];
            
            //parent info
            tuple<int, int> temp_parent = this->heap[child];
            int temp_parent_loc = this->handle[get<0>(this->heap[child])];
            
            //exchange info
            this->heap[child] = temp_i;
            this->handle[get<0>(this->heap[child])] = temp_parent_loc;//temp_i_loc;
            
            this->heap[i] = temp_parent;
            this->handle[get<0>(this->heap[i])] = temp_i_loc;//temp_parent_loc;
            
            i = child;
        }

        cout << "Contestant <" << k << ">’s score increased by <" << p << "> points to <" << get<1>(this->heap[this->handle[k]]) << ">." << endl;
    }
}

void contestExtHeap::losePoints(int k, int p){
    if(this->handle[k] == -1){
        cout << "Contestant <" << k << "> is not in the extended heap." << endl;
    }
    else{
        
        get<1>(this->heap[this->handle[k]]) = get<1>(this->heap[this->handle[k]]) - p;
    
        int i = 1;
        
        //while((i > 1) && (get<1>(this->heap[int(floor(i / 2))]) > get<1>(this->heap[i]))){
        while((i < this->num_elements) && ((get<1>(this->heap[2*i])) < get<1>(this->heap[i]) || (get<1>(this->heap[(2*i)+1])) < get<1>(this->heap[i]))){
            
            int left = get<1>(this->heap[2*i]);
            int right = get<1>(this->heap[(2*i) + 1]);
            int child;
            if(left <= right){
                child = 2*i;
            }
            else{
                child = (2*i) + 1;
            }
        
            //i info
            tuple<int, int> temp_i = this->heap[i];
            int temp_i_loc = this->handle[get<0>(this->heap[i])];
            
            //parent info
            tuple<int, int> temp_parent = this->heap[child];
            int temp_parent_loc = this->handle[get<0>(this->heap[child])];
            
            //exchange info
            this->heap[child] = temp_i;
            this->handle[get<0>(this->heap[child])] = temp_parent_loc;//temp_i_loc;
            
            this->heap[i] = temp_parent;
            this->handle[get<0>(this->heap[i])] = temp_i_loc;//temp_parent_loc;
            
            i = child;
        }
        
        cout << "Contestant <" << k << ">’s score decreased by <" << p << "> points to <" << get<1>(this->heap[this->handle[k]]) << ">." << endl;
    }
}

void contestExtHeap::showContestants(){
    for(int i = 1; i < this->size; i++){
        if(get<0>(this->heap[i]) != -1){
            cout << "Contestant <" << get<0>(this->heap[i]) << "> in extended heap location <" << this->handle[get<0>(this->heap[i])] << "> with score <" << get<1>(this->heap[i]) << ">." << endl;
            
            //cout << "Heap[" << i << "]: (" << get<0>(this->heap[i]) << "," << get<1>(this->heap[i]) << ")" << endl;
        }
    }
}

void contestExtHeap::showHandles(){
    for(int i = 1; i < this->size; i++){
        if(this->handle[i] == -1){
            cout << "There is no Contestant <" << i << "> in the extended heap: handle[<" << i << ">] = -1." << endl;
        }
        else{
            cout << "Contestant <" << i << "> stored in extended heap location <" << this->handle[i] << ">." << endl;
        }
    }
}

void contestExtHeap::crownWinner(){
    while(this->num_elements > 1){
        
        //1 info
        tuple<int, int> temp_1 = this->heap[1];
        int temp_1_loc = this->handle[get<0>(this->heap[1])];
        
        //num_elements info
        tuple<int, int> temp_ne = this->heap[this->num_elements];
        int temp_ne_loc = this->handle[get<0>(this->heap[this->num_elements])];
        
        //exchange info
        this->heap[this->num_elements] = temp_1;
        this->handle[get<0>(this->heap[this->num_elements])] = temp_ne_loc;
        
        this->heap[1] = temp_ne;
        this->handle[get<0>(this->heap[1])] = temp_1_loc;
        
        //remove last node
        this->handle[get<0>(this->heap[this->num_elements])] = -1;
        get<0>(this->heap[this->num_elements]) = -1;
        get<1>(this->heap[this->num_elements]) = std::numeric_limits<int>::max();
        
        this->num_elements--;
        
        /////////////////////////////////////////////////////////////////////////////
        int i = this->num_elements;
        
        while((i > 1) && (get<1>(this->heap[int(floor(i / 2))]) > get<1>(this->heap[i]))){
            
            //i info
            tuple<int, int> temp_i = this->heap[i];
            int temp_i_loc = this->handle[get<0>(this->heap[i])];
            
            //parent info
            tuple<int, int> temp_parent = this->heap[int(floor(i / 2))];
            int temp_parent_loc = this->handle[get<0>(this->heap[int(floor(i / 2))])];
            
            //exchange info
            this->heap[int(floor(i / 2))] = temp_i;
            this->handle[get<0>(this->heap[int(floor(i / 2))])] = temp_i_loc;
            
            this->heap[i] = temp_parent;
            this->handle[get<0>(this->heap[i])] = temp_parent_loc;
            
            i = int(floor(i / 2));
        }
    }
    
    cout << "Contestant <" << get<0>(this->heap[1]) << "> wins with score <" << get<1>(this->heap[1]) << ">!" << endl;

}

void contestExtHeap::showLocation(int k){
    if(this->handle[k] == -1){
        cout << "There is no Contestant <" << k << "> in the extended heap: handle[<" << k << ">] = -1." << endl;
    }
    else{
        cout << "Contestant <" << k << "> stored in extended heap location <" << this->handle[k] << ">." << endl;
    }
}

int main(int argc, char *argv[]) {
    
    //input/output files
    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);
    
    cout.rdbuf(outfile.rdbuf());
    
    //get size of heap from input file
    string line;
    getline(infile, line);
    string size = line;
    
    //initialize extended heap
    contestExtHeap * h = new contestExtHeap(stoi(size) + 1);
    
    //strings used for stringstream
    string s1;
    string s2; 
    string s3;
    
    while(getline(infile, line)){
       
        stringstream s(line);
        s >> s1 >> s2 >> s3;
        
        cout << line << endl;
        
        if(s1 == "findContestant"){
            s2.erase(remove(s2.begin(), s2.end(), '<'), s2.end());
            s2.erase(remove(s2.begin(), s2.end(), '>'), s2.end());
            h->findContestant(stoi(s2));
        }
        else if(s1 == "insertContestant"){
            s2.erase(remove(s2.begin(), s2.end(), '<'), s2.end());
            s2.erase(remove(s2.begin(), s2.end(), '>'), s2.end());
            s3.erase(remove(s3.begin(), s3.end(), '<'), s3.end());
            s3.erase(remove(s3.begin(), s3.end(), '>'), s3.end());
            h->insertContestant(stoi(s2), stoi(s3));
        }
        else if(s1 == "eliminateWeakest"){
            h->eliminateWeakest();
        }
        else if(s1 == "earnPoints"){
            s2.erase(remove(s2.begin(), s2.end(), '<'), s2.end());
            s2.erase(remove(s2.begin(), s2.end(), '>'), s2.end());
            s3.erase(remove(s3.begin(), s3.end(), '<'), s3.end());
            s3.erase(remove(s3.begin(), s3.end(), '>'), s3.end());
            h->earnPoints(stoi(s2), stoi(s3));
        }
        else if(s1 == "losePoints"){
            s2.erase(remove(s2.begin(), s2.end(), '<'), s2.end());
            s2.erase(remove(s2.begin(), s2.end(), '>'), s2.end());
            s3.erase(remove(s3.begin(), s3.end(), '<'), s3.end());
            s3.erase(remove(s3.begin(), s3.end(), '>'), s3.end());
            h->losePoints(stoi(s2), stoi(s3));
        }
        else if(s1 == "showContestants"){
            h->showContestants();
        }
        else if(s1 == "showHandles"){
            h->showHandles();
        }
        else if(s1 == "showLocation"){
            s2.erase(remove(s2.begin(), s2.end(), '<'), s2.end());
            s2.erase(remove(s2.begin(), s2.end(), '>'), s2.end());
            h->showLocation(stoi(s2));
        }
        else if(s1 == "crownWinner"){
            h->crownWinner();
        }
    }
    
    return 0;
}
