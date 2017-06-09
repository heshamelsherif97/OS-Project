void printString(char*);
void executeProgram(char*);
int founds(char*, char*);
void readFile(char*, char*);
void terminate();
int Mod(int, int);
int Div(int, int);
char buffer[512];
void writeFile(char*, char* , int);
void readSector(char*, int);
void writeSector(char*, int);
void readString(char*);
void handleInterrupt(int, int, int, int);
void deleteFile(char*);
int found2(char*, char*);
void getDir();
void printChar(char);
void handleTimerInterrupt(int , int );
void killProcess(int);
int active[8];
int stackPointer[8];
int currentProcess;
int count;
int finished;
main()
{
// int i=0;
// char buffer1[13312];
// char buffer2[13312];
// buffer2[0]='h';
// buffer2[1]='e';
// buffer2[2]='l';
// buffer2[3]='l';
// buffer2[4]='o';
// for(i=5; i<13312; i++) buffer2[i]=0x0;
// makeInterrupt21();
// interrupt(0x21,8, "testW2\0", buffer2, 1); //write file testW
// interrupt(0x21,3, "testW2\0", buffer1, 0); //read file testW
// interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW
// while(1);
// char buffer[13312];
// makeInterrupt21();
// // interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
// interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
// interrupt(0x21, 0, buffer, 0, 0);
// while(1);
// //while1(1); //print out the contents of buffer
	char buff[13312];
	char b[512];
	int i=0;
	for(i=0;i<8;i++)
	{
		// printString("zerooos ");
		active[i]=0;
		stackPointer[i]= 0xFF00;
	}
	currentProcess=0;
	count=0;
	makeInterrupt21();
	// makeTimerInterrupt();

	// interrupt(0x21, 4, "hello1\0", 0, 0);
	// interrupt(0x21, 4, "hello2\0", 0, 0);

	// makeTimerInterrupt();

	interrupt(0x21, 4, "shell\0", 0x2000, 0);
	makeTimerInterrupt();


    while(1);

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
else if(ax==3){
readFile(bx, cx);
}
else if(ax==4){
executeProgram(bx);//shelt cx
}
else if(ax==5){
terminate();
}
else if(ax==6){
writeSector(bx, cx);
}
else if(ax==7){
deleteFile(bx);
}else if(ax==8){
writeFile(bx, cx, dx);
}
else if(ax==9){
getDir();
}else if(ax=10){
	// printString("d5l 21");
	killProcess(bx);
}
else{
printString("Error \0");
}
}

int strComp(char* buffer, char* fileName){
	int i, j;

	int checkFound = 0;


     for (i = 0; i < 16; i++)
 		{
		 /* If there is a file at the location */
		if (buffer[32*i] != 0x0){
			/* Check the first characters */
			for (j=0; j < 6; j++){
				/* This is a match */
				/*printString("Checking: ");
				printChar(buffer[j+32*i]);
				printString(" with ");
				printChar(fileName[j]);*/

				if (buffer[j+32*1] == 0x0 || buffer[j+32*1] == '\r' || buffer[j+32*1] == '\n' || fileName[j] == 0x0 || fileName[j] == '\r' || fileName[j] == '\n'){
					break;
				}
				else if (buffer[j+32*i] == fileName[j]){
					checkFound = 1;
				}
				else {
					checkFound = 0;
					break;
				}

			}

			if (checkFound == 1){

				 return i;
			}
			else{
				/*printString("Next check");*/

			}
		}
		}
			 if (checkFound == 0){
				 for (i=0;i<13312;i++){
					buffer[i] = 0x0;
				 }


				return 0;
			 }



}

void readFile(char* fileName,char* buffer){
    int fileFound;
    int nameCt = 0;
	int index, k,h;
	int sectors[27];
	int j = 0;
	int i;
	int buffAddress = 0;


    /* Read in the directory sector */
    readSector(buffer, 2);

    /* Try to find the file name */
	fileFound = strComp(buffer,fileName);

	if (fileFound!=0){

		/* 1.Using the sector numbers in the directory, load the file, sector by sector, into the buffer array.
			You should add 512 to the buffer address every time you call readSector*/
		/* Get the line that the file info is on from the buffer */
		index = fileFound*32+6;
		for (j=0;j<26;j++){
			sectors[j] = buffer[index+j];

		}

		sectors[26] = 0;
		k = 0;
		while(sectors[k]!=0x0){
			readSector(buffer+buffAddress,sectors[k]);
			buffAddress += 512;
			k++;
		}

	}
	else{
		printString("File Not Found!");
		return;
	}

}

