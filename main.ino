

#include <Esplora.h>
#include <TFT.h>   
#include <EEPROM.h>



#define DOWN  SWITCH_1
#define LEFT  SWITCH_2
#define RIGHT SWITCH_4
#define UP  SWITCH_3
long unsigned int downtime=0;
int bup=0, bdo=0, bri=0,ble=0;
int score=0,level=1;
int difficuty=800;
int block_wait=0;

enum states {
  CUBE, LINE_STRAIGHT,LINE_UP, SHIP_UP,SHIP_LEFT,SHIP_RIGHT,SHIP_DOWN,
  HORSEY_UP , HORSEY_STRAIGHT , GREEN, RED, BLUE,OVER,IN_PROGRESS,START,
  MENU_START,MENU_LEADERBOARD
};
enum states STATE, NEXT_STATE , COLOUR, GAME,NEXT_COLOUR,MENU;
// funkce kontrolující stisknutí stlačítka
bool buttonEvent(int button)
{
  switch(button)
  {
    case LEFT:
     if (Esplora.readButton(LEFT) == LOW)
     {
       ble |= 1;
     }
     else if (ble & 1)
     {
       ble ^= 1;
       return true;
     }
     break;
 
    case DOWN:
     if (Esplora.readButton(DOWN) == LOW)
     {
       bdo |= 1;
     }
     else if (bdo & 1)
     {
       bdo ^= 1;
       return true;
     }
     break;
 
    case UP:
     if (Esplora.readButton(UP) == LOW)
     {
       bup |= 1;
     }
     else if (bup & 1)
     {
       bup ^= 1;
       return true;
     }
     break;
 
    case RIGHT:
     if (Esplora.readButton(RIGHT) == LOW)
     {
      bri |= 1;
     }
     else if (bri & 1)
     {
       bri ^= 1;
       return true;
     }
  }
   return false;
}
 
// struct padajícího bloku který obsahuje souřadnice čtverečků a pivotní souřadnice pro otáčení bloků
struct block_struct
{
  char a[2],b[2],c[2],d[2];
  int pivot[2];
  
  
};
 block_struct block;
 block_struct dummy;



// inicializace pole zapsaných bloků
char fields[21][10]={
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1,1}
};


void rewrite_score(void)
{
EEPROM.write(0,score);
if(score>EEPROM.read(1))
  EEPROM.write(1,score);
  
  
  
  
  }

void draw_scoreboard(void)
{
 EsploraTFT.stroke(255,255,255);
EsploraTFT.setTextSize(1);
String str = "";
str+= EEPROM.read(0);
char arr[50];
str.toCharArray(arr, 50) ;
EsploraTFT.text("Your recent score is:",10,40);
EsploraTFT.text(arr, 140, 40);
str = "";

str+= EEPROM.read(1) ;
char arr_[50];
str.toCharArray(arr_, 50) ;
EsploraTFT.text("Your best score is:",10,80);
 EsploraTFT.text(arr_, 130, 80);
  
  
  EsploraTFT.text("Press right button to exit",1,10);
}

// funkce vypíše skóre během hry
void draw_score (void)
{
String str = "";
str+= score;
char arr[50];
str.toCharArray(arr, 50) ;

EsploraTFT.stroke(20,40,60);
EsploraTFT.fill(20,40,60);
EsploraTFT.rect(40,10,19,8);

EsploraTFT.stroke(255,255,255);
EsploraTFT.setTextSize(1);
EsploraTFT.text("score", 5, 10);
EsploraTFT.text(arr, 40, 10);
  
  
  
  }
// funkce vypíše aktuální obtížnost hry
void draw_level (void)
{
String str = "";
str+= level;
char arr[50];
str.toCharArray(arr, 50) ;

EsploraTFT.stroke(20,40,60);
EsploraTFT.fill(20,40,60);
EsploraTFT.rect(40,30,19,8);

EsploraTFT.stroke(255,255,255);
EsploraTFT.setTextSize(1);
EsploraTFT.text("level", 5, 30);
EsploraTFT.text(arr, 40, 30);
  
  
  
  }




  
