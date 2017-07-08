#include "ofApp.h"
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <string>
#include <list>

using namespace std;


#define DIGIT_RESOLUTION (3)
#define BRICKS (4)
#define BRICKSIZE (160)
#define BRICKSIZE_2 (40)
#define PLAY (0)
#define MENU (1)
#define HOWTOPLAY (7)
#define MADEBY (3)
#define FINISH (4)
#define Alpha (13)

#define MainPage (1)
#define GamePage (2)
#define GameEnd (3)
#define RankingPage (4)
#define MakerPage (5)
#define UpdatePage  (6)


#define Keyboard (40)
#define KeyboardX (9)
#define KeyboardY (4)
#define Keyboardheight (40)

struct bulletType{
    int x;
    int y;
    int visible;
};

struct bulletType bullet[100];

void initBullet();
void updateBullet();
void addBullet(int x ,int y);
void drawBullet();



void setStartTime();


int bricks[DIGIT_RESOLUTION][DIGIT_RESOLUTION];
char keyboards[KeyboardX][KeyboardY];
char keyvalue[36];
string nickname;
int turn;

int ranX, ranY;
int openkeyboard;
int opensenddata;
int count; //getData의 수

int score;
float startTime;// game start - time
float endTime;

float readytoTime;

int TimerSize;
int TotalTime;

int frame;

int menuX;
int menuY;
int menuwidth;
int menuheight;


int gameSpeed;
int clicked;

int effectX;
int effectY;

int mouseX;
int mouseY;
int rotatecheck;

int brickNumberX, bricksNumberY;

int Gamemode;
int GameStatus;
int timeend = 0;



#define BAllRadius 20
#define BAllWidth 40
#define BallCount 10

int Ball[BallCount];

void FinalTest(){
    for(int i = 0;i<BallCount;i++){
        Ball[i] = 1;
    }
}


ofTrueTypeFont myFont;


int Rankinglength;

void setData(string name, int score){
    ofHttpResponse resp = ofLoadURL("http://easyfinder.azurewebsites.net/userdata?user_ID="+ name +"&user_SCORE="+ ofToString(score));
    printf("점수 입력! \n");
}

string TimeCtrl(int total){
    int timer = ofGetElapsedTimeMillis() - startTime;
    total = total - (timer/1000);
    
    TimerSize = total;
    string result = "";
    
    if(total <= 0){
        TimerSize = 0;
        turn  = 0; //셔플을 종료
        GameStatus = GameEnd;
        result = "Time Over";
        return result;
    }else{
        result = ofToString(total);

        if(TimerSize >= 121){
            if(TimerSize == 121){
                result = "Start!";
                turn = 1;
            }else{
                score = 0; //점수 고정
                result = ofToString(TimerSize - 121);
            }
        }
        
        if(TimerSize < 60){
            Gamemode = 3;
            if (TimerSize < 30){
                Gamemode = 4;
            }
        }
        
        return result;
    }
}

void getData(){
    printf("GetData 실행 \n");
    Rankinglength = 0;
    myFont.load("BMJUA_ttf.ttf",30);

    ofHttpResponse resp = ofLoadURL("http://easyfinder.azurewebsites.net/ranking");
    string mystring = resp.data;
    string token;
    //get server data
    int count = 0;
    int Player = 0;
    
    
    while(token != mystring){
        token = mystring.substr(0,mystring.find_first_of(":"));
        mystring = mystring.substr(mystring.find_first_of(":")+1);
        cout << token.c_str() <<endl;
        
        if(count == 0){
            Player = atoi(token.c_str()); //플레이어의 수를 결정
        }else{
            Player--;
            if(Player >= 0){
                ofSetColor(255, 255, 255);
                myFont.drawString(to_string(Player+1) + "th: " + token, ofGetWidth()/2-90, (ofGetHeight()/2-80) + 35 * count); //위치 조정
            }
        }
        count++;
    }
    
    
}


