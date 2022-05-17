#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>

char *varList[9999]; /*This array is holding integer variables*/
int varValues[9999];//This array is holding values of integer variables
int varCount = 0;	/*This variable is holding integer variables count*/
int countOfChar = 0; //This variable holds the number of characters in the .ba file.
int readingChar = 0; //This variable holds which indexed character was read.
int lineCounter = 1; //This variable holds number of line
int isErrorFound = 0; //This variable is used to check if there is an error.

bool isInteger(char* intWord){ /*This method checks if the word is a integer.*/
	int lenInt = strlen(intWord);
	int i;
	
	if(lenInt > 100){  /*If length of word is above 100 digit, method return false*/
		return false;
	}
	
	if(lenInt == 1 && !strcmp(intWord,"-")){   /*If length of word is one and it is '-', method return false*/
		return false;
	}
	for (i = 0;i < lenInt ;i++){ /*Checking every digit*/
		if(i == 0){  /*If first char is not '-' or a digit, return false*/
			if(intWord[i] != '-' && !isdigit(intWord[i])){
				return false;
			}
		}
		else if(i > 0){	
			if(!isdigit(intWord[i])){
				return false;
			}
		}
	}
	
	return true;	
}

bool isVariable(char *var){/*This method checks if the word is a variable*/
	int lenVar = strlen(var);
	int i;
	
	if(lenVar>20){ /*If length of word is above 20 char, method return false*/
		return false;
	}
	for(i=0;i < lenVar;i++){ /*Checking every digit*/
		if(i==0 && !isalpha(var[i])){ /*If first char is not alphabethic,method return false*/
			return false;
		}
		if(i>0 && (!isalpha(var[i]) && !isdigit(var[i]) && var[i] != '_')){/*If other digit is not alphabethic or underline,method return false*/
			return false;
		}
	}
	return true;	
}

bool isVariableExist(char *var){ /*This method checks if the word is a predefined variable*/
	int i;
	for(i=0;i<varCount;i++){ /*Checking variable array*/
		if(!strcmp(var,varList[i])){   /*If found this,method return true*/
			return true;
		}
	}
	return false;
		
}

int getVariableValue(char *var){
	int i;
	for(i=0;i<varCount;i++){ /*Checking variable array*/
		if(!strcmp(var,varList[i])){   /*If found this,method return variable value*/
			return varValues[i];
		}
	}
}

void setVariableValue(char *var,char *keyword,int operatingValue){
	int i;
	for(i=0;i<varCount;i++){ /*Checking variable array*/
		if(!strcmp(var,varList[i])){  	//If found variable in list, function will control keyword variable
			if(!strcmp(keyword,"add")){   //If keyword is 'add', varValue =  varValue + opValue
				int sum = varValues[i] + operatingValue;
				varValues[i] = sum;
				break;
			}else if(!strcmp(keyword,"sub")){   //If keyword is 'add', varValue = varValue - opValue
				int sub = varValues[i] - operatingValue;
				varValues[i] = sub;
				break;
			}else if(!strcmp(keyword,"move")){   //If keyword is 'add', varValue = opValue
				varValues[i] = operatingValue;
				break;
			}
		}
	}
}
bool isKeyword(char* word) { //This method checks if the word is a keyword

	if (!strcmp(word, "int") || !strcmp(word, "move")  || !strcmp(word, "add") || !strcmp(word, "sub") || !strcmp(word, "to") || !strcmp(word, "from") ||
		!strcmp(word, "loop") || !strcmp(word, "times") || !strcmp(word, "out") || !strcmp(word, "newline")) {
		return true;
	}
	return false;
}

void codeBlockControl(char* allLines){ // This method checks if the code blocks are opened correctly.
	int controlIndex = 0;
	int openBlock = 0;
	int closeBlock = 0;
	while(controlIndex != countOfChar-1){//Code block control
		if(allLines[controlIndex] == '['){
			openBlock++;
		}else if(allLines[controlIndex] == ']'){
			closeBlock++;
		}
		controlIndex++;
	}
	
	if(openBlock != closeBlock){//If number of open block is not equal number of close block, error prints
		printf("\n%s\n","Error: The number of code blocks opened must be equal to the number of closed code blocks.");
		isErrorFound++;
		readingChar = countOfChar - 1;
	}
}
void commentProcess(char* allLines){ //This method control comment process
	if(allLines[readingChar] == ' '){//If program see a blank, will control another char
		do{
			readingChar++;
			if(allLines[readingChar] != ' '){
				break;
			}
		}while(true);
	} 
	if(allLines[readingChar] == '{'){//If found '{' sign, comment code pass until program see '}'
		do{
			if(allLines[readingChar] == '}'){
				readingChar++;
				break;
			}else if(allLines[readingChar] == '\0'){//If program did not see '}', Error prints on screen
				printf("\nLine %d | %s\n",lineCounter,"Error: An opened comment line was not closed... (Expected '}') ");
				isErrorFound++;
				readingChar = countOfChar-1;
				break;
			}
			if(allLines[readingChar] == '\n'){
				lineCounter++;
			}
			readingChar++;
		}while(true);
	}
}

