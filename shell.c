void parseInput(char buffer);
int Div(int, int);
void clear(char*, int);
main()
{
while(1){
char buffer[512];
//interrupt(0x21, 4, "hello1\0", 0, 0);
interrupt(0x21, 0, "\r\n", 0, 0);
interrupt(0x21, 0, "SHELL:>", 0, 0);
interrupt(0x21, 1, buffer, 0, 0);
parseInput(buffer);

}
}
void parseInput(char* buff){
int indexIn;
int index;
int len;
int input = 1;
int lnCt=0;
int createIndex = 0;
char grabBuff[180];
char createBuff[512];
int sector;
char fileName[7];
char fileName2[7];
char fileBuff[13312];
int i;
	for (indexIn = 0; indexIn <180; indexIn++){

		if(buff[indexIn] == 0xd){
			break;
					}
		if( buff[indexIn]=='v' && buff[indexIn+1]=='i' && buff[indexIn+2]=='e' && buff[indexIn+3]=='w')
			{
			indexIn=indexIn+5;
		for(i=0;i<6;i++){
			fileName[i] = buff[indexIn+i];
		}

		fileName[6] = "\0";
		interrupt(0x21, 3, fileName, fileBuff, 0);
		interrupt(0x21,0,fileBuff,0,0);
break;
			}
		else if( buff[indexIn]=='e' && buff[indexIn+1]=='x' && buff[indexIn+2]=='e' && buff[indexIn+3]=='c' && buff[indexIn+4]=='u' && buff[indexIn+5]=='t' && buff[indexIn+6]=='e')
			{

			indexIn=indexIn+8;
		for(i=0;i<6;i++){
			fileName[i] = buff[indexIn+i];
		}

		fileName[6] = "\0";
		interrupt(0x21, 4, fileName, 0x2000, 0);
		//interrupt(0x21, 5, 0, 0, 0);
break;
			}
			else if( buff[indexIn]=='d' && buff[indexIn+1]=='e' && buff[indexIn+2]=='l' && buff[indexIn+3]=='e' && buff[indexIn+4]=='t' && buff[indexIn+5]=='e')
				{

				indexIn=indexIn+7;
			for(i=0;i<6;i++){
				fileName[i] = buff[indexIn+i];
			}

			fileName[6] = "\0";
			interrupt(0x21, 7, fileName, 0x2000, 0);
			//interrupt(0x21, 5, 0, 0, 0);
	break;
				}
				else if( buff[indexIn]=='c' && buff[indexIn+1]=='o' && buff[indexIn+2]=='p' && buff[indexIn+3]=='y')
					{

					index=indexIn+5;
					i=0;
				while(buff[index] != 0x20 && buff[index] != 0x0){
					fileName[i] = buff[index];
					index++;
					i++;
				}
				fileName[6] = "\0";
				i=0;
				index++;
				while(buff[index] != 0x20 && buff[index] != 0x0){
				fileName2[i] = buff[index];
				index++;
				i++;
			  }
				fileName2[6] = "\0";

				interrupt(0x21, 3, fileName, fileBuff, 0);

				while(fileBuff[len] != 0x0){
					len++;
				}

				sector=Div(len, 512);
				interrupt(0x21, 8, fileName2, fileBuff, sector+1);
				//interrupt(0x21, 5, 0, 0, 0);
		break;
					}
					else if( buff[indexIn]=='d' && buff[indexIn+1]=='i' && buff[indexIn+2]=='r')
						{
							interrupt(0x21,9,0,0,0);
			break;
						}

						else if (buff[indexIn]=='c' && buff[indexIn+1]=='r' && buff[indexIn+2]=='e' && buff[indexIn+3]=='a'&& buff[indexIn+4]=='t' && buff[indexIn+5]=='e'){
		indexIn = indexIn + 7;
		for(i=0;i<6;i++){
			fileName[i] = buff[indexIn+i];
		}
		fileName[6] = "\0";

		while(input){
			/* Get user input */
			clear(grabBuff,180);

			interrupt(0x21,1,grabBuff,0,0);

			if (grabBuff[2] == 0x0){
				input = 0;
				break;
			}
			else{

				for(i=0;i<180;i++){
					if(grabBuff[i] == 0x0 || grabBuff[i] == '\0'||grabBuff[i]=='\r'||grabBuff[i]=='\n'||grabBuff[i]=='\t'){
						grabBuff[i] = 0x20;
					}
					createBuff[i+180*createIndex] = grabBuff[i];
				}
				lnCt++;
				createIndex++;
			}
		}

		sector = Div(lnCt, 512);
		/* Now create the file */
		interrupt(0x21,8,fileName,createBuff,lnCt);
		break;
	}else{
		if( buff[indexIn]=='k' && buff[indexIn+1]=='i' && buff[indexIn+2]=='l' && buff[indexIn+3]=='l')
			{
				// interrupt(0x21, 0, "\r\n wasaaaaaaaal!\r\n", 0, 0);
				interrupt(0x21,10,buff[indexIn+5]-48,0,0);
break;
			}else{
			interrupt(0x21, 0, "\r\n BAD COMMAND!\r\n", 0, 0);
			break;
			}
	}

	}
}

int Div(int x, int y){
int i=0;
while(x>=y){
x= x-y;
i++;
}
return i;
}
void clear(char* buff, int len){
	int i;
	for(i=0;i<len;i++){
		buff[i] = 0x0;
	}
}