// funkce pro vykreslení startovní obrazovky
void draw_start(void)
{
int i;
EsploraTFT.background(20,40,60);
 EsploraTFT.stroke(255,255,255);
EsploraTFT.setTextSize(1);
  EsploraTFT.text("Button right to toggle",0,0);
   EsploraTFT.text("Button down to select",0,10);

switch (MENU)
case MENU_START:
{
for(i=0;i<6;i++)
{

  if(i%3==0)
  {
  EsploraTFT.stroke(0,0,255);
  EsploraTFT.setTextSize(4);
  EsploraTFT.text("START",25+i,40+i);
  }
    if(i%3==1)
  {
  EsploraTFT.stroke(0,255,0);

  EsploraTFT.text("START",25+i,40+i);
  }
    if(i%3==2)
  {
  EsploraTFT.stroke(255,0,0);
  
  EsploraTFT.text("START",25+i,40+i);
  }
  
  
  
  
  
  
  }
EsploraTFT.stroke(255,0,255);
  EsploraTFT.setTextSize(2);
  EsploraTFT.text("SCORE",10,80);
  break;
  case MENU_LEADERBOARD:


  
 for(i=0;i<6;i++)
{

  if(i%3==0)
  {
  EsploraTFT.stroke(0,0,255);
  EsploraTFT.setTextSize(4);
  EsploraTFT.text("SCORE",25+i,40+i);
  }
    if(i%3==1)
  {
  EsploraTFT.stroke(0,255,0);

  EsploraTFT.text("SCORE",25+i,40+i);
  }
    if(i%3==2)
  {
  EsploraTFT.stroke(255,0,0);
  
  EsploraTFT.text("SCORE",25+i,40+i);
  }
  
  
  
  
  
  
  }
  
  
  
  
  
  
  
EsploraTFT.stroke(255,0,255);
  EsploraTFT.setTextSize(2);
  EsploraTFT.text("START",10,80);

  
  break;
}
  }
// funkce k vykreslení konce hry
  void draw_game_over(void)
{
 EsploraTFT.stroke(0,0,255);
  EsploraTFT.setTextSize(4);
  EsploraTFT.text("GAME",10,20);
  EsploraTFT.stroke(0,255,0);
  EsploraTFT.text("GAME",11,21);
  EsploraTFT.stroke(255,0,0);
  EsploraTFT.text("GAME",12,22);
    EsploraTFT.setTextSize(1);
EsploraTFT.stroke(255,255,255);
EsploraTFT.text("Your score is:",10,60);
String str = "";
str+= score;
char arr[50];
str.toCharArray(arr, 50) ;
EsploraTFT.text(arr,100,60);
  
  EsploraTFT.stroke(0,0,255);
  EsploraTFT.setTextSize(5);
  EsploraTFT.text("OVER",10,80);
  EsploraTFT.stroke(0,255,0);
  EsploraTFT.text("OVER",11,81);
  EsploraTFT.stroke(255,0,0);
  EsploraTFT.text("OVER",12,82);
 
  
  
  }





// funkce posune zapsané bloky o řadu níž
void sort_row(int row)
{
int x;
char tmp;
 for(row=row;row>0;row--)
  {
   for(x=0;x<10;x++)
    {
      tmp=fields[row-1][x];
      fields[row][x]=tmp;
    }
    
  }
  
}
// funkce zjistí zda je celá řada plná
void resolve_row(void)
{
 int x,y,count=0;
for(y=0;y<20;y++)
  {
  for(x=0;x<10;x++)
  {
    if(fields[y][x]!=0)
      {
      count++;
      }
  }
    if (count==10)
    {
      Esplora.tone(3000,60);
      delay(60);
      sort_row(y);
      score++;
      draw_score();
      
      draw_all();
      if(score%3==0)
      {
      level++;
      draw_level();
      difficuty*=0.9;
      }
    }
    count=0;
  }

}
// funkce vykreslí zapsahá pole
void draw_all(void)
{
int x,y;
EsploraTFT.stroke(255,255,255);
EsploraTFT.fill(0,0,0);
EsploraTFT.rect(59,4,62,122);
  for(y=0;y<20;y++)
  {
  for(x=0;x<10;x++)
  {
  if(fields[y][x]!=0)
  {
  draw_square(x,y);
  }}}
 }
