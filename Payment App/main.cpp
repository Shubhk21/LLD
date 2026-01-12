#include<bits/stdc++.h>

using namespace std;

class Transaction{
    private:
    string cardType,cardNumber,cardName;
    int amount, balanceBefore,balanceAfter;
    
    public:
    
    Transaction(string cardType, string cardNumber,string cardName, int amount, int balanceBefore,int balanceAfter){
        this->cardType = cardType;
        this->cardNumber = cardNumber;
        this->cardName = cardName;
        this->amount = amount;
        this->balanceAfter = balanceAfter;
        this->balanceBefore = balanceBefore;
    }
    
    void printTransaction(){
        cout<<cardType<<" "<<cardNumber<<" "<<cardName<<" "<<amount<<" "<<balanceBefore<<" "<<balanceAfter<<endl;
    }
    
};

class History{
    private:
    deque<Transaction *> lastntransactions;
    
    public:
    
    void showHistory(){
        
        if(lastntransactions.size()==0){
            cout<<"You have not made any transaction yet\n";
            return;
        }
        
            
        for(auto x: lastntransactions){
            x->printTransaction();
        }
        
    }
    
    void addtoHistory(Transaction * t){
        
        if(lastntransactions.size()==5){
            lastntransactions.pop_front();
        }
        lastntransactions.push_back(t);
        
    }
    
};

class Card{
    
    protected:
    
    string cardNumber,cardName;
    int balance;
    
    public:
    
    Card(string cardNumber,string cardName, int initialbalance):cardNumber(cardNumber),cardName(cardName),balance(initialbalance){}
    
    virtual Transaction * pay(int amount, string purpose) = 0;
    
    virtual int getBalance() = 0;
    
    void showDetails(){
        cout<<cardName<<" "<<cardNumber<<" "<<balance<<endl;
    }
    
    
};


class DebitCard: public Card{
    
    public:
    
    DebitCard(string cardNumber,string cardName, int initialbalance):Card(cardNumber,cardName,initialbalance){
        cout<<"Debit card successfully added to your wallet\n";
    }
    
    Transaction * pay(int amount, string purpose){
        
        if(amount>balance){
            cout<<"Transaction failed due to unsufficient balance!\n";
            return nullptr;
        }
        
        Transaction * t = new Transaction("Debit",cardNumber,cardName,amount,balance,balance-amount);
        balance -= amount;
        cout<<"You have successfully made the transaction\n";
        return t;
    }
    
    int getBalance(){
        return balance;
    }
    
};


class CreditCard: public Card{
  
    public:
    
    CreditCard(string cardNumber,string cardName, int initialbalance):Card(cardNumber,cardName,initialbalance){
        cout<<"Credit card successfully added to your wallet\n";
    }
    
    Transaction * pay(int amount, string purpose){
        
        if(amount>balance){
            cout<<"Transaction failed due to unsufficient balance!\n";
            return nullptr;
        }
        
        Transaction * t = new Transaction("Credit",cardNumber,cardName,amount,balance,balance-amount);
        balance -= amount;
        cout<<"You have successfully made the transaction\n";
        return t;
    }
    
    int getBalance(){
        return balance;
    }
    
    
};


class PaymentApp{

    private:
    History h;
    map<string,Card*> creditcards,debitcards;
    
    public:

    
    void addPaymentMethods(string type, string name,string number, int intbalance){
        
        if(type=="Credit"){
            
            if(creditcards.size()==5){
                cout<<"You can add only 5 Credit cards max\n";
                return;
            }
            auto fd = creditcards.find(name);
            
            if(fd!=creditcards.end()){
                cout<<"Cannot add same credit card\n";
                return;
            }
            
            Card * cd = new CreditCard(number,name,intbalance);
            
            creditcards[name] = cd;
            
        }
        else if(type=="Debit"){
            
            if(debitcards.size()==5){
                cout<<"You can add only 5 Debit cards max\n";
                return;
            }
            auto fd = debitcards.find(name);
            
            if(fd!=debitcards.end()){
                cout<<"Cannot add same debit card\n";
                return;
            }
            
            Card * cd = new DebitCard(number,name,intbalance);
            
            debitcards[name] = cd;

        }
        else{
            cout<<"Incorrect card type.. card not saved!\n";
        }
        
    }
    
    void removeCard(string type, string name){
        
        if(type=="Credit"){
            
            auto fd = creditcards.find(name);
            
            if(fd!=creditcards.end()){
                creditcards.erase(name);
                cout<<"Card removed successfully from wallet\n";
            }
            else{
                cout<<"No such card exists\n";
            }
            
        }
        else if(type=="Debit"){
            auto fd = debitcards.find(name);
            
            if(fd!=debitcards.end()){
                debitcards.erase(name);
                cout<<"Card removed successfully from wallet\n";
            }
            else{
                cout<<"No such card exists\n";
            }
        }
        else{
            cout<<"Incorrect card type\n";
        }
        
    }
    
    void showAllCards(){
        
        cout<<"Credit Cards: \n";
        for(auto x: creditcards){
            x.second->showDetails();
        }
        
        cout<<"Debit Cards: \n";
        for(auto x: debitcards){
            x.second->showDetails();
        }
        
    }
    
    void showHistory(){
        h.showHistory();
    }
    
    
    void makePayment(string type, string name, int amount, string purpose){
        
        if(type=="Credit"){
            
            auto fd = creditcards.find(name);
            
            if(fd!=creditcards.end()){
                Transaction * t = fd->second->pay(amount,purpose);
                
                if(t){
                    h.addtoHistory(t);
                }
                else{
                    
                }
            }
            else{
                cout<<"No such card exists\n";
            }
            
        }
        else if(type=="Debit"){
            
            auto fd = debitcards.find(name);
            
            if(fd!=debitcards.end()){
                Transaction * t = fd->second->pay(amount,purpose);
                
                if(t){
                    h.addtoHistory(t);
                }
                else{
                    
                }
            }
            else{
                cout<<"No such card exists\n";
            }
            
        }
        else{
            cout<<"Incorrect card type\n";
        }
        
    }
    
};


int main(){
    
    PaymentApp app;
    
    app.addPaymentMethods("Credit","Chase","1234556",100);
    app.addPaymentMethods("Credit","Truist","1234556",100);
    app.addPaymentMethods("Credit","Regions","1234556",100);
    app.addPaymentMethods("Credit","BOFA","1234556",100);
    app.addPaymentMethods("Credit","Discover","1234556",100);
    app.removeCard("Credit","Regions");
    app.showHistory();
    
    app.makePayment("Credit","Chase",23,"Walmart");
    app.makePayment("Credit","Chase",23,"Walmart");
    app.makePayment("Credit","BOFA",40,"Walmart");
    app.makePayment("Credit","Discover",120,"Walmart");
    
    app.showHistory();
    
    app.showAllCards();
    
}
