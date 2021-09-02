#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char building_name[100];
	int room_number;
	char room_name[100];
} Handong;


int main()
{
    // Create structure type (Handong)  variable  room1
    Handong room1;
    // Create structure type (Handong)  variable  room2, room3
    // Your code goes here
	// Your code goes here

    // Create structure type (Handong)  Pointer , roomPt
    Handong *roomPt= malloc(sizeof(Handong));
    
    // Assign room3 address to roomPt
    // Your code goes here
	
  
    // Define structure variable  memeber values: room1
	strcpy(room1.building_name, "Newton");
	room1.room_number = 109;
	strcpy(room1.room_name, "SSSLAB");

    // Define structure variable  memeber values: room2
	// Your code goes here
	// Your code goes here
	// Your code goes here


    // Define structure variable  memeber values: room3
	// Your code goes here
	// Your code goes here
	// Your code goes here


    
    // Print each member values  : room1, room2, room3
	printf("%s building, room  %d  is %s\n", room1.building_name, room1.room_number, room1.room_name);
	// Your code goes here
	// Your code goes here

	// Print each member values by  pointer variable: roomPt
	// Your code goes here

	
	// Print address of   room3  and value of roomPt and compare. 
	printf("\n room3 address=%x ,  roomPt = %x  \n", &room3, roomPt);
	
	// Free mallocated variable 
	//free(roomPt);

}