// funkce vykreslí 1 čtvereček
void draw_square(int wid, int hei)
{
 char col=fields[hei][wid];
  if(col==1)
{
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(255,0,0);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
}
if(col==2)
{
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(0,255,0);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
}
if(col==3)
{
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(0,0,255);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
}
  
 }
// funkce vykreslí čtvereček padajícího bloku
 void draw_square_block(int wid, int hei)
{

  switch (COLOUR)
   {
      case RED:
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(255,0,0);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
  break;
   case GREEN:
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(0,255,0);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
  break;
   case BLUE:
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(0,0,255);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
  break;
   }
 }

  void draw_next_block_square(int wid, int hei)
{

  switch (NEXT_COLOUR)
   {
      case RED:
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(255,0,0);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
  break;
   case GREEN:
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(0,255,0);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
  break;
   case BLUE:
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.fill(0,0,255);
  EsploraTFT.rect(wid*5+60+wid,hei*5+5+hei,6,6);
  break;
   }
 }


 
// funkce vykreslí padající blok
void draw_block(void)
{
  draw_square_block(block.a[0],block.a[1]);
  draw_square_block(block.b[0],block.b[1]);
  draw_square_block(block.c[0],block.c[1]);
  draw_square_block(block.d[0],block.d[1]);
  }


// funkce umaže blok ny dyspleji
void erase_block (int wid, int hei)
{
  EsploraTFT.stroke(0,0,0);
  EsploraTFT.fill(0,0,0);
  EsploraTFT.rect(wid*5+60+wid,hei*5+hei+5,6,6);
  
  
  
}

// funkce zapíše padající blok do pole zapsaných bloků a zjistí zda není blok ve vrchní řadě (game over)
void write_block(void)
{
  int i=0;
  int count=0;
  Esplora.tone(500,100);
  delay(100);
switch (COLOUR)
   {
      case RED:
  fields[block.a[1]][block.a[0]]=1;
  fields[block.b[1]][block.b[0]]=1;
  fields[block.c[1]][block.c[0]]=1;
  fields[block.d[1]][block.d[0]]=1;
  break;
      case GREEN:
  fields[block.a[1]][block.a[0]]=2;
  fields[block.b[1]][block.b[0]]=2;
  fields[block.c[1]][block.c[0]]=2;
  fields[block.d[1]][block.d[0]]=2;
  break;
      case BLUE:
  fields[block.a[1]][block.a[0]]=3;
  fields[block.b[1]][block.b[0]]=3;
  fields[block.c[1]][block.c[0]]=3;
  fields[block.d[1]][block.d[0]]=3;
  break;
  
   }
for(i=0;i<10;i++)
count+=fields[0][i];

if (count!=0)
GAME=OVER;
COLOUR=NEXT_COLOUR;
   
}