bool isCharSign(char sign){ //This method control a char that is a sign or is not a sign
	if (sign == '[' || sign == ']' || sign == '.' || sign == ',' || sign == '{' || sign == '\n' || sign == '"' || sign == '\n' || sign == '}'){
		return true;
	}
	return false;
}

char* wordFound(char *allLines){//This method return a word or a sign in string form.
	char *word = calloc(countOfChar,sizeof(char)); //A dynamic memory is created that holds the word value.
	int wordIndex = 0;//Word index variable is creating for assing characters to string name of word respectively.
	do{//Loop work until a word or sign found
		if((allLines[readingChar] == ' ' || allLines[readingChar] == '\t') && wordIndex == 0){//If method see a tab or blank,pass this char and control another char
			do{
				readingChar++;
				if(allLines[readingChar] != ' ' || allLines[readingChar] != '\t'){
					break;
				}
			}while(true);
			
		}else if(isCharSign(allLines[readingChar]) && wordIndex == 0){//If method found a sign, return this sign 
			word[wordIndex] = allLines[readingChar];
			readingChar++;
			return word;
		}else if(allLines[readingChar] == '\0'){//If method found null char(end of .ba file), return null char and finish operation
			word[wordIndex] = allLines[readingChar];
			return word;
		}
		else if(isCharSign(allLines[readingChar])){//If method found a char but word is not empty, readingChar is waiting this sign and return word
			return word;
		}
		else if(allLines[readingChar] == ' '){//If method found a blank but word is not empty, passed blank and return word
			readingChar++;
			return word;
		}
		else if(readingChar < countOfChar){//If found a char except sign,blank or tab , this char include next index of word and control next char
			word[wordIndex] = allLines[readingChar];
			wordIndex++;
			readingChar++;
		}
	}while(true);
}

