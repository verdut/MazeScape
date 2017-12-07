#include <EEPROM.h>
#include "LedControl.h" 
#include <LiquidCrystal.h>


#define LOAD 10
#define CLK 11
#define DIN 12
#define X 0
#define Y 1
#define SW 13
#define V0 9
#define RS 1
#define E 2
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define contrast 100



LiquidCrystal lcd(1,2,4,5,6,7);
LedControl lc=LedControl(DIN,CLK,LOAD,1);


int playerX, playerY;                     //coordonatele player-ului
int xObj, yObj;                          //coordonatele obiectului impins
int moveX=0, moveY=0;                   //cu cat se misca pe axa X si Y player-ul, iau valori {-1,0,1}
int xWin, yWin;                        //coordonatele punctului de iesire din labirint 

int collide;                          //daca e 1 inseamna ca player-ul s-a lovit de obiect
int wallCollide;                     //daca e 1 - coliziune cu zidurile labirintului atat si pentru player cat si pentru obiect

int levelPassed=0;                    //indica daca un nivel a fost trecut
int selectedLevel=0;                 // indica nivelul selectat pentru a fi jucat
int (*curLevel)[3];                 //pointer catre va fi folosit pentru accesarea matricei fiecarui nivel
int numRow;                        //numarul de linii din matricea unui nivel


bool levelInitialized=false;      //arata daca nivelul a fost initializat

int numOfTotalMoves;              //nr. total de miscari facut de player intr-un nivel, pentru a calcula scorul
int maxScoreMoves;               //numarul minim de miscari pentru a obtine scor maxim
int currentScore;               //scor curent

int intensity=8;                //intensitatea led-urilor matricei 8*8



// Matricele pentru fiecare nivel in parte, primele n-4 randuri contin coordonatele fiecarui zid in parte.
// Urmatoarele 3 randuri contin in ordine: coordonatele player-ului, coordonatele obiectului, coordonatele punctului de iesire din labirint.
//Ultimul rand contine scorul curent si numarul minim de miscari pentru highscore maxim. 


//##start zona matrice nivele

int levelOne[][3]={   
                      {1,2},
                      {1,5},
                      {1,6},
                      {2,5},
                      {2,6},
                      {3,1},
                      {3,2},
                      {5,2},
                      {5,4},
                      {6,2},
                      {6,4},
                      {6,5},
                      {6,6},
                      
                      {1,1},
                      {2,2},
                      {5,7},
                      {0,15}
                     };

int levelTwo[][3]={   
                      {1,3},
                      {2,3},
                      {2,5},
                      {4,1},
                      {4,2},
                      {4,3},       
                      {4,5},
                      {5,3},
                      {6,1},
                      
                      {1,1},
                      {2,2},
                      {5,7},
                      {0,23}
                
                     };              


int levelThree[][3]={  
                       {1,2},
                       {1,4},
                       {3,1},
                       {3,3},
                       {3,5},
                       {5,5},
                       {6,1},
                       {6,2},
                       {6,5},

                       {1,1},
                       {3,2},
                       {2,7},
                       {0,22}
                        
                    };

int levelFour[][3]={  
                      {1,3},
                      {1,4},
                      {1,5},
                      {2,1},
                      {2,2},
                      {2,6},
                      {3,6},
                      {4,3},
                      {5,6},

                      {4,4},
                      {3,4},
                      {4,0},
                      {0,17}
                      
                   };

int levelFive[][3]={  
                      {1,3},
                      {2,3},
                      {4,3},
                      {4,4},
                      {5,2},
                      {5,3},
                      
                      {6,1},
                      {2,2},
                      {7,4},
                      {0,23}
                      
                   };


 int levelSix[][3]={  
                      {3,1},
                      {3,2},
                      {3,4},
                      {4,4},
                      {6,3},
                      
                      {6,2},
                      {5,5},
                      {0,2},
                      {0,17}
                      
                   };  


int levelSeven[][3]={ 
                      {2,4},
                      {2,5},
                      {3,3},
                      {5,2},
                      {5,3},
                      {5,5},
                      {5,6},
                      
                      {3,1},
                      {4,4},
                      {6,7},
                      {0,24}
                      
                   }; 