// funkce zjistí zda se padající blok může posunot o řadu níže
bool check_block_down(void)
{
  int a=fields[block.d[1]+1][block.d[0]];
  int b=fields[block.c[1]+1][block.c[0]];
  int c=fields[block.b[1]+1][block.b[0]];
  int d=fields[block.a[1]+1][block.a[0]];
  if(a!=0 || b!=0 || c!=0 || d!=0)
  return false;
  else
  return true;
}
// funkce zjistí zda jde padající blok otočit
bool check_block_dummy(void)
{
  int a=fields[dummy.d[1]][dummy.d[0]];
  int b=fields[dummy.c[1]][dummy.c[0]];
  int c=fields[dummy.b[1]][dummy.b[0]];
  int d=fields[dummy.a[1]][dummy.a[0]];
 
  if(dummy.a[0]==10 || dummy.b[0]==10 || dummy.c[0]==10 || dummy.d[0]==10 || dummy.a[1]==-1 || dummy.b[1]==-1 || dummy.c[1]==-1 || dummy.d[1]==-1)
  return false;
  
  
  
  if(a!=0 || b!=0 || c!=0 || d!=0)
  return false;
  else
  return true;
}
// funkce zjistí zda se může padající blok posunout o sloupec vlevo
bool check_block_left(void)
{
  int a=fields[block.d[1]][block.d[0]-1];
  int b=fields[block.c[1]][block.c[0]-1];
  int c=fields[block.b[1]][block.b[0]-1];
  int d=fields[block.a[1]][block.a[0]-1];
 
  if(block.a[0]==0 || block.b[0]==0 || block.c[0]==0 || block.d[0]==0)
  return false;
  
  
  
  if(a!=0 || b!=0 || c!=0 || d!=0)
  return false;
  else
  return true;
}
// funkce zjistí zda se může padající blok posunout o sloupec doprava
bool check_block_right(void)
{
  int a=fields[block.d[1]][block.d[0]+1];
  int b=fields[block.c[1]][block.c[0]+1];
  int c=fields[block.b[1]][block.b[0]+1];
  int d=fields[block.a[1]][block.a[0]+1];
 
  if(block.a[0]==9 || block.b[0]==9 || block.c[0]==9 || block.d[0]==9)
  return false;
  
  
  
  if(a!=0 || b!=0 || c!=0 || d!=0)
  return false;
  else
  return true;
}
// smaže z displeje padající blok
void erase_full_block(void)
{
  
erase_block(block.a[0],block.a[1]);
erase_block(block.b[0],block.b[1]);
erase_block(block.c[0],block.c[1]);
erase_block(block.d[0],block.d[1]);


}

// funkce posune padající blok o řadu níž
void move_block_down(void)
{
  Esplora.tone(200,15);
erase_full_block(); 
block.a[1]+=1;
block.b[1]+=1;
block.c[1]+=1;
block.d[1]+=1;
block.pivot[1]+=1;

}
// funkce posune padající blok o sloupec vlevo
void move_block_left(void)
{
  
erase_full_block(); 
block.a[0]-=1;
block.b[0]-=1;
block.c[0]-=1;
block.d[0]-=1;
block.pivot[0]-=1;

}
// funkce posune padající blok o sloupec vpravo
void move_block_right(void)
{
  
erase_full_block(); 
block.a[0]+=1;
block.b[0]+=1;
block.c[0]+=1;
block.d[0]+=1;
block.pivot[0]+=1;

}

// funkce vygeneruje nový padající blok
void make_new_block (void)
{
  
  if (block_wait%3==0)
  NEXT_COLOUR=RED;
  if (block_wait%3==1)
  NEXT_COLOUR=GREEN;
  if (block_wait%3==2)
  NEXT_COLOUR=BLUE;
  
  
  
  if(block_wait%4==0)
  {
    STATE=CUBE;
block.a[0]=4;
block.a[1]=0;
block.b[0]=5;
block.b[1]=0;
block.c[0]=4;
block.c[1]=1;
block.d[0]=5;
block.d[1]=1;
block.pivot[0]=4;
block.pivot[1]=1;
  }

  if(block_wait%4==1)
  {
    STATE=LINE_STRAIGHT;
block.a[0]=4;
block.a[1]=0;
block.b[0]=5;
block.b[1]=0;
block.c[0]=6;
block.c[1]=0;
block.d[0]=7;
block.d[1]=0;
block.pivot[0]=4;
block.pivot[1]=0;
  }
  if(block_wait%4==2)
  {
    STATE=HORSEY_STRAIGHT;
block.a[0]=4;
block.a[1]=0;
block.b[0]=5;
block.b[1]=0;
block.c[0]=5;
block.c[1]=1;
block.d[0]=6;
block.d[1]=1;
block.pivot[0]=4;
block.pivot[1]=1;
  }

    if(block_wait%4==3)
  {
    STATE=SHIP_UP;
block.a[0]=4;
block.a[1]=1;
block.b[0]=5;
block.b[1]=0;
block.c[0]=5;
block.c[1]=1;
block.d[0]=6;
block.d[1]=1;
block.pivot[0]=4;
block.pivot[1]=1;
  }



draw_block();
block_wait=rand();
draw_next_block();





  
  }

