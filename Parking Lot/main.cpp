#include<bits/stdc++.h>
#include<chrono>
using namespace std;


enum Size{
    small,medium,large
};


class Vehicle{
    
    protected:
    
    string NumberPlate;
    
    public:
    
    Vehicle(string NumberPlate){
        this->NumberPlate = NumberPlate;
    }
    
    virtual Size getsize()=0;
    
};


class MotorCycle : public Vehicle{
  
    public:
    
    MotorCycle(string NumberPlate):Vehicle(NumberPlate){
        cout<<"MotorCycle is Created\n";
    }
    
    Size getsize(){
        return Size::small;
    }
};

class Car : public Vehicle{
  
    public:
    
    Car(string NumberPlate):Vehicle(NumberPlate){
        cout<<"Car is Created\n";
    }
    
    Size getsize(){
        return Size::medium;
    }
};

class Truck : public Vehicle{
  
    public:
    
    Truck(string NumberPlate):Vehicle(NumberPlate){
        cout<<"Truck is Created\n";
    }
    
    Size getsize(){
        return Size::large;
    }
};



class ParkingSpot{
  
  protected:
  unsigned int spotnumber;
  Vehicle * vh;
  
  public:
  
  ParkingSpot(unsigned int spotnumber,Vehicle * vh){
      this->spotnumber = spotnumber;
      this->vh = vh;
  }
  
   virtual Size getsize()=0; 
  
   bool ifavailable(){
        return vh!=nullptr;
    }
    
    void setVehicle(Vehicle * vh){
        this->vh = vh;
    }
    ~ParkingSpot(){
        vh = nullptr;
    }
        
};

class CompactSpot : public ParkingSpot{
  
    public:
    
    CompactSpot(unsigned int spotnumber,Vehicle * vh):ParkingSpot(spotnumber,vh){}
    
    Size getsize(){
        return Size::small;
    }
};

class RegularSpot : public ParkingSpot{
  
    public:
    
    RegularSpot(unsigned int spotnumber,Vehicle * vh):ParkingSpot(spotnumber,vh){}
    
    Size getsize(){
        return Size::medium;
    }
    
};

class OversizedSpot : public ParkingSpot{
  
    public:
    
    OversizedSpot(unsigned int spotnumber,Vehicle * vh):ParkingSpot(spotnumber,vh){}
    
    Size getsize(){
        return Size::large;
    }
};


class SpotFactory{
  
  static int smallSpotid,mediumSpotid,largeSpotid;
  
  public:
  
  SpotFactory(){}
  
  ParkingSpot * getSpot(Size size){
      
      ParkingSpot * ps;
      
      if(size==Size::small){
          ps = new  CompactSpot(smallSpotid++,nullptr);
      }
      else if(size==Size::medium){
          ps = new  RegularSpot(mediumSpotid++,nullptr);
      }
      else{
          ps = new  OversizedSpot(largeSpotid++,nullptr);
      }
      
      return ps;
      
  }
    
};

int SpotFactory::smallSpotid = 0;
int SpotFactory::mediumSpotid = 0;
int SpotFactory::largeSpotid = 0;


class Ticket{
  
    static int counter;
    
    int ticketid; 
    
    Vehicle * vh;
    
    ParkingSpot * ps;
    
    std::chrono::system_clock::time_point inTime;
    
    public:
    
    Ticket(Vehicle * vh, ParkingSpot *ps){
        ticketid = counter++;
        this->vh = vh;
        this->ps = ps;
        inTime = std::chrono::system_clock::now();
    }
    
    std::chrono::system_clock::time_point getInTime(){
        return inTime;
    }
    
    ParkingSpot * getSpot(){
        return ps;
    }
    
    
    ~Ticket(){
        vh = nullptr;
        ps = nullptr;
    }
    
};

int Ticket::counter = 0;

class Bill{
    
  int amount;
  std::chrono::system_clock::time_point outTime;
  
  public:
  
  Bill(int amount ,std::chrono::system_clock::time_point outTime){
      this->amount = amount;
      this->outTime = outTime;
  }
  
  int getAmount(){
      return amount;
  }
    
};



class ParkingManager{
    
  SpotFactory sf;
  
  set<ParkingSpot*> availablesmallspots,availablemediumspots,availablelargespots;
  
  set<ParkingSpot*> unavailablesmallspots,unavailablemediumspots,unavailablelargespots;
  
  public:
  
  ParkingManager(int s,int m, int l){
      
      for(int i=0;i<s;i++){
          availablesmallspots.insert(sf.getSpot(Size::small));
      }
      
      for(int i=0;i<m;i++){
          availablemediumspots.insert(sf.getSpot(Size::medium));
      }
      
      for(int i=0;i<l;i++){
          availablelargespots.insert(sf.getSpot(Size::large));
      }
      
  }
  
  private:
  
  Ticket * park_smallSpots(Vehicle * vh){
     if(availablesmallspots.size()>0){
              
              auto spot = *availablesmallspots.begin();
              
              availablesmallspots.erase(spot);
              
              unavailablesmallspots.insert(spot);
              
              spot->setVehicle(vh);
              
              Ticket * tk =  new Ticket(vh,spot);
              
              return tk;
              
        } 
        return nullptr;
  }
  
