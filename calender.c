#include <stdio.h>
#include<conio.h>
#include <stdlib.h>
#include<dos.h>
#include<graphics.h>

int isLeapYear( int year );        /* True if leap year */
int leapYears( int year );         /* The number of leap year */
int todayOf( int y, int m, int d); /* The number of days since the beginning of the year */
long days( int y, int m, int d);   /* Total number of days */
void calendar(int y, int m);       /* display calendar at m y */
int getDayNumber(int d,int m,int y);
char *getName(int day);

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

typedef struct {
  int day;
  int month;
  int year;
  char note[255];
} Note;

int main(int argc, char* argv[]){
    int year,month, day;
    char choice;
    Note note;
    FILE *fp;
   // int gdriver=DETECT,gmode;
    // int col;
    clrscr();
   //
   /*
   initgraph(&gdriver,&gmode,"");

    for(col=1;col<=5;col++)
    {
    setbkcolor(col);
    }
     */

    fp = fopen("note.bin", "r");
    if (fp == NULL) {
      fp = fopen("note.bin", "w");
    }
    fclose(fp);

    while(1) {
      printf("\033[0;36m");
      printf("\n1. Find the day\n");
      printf("2. Print calendar of a month\n");
      printf("3. Add Note\n");
      printf("4. Exit\n");
      printf("Enter your choice: ");
      printf("\033[0m");
      scanf("\n%c", &choice);
      switch(choice) {
	case '1':
	textcolor(LIGHTGREEN);
	cprintf("Enter the day, month and year: ");
	scanf("%d %d %d", &day, &month, &year);
	printf("\n");
	textcolor(YELLOW);
	cprintf("The day is : %s\n", getName(getDayNumber(day, month, year)));
	printf("\n\n");
	break;
	case '2':
	printf("\033[0;35m");
	printf("\nEnter the month and year: ");
	scanf("%d %d", &month, &year);
	printf("\nPlease enter 's' to see the notes\n Press any other key to continue\n");
	printf("\033[0m");
	calendar(year, month);
	break;
	case '3':
	printf("\033[01;33m");
	printf("\nEnter the day, month and year: ");
	scanf("%d %d %d", &note.day, &note.month, &note.year);
	flush();
	printf("\nEnter the note: ");
	fgets(note.note, 255, stdin);
	fp = fopen("note.bin", "w");
	if (fp == NULL) {
	  printf("File note.bin can not be opened\n");
	  exit(1);
	}
	fwrite(&note, sizeof(Note), 1, fp);
	printf("Note added sucessfully\n");
	fclose(fp);
	printf("\033[0m");
	break;
	case '4':
	exit(0);
	break;
	default:
	printf("Not a valid option\n");
	break;
      }
    }

  //  return 0;
   // getch();
   // closegraph();
  //  getch()
}

int isLeapYear( int y ){
    return(y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}

int leapYears( int y ){
    return y/4 - y/100 + y/400;
}

int todayOf( int y, int m, int d) {
    static int DayOfMonth[] =
	{ -1,0,31,59,90,120,151,181,212,243,273,304,334};
    return DayOfMonth[m] + d + ((m>2 && isLeapYear(y))? 1 : 0);
}

long days( int y, int m, int d){
    int lastYear;
    lastYear = y - 1;
    return 365L * lastYear + leapYears(lastYear) + todayOf(y,m,d);
}

void calendar(int y, int m){
    FILE *fp;
    Note* notes, note;
    int len, j, hasNote = 0;
    char choice;
    const char *NameOfMonth[] = { NULL/*dummp*/,
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
    };
    int DayOfMonth[] =
	{ -1,31,28,31,30,31,30,31,31,30,31,30,31 };
    int weekOfTopDay;
    int i,day;

    weekOfTopDay = days(y, m, 1) % 7;

    fp = fopen("note.bin", "rb");
    if (fp == NULL) {
      printf("Couldn't read notes\n");
    }
    len = 0;
    while(fread(&note, sizeof(Note), 1, fp)) {
      if (note.year == y && note.month == m) {
	len++;
      }
    }
    rewind(fp);
    j = 0;
    notes = (Note*) malloc (sizeof(Note) * len);
    while(fread(&note, sizeof(Note), 1, fp)) {
      if (note.year == y && note.month == m) {
	notes[j] = note;
	j++;
      }
    }

    fclose(fp);

    if(isLeapYear(y))
	DayOfMonth[2] = 29;
    textcolor(LIGHTGREEN);
    printf("\t\t\t");
    cprintf("\n\n     %s %d\n\n", NameOfMonth[m], y);
   printf("\n");
   printf("\033[1;31m");
   printf("\tSUN");
   printf("\033[0m");
   printf("\033[01;33m");
   printf("\tMON     TUE     WED     THU     FRI     SAT\n\n\n");
   printf("\033[0m");
   for(i=0;i<weekOfTopDay;i++)
	printf("  \t");
    for(i=weekOfTopDay,day=1;day <= DayOfMonth[m];i++,day++){
	hasNote = 0;
	for (j = 0; j < len; j++) {
	//to check the day of added note
	  if (notes[j].day == day) {
	  textcolor(9);
	  printf("\t");
	   cprintf("|%2d| ",day);
	    hasNote = 1;
	    break;
	  }
	}

	if (hasNote == 0) {
	//To check it is sunday or not
	if(i%7==0)
	{
	textcolor(12);
	printf("\t");
	cprintf("%2d   ",day);
	}
	else{
	 textcolor(WHITE);
	 printf("\t");
	cprintf("%2d   ",day);
	}
	}
	if(i % 7 == 6)
	    printf("\n\n");
    }
    printf("\n");
    scanf("\n%c", &choice);
    if (choice == 's') {
      textcolor(LIGHTGREEN);
      cprintf("The Note for %d-%d\n", m, y);
      printf("\n");
      for (j = 0; j < len; j++) {
	textcolor(YELLOW);
	cprintf("Day %d: %s\n", notes[j].day, notes[j].note);
	printf("\n");
      }
    } else {
      return;
    }
}

int getDayNumber(int d, int m, int y){ //retuns the day number
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

char *getName(int day){ //returns the name of the day
   switch(day){
      case 0 :return("Sunday");
      case 1 :return("Monday");
      case 2 :return("Tuesday");
      case 3 :return("Wednesday");
      case 4 :return("Thursday");
      case 5 :return("Friday");
      case 6 :return("Saturday");
      default:return("Error: Invalid Argument Passed");
   }
}