void executeProgram(char* name){
  int i;
	int j=0;
  int address;
	int segment;
  char buffer[13312];


setKernelDataSegment();
	while(j<8){
		if(active[j]==0){
			active[j]=1;
			segment=j+2;
			segment= segment*0x1000;
			//currentProcess=j;
			// printString("found an in active entry \0");
			// interrupt(0x10, 0xE*256+j+'0');

			break;

		}
		//restoreDataSegment();
		j++;
	}
	restoreDataSegment();
	if (j < 8) {
		readFile(name, buffer);


	  for(i=0; i<13312; i++){
	    putInMemory(segment, i, buffer[i]);
	  }
	  // launchProgram(segment);
		initializeProgram(segment);
	}



	//code trial
	// int segment;
	// 	char buffer[4096];
	// 	int i,loc;
	// 	// finish =0;
	// 	for(i=0;i<8;i++)
	// 	{
	// 		setKernelDataSegment();
	// 		if(active[i]==0)
	// 		{
	// 			restoreDataSegment();
	// 			loc=i;
	// 			break;
	// 		}
	// 		restoreDataSegment();
	//
	// 	}
	// 	segment=(loc*0x1000)+0x2000;
	// 	readFile(name,buffer);
	//
	// 	for(i=0;i<4096;i++)
	// 	{
	// 		putInMemory(segment,i,buffer[i]);
	//
	// 	}//end of for
	//   	setKernelDataSegment();
	// 	active[loc]=1;
	// 	printString("I am out of the second loop\n\r ");
	//  	restoreDataSegment();
	//
	// 	//    launchProgram(segment);
	// 	initializeProgram(segment);

}

void terminate(){
  // char shell[6];
	// shell[0] = 's';
	// shell[1] = 'h';
	// shell[2] = 'e';
	// shell[3] = 'l';
	// shell[4] = 'l';
	// shell[5] = 0x0;
	// interrupt(0x21,4,shell,0x2000,0);
	setKernelDataSegment();
	active[currentProcess]=0;
	restoreDataSegment();
	while(1);

}

void writeSector(char* buffer, int sector){
int relative_sector= Mod(sector, 18) +1 ;
int temp= Div(sector, 18);
int head=Mod(temp, 2);
int track=Div(sector, 36);
interrupt(0x13, 3*256+1, buffer, track*256+relative_sector, head*256+0);
}

void deleteFile(char* name){
	char map[512];
	char directory[512];
	int sectors[27];
	int count = 0;
	int i;
	int j;
	int k;
	int found;
	int sectNum;
	int sectorNumber;

	readSector(map,1);
	readSector(directory,2);


	found = found2(directory,name);

	if (found!=0){
		for(i=0;i<6;i++){
			directory[found+i] = 0x00;
		}
		found = found+6;
		for (j=0;j<26;j++){
			sectors[j] = directory[found+j];
			directory[found+j] = 0x00;
			count++;

		}
		sectors[26] = 0x0;
		for(k=0;k<count;k++){
			sectorNumber = sectors[k];
			if(sectorNumber == 0)
				break;
			map[sectorNumber] = 0x00;
		}
	}
	else{
		printString("Cannot find file!");
	}

	writeSector(map,1);
	writeSector(directory,2);
}

int found2(char* buffer, char* fileName){
	int i, j;

	int checkFound = 0;


     for (i = 0; i < 512; i+=32)
 		{
		if (buffer[i] != 0x0){
			for (j=0; j < 6; j++){

				 if (buffer[j+i] == fileName[j]){
					checkFound = 1;
				}
				else {
					checkFound = 0;
					break;
				}

			}

			if (checkFound == 1){

				 return i;
			}

		}
		}



				return 0;
			 }

void writeFile(char* name, char* buffer, int secNum) {
  char map[512];
	char directory[512];
  int dir;
  int found = 0;
  int length = 0;
  int temp=0;
  int i=0;
  int sectors=0;
  int x;
  int j;
  char buff[512];
  int k;

  readSector(map,1);
	readSector(directory,2);


  for (dir = 0; dir < 16; dir++){

		if (directory[32*dir] == 0x00){
			found = 1;
			break;
		}
}

if (found == 0){
		printString("No empty Location.");
		return;
	}
    while(name[length] != '\0' && name[length] != 0x0){
		length++;
	}

  for (j=0;j<length;j++){
		directory[32*dir+j] = name[j];
	}


	if (length < 6){
		temp = 6-length;
		for(j=0;j<temp;j++){
			i = j+length;
			directory[32*dir+i] = 0x0;
		}
	}




  for (k = 0; k < secNum; k++){


  		sectors=0;
  		while(map[sectors] != 0x0){
  			sectors++;
  		}
  		if (sectors==26)
  		{
        printString("no space found\n");
  			return;
  		}

  		map[sectors] = 0xFF;

  		directory[32*dir+6+k] = sectors;
  		for(j=0;j<512;j++){
  			x = k+1;
  			buff[j] = buffer[j*x];
  		}
  		writeSector(buff,sectors);
  	}

    writeSector(map,1);
    writeSector(directory,2);

}


