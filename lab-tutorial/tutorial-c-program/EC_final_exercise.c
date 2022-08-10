

//GPIO_mode_type 
#define MODE_IN				    0
#define MODE_OUT			    1
#define MODE_ALT                2
#define MODE_ANA                3



typedef struct
{
    unsigned int MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
	unsigned int OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
} GPIO_TypeDef;


#define LED_PIN 5


void main() {
unsigned int valA = 0;
	unsigned int valB = 0;

	// Create GPIOA_BASE Structure 	
    GPIO_TypeDef GPIOA_BASE;
    // Create GPIOA Structure (Pointer-type) and assign to GPIOA_BASE
	GPIO_TypeDef *GPIOA;
    GPIOA=&GPIOA_BASE;
    
	
	// Initialize GPIOA-> MODER=0 and OTYPER= 0x000F
	// YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

	
	// Create GPIOB_BASE Structure 	
	GPIO_TypeDef GPIOB_BASE;
	// Create GPIOB Structure (Pointer-type) and assign to GPIOB_BASE
    // YOUR CODE GOES HERE
	// YOUR CODE GOES HERE
	
	// Initialize GPIOB-> MODER=0 and OTYPER= 0x0001
    // YOUR CODE GOES HERE
	// YOUR CODE GOES HERE

	


	// GPIOA Mode Register
	// Clear by writing '00' on  MODER[5:6] .  
	// YOUR CODE GOES HERE

	
	// Set  by writing '01' on  MODER[5:6].  Use  GPIO_mode_type(MODE_IN etc..)  
	// YOUR CODE GOES HERE

	// Read MODER[5:6] and save it on  valA.  
	// YOUR CODE GOES HERE

	
	
	// GPIOB Mode Register
	// Clear by writing '00' on  MODER[5:6] 
	// YOUR CODE GOES HERE

	// Set  by writing '10' on  MODER[5:6].  Use  GPIO_mode_type(MODE_IN etc..)    
	// YOUR CODE GOES HERE

	// Read MODER[5:6] and save it on  valB
	// YOUR CODE GOES HERE

	
	
	// print the address of GPIOA in both integer and hexadecimal
	printf("gpioa  memory address  =       %x\n", GPIOA);
	
	// print the address of   GPIOA_BASE.MODER 
	printf("gpioa-moder  memory as hexadecimal  =       %x\n", &(GPIOA->MODER));	


	// print the value of   GPIOA->MODER in integer
	printf("gpioa-moder  value is  =       %d\n", (GPIOA->MODER));
	
	// print the value of   GPIOA_BASE.MODER in integer
	printf("gpioa_base-moder  value is       %d\n", (GPIOA_BASE.MODER));
	

    // print the value of   GPIOA->OTYPER  in integer
    printf("gpioa-otyper value is  =       %d\n", (GPIOA->OTYPER));
    
	// print the address of   GPIOA_BASE.OTYPER 
	printf("gpioa-otyper  memory as hexadecimal  =       %x\n", &(GPIOA->OTYPER));	

	
	
	// REPEAT for GPIOB
	
	
	

}