void printString(char*);
int Mod(int, int);
int Div(int, int);
char buffer[512];
void readSector(char*, int);
void readString(char*);
void handleInterrupt(int, int, int, int);
main()
{
char line[80];	
makeInterrupt21();
interrupt(0x21,1,line,0,0);
interrupt(0x21,0,line,0,0);
while (1>0){
}
}

void printString(char* chars){
while(*(chars) != '\0'){
interrupt(0x10, 0xE*256+*(chars), 0, 0, 0);
chars++;
}
}

int Mod(int x, int y){
while(x>=y){
x= x-y;
}
return x;
}

int Div(int x, int y){
int i=0;
while(x>=y){
x= x-y;
i++;
}
return i;
}


void readSector(char* buffer, int sector){
int relative_sector= Mod(sector, 18) +1 ;
int temp= Div(sector, 18);
int head=Mod(temp, 2);
int track=Div(sector, 36);
interrupt(0x13, 2*256+1, buffer, track*256+relative_sector, head*256+0);
}


void readString(char* line){
int i=0; 
char x=0x0;           //check size
while(x != 0xd){

x=interrupt(0x16, 0, 0, 0, 0 );
if(x == 0x8){
interrupt(0x10, 0xe*0x100+x, 0, 0, 0);
interrupt(0x10, 0xe*0x100+' ', 0, 0, 0);
interrupt(0x10, 0xe*0x100+x, 0, 0, 0);
i--;
line[i]="";
}
else{
line[i]= x;
interrupt(0x10, 0xe*0x100+x, 0, 0, 0);
i++;
}
}
line[i]=0xa;
interrupt(0x10, 0xe*0x100+0xa, 0, 0, 0);
line[i+1]=0x0;	
return ;
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
if(ax==0){
printString(bx);
}
else if(ax==1){
readString(bx);
}
else if(ax==2){
readSector(bx, cx);
}
else{
printString("Error \0");
}
}


