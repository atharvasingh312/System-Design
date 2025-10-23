#include<bits/stdc++.h>
using namespace std;

class Entities{
    private:
    static Entities* entptr;
    Entities(){
    }
    public:
    unordered_map<int,int> snake;
    unordered_map<int,int> ladder;
    unordered_map<string,int> player;
    static Entities* getEntity(){
        //for creating singleton
        if(entptr==NULL) entptr=new Entities();
        return entptr;
    }
    unordered_map<int,int> getsnake(){
        return snake;
    }
    void setsnake(int start,int end){
        snake[start]=end;
    }
    unordered_map<int,int> getladder(){
        return ladder;
    }
    void setladder(int start,int end){
        ladder[start]=end;
    }
    unordered_map<string,int> getplayer(){
        return player;
    }
    void setplayer(string name,int id){
        player[name]=id;
    }
};
Entities* Entities::entptr=NULL;
class Dice{
    public:
    int mxval,mnval;
    Dice(){
        mxval=6;
        mnval=1;
    }
    int getnumber(){
        return (rand()%mxval)+mnval;
    }
};

class Player{
    public:
    map<int,int> playerpos;
    void inititateplayer(int id){
        playerpos[id]=0;
    }
    int getpos(int id){
        return playerpos[id];
    }
    void setpos(int id,int end){
        playerpos[id]=end;
    }
};

class Board{
    public:
    string startgame(){
        Entities* instance=Entities::getEntity();
        Player playerinfo;
        Dice dice;
        cout<<instance->getplayer().size()<<endl;
        for(auto player:instance->getplayer()){
            int id=player.second;
            playerinfo.inititateplayer(id);
        }
        while(true){
            for(auto player:instance->getplayer()){
                string stmnt="";
                stmnt+=player.first+" rolled a dice ";
                int id=player.second;
                int roll=dice.getnumber();
                stmnt.push_back((roll+'0'));
                stmnt+=" and moved from ";
                int initialpos=playerinfo.getpos(id);
                stmnt+=to_string(initialpos)+" to ";
                int endpos=initialpos+roll;
                if(endpos<=100){
                    unordered_map<int,int> snake=instance->getsnake();
                    unordered_map<int,int> ladder=instance->getladder();
                    if(snake.find(endpos)!=snake.end()){
                        endpos=snake[endpos];
                        stmnt+=to_string(endpos);
                        stmnt+=" through snake";
    
                    }
                    else if(ladder.find(endpos)!=ladder.end()){
                        endpos=ladder[endpos];
                        stmnt+=to_string(endpos);
                        stmnt+=" through ladder";
                    }
                    else{
                        stmnt+=to_string(endpos);
                        stmnt+=" .";
                    }
                    playerinfo.setpos(id,endpos);
                    cout<<stmnt<<endl;
                    if(endpos==100) return player.first;
                }
            }
        }
    }
};

int main(){
    int numberofsnakes=0,numberofladders=0,numberofplayer=0;
    cin>>numberofsnakes;
    Entities* instance=Entities::getEntity();
    while(numberofsnakes--){
        int start,end;
        cin>>start>>end;
        instance->setsnake(start,end);
    }
    cin>>numberofladders;
    while(numberofladders--){
        int start,end;
        cin>>start>>end;
        instance->setladder(start,end);
    }
    cin>>numberofplayer;
    int id=0,n=numberofplayer;
    while(numberofplayer--){
        string name;
        cin>>name;
        instance->setplayer(name,id);
        id++;
    }
    Board game;
    cout<<"hello"<<endl;
    cout<<"The Player "+game.startgame()+" won"<<endl;
    return 0;
}