void getDir(){
	char buff[512];
	char dirBuff[512];
	int i,j;
	int index = 0;
	int jj=0;
	int sectorNumber=0;
	int size;
	char* s[20];
	int tmp1;
	int tmp2;
	int k;
	int x;
	char t;
	for(i=0;i<512;i++){
		buff[i] = 0x0;
		dirBuff[i]=0x0;
	}
    readSector(dirBuff, 2);
	for(i=0;i<16;i++){
		if (dirBuff[32*i] != 0x0){
			for (j=0; j < 6; j++){
				buff[index] = dirBuff[j+32*i];
				index++;
				jj=j;
			}
			//trial for size
			while (dirBuff[jj+32*i] != 0x0 && (jj+32*i <32) ){
				sectorNumber++;
				jj++;
				// buff[index] = dirBuff[j+32*i];
				// index++;
			}
			// size=sectorNumber*512;
			// tmp1=size;
			// while(tmp1>0){
			// tmp2= Mod(tmp1,10);
			// t=tmp2;
			// // t+='\0';
			// s[k]=t;
			// k++;
			// tmp1=Div(tmp1,10);
			//
			// }
			// s[k]='\0';
			// k=0;
			// sectorNumber=0;

			// buff[index] = '\r';
			// index++;
			// buff[index] = '\n';
			// index++;

			// buff[index] = s[0];//add size to the buff
			// index++;
			// size=0;
			buff[index] = '\r';
			index++;
			buff[index] = '\n';
			index++;
		}
	}
	printString("Error \0");
	for(i=0;i<512;i++){
		printChar(buff[i]);
	}
	// for(i=0;i<20;i++){
	// 	printChar(s[i]);
	// }
	return;

}
void printChar(char ch){
	char* chars[2];
	chars[0] = ch;
	chars[1] = '\0';
	printString(chars);
}



void handleTimerInterrupt(int segment, int sp){

	int i=0;
	int f=0;

// printString("tic");
	//setKernelDataSegment();

	///=--------
	if(finished ){
if(count==99){


	stackPointer[currentProcess]=sp;
	for(i=currentProcess+1 ;i<8;i++)
	{
		// printString("1st loop ");
		if(active[i]==1)
		{
			// printString("wasaaaal 1 ");
			currentProcess=i;
			sp=stackPointer[i];
			f=1;

			break;
		}


	}
	//restoreDataSegment();
	// printString("5arag mn 1 ");
	if(f==0){
		//setKernelDataSegment();
		for (i = 0; i < currentProcess; i++) {
			// printString("2nd loop ");

			if(active[i]==1)
			{
				// printString("wasaaaal 2 ");
			currentProcess=i;
			sp=stackPointer[i];
			break;
		}
		}
	}
	segment=currentProcess+2;
	segment=segment*0x1000;
	count=0;
	returnFromTimer(segment,sp);
}else{
	count++;
	returnFromTimer(segment,sp);
}

}else{

	finished=1;
	returnFromTimer(0x2000, 0xff00);
}
//restoreDataSegment();
// if(finished==0){
// 	finished=1;
// 	returnFromTimer(0x2000, 0xff00);
// }else{
//
// if(count<100){
// // printString("handling");
// if(segment==0x1000)
// 	{
// 		i=0;
// 		while(i==Mod(i,8))
//         {
// 				// printString("loop 1 ");
// 			// setKernelDataSegment();
// 			if(active[i]==1)
// 			{
// 				// printString("la2a process active f 1 ");
// 				currentProcess=i;
// 				sp=active[1];
// 				// restoreDataSegment();
// 				break;
// 			}
// 			i++;
//         }
// 				// printString(" segment set");
// 				segment=(currentProcess*0x1000)+0x2000;
//         // restoreDataSegment();
// 	}else {
// 		// setKernelDataSegment();
// 		stackPointer[currentProcess]=sp;
// 		i=currentProcess;
//         // restoreDataSegment();
// 				i++;
// 		while(i==Mod(i,8))
// 		{
// 			// printString("loop 2 ");
// 			// setKernelDataSegment();
// 			if(active[i]==1)
// 			{
// 				// printString("laa2a process active f 2 ");
// 				currentProcess=i;
// 				sp=stackPointer[i];
// 				// restoreDataSegment();
// 				break;
// 			}
// 			i++;
// 		}
//         // setKernelDataSegment();
// 				// printString(" segment set");
// 		segment=currentProcess+2;
// 		segment=segment * 0x1000;
// 		// restoreDataSegment();
// 		count++;
// 	}
//
// }
//
//
// 	returnFromTimer(segment, sp);
// }
}
void killProcess(int i){
// printString("killinggggg");
setKernelDataSegment();
active[i]=0;
restoreDataSegment();
}