// funkce provede kontrolu posunutí dolů a posune blok
bool block_down_action(void)
{
  if(check_block_down())
  {
    move_block_down();
    draw_block();
    return true;
    }
  else
  {
  write_block();
make_new_block();
return false;

  }
  
}
// funkce provede kontrolu posunutí vlevo a posune block
void block_left_action(void)
{
  if(check_block_left())
  {
    move_block_left();
    draw_block();
    }
  
  
}
// funkce provede kontrolu posunutí vpravo a posune block
void block_right_action(void)
{
  if(check_block_right())
  {
    move_block_right();
    draw_block();
    }
  
  
}

// funkce otočí blok podle pivotu
void flip_block(void)
{
int pivot_x=block.pivot[0];
int pivot_y=block.pivot[1];

 
  switch (STATE)
   {
      case CUBE:
       NEXT_STATE=CUBE;                    
          break;
      case LINE_STRAIGHT:   
                  
dummy.a[0]=pivot_x;
dummy.a[1]=pivot_y;
dummy.b[0]=pivot_x;
dummy.b[1]=pivot_y-1;
dummy.c[0]=pivot_x;
dummy.c[1]=pivot_y-2;
dummy.d[0]=pivot_x;
dummy.d[1]=pivot_y-3;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
   block=dummy;
  draw_block();
  NEXT_STATE=LINE_UP;
}
else
NEXT_STATE=LINE_STRAIGHT;
          break;      

case LINE_UP:
                       
dummy.a[0]=pivot_x;
dummy.a[1]=pivot_y;
dummy.b[0]=pivot_x+1;
dummy.b[1]=pivot_y;
dummy.c[0]=pivot_x+2;
dummy.c[1]=pivot_y;
dummy.d[0]=pivot_x+3;
dummy.d[1]=pivot_y;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
  block=dummy;
  draw_block();
  NEXT_STATE=LINE_STRAIGHT;
}
else
NEXT_STATE=LINE_UP;
          break;     

case SHIP_UP:   
                  
dummy.a[0]=pivot_x+1;
dummy.a[1]=pivot_y;
dummy.b[0]=pivot_x+1;
dummy.b[1]=pivot_y-1;
dummy.c[0]=pivot_x+1;
dummy.c[1]=pivot_y-2;
dummy.d[0]=pivot_x;
dummy.d[1]=pivot_y-1;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
   block=dummy;
  draw_block();
  NEXT_STATE=SHIP_LEFT;
}
else
NEXT_STATE=SHIP_UP;
          break;
case SHIP_LEFT:   
                  
dummy.a[0]=pivot_x+1;
dummy.a[1]=pivot_y;
dummy.b[0]=pivot_x;
dummy.b[1]=pivot_y-1;
dummy.c[0]=pivot_x+1;
dummy.c[1]=pivot_y-1;
dummy.d[0]=pivot_x+2;
dummy.d[1]=pivot_y-1;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
   block=dummy;
  draw_block();
  NEXT_STATE=SHIP_DOWN;
}
else
NEXT_STATE=SHIP_LEFT;
          break; 

 case SHIP_DOWN:   
                  
dummy.a[0]=pivot_x;
dummy.a[1]=pivot_y;
dummy.b[0]=pivot_x;
dummy.b[1]=pivot_y-1;
dummy.c[0]=pivot_x;
dummy.c[1]=pivot_y-2;
dummy.d[0]=pivot_x+1;
dummy.d[1]=pivot_y-1;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
   block=dummy;
  draw_block();
  NEXT_STATE=SHIP_RIGHT;
}
else
NEXT_STATE=SHIP_DOWN;
          break; 

case SHIP_RIGHT:   
                  
dummy.a[0]=pivot_x;
dummy.a[1]=pivot_y;
dummy.b[0]=pivot_x+1;
dummy.b[1]=pivot_y;
dummy.c[0]=pivot_x+2;
dummy.c[1]=pivot_y;
dummy.d[0]=pivot_x+1;
dummy.d[1]=pivot_y-1;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
   block=dummy;
  draw_block();
  NEXT_STATE=SHIP_UP;
}
else
NEXT_STATE=SHIP_RIGHT;
          break; 
          


             case HORSEY_STRAIGHT:   
                  
