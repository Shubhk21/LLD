#include<bits/stdc++.h>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;


class Pod{
    
    public:
    
    int floor_number;
    Pod * next;
    
    Pod(int f){
        floor_number = f;
    }
    
};


class Button{
  
  bool is_on;
  
  public:
  
  Button(){
      is_on = false;
  }
  
  bool getifOn(){
      return is_on;
  }
  
  void setOn(){
      is_on = true;
  }
  
  void setOf(){
      is_on = false;
  }
    
};


class Person{
  
  unsigned int weight;
  
  unsigned int destination_floor;
  
  public:
  
  Person(unsigned int w, unsigned int df):weight(w),destination_floor(df){
      cout<<"Person created\n";
  }
  
  int getWeight(){
      return weight;
  }
  
  int getDestFloor(){
      return destination_floor;
  }
  
  bool operator<(const Person& other) const {
        return weight < other.weight;   
    }
    
};



class State{
  
  bool if_working;
  
  bool if_ideal;
  
  bool if_up;
  
  public:
  
  State(){
      if_working = true;
      if_ideal = true;
  }
  
  void setWorking(bool of){
      if_working = of;
  }
  
  void setUp(){
      if_up = true;
  }
  
  void setDown(){
      if_up = false;
  }
    
};


class Lift{
  
  int id;
  int capacity;
  int current_weight;
  int number_of_people;
  int current_floor;
  State * s;
  
  
  public:
  
  vector<Button> buttons;
  set<Person> people;
  map<int,vector<Person>> tracker;
  
  Lift(int floors,int capacity, int id){
      
    for(int f=0;f<floors;f++){
        Button b;
        buttons.push_back(b);
    }      
    
    current_floor = 0;
    
    this->id = id;
    
    this->capacity = capacity;
    
    current_weight = 0;
    
    number_of_people = 0;
    
    s = new State();
      
  }
  
  ~Lift(){
      s = nullptr;
  }
   
   int getCurrentFloor(){
       return current_floor;
   }
   
   void setCurrentFloor(int f){
       current_floor = f;
   }
   
   int getId(){
       return id;
   }
   
   int getCurrentWeight(){
       return current_weight;
   }
   
   void increaseWeight(int w){
       current_weight += w;
   }
   
   void decreaseWeight(int w){
       current_weight -= w;
   }
   
   int getCapacity(){
       return capacity;
   }
   
   void increasePeople(){
       number_of_people++;
   }
   
   void decreasePeople(){
       number_of_people--;
   }
    
};


class Request{
  
  int start_floor;  
  int liftid;
  bool if_up;
  
  public:
  
  Request(int start_floor, bool if_up,int li):start_floor(start_floor),if_up(if_up),liftid(li){}
  
  int getStartFloor(){
      return start_floor;
  }
    
  bool isUp(){
      return if_up;
  }
    
  int getLiftId(){
      return liftid;
  }
    
};


class LiftManagement{
    
    
  vector<queue<Request>> requests;
  vector<mutex> mutexes;    
  vector<condition_variable> cvs;
  public:
  