int levelEight[][3]={  
                      {2,1},
                      {2,3},
                      {2,4},
                      {3,4},
                      {3,6},
                      {4,4},
                      {5,1},
                      {6,3},
                      
                      {1,1},
                      {2,2},
                      {1,0},
                      {0,26}
                      
                   };   


int levelNine[][3]={  
                      {1,3},
                      {1,4},
                      {2,3},
                      {2,5},
                      {2,6},
                      {3,3},
                      {4,1},
                      {4,4},
                      {5,3},
                      {5,5},
                      {6,5},

                      {2,1},
                      {1,6},
                      {1,7},
                      {0,9}
                      
                   };



int levelTen[][3]={  
                      {2,3},
                      {3,1},
                      {3,3},
                      {3,4},
                      {3,6},
                      {4,1},
                      {4,3},
                      {4,4},
                      {5,2},
                      {5,3},
                      {5,4},
                      {5,5},
                      {6,1},
                      {6,6},

                      {6,3},
                      {3,2},
                      {1,0},
                      {0,20}
                      
                   };



//##finish zona matrice nivele




//seteaza highscore-urile in EEPROM cu 0; seteaza starea nivelelor in EEPROM (X)
void setScore_LevelStatus()
{
     EEPROM[0]=1;            // Daca e 1 inseamna ca in memorie au fost salvate datele mentionate la (X);

     EEPROM[1]=0;            //nivelul 1 este mereu deblocat.
    
   for(int i=2;i<=10;i++)    //toate nivelele in afara de primul sunt blocate. Acestea se deblocheaza cand nivelul precedent lor este terminat.
     EEPROM[i]=1; 
     
    for(int i=11; i<=20;i++)   //highscore-ul initial pentru fiecare nivel este 0
      EEPROM[i]=0;

     

}



//aprinde conturul labirintului.
void fillBorder()                         
{
   for(int i=0;i<=7;i++)
     for(int j=0;j<=7;j++)
      {
         if(  i==0 || j==0 || i==7 || j==7  )
            lc.setLed(0,i,j,true);      
      }
      
 }




// face initializarile pentru nivelul 'level' primit ca parametru. Creeaza labirintul conform matricei corespunzatoare si pozitioneaza player-ul, obiectul si iesirea din labirint
void levelNumberSet(int level)
{
   
   
    if(level==1)
      { 
        curLevel=levelOne;                                //pointer catre nivel
        numRow=sizeof(levelOne)/sizeof(levelOne[0]);     //aceasta instructiune calculeaza cate linii are matricea levelOne; idem pentru cele de mai jos.
               
      }
          else if(level==2)
             { 
               curLevel=levelTwo;  
               numRow=sizeof(levelTwo)/sizeof(levelTwo[0]);
           
              }
               else if(level==3)
                 { 
                    curLevel=levelThree; 
                    numRow=sizeof(levelThree)/sizeof(levelThree[0]);
           
                 }
                    else if(level==4)
                      { 
                        curLevel=levelFour; 
                        numRow=sizeof(levelFour)/sizeof(levelFour[0]);
           
                      }
                         else if(level==5)
                            { 
                              curLevel=levelFive; 
                              numRow=sizeof(levelFive)/sizeof(levelFive[0]);
           
                            }
                                else if(level==6)
                                  { 
                                      curLevel=levelSix; 
                                      numRow=sizeof(levelSix)/sizeof(levelSix[0]);
           
                                  }
                                        else if(level==7)
                                            { 
                                                curLevel=levelSeven; 
                                                numRow=sizeof(levelSeven)/sizeof(levelSeven[0]);
           
                                            }
                                            else if(level==8)
                                              { 
                                                curLevel=levelEight; 
                                                numRow=sizeof(levelEight)/sizeof(levelEight[0]);
           
                                              }
                                                else if(level==9)
                                                  { 
                                                    curLevel=levelNine; 
                                                    numRow=sizeof(levelNine)/sizeof(levelNine[0]);
           
                                                  }
                                                    else if(level==10)
                                                      { 
                                                        curLevel=levelTen; 
                                                        numRow=sizeof(levelTen)/sizeof(levelTen[0]);
           
                                                      }
                                          
  
}



