#include<bits/stdc++.h>

using namespace std;


class Piece{
    
    bool isWhite;
    string Type;
    
    public:
    
    Piece(string Type, bool isWhite):isWhite(isWhite),Type(Type){}
    
    bool getifwhite(){
        return isWhite;
    }
    
    string getType(){
        return Type;
    }
    
    ~Piece(){
        cout<<((isWhite)?"white ":"black ")<<Type<<" is dead!!!!\n";
    }
    
};


class PieceFactory{
  
  public:
  
   vector<Piece*> getPiece(int count,string color,string type){
        
        
        vector<Piece*> plist(count);
        
        for(int i=0;i<count;i++){
            plist[i] = new Piece(type,(color=="white"?true:false));
        }
        
        return plist;
    } 
  
  vector<Piece*> Lineup(string color){
        
    vector<Piece*> lpieces;
    
    vector<Piece*> h1 = getPiece(1,color,"hatti");
    vector<Piece*> g1 = getPiece(1,color,"ghoda");
    vector<Piece*> u1 = getPiece(1,color,"untha");
    vector<Piece*> ri = getPiece(1,color,"rani ");
    vector<Piece*> ra = getPiece(1,color,"raja ");
    vector<Piece*> u2 = getPiece(1,color,"untha");
    vector<Piece*> g2 = getPiece(1,color,"ghoda");
    vector<Piece*> h2 = getPiece(1,color,"hatti");
    vector<Piece*> pd = getPiece(8,color,"pyada");
    
    lpieces.insert(lpieces.end(),h1.begin(),h1.end());
    lpieces.insert(lpieces.end(),g1.begin(),g1.end());
    lpieces.insert(lpieces.end(),u1.begin(),u1.end());
    lpieces.insert(lpieces.end(),ri.begin(),ri.end());
    lpieces.insert(lpieces.end(),ra.begin(),ra.end());
    lpieces.insert(lpieces.end(),u2.begin(),u2.end());
    lpieces.insert(lpieces.end(),g2.begin(),g2.end());
    lpieces.insert(lpieces.end(),h2.begin(),h2.end());
    lpieces.insert(lpieces.end(),pd.begin(),pd.end());
    
    return lpieces;
      
  }
    
};

class Player{
  
  string Name;
  int wins,loss,draw;
  public:
  
  Player(string Name):Name(Name),wins(0),loss(0),draw(0){}
    
};


class Board{
 
  PieceFactory pf;
  int kbx,kby,kwx,kwy;
  
  public:
  map<string,vector<Piece*>> dblack,dwhite;
  vector<vector<Piece *>> grid;
  
  Board(){
      grid.resize(8,vector<Piece*>(8,nullptr));
      kbx = 7;
      kby = 4,
      kwx = 0;
      kwy = 4;
  }
  
  Piece * getmePiece(int x, int y){
      return grid[x][y];
  }
  
  void setup(){
      
    vector<Piece*> whitelineup = pf.Lineup("white");
     
    vector<Piece*> blacklineup = pf.Lineup("black");
    
    for(int i=0;i<2;i++){
        for(int j=0;j<8;j++){
            grid[i][j] = whitelineup[(i*8)+j];
        }
    }
    
    for(int i=7;i>5;i--){
        for(int j=0;j<8;j++){
            grid[i][j] = blacklineup[((7-i)*8)+j];
        }
    }
    
    cout<<"Board setup is done..Ready to play!\n";
      
  }
  
  void printboard(){
      cout<<"----a--------b--------c--------d--------e--------f--------g--------h----\n";
      for(int i=0;i<8;i++){
          cout<<"\n------------------------------------------------------------------------\n";
          for(int j=0;j<8;j++){
              
              if(grid[i][j]){
                  cout<<"|"<<((grid[i][j]->getifwhite())?"W ":"B ")<<grid[i][j]->getType()<<"|";
              }
              else{
                  cout<<"|"<<"       "<<"|";
              }
              
          }
          cout<<(char)('8'-(char)(i));
      }
      cout<<"\n------------------------------------------------------------------------\n";
      cout<<"\n----a--------b--------c--------d--------e--------f--------g--------h----\n";
  }
  
