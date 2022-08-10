#include <stdio.h>

void main() {

	unsigned int a = 118;
	printf("\n118 as binary = ");
	dec2bin(a);

	//read 8th bit of a :   a[7]
	unsigned int bit_check = a & (1 << 7);
	printf("\nresult1 = %d", bit_check);


	//set 8th (a[7])  bit as HIGH
	// YOUR CODE GOES HERE	
	printf("\nresult2 = ");
	dec2bin(a);
	
	
	//set 3th bit as LOW
	// YOUR CODE GOES HERE
	printf("\nresult3 = ");
	dec2bin(a);
	
	
	//set 3th and 4th bit as HIGH .  Use only one code line
	// YOUR CODE GOES HERE
	printf("\nresult4 = ");
	dec2bin(a);
	
	
	//toogle 1st bit
	// YOUR CODE GOES HERE
	printf("\nresult5 = ");
	dec2bin(a);
	
	
	//set 1st and 8th bit as LOW. Use only one code line 
	// YOUR CODE GOES HERE
	printf("\nresult6 = ");
	dec2bin(a);
	
	printf("\n");
}



void dec2bin(unsigned int n) {
	unsigned int a = 0x80;
	for (int i = 0; i < 8; i++) {
		if ((a & n) == a)
			printf("1");
		else
			printf("0");
		a = a>> 1;
	}
}