void parse(char *allLines){ //This method does all the parse operations.
	commentProcess(allLines);//Comment control
	char *word = calloc(countOfChar,sizeof(char));//A dynamic memory is created that holds the word value.
	word = wordFound(allLines);//wordFound method return a sign or word and hold word dynamic memory.
	
	//For int keyword
	if(!strcmp(word,"int")){//If word is 'int'
		commentProcess(allLines);//Comment control
		char *word = calloc(countOfChar,sizeof(char));//another dynamic memory is created for holding next word 
		word = wordFound(allLines);//wordFound method return a word
		if(isVariableExist(word)){//If word is a predefined variable,error prints on screen
			printf("\nLine %d | Error:'%s' is a predefined variable...\n",lineCounter,word);
			isErrorFound++;
			readingChar = countOfChar-1;
		}else if(isVariable(word)){//If word is a variable, process continue
			commentProcess(allLines);//Comment control
			char *var = word;//variable holds a string
			char *word = calloc(countOfChar,sizeof(char));//another dynamic memory is created and method called
			word = wordFound(allLines);
			if(!strcmp(word,".")){//If word is '.' 
				varList[varCount] = var;//This variable placed in the array
				varValues[varCount] = 0;
				varCount++;//Array count is increase one
				commentProcess(allLines); //Comment Control,
				do{//After '.' sign, line must finish. This loop control the line that is finish
					char *word = calloc(countOfChar,sizeof(char));//another dynamic memory is created and method called
					word = wordFound(allLines);
					if(!strcmp(word,"\n") || !strcmp(word,"\0")){//If found '\n' , lineCounter increments by 1 and loop finished
						lineCounter++;
						break;
				 	}
					else if(readingChar != countOfChar - 1){//If this line is not finished after '.' sign, error prints on screen
						printf("\nLine %d | %s\n",lineCounter,"Error: If the operation on a line is finished, nothing else is written on this line...");
						isErrorFound++;
						//If the operation on a line is finished, nothing else is written on this line.
						readingChar = countOfChar-1; // After error prints, readingChar move to end of file
						break;
					}
				}while(true);
			}else if(readingChar != countOfChar - 1){//If '.' sign is not found, error prints
				printf("\nLine %d | %s\n",lineCounter,"Error: Expected '.' after the correctly defined line...");//An error message is printed to the console because the line does not end with a dot.
				isErrorFound++;
				readingChar = countOfChar-1;
			}
		}else if(readingChar != countOfChar - 1){//If a variable did not come int keyword, error prints
			printf("\nLine %d | %s\n",lineCounter,"Error: After defined 'int' keyword, it must come a not predefined variable.");
			isErrorFound++;
			readingChar = countOfChar-1;
		}
	}
	//For add,sub,move keywords
	else if(!strcmp(word,"add") || !strcmp(word,"sub") || !strcmp(word,"move")){//If word is 'add', 'sub' or 'move'
		commentProcess(allLines);//Comment process
		char *keyword = word;//keyword variable holds keyword value
		char *word = calloc(countOfChar,sizeof(char));//next word
		word = wordFound(allLines);
		
		if(isVariableExist(word)){//word is a predefined variable
			commentProcess(allLines);//comment process
			int varValue = getVariableValue(word);//a int variable holds variable value
			char *word = calloc(countOfChar,sizeof(char));//next word
			word = wordFound(allLines);
			if(!strcmp(word,"to")){//word is 'to'
				commentProcess(allLines);//comment control
				char *word = calloc(countOfChar,sizeof(char));//next word
				word = wordFound(allLines);	
				if(isVariableExist(word)){//word is a predefined variable.
					commentProcess(allLines);//comment control
					char *var = word;//a string variable holds integer variable 
					char *word = calloc(countOfChar,sizeof(char));//next word
					word = wordFound(allLines);	
					if(!strcmp(word,".")){//word is '.'
						setVariableValue(var,keyword,varValue);//set variable value by keyword value.
						commentProcess(allLines);//Comment Process
						do{				//End of line control
							char *word = calloc(countOfChar,sizeof(char));
							word = wordFound(allLines);
							if(!strcmp(word,"\n") || !strcmp(word,"\0")){
								lineCounter++;
								break;
							}else if(readingChar != countOfChar - 1){
								printf("\nLine%d | %s\n",lineCounter,"Error: If the operation on a line is finished, nothing else is written on this line...");//If the operation on a line is finished, nothing else is written on this line.
								isErrorFound++;
								readingChar = countOfChar-1;
								break;
							}
						}while(true);
					}else if(readingChar != countOfChar - 1){
						printf("\nLine %d | %s\n",lineCounter,"Error: Expected '.' after the correctly defined line...");//An error message is printed to the console because the line does not end with a dot.
						isErrorFound++;
						readingChar = countOfChar-1;
					}
				}else if(readingChar != countOfChar - 1){//If a predefined variable did not come after 'to' keyword, error prints
					printf("\nLine %d | Error: '%s' is not a predefined variable...\n",lineCounter,word);
					isErrorFound++;
					readingChar = countOfChar-1;
				}	
			}else if(readingChar != countOfChar - 1){//If 'to' keyword did not come , error prints
				printf("\nLine %d | %s\n",lineCounter,"Error: Expected 'to' keyword...");// a error prints on the console
				isErrorFound++;
				readingChar = countOfChar-1;
			}
		}else if(isInteger(word)){//If a integer come after 'move' , 'add' or 'sub' keywords
			commentProcess(allLines);
			int intConst = atoi(word);//int constant value convert string to integer
			char *word = calloc(countOfChar,sizeof(char));//next word
			word = wordFound(allLines);
			if(!strcmp(word,"to")){			//THE SAME OPERATIONS WITH THE ABOVE OPERATIONS ARE ALSO DONE HERE.
				commentProcess(allLines);
				char *word = calloc(countOfChar,sizeof(char));
				word = wordFound(allLines);	
				if(isVariableExist(word)){
					commentProcess(allLines);
					char *var = word;
					char *word = calloc(countOfChar,sizeof(char));
					word = wordFound(allLines);	
					if(!strcmp(word,".")){
						setVariableValue(var,keyword,intConst);
						commentProcess(allLines);
						do{
							char *word = calloc(countOfChar,sizeof(char));
							word = wordFound(allLines);
							if(!strcmp(word,"\n") || !strcmp(word,"\0")){
								lineCounter++;
								break;
							}else if(readingChar != countOfChar - 1){
								printf("\nLine %d | %s\n",lineCounter,"Error: If the operation on a line is finished, nothing else is written on this line...");//If the operation on a line is finished, nothing else is written on this line.
								isErrorFound++;
								readingChar = countOfChar-1;
								break;
							}
						}while(true);
					}else if(readingChar != countOfChar - 1){
						printf("\nLine %d | %s\n",lineCounter,"Error: Expected '.' after the correctly defined line...");//An error message is printed to the console because the line does not end with a dot.
						isErrorFound++;
						readingChar = countOfChar-1;
					}
				}else if(readingChar != countOfChar - 1){ 
					printf("\nLine %d | Error: '%s' is not a predefined variable...\n",lineCounter,word);
					isErrorFound++;
					readingChar = countOfChar-1;
				}	
			}else if(readingChar != countOfChar - 1){
				printf("\nLine %d | %s\n",lineCounter,"Error: Expected 'to' keyword...");// a error prints on the console
				isErrorFound++;
				readingChar = countOfChar-1;
			}
			
		}else if(readingChar != countOfChar - 1){  //If a integer or predefined variable did not come after 'move' , 'add','sub' keyword, error prints
			printf("\nLine %d | Error: After '%s' keyword is defined, it must come a integer or predefined variable...\n",lineCounter,keyword);//Error prints on screen
			isErrorFound++;
			readingChar = countOfChar-1;
		}
	}
	
	
	//For loop keyword
	else if(!strcmp(word,"loop")){//If word is 'loop'
		commentProcess(allLines);//comment control
		char *word = calloc(countOfChar,sizeof(char));//next word
		word = wordFound(allLines);
		if(isVariableExist(word)){//If a predefined variable come after 'loop' keyword
			commentProcess(allLines);//comment control
			char *loopVar = word;//a string variable holds predefined variable 
			int loopTimes = getVariableValue(loopVar);//this variable holds value of predefined variable
			char *word = calloc(countOfChar,sizeof(char));
			word = wordFound(allLines);
			if(!strcmp(word,"times")){//If next word is 'times
				commentProcess(allLines);
				int returnLoopHead = readingChar;//This variable holds loop head for return where needed
				char *word = calloc(countOfChar,sizeof(char));
				word = wordFound(allLines);
				if(!strcmp(word,"\n")){ //If next word is '\n' , The loop operation is only done with code blocks.
					lineCounter++;// line counter incresed by 1
					char *word = calloc(countOfChar,sizeof(char));
					do{// end of line control
						commentProcess(allLines);
						word = wordFound(allLines);
						if(strcmp(word,"\n")){//If this word is not '\n' sign , loop break
							break;
						}
						lineCounter++; //If this word is '\n' sign, line counter incresed by 1
					}while(true);
					
					if(!strcmp(word,"[")){//If next word is '['
						commentProcess(allLines);
						returnLoopHead = readingChar; //new loop head adjust
						int countLoopTimes = 0;//This variable holds how many times looped.
						while(countLoopTimes < loopTimes){ 
							parse(allLines);//In a recursive way, the operations of the loop contents are performed.
							if(countLoopTimes != 0){//If countLoopTimes is not 0, line counter reduced by 1
								lineCounter--;
							}
							int controlIndex = readingChar;//This variable holds the starting index of the other process.
							if(allLines[readingChar] == '\0'){ // If a error found or come end of file, loop breaks
								break;
							}
							char *word = calloc(countOfChar,sizeof(char));
							word = wordFound(allLines);
							if(!strcmp(word,"]") && loopTimes-1 == countLoopTimes){//If this word ']' and last loop process is finished, loop breaks
								setVariableValue(loopVar,"sub",1);//loop variable decreased by 1 
								break;
							}else if(!strcmp(word,"]")){//If this word ']' and last loop process is not finished
								setVariableValue(loopVar,"sub",1);//loop variable decreased by 1 
								countLoopTimes++;//loop times increased by 1 
								readingChar = returnLoopHead;//reading char return to loop head index
							}
							else{//If this word is not ']'
								readingChar = controlIndex; // reading char return to starting index of the other process.
							}
						}
					}else if(readingChar != countOfChar - 1){ // If loop block was not opened , error prints
						printf("\nLine %d | %s\n",lineCounter,"Error: loop block was not opened...(Expected '[')");
						isErrorFound++;
						readingChar = countOfChar-1;
					}
				}else if(!strcmp(word,"[")){ 	//THE SAME OPERATIONS WITH THE ABOVE OPERATIONS ARE ALSO DONE HERE.
					commentProcess(allLines);
					returnLoopHead = readingChar;
					int countLoopTimes = 0;
					while(countLoopTimes < loopTimes){
						parse(allLines);
						if(countLoopTimes != 0){
							lineCounter--;
						}
						int controlIndex = readingChar;
						if(allLines[readingChar] == '\0'){
							break;
						}
						char *word = calloc(countOfChar,sizeof(char));
						word = wordFound(allLines);
						if(!strcmp(word,"]") && loopTimes-1 == countLoopTimes){
							setVariableValue(loopVar,"sub",1);//loop variable decreased by 1 
							break;
						}else if(!strcmp(word,"]")){
							setVariableValue(loopVar,"sub",1);
							countLoopTimes++;
							readingChar = returnLoopHead;
						}
						else{
							readingChar = controlIndex;
						}
					}
				}else if(isKeyword(word)){	//If '[' or '\n' sign did not come after loop definition, expected a keyword
					readingChar = returnLoopHead;
					int countLoopTimes = 0;	//THE SAME OPERATIONS WITH THE ABOVE OPERATIONS ARE ALSO DONE HERE.
					while(countLoopTimes < loopTimes){	//the only difference is that the code blocks are not checked and a single transaction is returned.
						parse(allLines);
						if(countLoopTimes != 0){
							lineCounter--;
						}
						if(isErrorFound != 0){ //If an error is found, the loop is stopped. We wrote some extra code so that this inner loop works better.
							break;
						}
						setVariableValue(loopVar,"sub",1);
						if(loopTimes-1 == countLoopTimes){
							break;
						}
						countLoopTimes++;
						readingChar = returnLoopHead;
					}
				}else if(readingChar != countOfChar - 1){//If loop structure was not created correctly , error prints
					printf("\nLine %d | %s\n",lineCounter,"Error: loop block was not created correctly.");
					isErrorFound++;
					readingChar = countOfChar-1;
				}
				
			}else if(readingChar != countOfChar - 1){ 
				printf("\nLine %d | %s\n",lineCounter,"Error: Expected 'times' keyword definition...");
				isErrorFound++;
				readingChar = countOfChar-1;
			}
		}else if(isInteger(word)){ //If a integer come after 'loop' keyword
			int loopTimes = atoi(word);//loop times converted string to integer and a integer variable holds this integer value.
			char *word = calloc(countOfChar,sizeof(char));	//THE SAME OPERATIONS WITH THE ABOVE OPERATIONS ARE ALSO DONE HERE.
			word = wordFound(allLines);
			if(!strcmp(word,"times")){
				commentProcess(allLines);
				int returnLoopHead = readingChar;
				char *word = calloc(countOfChar,sizeof(char));
				word = wordFound(allLines);
				if(!strcmp(word,"\n")){
					char *word = calloc(countOfChar,sizeof(char));
					do{
						commentProcess(allLines);
						word = wordFound(allLines);
						if(strcmp(word,"\n")){
							break;
						}	
					}while(true);
					
					if(!strcmp(word,"[")){
						returnLoopHead = readingChar;
						int countLoopTimes = 0;
						while(countLoopTimes < loopTimes){
							parse(allLines);
							if(countLoopTimes != 0){
								lineCounter--;
							}
							int controlIndex = readingChar;
							if(allLines[readingChar] == '\0'){
								break;
							}
							char *word = calloc(countOfChar,sizeof(char));
							word = wordFound(allLines);
							if(!strcmp(word,"]") && loopTimes-1 == countLoopTimes){
								break;
							}else if(!strcmp(word,"]")){
								countLoopTimes++;
								readingChar = returnLoopHead;
							}
							else{
								readingChar = controlIndex;
							}
						}
					}else if(readingChar != countOfChar - 1){
						printf("\nLine %d | %s\n",lineCounter,"Error: loop block was not opened...(Expected '[')");
						isErrorFound++;
						readingChar = countOfChar-1;
					}
				}else if(!strcmp(word,"[")){
					returnLoopHead = readingChar;
					int countLoopTimes = 0;
					while(countLoopTimes < loopTimes){
						parse(allLines);
						if(countLoopTimes != 0){
							lineCounter--;
						}
						int controlIndex = readingChar;
						if(allLines[readingChar] == '\0'){
							break;
						}
						char *word = calloc(countOfChar,sizeof(char));
						word = wordFound(allLines);
						if(!strcmp(word,"]") && loopTimes-1 == countLoopTimes){
							break;
						}else if(!strcmp(word,"]")){
							countLoopTimes++;
							readingChar = returnLoopHead;
						}
						else{
							readingChar = controlIndex;
						}
					}
				}else if(isKeyword(word)){
					readingChar = returnLoopHead;
					int countLoopTimes = 0;
					while(countLoopTimes < loopTimes){
						parse(allLines);
						if(countLoopTimes != 0){
							lineCounter--;
						}
						if(isErrorFound != 0){
							break;
						}
						if(loopTimes-1 == countLoopTimes){
							break;
						}
						countLoopTimes++;
						readingChar = returnLoopHead;
					}
				}else if(readingChar != countOfChar - 1){
					printf("\nLine %d | %s\n",lineCounter,"Error: loop block was not created correctly.");
					isErrorFound++;
					readingChar = countOfChar-1;
				}
				
			}else if(readingChar != countOfChar - 1){
				printf("\nLine %d | %s\n",lineCounter,"Error: Expected 'times' keyword definition...");
				isErrorFound++;
				readingChar = countOfChar-1;
			}
		}else if(readingChar != countOfChar - 1){//If a predefined variable or a integer value did not come after 'loop' keyword , error prints
			printf("\nLine %d | %s\n",lineCounter,"Error: After 'loop' keyword is defined, it must come a predefined variable or a integer value...");
			isErrorFound++;
			readingChar = countOfChar-1;
		}
	}
	
	
	//For out keyword
	else if(!strcmp(word,"out")){ // If word is 'out'
		commentProcess(allLines); //Comment Process
		char *outWords[countOfChar];// This array hold printing item
		int countOfOutWords = 0;//This variable holds count of printing items
		int i;//This variable was created for use in the for loop.
		
		do{	//Word control after 'out' keyword
			commentProcess(allLines);//comment control
			char *word = calloc(countOfChar,sizeof(char));
			word = wordFound(allLines);
			if(isVariableExist(word)){//if word is a predefined variable
				commentProcess(allLines);//comment control
				int varValue = getVariableValue(word);// this variable holds variable value in integer form
				char *varValueString = calloc(countOfChar,sizeof(char));
				itoa(varValue,varValueString,10);//variable value converted int to string
				outWords[countOfOutWords] = varValueString; // this variable value put in outWords array
				countOfOutWords++;// outWords array index + 1
				char *word = calloc(countOfChar,sizeof(char)); 
				word = wordFound(allLines);
				if(!strcmp(word,".")){ //If word is '.', this process will be finish
					commentProcess(allLines);//comment process
					
					for(i=0;i < countOfOutWords;i++){//Printing array items on console
						printf("%s",outWords[i]);
					}
					
					do{	//END OF LINE CONTROL
						char *word = calloc(countOfChar,sizeof(char));
						word = wordFound(allLines);
						if(!strcmp(word,"\n") || !strcmp(word,"\0")){
							lineCounter++;
							break;
						}else if(readingChar != countOfChar - 1){
							printf("\nLine %d | %s\n",lineCounter,"Error: If the operation on a line is finished, nothing else is written on this line...");
							isErrorFound++;
							//If the operation on a line is finished, nothing else is written on this line.
							readingChar = countOfChar-1;
							break;
						}
					}while(true);
					break;//when process was finished, loop breaks
				}else if(strcmp(word,",")){//If word is not ',' or '.' , error prints
					printf("\nLine %d | %s\n",lineCounter,"Error: Expected a seperator (,) or a EndOfLine (.) sign...");
					isErrorFound++;
					readingChar = countOfChar-1;
					break;
				}
			}else if(isInteger(word)){//If word is a int constant
				commentProcess(allLines);//comment control
				char *intConst = word;//a variable holds int constant value
				outWords[countOfOutWords] = intConst; // this variable value put in outWords array 
				countOfOutWords++;	// array index + 1
				char *word = calloc(countOfChar,sizeof(char));
				word = wordFound(allLines);
				if(!strcmp(word,".")){		// '.' and ',' control. This process is as I described above.
					commentProcess(allLines);
					for(i=0;i < countOfOutWords;i++){
						printf("%s",outWords[i]);
					}
					do{
						char *word = calloc(countOfChar,sizeof(char));
						word = wordFound(allLines);
						if(!strcmp(word,"\n") || !strcmp(word,"\0")){
							lineCounter++;
							break;
						}else if(readingChar != countOfChar - 1){
							printf("\nLine %d | %s\n",lineCounter,"Error: If the operation on a line is finished, nothing else is written on this line...");//If the operation on a line is finished, nothing else is written on this line.
							isErrorFound++;
							readingChar = countOfChar-1;
							break;
						}
					}while(true);
					break;
				}else if(strcmp(word,",")){
					commentProcess(allLines);
					printf("\nLine %d | %s\n",lineCounter,"Error: Expected a seperator (,) or a EndOfLine (.) sign...");
					isErrorFound++;
					readingChar = countOfChar-1;
					break;
				}
			}else if(word[0] == '"'){ //If word is '"'(Start of string constant)
				char *strConst = calloc(countOfChar,sizeof(char));//this string holds string constant value
				int indexStr = 0;//this variable holds index value of strConst
				do{	//Control chars in file until end of string constant
					if(allLines[readingChar] == '"'){//If string constant was closed(found '"' char)
						outWords[countOfOutWords] = strConst;//string constant value put in outWords array
						countOfOutWords++;//array index + 1
						readingChar++;//next char
						break;
					}
					if(readingChar == countOfChar - 1){//If string constant was not closed, error prints on screen
						printf("Line %d | %s\n",lineCounter,"Error: An opened string constant value was not closed... ");
						isErrorFound++;
						break;
					}
					strConst[indexStr] = allLines[readingChar];//If '"' char was not found and file was not finished, this char put in strConst
					indexStr++;//Index increased by 1
					readingChar++;//next char
				}while(true);
				
				if(readingChar == countOfChar - 1){//If string was not closed,loop breaks
					break;
				}
				char *word = calloc(countOfChar,sizeof(char));
				word = wordFound(allLines);
				if(!strcmp(word,".")){		// '.' and ',' control.
					commentProcess(allLines);
					for(i=0;i < countOfOutWords;i++){
						printf("%s",outWords[i]);
					}
					
					do{		//End of line control
						char *word = calloc(countOfChar,sizeof(char));	
						word = wordFound(allLines);
						if(!strcmp(word,"\n") || !strcmp(word,"\0")){
							lineCounter++;
							break;
						}else if(readingChar != countOfChar - 1){
							printf("\nLine %d | %s\n",lineCounter,"Error: If the operation on a line is finished, nothing else is written on this line...");
							isErrorFound++;
							readingChar = countOfChar-1;
							break;
						}
					}while(true);
					break;	
				}else if(strcmp(word,",")){
					printf("\nLine %d | %s\n",lineCounter,"Error: Expected a seperator (,) or a EndOfLine (.) sign...");
					isErrorFound++;
					readingChar = countOfChar-1;
					break;
				}
			}else if(!strcmp(word,"newline")){ // If word is newline keyword
				commentProcess(allLines);//comment process
				outWords[countOfOutWords] = "\n";// newline char put in outWords array
				countOfOutWords++;//array index + 1
				char *word = calloc(countOfChar,sizeof(char));
				word = wordFound(allLines);
				if(!strcmp(word,".")){			//'.' , ',' control
					commentProcess(allLines);
					for(i=0;i < countOfOutWords;i++){
						printf("%s",outWords[i]);
					}
					do{
						char *word = calloc(countOfChar,sizeof(char));
						word = wordFound(allLines);
						if(!strcmp(word,"\n") || !strcmp(word,"\0")){
							lineCounter++;
							break;
						}else if(readingChar != countOfChar - 1){
							printf("\nLine %d | %s\n",lineCounter,"Error: If the operation on a line is finished, nothing else is written on this line...");
							isErrorFound++;
							readingChar = countOfChar-1;
							break;
						}
					}while(true);
					break;	
				}else if(strcmp(word,",")){
					printf("\nLine %d | %s\n",lineCounter,"Error: Expected a seperator (,) or a EndOfLine (.) sign...");
					isErrorFound++;
					readingChar = countOfChar-1;
					break;
				}
			}else if(isKeyword(word)){//If word is keyword except newline keyword , error prints on screen
				printf("\nLine %d | Error: '%s' keyword not used after 'out' keyword...\n",lineCounter,word);
				isErrorFound++;
				readingChar = countOfChar-1;
				break;
			}else if(readingChar != countOfChar - 1){//If word is a unrecognized word , error prints
				printf("\nLine %d | Error: '%s' is a unidentify word in out keyword...\n",lineCounter,word);
				isErrorFound++;
				readingChar = countOfChar-1;
				break;
			}
		}while(true);
				
	}
	else if(!strcmp(word,"[")){ //If '[' used wrong places , error prints
		printf("\nLine %d | %s\n",lineCounter,"Error: There must be correctly loop structure for using '['");
		isErrorFound++;
		readingChar = countOfChar-1;
	}else if(!strcmp(word,"]")){//If ']' used wrong places , error prints
		printf("\nLine %d | %s\n",lineCounter,"Error: There must be block opener '[' with correctly loop structure for using ']'...");
		isErrorFound++;
		readingChar = countOfChar-1;
	}else if(!strcmp(word,"}")){//If '}' used wrong places , error prints
		printf("\nLine %d | %s\n",lineCounter,"Error: Despite there is not a comment opened, used '}'...");
		isErrorFound++;
		readingChar = countOfChar-1;
	}else if(!strcmp(word,"to")){//If to keyword used wrong places
		printf("\nLine %d | %s\n",lineCounter,"Error: 'to' keyword must use after correctly formed 'move','add' or 'sub' keywords. This keyword was not used correctly...");
		isErrorFound++;
		readingChar = countOfChar-1;
	}else if(!strcmp(word,"times")){//If times keyword used wrong places
		printf("\nLine %d | %s\n",lineCounter,"Error: 'times' keyword must use after correctly formed 'loop' keyword. This keyword was not used correctly...");
		isErrorFound++;
		readingChar = countOfChar;
	}else if(!strcmp(word,"newline")){//If newline keyword used wrong places
		printf("\nLine %d | %s\n",lineCounter,"Error: 'newline' keyword must use after correctly formed 'out' keyword. This keyword was not used correctly...");
		isErrorFound++;
		readingChar = countOfChar-1;
	}else if(isVariableExist(word)){//If predefined variable used wrong places , error prints
		printf("\nLine %d | Error: '%s' variable was called wrong...\n",lineCounter,word);
		isErrorFound++;
		readingChar = countOfChar-1;
	}else if(isInteger(word)){//If integer constant used wrong places , error prints
		printf("\nLine %d | Error: '%s' integer value was called wrong...\n",lineCounter,word);
		isErrorFound++;
		readingChar = countOfChar-1;		
	}else if(!strcmp(word,".") || !strcmp(word,",") || word[0] == '"'){//if '.' and ',' sign used wrong places , error prints
		printf("\nLine %d | Error: '%s' this sign used wrong place...\n",lineCounter,word);
		isErrorFound++;
		readingChar = countOfChar-1;
	}else if(!strcmp(word,"\n")){// if word is '\n' , line counter increased by 1
		lineCounter++;
	}else{ //If word is a unrecognized word , error prints
		printf("\n Line %d | Error: '%s' is unidentified word...\n",lineCounter,word);
		isErrorFound++;
		readingChar = countOfChar-1;
	}
		
	
	
}
int main(int argc, char *argv[]) {
	char fileName[100] = ""; //a string variable holds file name
	printf("%s","Please Enter The File Name (With the extension): ");
	gets(fileName);//File names get from user.
	printf("\n");
	FILE *baFile = fopen(fileName,"r");// .ba file called for read
	if(baFile == NULL){//If file did not found, Error message prints on screen
		printf("%s","Error: The file could not opened...");
	}else{
		
		while(!feof(baFile)){ //characters of .ba file are counted.				
			fgetc(baFile);
			countOfChar++;	
		}
		
		fseek(baFile,0,SEEK_SET);//Returning to head in .ba file
		char *allLines = calloc(countOfChar,sizeof(char));//A dynamic memory adjusts for holding characters of .ba
		countOfChar = 0;//character counter sets to zero.
		
		while(!feof(baFile)){//These characters are thrown into alllines variable.
			allLines[countOfChar] = fgetc(baFile);
			countOfChar++;
		}
		allLines[countOfChar-1] = '\0'; //calloc and malloc dynamic memories has created weird characters in last of the words. That is why last char of the word must be null.
		
		
		codeBlockControl(allLines); // Code block control
		while(allLines[readingChar] != '\0'){//Analyze words until there are no char.
			parse(allLines);//Method called
		}
		printf("\n\n-----%s-----","Interprinter was finished...");//The end of the project and creating the .lx file is printed to console.
	}
	
	
	getch();//For console not closed.		
}
