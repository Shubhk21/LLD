#include<bits/stdc++.h>

using namespace std;


class State{
    public:
  
    virtual void write(string data)=0;
    
    virtual void read()=0;
    
    virtual void undo()=0;
    
    virtual void redo()=0;
    
    virtual void close()=0;
};

class Editor : public State{
 
 private:
    stack<string> writedata;
    stack<string> undodata;
    string text;
    
    
public:
    Editor(){
        cout<<"File opened!\n";
    }
    
    void write(string data){
        
        text += data;
        
        writedata.push(text);
        
        while(!undodata.empty()){
            undodata.pop();
        }
    }
    
    void read(){
        cout<<text<<endl;
    }
    
    void undo(){
        
        if(!writedata.empty()){
            undodata.push(writedata.top());
            writedata.pop();
            if(!writedata.empty()){
                text = writedata.top();
            }
            else{
                text = "";
            }
        }
    }
    
    void redo(){
        if(!undodata.empty()){
            writedata.push(undodata.top());
            undodata.pop();
            text = writedata.top();
        }
    }
    
    void close(){
        while(!undodata.empty()){
            undodata.pop();
        }
    }
    
    ~Editor(){
        cout<<"File closed!\n";
    }
    
};

int main(){
    
    
        
    Editor doc;
    
    doc.write("a");
    doc.write(" b");
    doc.undo();
    doc.write(" b");
    doc.write(" c");
    
    doc.undo();
    doc.undo();
    
    doc.write(" d");
    doc.redo();
    doc.undo();
    
    doc.redo();
    doc.read();
    
    doc.write(" e");
    doc.write(" f");
    
    doc.undo();
    doc.undo();
    doc.undo();
    doc.undo();
    doc.read();
    
    doc.redo();
    doc.redo();
    doc.redo();
    doc.redo();
    doc.redo();
    doc.redo();
    
    doc.read();
    
    return 0;
}