void shuffle(){
    
    for(int i=0;i<DIGIT_RESOLUTION;i++){
        for(int j=0;j<DIGIT_RESOLUTION;j++)
            {
                bricks[i][j]= rand()%Gamemode;
            }
        }
}


void GamePlay(){
    //게임이 실행될때 사용
    frame++;
    
    if(frame == 40){
        frame = 0;
        clicked = 0;
        if(turn == 1){
            shuffle();
        }
    }
    
    if(score < 0 ){
        GameStatus = GameEnd;
    }
    
    timeend++;
}

void GameOver(){
    if(score < 0 ){
        GameStatus = GameEnd;
    }
}

void keyboard(){
    char a = 'A';
    
    for(int i = 0;i<KeyboardY;i++){
        for(int j =1; j<KeyboardX;j++){
            keyboards[j][i] = a++;
            //printf("%d %d %c \n",i ,j, keyboards[j][i]);
        }
    }
}

void GotoMain(){
    GameStatus = MainPage;
}

void GotoGame(){
    GameStatus = GamePage;
}

void GotoEnd(){
    GameStatus = GameEnd;
}




void newGAMESETUP(){
    timeend = false;
    endTime = 0.0;
    TotalTime = 123;
    frame = 0;
    score = 0;
    Gamemode = 2;
    clicked = 0;
    openkeyboard = 0; // close keyboard

    
    menuX = ofGetWidth()/2-120;
    menuY = ofGetWidth()/2+55;
    menuwidth = 240;
    menuheight = 60;
    GameStatus = MainPage; //Game mode
    opensenddata = 1;
    rotatecheck= 0;
    effectX = 0;
    effectY = 0;
    turn = 0;

    
    FinalTest();
    
  //  getData(); // 게임 시작과 동시에 유저의 데이터를 가져옴
    
    for(int i=0;i<DIGIT_RESOLUTION;i++){
        for(int j=0;j<DIGIT_RESOLUTION;j++)
        {
            bricks[i][j]= 0;
        }
    }
}


int setTime(int readythree){ //정해진 시간을 보내줌
    
    int timer = ofGetElapsedTimeMillis() - readytoTime;
    readythree = readythree - (timer/1000);
    
    if(readythree == 0){
        printf("시간 종료! \n");
    }else{
        printf("시간을 보여줍니다. \n");
        printf("readythree : %d \n",readythree);
    }
    
    return timer;
}

void setReadytoTime(){
    readytoTime = ofGetElapsedTimeMillis();
}

void initBullet(){
    for(int i = 0;i<100;i++){
        bullet[i].x =0;
        bullet[i].y =0;
        bullet[i].visible = 0;
    }
}

void updateBullet(){
    int i,x,y;
    
    for(i=0;i<10;i++){
        if(bullet[i].visible == 1){
            bullet[i].y -= 10;
            x = bullet[i].x;
            y = bullet[i].y;
            if(Ball[x/BAllWidth] == 1){
                Ball[x/BAllWidth] = 0;
                bullet[i].visible = 0;
            }
            if(bullet[i].y<0){
                bullet[i].visible = 0;
            }
        }
    }
}

void addBullet(int x ,int y){
    for(int i = 0;i<10;i++){
        if(bullet[i].visible == 0){
            bullet[i].x = x;
            bullet[i].y = y;
            bullet[i].visible = 1;
            break;
        }
    }
}

void drawBullet(){
    ofFill();
    ofSetColor(0,0,0);
    for(int i =0;i<10;i++){
        if(bullet[i].visible)
        ofCircle(bullet[i].x, bullet[i].y, 20);
    }
}


void ofApp::setup(){
    ofSetWindowTitle("Catch!");
    ofSetWindowShape(480, 600);
    ofSetFrameRate(60);
    ofBackground(255, 255, 255);
    ofSetColor(0, 0, 0);
    ofSetLineWidth(1);
    newGAMESETUP();
    myFont.load("BMJUA_ttf.ttf",30);
    
    //initBullet();
}




