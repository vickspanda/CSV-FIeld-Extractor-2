/*
 *
 * Here is the Program to Display the Fields as per specified by the User along with conditions
 * For Example: displaying all First names where the Degree is MSc followed by Gender to be 'F' and the Year as 2024
 * Then Program must check the details from .csv file provided, Firstly it must check if the row is of Degree MSc or Not and will create .csv files with All the Rows where Degree is MSc
 * And then this generated .csv file will be passed as an input for checking Gender to be 'F' and then also new .csv file with all the rows where Gender is F will be generated and it will be passed for validating rest conditions with same Algorithm
 * And At FInal Stage 'outFile.txt' will be generated which will contain the expected Field as an Result
 * If All Conditions satisfied, then this File must have the First Name of the Students */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* This Problem is Solved as per the expectations like, 
 * if the user have given the conditions to check as Degree Msc followed by Gender to be 'F' and the Year as 2024, 
 * Then this program will search for the Degree Field in the Particular Row, after meeting the requirements, it will write that row in the New .csv File,
 * After Doing for All Rows, it will proceed to search for Gender Field, and then Year Field, and for both gender and Year also, new Filtered .csv Files will be generated
 * preserving the order given by the user and will generate the output File for output Field if all conditions are checked */

/* Here the Keywords are defined as
 * -r : Roll No
 * -fn : First Name
 * -mn : Middle Name
 * -ln : Last Name
 * -g : Gender
 * -y : Year
 * -d : Degree
 *  These are the Fields of the csv file
 *
 *  And Assumption is taken that the Structure for csv file is as follows:
 *  
 *  RollNo, FirstName MiddleName LastName, Gender, Year, Degree
 *
 *  without having extra spaces
 *
 *  -print is defined for mentioning which field you want as an Output
 *
 *  Note: -print only takes one field at a time and is not programmed to take multiple fields for Output*/


/* Syntax For Using this Program, run following commands in terminal
 * $ make
 * $ ./extractField <your csv File> <field1> <value1> <field2> <value2> .. -print <fieldx>
 *
 * here second argument is name of your csv file
 * <field1>, <field2> and so on are the Keywords for the fields as defined above;
 * For Eg: Your have to type -r for Roll No
 * <value1>, <value2> and so on are the respective values as per the fields passed for providing the search Conditions
 * after giving conditions -print is given so to understand that after -print that field is mentioned which user supposes to be generated 
 * in outFile.txt as <fieldx>
 *
 * For the Example Discussed Above,
 * If User want to Display all First names where the Degree is MSc followed by Gender to be 'F' and the Year as 2024
 * For that, User have to Type: ./extractField <your-csv-file> -d MSc -g F -y 2024 -print -fn
 * */

/* Global Declarations, So that these can be accessed throughout the Program,
 * *args[] contains the fields that can be selected for conditions as well as for print operation,
 * Note : print is also defined in that but is not part of it,
 * *fields[] contains the condition values,
 * delimitor[] is designed to separate out the fields of csv,
 * printField contains the index for printing Field*/

char *args[] = {"-r","-fn","-mn","-ln","-g","-y","-d","-print"};
char *fields[7] = {0,0,0,0,0,0,0};
char delimitor[7] = {',',' ',' ',',',',',',','\n'};
int printField;


/* It is the Code to Check Whether the Input Data File have extension as .csv or Not */
int checkForCSV(char *fileName)
{
        int len = strlen(fileName);

        if(len > 4 && strcmp(&fileName[len-4],".csv")==0)
                return 1;
        else
                return 0;
}


/* It is function to validate whether the Number of Arguments given by User are Valid or Not */
int validateNoOfArgs(int argc){
        if((argc%2)==1 || argc>18 || argc < 6)
                return 0;
        else
                return 1;
}


/* It is the Function which checks whether the passed value in match function does exist in *args[] or not and returns the index if found else -1 */
int match(char *value)
{
        for(int i=0;i<8;i++){
                if(strcmp(value,args[i])==0){
                        return i;
                }
        }
        return -1;
}