//construieste nivelul pe matricea 8*8 si initializeaza datele pentru nivelul 'level' 
void initLevel(int level)
{
    numOfTotalMoves = 0;
    maxScoreMoves = curLevel[numRow-1][1];
    currentScore  = 0;
   
   lc.clearDisplay(0);
    
   fillBorder();
   
   levelNumberSet(level);

    
   playerX=curLevel[numRow-4][0];
   playerY=curLevel[numRow-4][1];

   xObj=curLevel[numRow-3][0];
   yObj=curLevel[numRow-3][1];

   xWin=curLevel[numRow-2][0];
   yWin=curLevel[numRow-2][1];

   
     
      for(int i=0;i<numRow-4;i++)      
          lc.setLed(0,curLevel[i][0], curLevel[i][1],true);
       
       
      
      
   lc.setLed(0,playerX,playerY,true);
   lc.setLed(0,xWin,yWin,false);

   levelPassed=0;
   
  
}



//acesta este engine-ul jocului, cel care se ocupa de miscarea player-ului si interactiunile dintre player, obiect si zidurile labirintului. 
//De asemenea spune si daca un nivel este trecut sau nu.

void engine()
{


  int coordX, coordY;
  int moveX=0, moveY=0;

  
  
  
  collide=0;
  wallCollide=0;

  //se citesc miscarile joystick-ului
  coordX=analogRead(X);   
  delay(80);
  coordY=analogRead(Y);
  
   lc.setLed(0,xObj,yObj,true);
   
     if(coordX>530) moveY=1;           //mapare miscari joystick
     if(coordX<490) moveY=-1;
     if(coordY>530) moveX=1;
     if(coordY<490) moveX=-1;

      // daca playerul iese din matrice apare pe marginea opusa locului prin care a iesit. Dar pentru ca am border, acest lucru nu este neaparat de folos.
     if(playerX+moveX >7)   moveX=-7;           
        else if(playerX+moveX<0)  moveX=7;

     if(playerY+moveY >7)   moveY=-7;
        else if (playerY+moveY<0)   moveY=7;
      
     

      if(playerX+moveX==xObj && playerY+moveY==yObj) collide=1;         //playerul impinge obiectul

      if(  playerX+moveX==0 || playerY+moveY==0 || playerX+moveX==7 || playerY+moveY==7  ) return;
            
            
      for(int i=0;i<numRow-4;i++)
          if(playerX+moveX==curLevel[i][0] && playerY+moveY==curLevel[i][1]) wallCollide=1;
       
           

     if(wallCollide) return;

     if(collide==1) 
        {   
           if(xObj+moveX==xWin && yObj+moveY==yWin ) 
              {  
                 if(numOfTotalMoves<=maxScoreMoves)
                    currentScore=currentScore+ 5*maxScoreMoves - 3*(numOfTotalMoves-maxScoreMoves);
                 if(numOfTotalMoves>maxScoreMoves && numOfTotalMoves<=maxScoreMoves+10)
                    currentScore+=5*maxScoreMoves - 2*(numOfTotalMoves-maxScoreMoves);

                  currentScore+=10;

                  curLevel[numRow-1][0]=currentScore;

                  if(currentScore > 255)
                    currentScore=255;
                      
                  if(currentScore > EEPROM[selectedLevel+10])
                     EEPROM[selectedLevel+10]=currentScore;
              
                  
                 levelPassed=1;

                 if(selectedLevel+1<=10)
                    EEPROM[selectedLevel+1]=0;
                    
                 return;
                }
    

             if(xObj+moveX==0 || xObj+moveX==7 || yObj+moveY==0 || yObj+moveY==7) return; 

                     
                for(int i=0;i<numRow-4;i++)
                   {
                       if(xObj+moveX==curLevel[i][0] && yObj+moveY==curLevel[i][1]) wallCollide=1;
       
                    }

               if(wallCollide) return;
                     
                      
               lc.setLed(0,xObj+moveX, yObj+moveY, true); 
               lc.setLed(0,playerX,playerY,false);
               numOfTotalMoves++;

               playerX=xObj;
               playerY=yObj;
               xObj=xObj+moveX;
               yObj=yObj+moveY;
           
           }    else
                  {
                     lc.setLed(0,playerX+moveX,playerY+moveY,true);
                     delay(10);

                     if(moveX!=0 || moveY!=0)
                        {
                          lc.setLed(0,playerX,playerY,false); 
                          numOfTotalMoves++;
                        }
                        
                      playerX=playerX+moveX;
                      playerY=playerY+moveY;
         
                   }
        
     delay(50);

     
     lc.setLed(0,xObj,yObj,false);  //ca sa clipeasca obiectul
   
}