//--------------------------------------------------------------
void ofApp::update(){
    
    //updateBullet();
    
    if(GameStatus == MainPage){

    }else if(GameStatus == GamePage){
        //setStartTime();//시간대를 정함
        GamePlay();
        GameOver();
    }
}

void setStartTime(){
    startTime = ofGetElapsedTimeMillis();
    printf("startTime : %f ",startTime);
}



void Finaldraw(){
    ofBackground(255, 0, 255);
    for(int i =0;i<BallCount;i++){
        if(Ball[i] == 1){
            ofSetColor(0, 0, 0);
        }else{
            ofSetColor(255, 255, 255);
        }
        ofCircle(i*BAllWidth+20, 20, BAllRadius);
    }
}


//--------------------------------------------------------------
void ofApp::draw(){ //요이땅 만들기
    
    //drawBullet();
    
    //printf("GameStatus: %d \n",GameStatus);
    
    if(GameStatus == MainPage){
        background.load("images/background1.jpg");
        background.draw(0,0,480,600);
        
        rankingIcon.load("images/rankingicon.png");
        rankingIcon.draw(390,500,80,80);
        
        ofSetColor(0, 0, 0);
        
        ofRect(menuX,menuY,240,60);
        ofRect(menuX,menuY+100,menuwidth,menuheight);
        ofRect(menuX,menuY+200,menuwidth,menuheight);
        
        //Finaldraw();
        
        
        
    }
     else if(GameStatus == GamePage){
       
        backgroundUp.load("images/cropUp.png");
        backgroundDown.load("images/cropDown.png");
        sand.load("images/hole.png");
        backgroundUp.draw(0,0,480,480);
        backgroundDown.draw(0,480,480,120);
        ofSetColor(0, 0, 0);
        myFont.drawString( ofToString(score), 300, 520);
        myFont.drawString( TimeCtrl(TotalTime), 250, 570);
         
        
        ofFill();
        for(int i=0;i<DIGIT_RESOLUTION;i++){
            for(int j = 0;j<DIGIT_RESOLUTION;j++){
                if(bricks[i][j]==0){
                    ofSetColor(255, 255, 255);
                    mouse.load("images/hide.png");
                }
                else if(bricks[i][j] == 1){
                    ofSetColor(255, 255, 255);
                    mouse.load("images/characters.png");
                }
                else if (bricks[i][j] == 2){ //Ìè≠ÌÉÑ
                    ofSetColor(255, 255, 255);
                    mouse.load("images/rab.png");
                }
                else if (bricks[i][j] == 3){ //Ìè≠ÌÉÑ
                    ofSetColor(255, 255, 255);
                    mouse.load("images/characters3.png");
                }
                mouse.draw(i*BRICKSIZE,j*BRICKSIZE,BRICKSIZE,BRICKSIZE);
                sand.draw(i*BRICKSIZE,j*BRICKSIZE+ 120,BRICKSIZE,40);
            }
        }
        
         if(TimerSize <= 120){
             if( TimerSize < 60){
                 ofSetColor(255, 0, 0);
             }else{
                 ofSetColor(0, 0, 0);
             }
            ofRect(330,545,TimerSize*0.8,30);
        }
         
        if(clicked == 1){
             ofSetColor(255, 255, 255);
             impact.load("images/score1.png");
             impact.draw(effectX*BRICKSIZE+40,effectY*BRICKSIZE,80,20);
        }else if(clicked ==2){
             ofSetColor(255, 255, 255);
             impact.load("images/score2.png");
             impact.draw(effectX*BRICKSIZE+40,effectY*BRICKSIZE,80,20);
        }


    }
    else if (GameStatus == GameEnd){
       
        
        background.load("images/ending.jpg");
        background.draw(0,0,480,600);
       
        home.load("images/home.png");
        home.draw(ofGetWidth()-470,500,80,80);
        
        rankingIcon.load("images/upload.png");
        rankingIcon.draw(390,500,80,80);
        myFont.drawString(ofToString(score), ofGetWidth()/2-60, ofGetHeight()/2-110);

        
    }
    
    else if(GameStatus == RankingPage){
        background.load("images/rankingOp.png");
        background.draw(0,0,480,600);
        
        rankingIcon.load("images/home.png");
        rankingIcon.draw(390,500,80,80);
    
        getData();
    }
    
    else if(GameStatus == MakerPage){
        background.load("images/maker.png");
        background.draw(0,0,480,600);
        
    }
    
    else if(GameStatus == HOWTOPLAY){
        background.load("images/how.jpg");
        background.draw(0,0,480,600);
        
    }

    
    else if(GameStatus == UpdatePage){
        background.load("images/uploaddata.png");
        background.draw(0,0,480,600);
        
        home.load("images/keyboard.png");
        home.draw(ofGetWidth()-470,500,80,80);
        
        rankingIcon.load("images/home.png");
        rankingIcon.draw(390,500,80,80);
        
        ofSetColor(0, 0, 0);
        myFont.drawString(ofToString(score), ofGetWidth()/2-60, ofGetHeight()/2+10); //점수 로드
        myFont.drawString( ofToString(nickname), ofGetWidth()/2-60, ofGetHeight()/2-90);
        
        ofRect(ofGetWidth()/2-110, ofGetHeight()/2+55, 230 ,45); //점수 로드 //send data
        
    }
 

    ofNoFill();
    ofSetColor(255, 255, 255);
    if(rotatecheck == 0){
        Hammer.load("images/hammer.png");
    }else{
        Hammer.load("images/hammer_2.png");
        
    }
    Hammer.draw(ofGetMouseX()-33,ofGetMouseY()-11,76,122);

    
    if(openkeyboard == 1){
        
        char first = 'A';
        
        for(int i = 0;i<KeyboardY;i++){
            for(int j =1; j<KeyboardX;j++){
                ofFill();
                ofSetLineWidth(1);
                ofSetColor(128, 128, 128);
                ofRect(j*Keyboard+50, i*Keyboardheight+ofGetHeight()/2+50 ,40,40); //20, 300
                ofSetColor(0, 0, 0);
                myFont.drawString(ofToString(first++), j*Keyboard+50, i*Keyboardheight+30+ofGetHeight()/2+50);
                ofSetColor(255, 255, 255);
            }
        }
        ofNoFill();


    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'q'){
       // addBullet(ofGetMouseX(),ofGetMouseY());
    }
    
    if (key == 'r'){
        //name delete
        nickname.pop_back();
    }
    if (key == 'e'){
        //name delete
        GameStatus = GameEnd;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

void ofApp::mouseDragged(int x, int  y, int button){
    
}

void touch(int a, int b){
    effectX = 0;
    effectY = 0;
    
    effectX = a;
    effectY = b;
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    rotatecheck = 1;

    if(y < 20 + BAllRadius){
        printf("%d \n",x);
        
        if(x >= (x/BAllWidth)*BAllWidth && x<= (x/BAllWidth)*BAllWidth+20){
            printf("%d \n",x/BAllWidth);

            if(Ball[x/BAllWidth] == 1){
                Ball[x/BAllWidth] = 0;
            }
        }
    }
    
       if(GameStatus == MainPage){
           
           if(x >= menuX && x <= menuX + menuwidth){
               if( y>= menuY && y <= menuY + menuheight){
                   //turn = 1;
                   startTime = ofGetElapsedTimeMillis();
                   GameStatus = GamePage;
               }
               else if( y>= menuY+100 && y <= menuY+100 + menuheight){
                   GameStatus = HOWTOPLAY;
               }
               else if( y>= menuY+200 && y <= menuY+200 + menuheight){
                   GameStatus = MakerPage; //제작자
               }
           }
           
            if(x>= 390 && x<= 470){
               if(y>= 500 && y<= 580){
                   GameStatus = RankingPage;
               }
           }
           
       }
       else if(GameStatus == GamePage){
        if(x < DIGIT_RESOLUTION * BRICKSIZE && y<DIGIT_RESOLUTION*BRICKSIZE){
            if(bricks[y/BRICKSIZE][x/BRICKSIZE] == 0){
                score += 0;
            }else if(bricks[y/BRICKSIZE][x/BRICKSIZE] == 1){
                impact.load("images/score1.png");
                touch(x/BRICKSIZE,y/BRICKSIZE);
                clicked = 1;
                score += 100;
            }else if(bricks[y/BRICKSIZE][x/BRICKSIZE] == 2){
                impact.load("images/score2.png");
                touch(x/BRICKSIZE,y/BRICKSIZE);
                clicked = 2;
                if(score < 0){
                    score = 0;
                }else{
                    score -= 200;
                }
            }else if(bricks[y/BRICKSIZE][x/BRICKSIZE] == 3){
                clicked = 0;
                GameStatus = GameEnd;
            }
        }
        }
    
       else if(GameStatus == GameEnd){
           
           //printf("%d \n",GameStatus);
           TotalTime = 123;
           TimerSize = 123;
           turn = 0;
           
           if(x>= 390 && x<= 470){
               if(y>= 500 && y<= 580){
                   GameStatus = UpdatePage; //upload data
                   //printf("updatepage \n");
               }
           }
           
           if(x>= ofGetWidth() - 470 && x<= ofGetWidth() -390){
               if(y>= 500 && y<= 580){
                   GameStatus = MainPage; //upload data
                   //printf("MainPage \n");

               }
           }
           
           if(x >= ofGetWidth()/2-100 && x <= ofGetWidth()/2-100 + 240){
               if( y >= ofGetHeight()/2+200 && y <= ofGetHeight()/2+400){
                   score = 0;
                   TimerSize = 123;
                   Gamemode = 2;
                   //turn = 1;
                   //printf("셔플 시작\n");
                   startTime = ofGetElapsedTimeMillis();
                   GameStatus = GamePage;
                  // printf("GamePage \n");
               }
           }

       }

    
        else if(GameStatus == RankingPage){
            //printf("%d \n",GameStatus);
            
            if(x>= 390 && x<= 470){
                if(y>= 500 && y<= 580){
                    GameStatus = MainPage; //main page
                }
            }
        }

        else if(GameStatus == MakerPage){ //ranking
           // printf("%d \n",GameStatus);

            if(x>= 390 && x<= 470){
                if(y>= 500 && y<= 580){
                    GameStatus = MainPage;
                }
            }
        }
    
        else if(GameStatus == HOWTOPLAY){ //ranking
            
            if(x>= 390 && x<= 470){
                if(y>= 500 && y<= 580){
                    GameStatus = MainPage;
                }
            }
        }

    
        else if(GameStatus == UpdatePage){
            //printf("%d \n",GameStatus);

            if(x>= 390 && x<= 470){
                if(y>= 500 && y<= 580){
                    GameStatus = MainPage;
                }
            }
            
            if(x>= ofGetWidth() - 470 && x<= ofGetWidth() -390){
                if(y>= 500 && y<= 580){
                    if(openkeyboard == 0){
                        openkeyboard = 1; //upload data
                        keyboard();
                    }else{
                        openkeyboard = 0;
                    }
                }
            }
            
            if(openkeyboard == 1){ //키보드가 열렸을 때
                opensenddata = 1; //키보드가 열렸을 때 senddata를 할수 없음
              if(x < Keyboard * KeyboardX && y< Keyboardheight * KeyboardY+ofGetHeight()/2){
                  nickname += keyboards[(x/Keyboard)-2][(y/Keyboardheight)-5];
              }
            }else{
                 opensenddata = 0;
            }
            
            if(opensenddata == 0)
            {
                if(x >= ofGetWidth()/2-110 && x<=ofGetWidth()/2-110+230){
                    if(y>= ofGetHeight()/2+55 && y<= ofGetHeight()/2+55 +45){
                        printf("Send data Clicked \n");
                        //서버로 보내는 작업
                        setData(nickname,score);
                        GameStatus = MainPage;
                        score = 0;
                        nickname = "";
                    }
                }
            }
        }

    

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    rotatecheck = 0;

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