/* It is the Function to Validate the Passed Arguments as Fields and print operation */
int validateArgs(int argc, char *argv[], int seq[], int k)
{
        int flag=0;
        k--;
        if(strcmp(argv[argc-2],"-print")!=0)
                return 0;
        argc--;
        while(argc>=2)
        {
                if(match(argv[argc])==-1)
                        return 0;
                else
                {
                        if(flag==0){
                                printField = match(argv[argc]);
                                flag=1;
                                argc--;
                        }
                        else{
                                if(strcmp(argv[argc],"-print")!=0){
                                        fields[match(argv[argc])] = argv[argc+1];
                                        seq[k] = match(argv[argc]);
                                        k--;
                                }
                                argc = argc - 2;
                        }
                }
        }
        return 1;
}


/* It is written to write the Current Line in the File */
void printCurrLine(FILE * file, FILE * newFile)
{
	char ch;
	while((ch=fgetc(file))!='\n' && ch!=EOF)
		fputc(ch,newFile);
	fputc('\n',newFile);
}

/* It moves the pointer to the start of the Current Line */
void takeToStart(FILE *file)
{
	char ch;
	while((ch=fgetc(file))!='\n')
		fseek(file,-2,SEEK_CUR);
}

/* It moves the Pointer to the Start of the Next Line */
void takeToNextLine(FILE *file)
{
	char ch;
	while((ch=fgetc(file))!='\n' && ch!=EOF)
	{
		if(ch==EOF)
			break;
	}
}

/* It Prints the Expected Fields from the last Filtered CSV File */
void printFields(FILE *file)
{
	int pointer = 0;
	char ch;
	FILE *outFile = fopen("outFile.txt","w");

	while((ch=fgetc(file))!=EOF)
	{
		if(pointer==printField && ch!=delimitor[pointer])
			fputc(ch,outFile);
		if(ch==delimitor[pointer])
			pointer = (pointer+1)%7;
		if(ch=='\n')
			fputc('\n',outFile);
	}
	fclose(outFile);
}

/* It Checks and validates the Field with the Input Given by the User and returns 0 if match not found else 1 */
int checkForField(FILE *file, int pointer)
{
	char ch;
	int length = 0, i = 0;
	while((ch=fgetc(file))!=delimitor[pointer] && ch!=EOF)
		length++;

	fseek(file,-(length+1),SEEK_CUR);

	char word[length];
	word[length] = '\0';

	while((ch=fgetc(file))!=delimitor[pointer] && ch!=EOF)
		word[i++] = ch;

	fseek(file,-(length+1),SEEK_CUR);
	
	if(strcmp(word,fields[pointer])!=0)
		return 0;
	else
		return 1;
}


/* extractField is the function which creates the csv files as per the conditions provided by the User */
void extractField(FILE *file, int seq[], int k, int condNo)
{
	int pointer = 0;
	char command[100], ch;

	sprintf(command,"temp%d.csv",condNo+1);
	
	FILE *newFile = fopen(command,"w");

	printCurrLine(file,newFile);

	while((ch=fgetc(file))!=EOF)
	{
		if(ch==delimitor[pointer])
			pointer = (pointer+1)%7;

		if(pointer==seq[condNo])
		{
			if(seq[condNo]==0)
				fseek(file,-1,SEEK_CUR);
			if(checkForField(file,pointer))
			{
				takeToStart(file);
				printCurrLine(file,newFile);
			}
			else
				takeToNextLine(file);
			pointer = 0;
		}
	}

	fclose(newFile);
	newFile = fopen(command,"r");

	if(condNo==(k-1))
		printFields(newFile);
	else
		extractField(newFile,seq,k,condNo+1);

	fclose(newFile);
}	

/* Main Function is mainly dealing with Composition of all Functions along with all input Validations */
int main(int argc, char *argv[])
{
        if(!validateNoOfArgs(argc)){
                printf("Error: Invalid Number of Arguments\n");
                return 0;
        }
        int seq[(argc-4)/2];

        if(!checkForCSV(argv[1]))
        {
                printf("Error: .csv File is expected as an Input\n");
                return 0;
        }

        if(!validateArgs(argc,argv,seq,(argc-4)/2))
        {
                printf("Error: Invalid Fields Passed\n");
                return 0;
        }

        FILE * file = fopen(argv[1],"r");
        if(!file){
                perror("Error: ");
                return 0;
        }
	char ch;	
        extractField(file,seq,(argc-4)/2,0);
        fclose(file);
}