//afiseaza un smiley face pe matricea 8*8
void showWinScreen(){

   lc.clearDisplay(0);

   lc.setLed(0,0,1,true);
   lc.setLed(0,0,2,true);
   lc.setLed(0,0,5,true);
   lc.setLed(0,0,6,true);
   lc.setLed(0,1,1,true);
   lc.setLed(0,1,2,true);
   lc.setLed(0,1,5,true);
   lc.setLed(0,1,6,true);
   
   lc.setLed(0,3,3,true);
   lc.setLed(0,3,4,true);
   lc.setLed(0,4,3,true);
   lc.setLed(0,4,4,true);
   
   lc.setLed(0,5,0,true);
   lc.setLed(0,5,7,true);
   lc.setLed(0,6,1,true);
   lc.setLed(0,6,6,true);
   lc.setLed(0,7,2,true);
   lc.setLed(0,7,3,true);
   lc.setLed(0,7,4,true);
   lc.setLed(0,7,5,true);

   
  
}



// "interfata de selectare al nivelului"
void selectLevel()
{    
     lc.clearDisplay(0);
     lcd.clear();
     
     selectedLevel=0;
     
     bool locked=0;
     int i=0,j=0; //coordonate cu care calculez si afisez animatia de selectare nivel
     int coordX;  //directia in care ne mutam, pe coordonata x
     bool moved;  //daca s-a primit input de la joystick
   
    lc.setLed(0,0,0,true);
    
    lcd.setCursor(0,0);
    
    lcd.print("Select level");
    lcd.setCursor(0,1);
    lcd.print("Level: 1");
     
      
         while(digitalRead(SW)!=0 || locked==1)
             {    
                  moved=false;
                  coordX=analogRead(X);
                  delay(75);
            
                  if(coordX>530 && j+1<=7 && 8*i+j+2<=10) 
                     { 
                       lc.setLed(0,i,j+1,true); 
                       j++;
                       moved=true;
                       
                      } else if(coordX>530 && j==7 && i==0) 
                           { 
                               lc.setLed(0,1,0,true); 
                               i=1; 
                               j=0; 
                               moved=true;
                             }
                             
         
                    if(coordX<490 && j-1>=0) 
                        { 
                            lc.setLed(0,i,j,false); 
                            j--; 
                            moved=true;
                            
                         }
                         else if(coordX<490 && i==0 && j==0)
                         {
                            j=-1; 
                            moved=true;
                            lc.setLed(0,0,0, false);
                         }
                         
                            else if(coordX<490 && j==0 && i==1) 
                                { 
                                    lc.setLed(0,i,j,false); 
                                    i=0; 
                                    j=7;
                                    moved=true; 
                                  }

          
                         if(moved==true)
                          { 
                            locked=0; 
                            lcd.setCursor(8,1);
                            lcd.print("        ");
                            lcd.setCursor(7,1);

                            
                            if(j==-1)
                            {
                               lcd.print(" Return");
                            }
                             
                            else 
                            {
                               lcd.print(8*i+j+1);

                               if(EEPROM[8*i+j+1]==1)
                                {
                                  locked=1;
                                  lcd.setCursor(10,1);
                                  lcd.print("LOCKED");
                                }
                            }
                            delay(200);
                           }
           
          
         
           
                }

       delay(150);   

      if(j==-1)
        return;
          else selectedLevel=8*i+j+1;  //se calculeaza nivelul ales folosind i si j
    
 }



//Afisarea meniului pe LCD
int showMenu()
{   
   int coordX;
   int option=1;
   int moved;
   
           lcd.clear();   
           lcd.print("MazeScape");
           lcd.setCursor(0,1);
           lcd.print("1.Select level");
   
   
   while(digitalRead(SW)!=0)
     {
        coordX=analogRead(X);
        delay(100);
        
        moved=0;
        
        if(coordX >560){ option++; moved=1;}
        if(coordX <450){ option--; moved=1;} 

        if(option>4) option=4;
        if(option<1) option=1;

           
          if(moved)
           {
             lcd.clear();   
             lcd.print("MazeScape");
             lcd.setCursor(0,1);  
                       
        
           

               if(option==1 )
                  lcd.print("1.Select level");
                 else if(option==2 )
                      lcd.print("2.Best Score");
                    else if(option==3 )
                         lcd.print("3.Options");
                       else if(option==4)
                           lcd.print("4.Reset game");

              delay(100);     
          }
       }
   
        delay(100);
        return option;
  
}