dummy.a[0]=pivot_x;
dummy.a[1]=pivot_y;
dummy.b[0]=pivot_x;
dummy.b[1]=pivot_y-1;
dummy.c[0]=pivot_x+1;
dummy.c[1]=pivot_y-1;
dummy.d[0]=pivot_x+1;
dummy.d[1]=pivot_y-2;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
   block=dummy;
  draw_block();
  NEXT_STATE=HORSEY_UP;
}
else
NEXT_STATE=HORSEY_STRAIGHT;
          break; 
  case HORSEY_UP:   
                  
dummy.a[0]=pivot_x;
dummy.a[1]=pivot_y-1;
dummy.b[0]=pivot_x+1;
dummy.b[1]=pivot_y;
dummy.c[0]=pivot_x+1;
dummy.c[1]=pivot_y-1;
dummy.d[0]=pivot_x+2;
dummy.d[1]=pivot_y;
dummy.pivot[0]=pivot_x;
dummy.pivot[1]=pivot_y;

if(check_block_dummy())
{
  erase_full_block(); 
   block=dummy;
  draw_block();
  NEXT_STATE=HORSEY_STRAIGHT;
}
else
NEXT_STATE=HORSEY_UP;
          break; 
     
}
 STATE=NEXT_STATE; 
  }


void draw_next_block(void)
{

EsploraTFT.stroke(20,40,60);
EsploraTFT.fill(20,40,60);
 EsploraTFT.rect(0,50,50,50);
 EsploraTFT.stroke(255,255,255);
 EsploraTFT.text("Next block",0,50);
if(block_wait%4==0)
{
draw_next_block_square(-7,9);
draw_next_block_square(-8,9);
draw_next_block_square(-7,10);
draw_next_block_square(-8,10);
}
else if(block_wait%4==1)
{
draw_next_block_square(-7,10);
draw_next_block_square(-6,10);
draw_next_block_square(-5,10);
draw_next_block_square(-8,10);
}
else if(block_wait%4==2)
{
draw_next_block_square(-9,9);
draw_next_block_square(-8,9);
draw_next_block_square(-8,10);
draw_next_block_square(-7,10);
}
else if(block_wait%4==3)
{
draw_next_block_square(-7,10);
draw_next_block_square(-6,10);
draw_next_block_square(-7,9);
draw_next_block_square(-8,10);
}
}

void setup() 
{
  Serial.begin(9600);
  // inicializace displeje
  EsploraTFT.begin();
// nastavení pozadí a nastavení stavu na START
EsploraTFT.background(20,40,60);
GAME=START;
MENU=MENU_START;
COLOUR=RED;
draw_start();


}

void loop() 
{
long unsigned int currtime=millis();


// po stisknutí tlačítka se hra posune do dostavu IN_PROGRESS ve kterém se po stisknutí tlačítka
// provádí button_actions
switch (GAME)
   {
      case IN_PROGRESS:
  resolve_row();
if(buttonEvent(UP))
flip_block();

if(buttonEvent(DOWN))
while(block_down_action())
{delay(15);}

if(buttonEvent(LEFT))
block_left_action();

if(buttonEvent(RIGHT))
block_right_action();

if(currtime>(downtime+difficuty))
{
block_down_action();
downtime=currtime;
}
break;

case OVER:
EsploraTFT.background(20,40,60);
draw_game_over();
delay(3000);
rewrite_score();
asm volatile ("jmp 0"); 
break;

case START:

if(buttonEvent(DOWN))
{
EsploraTFT.background(20,40,60);
switch (MENU)
{
  case MENU_START:
GAME=IN_PROGRESS;
draw_all();
draw_score();
draw_level();
make_new_block();
break;
case MENU_LEADERBOARD:
draw_scoreboard();
 break; 
}
}
if(buttonEvent(RIGHT))
{
switch (MENU)
case MENU_START:
{
MENU=MENU_LEADERBOARD;
break;
case MENU_LEADERBOARD:
MENU=MENU_START;
break;
}
draw_start();
}
break;


  
   }   

  
   
   }