  ~Board(){
      
      for(int i=0;i<8;i++){
          for(int j=0;j<8;j++){
              if(grid[i][j]){
                  delete grid[i][j];
              }
          }
      }
      
      for(auto x: dblack){
          delete x.second;
      }
      
      for(auto x: dwhite){
          delete x.second;
      }
      
      cout<<"Board is cleaned up!\n";
  }
  
};

class ChessGame{
  
  Player * p1, * p2;
  string p1color,p2color;
  Board chessboard;
  
  
  public:
  
  ChessGame(Player * p1, Player * p2,string p1color){
      
      
      this->p1 = p1;
      this->p2 = p2;
      
      this->p1color = p1color;
      this->p2color = (p1color=="white")?"black":"white";
      
      chessboard.setup();
  }
    
    bool checkifvalid(string color, int ix, int iy,int dx, int dy){
        
        if(ix<0 ||ix>7 || iy<0 || iy>7 || dx<0 ||dx>7 || dy<0 || dy>7) return false;
        if(ix==dx && iy==dy) return false;
        
        Piece * pc = chessboard.grid[ix][iy];
        
        if((pc->getifwhite() && color=="black") || !pc->getifwhite() && color=="white"){
            cout<<"Play with your pieces\n";
            return false;
        }
        
        if(pc->Type=="hatti"){
            
            if(!(ix==dx || iy==dy))return false;
            
            for(int i=ix;i<=dx;i++){
                for(int j=iy;j<=dy;j++){
                    if((i==ix && j==iy) || (i==dx && j==dy)) continue;
                    if(chessboard.getmePiece(i,j)!=nullptr) return false;
                }
            }
            
            return true;
            
        }
        else if(pc->Type=="ghoda"){
            if(abs(dx-ix)!=0 && abs(iy-dy)!=0 && abs(dx-ix)+abs(iy-dy)==3) return true;
            return false;
        }
        else if(pc->Type=="untha"){
            if(abs(dx-ix)!=abs(iy-dy)) return false;
            
            while(ix!=dx && iy!=dy){
                
                if(ix==dx && iy==dy){
                    ix = (ix<dx?ix++:ix--);
                    iy = (iy<dy?iy++:iy--);  
                    continue;
                }
                if(chessboard.getmePiece(ix,iy)!=nullptr){
                    return false;
                }
                ix = (ix<dx?ix++:ix--);
                iy = (iy<dy?iy++:iy--);  
            }
            return true;
        }
        else if(pc->Type=="raja"){
            
            if((ix-1>=0 && ix-1==dx && iy==dy)  || (iy-1>=0 && ix==dx && iy-1==dy) || (ix+1<8 && ix+1==dx && iy==dy) || (iy+1<8 && ix==dx && iy+1==dy) ) return true;
            if((ix-1>=0 && iy-1>=0 && ix-1==dx && iy-1==dy)  || (ix+1<8 && iy-1>=0 && ix+1<8 && iy-1==dy) || (ix+1<8 && iy+1<8 && ix+1==dx && iy+1==dy) || (ix-1>=0 && iy+1<8 && ix-1==dx && iy+1==dy) ) return true;
            
            return false;
            
        }
        else if(pc->Type=="rani"){
            
            if(ix==dx || iy==dy){
                for(int i=ix;i<=dx;i++){
                    for(int j=iy;j<=dy;j++){
                        if((i==ix && j==iy) || (i==dx && j==dy)) continue;
                        if(chessboard.getmePiece(i,j)!=nullptr) return false;
                    }
                }
                return true;
            }
            
            else if(abs(dx-ix)==abs(iy-dy)){
                while(ix!=dx && iy!=dy){
                
                    if(ix==dx && iy==dy){
                        ix = (ix<dx?ix++:ix--);
                        iy = (iy<dy?iy++:iy--);  
                        continue;
                    }
                    if(chessboard.getmePiece(ix,iy)!=nullptr){
                        return false;
                    }
                    ix = (ix<dx?ix++:ix--);
                    iy = (iy<dy?iy++:iy--);  
                }
                return true;
            }
            
            
            return false;
            
        }
        else{
            
            if(iy!=dy) return false;
            
            if(color=="white"){
                
                if(ix==1){
                    if((dx-ix==1 || dx-ix==2) && chessboard.getmePiece(dx,dy)==nullptr)return true;
                }
                if((ix+1<8 && iy-1>=0 && ix+1==dx && iy-1==dy && chessboard.getmePiece(dx,dy)!=nullptr)  || (ix+1<8 && iy+1<8 && ix+1==dx && iy+1==dy && chessboard.getmePiece(dx,dy)!=nullptr)) return true;
                return false;
                
            }
            else{
                
                if(ix==6){
                    if((ix-dx==1 || ix-dx==2) && chessboard.getmePiece(dx,dy)==nullptr)return true;
                }
                if((ix-1>=0 && iy-1>=0 && ix-1==dx && iy-1==dy && chessboard.getmePiece(dx,dy)!=nullptr)  || (ix-1>=0 && iy+1<8 && ix-1==dx && iy+1==dy && chessboard.getmePiece(dx,dy)!=nullptr))return true;
                return false;
            }
            
        }
        
        return false;
        
    }
    
