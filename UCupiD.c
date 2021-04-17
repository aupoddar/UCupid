/* Name: Auboni Poddar Spring Quarter 2018

Program Name: UCupiD.c

Descripton: This program takes a text file with information about people regardi
ng their interests and personal information and matches a specified person with 
other people based on specific similarities the two share.
         
        
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student { // define structure
    char SID[10];
    char Firstname[20];
    char Sex[2];
    char Orientation[2];
    int Age;
    int PoliticalLeaning;
    char MajorHobby[20];
    char MinorHobby[20];
    int Height;
    int Weight;
    struct student *next; 
};

struct student *read_a_record (char * buffer) 
{
    
    struct student * s;

    s = malloc(sizeof(struct student));

    //sscanf buffer
    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%d,%d", 
        s->SID, s->Firstname, s->Sex, s->Orientation, &s->Age,&s->PoliticalLeaning,
	s->MajorHobby, s->MinorHobby, &s->Height, &s->Weight);

    return (s);

}

void print_a_record(struct student * s) 
{

    printf("SID = %s,  Firstname = %s, Sex = %s, Orientation = %s, Age = %d, PoliticalLeaning = %d, MajorHobby = %s,  MinorHobby = %s, Height = %d, Weight = %d\n", 
        s->SID, s->Firstname, s->Sex, s->Orientation, s->Age, s->PoliticalLeaning,
	s->MajorHobby, s->MinorHobby, s->Height, s->Weight);

}

struct student *insert_back(struct student *head, struct student *s) 
{
    struct student *last;

    s->next = NULL;
    if (!head)
        return s;
    last = head;
    while (last->next != NULL) //(2)
        last = last->next;
    last->next = s; //(3)

    return head;

}

void print_list(struct student *s) {

    	while (s != NULL) 
	{
        	printf("SID = %s,  Firstname = %s, Sex = %s, Orientation = %s, Age = %d, PoliticalLeaning = %d, MajorHobby = %s,  MinorHobby = %s, Height = %d, Weight = %d\n", 
        	s->SID, s->Firstname, s->Sex, s->Orientation, s->Age, s->PoliticalLeaning,
		s->MajorHobby, s->MinorHobby, s->Height, s->Weight);
        	s = s->next;
    	}

}

struct student *find(struct student *head, char * sid) 
{
    struct student *s = head;
  
    while (s && (strcmp(s->SID,sid))) {
         s = s->next;
    }
   
    return s;

}

int compare_10_percent(int a, int b) 
{
    if(a > b) 
    {
	return ((0.9 * a <= b) && (1.1 * b >= a));
    } else {
	return ((0.9 * b <= a) && (1.1 * a >= b));
    }

}

int check_height_weight(struct student *s, struct student *sptr) 
{

	int a, b, c, d;

    	a = s->Height;
   	b = sptr->Height;
    	c = s->Weight;
    	d = sptr->Weight;

    	return (compare_10_percent(a, b) && compare_10_percent(c, d));

}

void match(struct student *head, struct student * sptr, FILE  *fout) 
{

    struct student *s = head;
    int short anymatch;
    anymatch = 0;

    while (s) 
    {
        if(strcmp(s->SID,sptr->SID) != 0) { //check if student is the same person to find a match
	    // look for matching criteria
	    if(
                  // Sex and Sexual orientation
                  ((s->Orientation[0] == sptr->Sex[0]) && (s->Sex[0] == sptr->Orientation[0])) && 
		  // and just one of the following three criteria
                  (
                      ((abs(s->Age - sptr->Age) <= 10) && (abs(s->PoliticalLeaning - sptr->PoliticalLeaning) <= 2)) ||
		      (((strcmp(s->MajorHobby, sptr->MajorHobby) == 0) && (strcmp(s->MinorHobby, sptr->MinorHobby) == 0)) ||
                      ((strcmp(s->MajorHobby, sptr->MinorHobby) == 0) && (strcmp(s->MinorHobby, sptr->MajorHobby) == 0))) ||
                      (check_height_weight(s, sptr))
                  )
	      ) {
		    // if there is a match print only once the if of the person matching against
                    if(anymatch == 0)
		        fprintf(fout, "Matches for user %s (%s): \n", sptr->SID, sptr->Firstname);
		    anymatch = 1;
		    // now print only the persons getting matched
		    fprintf(fout, "- %s (%s)\n", s->SID, s->Firstname);
	    }
	}

	s = s->next;
    }

}

void free_records(struct student *s) 
{
    struct student *tmp;

    while (s) {
	 tmp = s;
         s = s->next;
	 free (tmp);
    }
}

int main(int argc, char **argv)
{
    FILE * fpin;
    FILE * fout;

    struct student *head = NULL;
    struct student *new_record, *sptr;
    char buffer[256];
    char filename[30];
    // Check if the number of arguments are correct and if the file exists
    // if the #of arguments < 2 print error
    if(argc < 3) {
        printf("Usage: ./UCupiD db_file SID\n");
   	    return 1;
    }

    // if file does not exists
    if((fpin=fopen(argv[1], "r")) == NULL) {
        printf("Error: cannot open %s\n", argv[1]);
	return 1;
    }

    sprintf(filename,"match_%s.txt",argv[2]);
    // Opening the file for write
    if((fout=fopen(filename, "w")) == NULL) {
        printf("Error: cannot open %s for write\n", filename);
	return 1;
    }

    // read a recond from file and add to the list
    while (fgets(buffer, sizeof(buffer), fpin)) 
    {

        // Get formatted data in structure
        new_record = read_a_record(buffer);

        // Add a record
        head = insert_back(head, new_record);

    }

    // Given an SID, find a record
    if((sptr = find(head, argv[2])) != NULL) {
	// look for matching criteria
	match(head, sptr, fout);
    } else {
	printf("User with SID %s not found\n", argv[2]);
	return 1;
    }
    
    free_records(head);

    fclose (fpin);
    fclose (fout);

    return 0;
}