  Ticket * park_mediumSpots(Vehicle * vh){
      if(availablemediumspots.size()>0){
              
              auto spot = *availablemediumspots.begin();
              
              availablemediumspots.erase(spot);
              
              unavailablemediumspots.insert(spot);
              
              spot->setVehicle(vh);
              
              Ticket * tk =  new Ticket(vh,spot);
              
              return tk;
              
        }
        return nullptr;
          
  }
  
  Ticket * park_largeSpots(Vehicle * vh){
      if(availablelargespots.size()>0){
              
              auto spot = *availablelargespots.begin();
              
              availablelargespots.erase(spot);
              
              unavailablelargespots.insert(spot);
              
              spot->setVehicle(vh);
              
              Ticket * tk =  new Ticket(vh,spot);
              
              return tk;
              
          }
          return nullptr;
  }
  
  public:
  
  Ticket * findParking(Vehicle * vh){
      
      if(vh->getsize()==Size::small){
          
          Ticket * t;
          
          t = park_smallSpots(vh);
          
          if(t) return t;
          
          t = park_mediumSpots(vh);
          
          if(t) return t;
          
           t = park_largeSpots(vh);
          
          if(t) return t;
          
          cout<<"Sorry parking is full\n";
          return NULL;
          
      }
      else if(vh->getsize()==Size::medium){
          Ticket * t;
          
          t = park_mediumSpots(vh);
          
          if(t) return t;
          
          t = park_largeSpots(vh);
          
          if(t) return t;
          cout<<"Sorry parking is full\n";
          return NULL;
      }
      else{
          Ticket * t;
           t = park_largeSpots(vh);
          
          if(t) return t;
          
          cout<<"Sorry parking is full\n";
          return NULL;
      }
      
  }
  
  
  
  void vacateParking(Ticket * tk){
      
      auto spot = tk->getSpot();
      
      spot->setVehicle(nullptr);
      
      if(spot->getsize()==Size::small){
          availablesmallspots.insert(spot);
          unavailablesmallspots.erase(spot);
      }
      else if(spot->getsize()==Size::medium){
          
          availablemediumspots.insert(spot);
          unavailablemediumspots.erase(spot);
      }
      else{
          availablelargespots.insert(spot);
          unavailablelargespots.erase(spot);
      }
      
  }
  
  ~ParkingManager(){
      
      for(auto x: availablesmallspots){
          delete x;
      }
      
      for(auto x: availablemediumspots){
          delete x;
      }
      
      for(auto x: availablelargespots){
          delete x;
      }
      
      for(auto x: unavailablesmallspots){
          delete x;
      }
      
      for(auto x: unavailablemediumspots){
          delete x;
      }
      
      for(auto x: unavailablelargespots){
          delete x;
      }
      
  }
    
};


class BaseFareCalculator{
  
    public:
    
    Bill CalculatorBill(Ticket * tk){
        
      int costperhour=0;
     
      
      if(tk->getSpot()->getsize()==Size::small){
          costperhour = 100;
      }
      else if(tk->getSpot()->getsize()==Size::medium){
          costperhour = 200;
      }
      else if(tk->getSpot()->getsize()==Size::large){
          costperhour = 300;
      }
      
      auto outTime = std::chrono::system_clock::now();
      
      auto ttlminutes = std::chrono::duration_cast<std::chrono::minutes>(outTime - tk->getInTime()).count();
      
      int amount = ttlminutes * costperhour;
      
      Bill b(amount,outTime);
      
      return b;
      
  }
    
};


class ParkingLot{
  
    ParkingManager * pm;
    BaseFareCalculator * bfc;
    
    ParkingLot(int s,int m, int l){
        
        pm = new ParkingManager(s,m,l);
        
        bfc = new BaseFareCalculator();
        
    }
    
    ParkingLot(ParkingLot & pk) = delete;
    ParkingLot(ParkingLot && pk) = delete;
    
    void operator =(const ParkingLot & pk) = delete;
    void operator =(ParkingLot && pk) = delete;
    
    public:
    
    static ParkingLot* GetParkingInstance(int s,int m, int l){
        static ParkingLot pl(s,m,l);
        return &pl;
    }
    
    Ticket* park(Vehicle * vh){
        return pm->findParking(vh);
    }
    
    Bill vacate(Ticket * tk){
        
        Bill b = bfc->CalculatorBill(tk);
        
        pm->vacateParking(tk);
        
        return b;
        
    }
    
    ~ParkingLot(){
        delete pm;
        delete bfc;
    }
    
};


int main(){
    
    ParkingLot * p1 = ParkingLot::GetParkingInstance(2,3,4);
    
    Vehicle * v1 = new Car("sdsd7");
    
    Ticket * t = p1->park(v1);
    
    if(!t){
        delete v1;
        delete t;
        return 0;
    }
    
    //std::this_thread::sleep_for(std::chrono::minutes());
    
    Bill b = p1->vacate(t);
    
    cout<<b.getAmount();
    
    delete v1;
    
    delete t;
}