    void revivemyplayer(int x, Piece * p){
        
        string pname;
        
        if(color=="white"){
            if(x==7 && p->getType=="pyada"){
                
                cout<<"Which piece do you want to review\n";
                cin>>pname;
                cout<<""
                auto fd = dwhite[pname];
                
                if(fd!=dwhite.end()){
                    
                }
                
            }
        }
        else{
            if(x==0){
                
            }
        }
        
        
    }
    
  
    void play(){
      
      chessboard.setup();
      
      bool ifp1chance = true,draw = true;
      
      char irow,icol,drow,dcol;
      
      int ix,iy,dx,dy;
      
      string curplayercolor;
      
      while(true){
          
          printChess();
          
          curplayercolor = (ifp1chance?p1color:p2color);
          
          cout<<(ifp1chance?"Player 1 play your next move\n":"Player 2 play your next move\n");
          
          cin>>irow>>icol>>drow>>dcol;
          
          ix = irow - '1',iy = icol - 'a';
          
          dx = drow - '1',dy = dcol - 'a';
          
          bool valid = checkifvalid(curplayercolor,ix,iy,dx,dy);
          
          if(!valid){
              cout<<"Please play a valid move!\n";
              continue;
          }
          Piece * p = chessboard.grid[dx][dy];
          Piece * m = chessboard.grid[ix][iy]; 
          if(chessboard.grid[dx][dy]!=nullptr){
              if((p.getifwhite() && curplayercolor=="white") || (!p.getifwhite() && curplayercolor=="black"){
                  cout<<"Please play a valid move! you cannot kill your own piece\n";
                  continue;
              }
              
              if(curplayercolor=="black"){
                  chessboard.dblack[m->getType()].push_back(m);
              }
              else{
                  chessboard.dwhite[m->getType()].push_back(m);
              }
          }
          
          chessboard.grid[dx][dy] = chessboard.grid[ix][iy];
          chessboard.grid[ix][iy] = nullptr;
          
          revivemyplayer(dx,dy,m);
          
          bool ifcheckmate = checkifmate((ifp1chance?p2color:p1color));
          
          if(ifcheckmate){
              cout<<"Check Mate!\n";
              draw = false;
              if(ifp1chance){
                  p1->win++;
                  p2->loss--;
              }
              else{
                  p2->win++;
                  p1->loss--;
              }
          }
          
      }
      
      if(draw){
          cout<<"Match drawn!\n";
          p1->draw++;
          p2->draw++;
      }
      
  }
  
  void printChess(){
      
      chessboard.printboard();
      
  }
  
  ~ChessGame(){
      p1 = nullptr;
      p2 = nullptr;
  }
    
    
};



int main()
{
    Player p1("Niwant");
    Player p2("Yashodhan");
    
    ChessGame game(&p1,&p2,"white");
    
    
    

    return 0;
}
