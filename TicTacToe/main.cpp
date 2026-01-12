#include<bits/stdc++.h>
using namespace std;


class TicTacToe{
    
    vector<vector<int>>grid;
    bool symbol;
    int moves;
    
    public:
    
    TicTacToe(bool symbol){
        grid.resize(3,vector<int>(3,-1));
        this->symbol = symbol;
        moves = 0;
    }
    
    void play(){
        int x, y;
        bool currentplayer = symbol;
        display();
        while(moves<9){
            
            cout<<"Player "<<(currentplayer==symbol?1:2) <<" you position x and y : ";
            
            cin>>x>>y;
            
            if(x<=0 || x>3 || y<=0 || y>3 || grid[x-1][y-1]!=-1){
                cout<<"please choose unused and inbound position\n";
                continue;
            }
            
            grid[x-1][y-1] = currentplayer;
            
            display();
            
            if(checkifwon(x-1,y-1)){
                cout<<"Player "<<(currentplayer==symbol?1:2)<<" won!\n";
                return;
            }
            currentplayer = (currentplayer==0?1:0);
            moves++;
            cout<<moves<<endl;
        }
        if(moves==9){
            cout<<"Match draw!";
        }
    }
    
    bool checkifwon(int x, int y){
        
        bool win1 = true, win2 = true, win3 = true, win4=true;
        
        for(int i = 0;i<3;i++){
            if(grid[x][i]!=grid[x][y]){
                win1 = false;
            }
            if(grid[i][y]!=grid[x][y]){
                win2 = false;
            }
            if(grid[i][i]!=grid[x][y]){
                win3 = false;
            }
            if(grid[i][2-i]!=grid[x][y]){
                win4 = false;
            }
            
        }
        
        if(win1||win2||win3||win4) return true;
        
        return false;
    }
    
    void display(){
        
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                
                if(grid[i][j]==0){
                    cout<<"| 0 |";
                }
                else if(grid[i][j]==1){
                    cout<<"| X |";
                }
                else{
                    cout<<"|   |";
                }
            }
            cout<<endl;
        }
        
    }
};


int main()
{
    
    cout<<"LET'S START THE GAME!\n";
    
    cout<<"First player choose the symbol '0' or 'X' :";
    
    char symbol = '*';
    
    cin>>symbol;
    
    while(symbol!='0' && symbol!='X'){
        cout<<"Choose correct symbol\n";
        cin>>symbol;
    }
    
    TicTacToe game((symbol=='0'?0:1));
    
    game.play();
    
    return 0;
}