//Afisarea highscore-ului pentru un nivel selectat
void showHighScores()
{
    int coordX;
    int level=1;

    lcd.clear();
    lcd.print("Level ");
    lcd.print(level);
    lcd.setCursor(0,1);
    lcd.print("HScore:");
    lcd.print(EEPROM[11]);
    
    while(digitalRead(SW)!=0)
    {
       coordX=analogRead(X);
       
       int moved=0;
       
       if(coordX >530) {level++; moved=1;}
        else if(coordX <490) {level--; moved=1;}

       if(level>10) level=10;
       if(level<1)  level=1;

       
       

      levelNumberSet(level);
       
                                                       
       
        
       if(moved) 
       {
        lcd.clear();
        lcd.print("Level ");
        lcd.print(level);
        lcd.setCursor(0,1);
        lcd.print("HScore:");
        lcd.print(EEPROM[level+10]);
        delay(200);
       }
      
    }

   delay(100);
  
}



//alege intensitatea LED-urilor matricei 8*8
int chooseIntensity()
{
  
  int coordX;
  
  lc.clearDisplay(0);
  
  lcd.clear();
  lcd.print("Pick intensity:");
  lcd.setCursor(0,1);
  lcd.print("I=");
  lcd.setCursor(2,1);
  lcd.print(intensity);

  lc.setLed(0,3,3,true);
  lc.setLed(0,3,4,true);
  lc.setLed(0,4,3,true);
  lc.setLed(0,4,4,true);
  
    while(digitalRead(SW)!=0)
     {
        coordX=analogRead(X);
       
       int moved=0;
       
       if(coordX >530) {intensity++; moved=1;}
        else if(coordX <490) {intensity--; moved=1;}

       if(intensity>15) intensity=15;
       if(intensity<0)  intensity=0;
    
        
          if(moved) 
          {
             lcd.clear();
             lcd.print("Pick intensity:");
             lcd.setCursor(0,1);
             lcd.print("I=");
             lcd.setCursor(2,1);
             lcd.print(intensity);

             lc.clearDisplay(0);
             lc.setIntensity(0,intensity);
             
             lc.setLed(0,3,3,true);
             lc.setLed(0,3,4,true);
             lc.setLed(0,4,3,true);
             lc.setLed(0,4,4,true);
             
             delay(200);
          }
      
     }
  
   delay(100);
}




//initialiari
void setup() {
  
  //Serial.begin(9600);
  lcd.begin(16,2);
  analogWrite(V0, contrast);
  
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  pinMode(SW,INPUT);
  digitalWrite(SW,HIGH);

  curLevel=NULL;
  selectedLevel=0;
  
}




//loop
void loop() {

   int menuOption;

   lc.clearDisplay(0);
   menuOption=showMenu();
   delay(100);

   if(EEPROM[0]==0)
   {
      setScore_LevelStatus();
   }
   
  if(menuOption==1) 
   {
        if(selectedLevel==0)
          {  
            selectLevel();
            levelInitialized=false;
          }
     
        if(selectedLevel!=0 && levelInitialized==false)
          {  
            initLevel(selectedLevel);
            levelInitialized= true;
          }
     
   
        if(levelInitialized==true)
          while(levelPassed==0 && digitalRead(SW)!=0 )
              engine();
           
   
    
        if(levelPassed) 
          {
            showWinScreen();

            lcd.clear();
            lcd.print("Nivel trecut!");
            lcd.setCursor(0,1);
            lcd.print("Scor: ");
            lcd.print(curLevel[numRow-1][0]);
      
            while(digitalRead(SW)!=0)
              delay(100);

             selectedLevel=0;
             lc.clearDisplay(0);  
          } else selectedLevel=0;  

    }
       else  if(menuOption==2)
          {
             showHighScores();
          }

            else if(menuOption==3)
               {
                  chooseIntensity();  
               }
               else if(menuOption==4)
                   setScore_LevelStatus();

    delay(200);
  
 }