  LiftManagement(int lifts):mutexes(lifts),cvs(lifts),requests(lifts){}
  
  
  void insertRequest(Request r){
      
        int lid = r.getLiftId();
    
        mutexes[lid].lock();
        
        requests[lid].push(r);
        
        cvs[lid].notify_one();
         
        mutexes[lid].unlock();
        
        this_thread::sleep_for(chrono::seconds(1));
      
  }
  
  
  void processRequest(Lift l){
      int start, dest;
      int lid = l.getId();
      mutex & mq = mutexes[lid];
      condition_variable &cv = cvs[lid];
      bool goup;
      
      while(true){
          {
              std::unique_lock<std::mutex> lock(mq);
          
              cv.wait(lock,[&]{return (requests[lid].size()>0?true:false);});
              
              start = requests[lid].front().getStartFloor();
              
              goup = requests[lid].front().isUp();
              
              requests[lid].pop();
          }
          
          
          dest = (goup?0:INT_MAX);
          
          
          while(l.getCurrentFloor()!=start){
              
              if(l.getCurrentFloor()<start){
                  l.setCurrentFloor((l.getCurrentFloor())+1);
              }
              else{
                  l.setCurrentFloor((l.getCurrentFloor())-1);
              }
              
              this_thread::sleep_for(chrono::seconds(1));
              cout<<"Lift is at floor: "<<l.getCurrentFloor()<<endl;
              
          }
          
          cout<<"Lift "<<l.getId()<<"doors opened: Please get inside the lift\n";
          
          
          vector<Person> getinside;
          int ppl,w,d,ttlw=0;
          
          cout<<"How many people are getting inside the left?\n";
          
          cin>>ppl;
          
          for(int i=0;i<ppl;i++){
              
              cout<<"Enter the destination of "<<i<<"th person\n";
              cin>>d;
              cout<<"Enter the weight of "<<i<<"th person\n";
              cin>>w;
              
              Person p(w,d);
              
              if(l.getCurrentWeight()+w<=l.getCapacity()){
                  l.increaseWeight(w);
                  l.buttons[d].setOn();
                  l.people.insert(p);
                  l.tracker[d].push_back(p);
                  if(goup){
                      dest = max(dest,d);
                  }
                  else{
                      dest = min(dest,d);
                  }
              }
              else{
                  cout<<"Sorry we cannot take more people inside ..lift is full\n";
                  break;
              }
              this_thread::sleep_for(chrono::seconds(1));
              
          }
          
          while(l.getCurrentFloor()!=dest){
              
              cout<<"Lift is at floor: "<<l.getCurrentFloor()<<endl;
              if(l.getCurrentFloor()<dest){
                  l.setCurrentFloor((l.getCurrentFloor())+1);
              }
              else{
                  l.setCurrentFloor((l.getCurrentFloor())-1);
              }
              
              if(l.buttons[l.getCurrentFloor()].getifOn()){
                  
                  cout<<"Doors open..please get out of lift! Dont enter inside\n";
                  int outppl = l.tracker[l.getCurrentFloor()].size();
                  this_thread::sleep_for(chrono::seconds(outppl));
                  for(auto x: l.tracker[l.getCurrentFloor()]){
                      
                      l.people.erase(x);
                      l.decreaseWeight(x.getWeight());
                      
                      
                  }
                  cout<<outppl<<" people exited!\n";
                  l.tracker.erase(l.getCurrentFloor());
                  l.buttons[l.getCurrentFloor()].setOf();
              }
              
              this_thread::sleep_for(chrono::seconds(1));
              
          }
          
          
      }
      
      
      
  }
    
    
};



class Building{
  
  
  int floors;
  int no_of_lifts;
  int lift_capacity;
  vector<Lift> lifts;
  LiftManagement lm;
  public:
  
  Building(int floors, int no_of_lifts, int lift_capacity):floors(floors),no_of_lifts(no_of_lifts),lift_capacity(lift_capacity),lm(no_of_lifts){
      
      for(int l=0;l<no_of_lifts;l++){
          
          Lift lf(floors,lift_capacity,l);
          
          lifts.push_back(lf);
          
          thread t(&LiftManagement::processRequest, &lm,lf);
          
          t.detach();
          
      }
      
      
  }
  
  void processRequest(Request r){
      
      int liftid = r.getLiftId();
      
      lm.insertRequest(r);
      
      cout<<"Lift Requested\n";
      
  }
    
    
};




int main(){
    
    Building b(20,2,500);
    
    Request r1(10,false,0);
    Request r2(17,false,0);
    
    
    b.processRequest(r1);
    b.processRequest(r2);
    
    
    this_thread::sleep_for(chrono::seconds(50));
    
}

