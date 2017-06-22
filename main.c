#include <stdio.h> //Standard Library
#include<string.h> //String Library
#include <stdbool.h> //Boolean Library

int ignore_space(char exp[], int i, int length);  // Function prototype for ignore_space
int calculator(int int1, int int2, char op);      // Function prototype for calculator


//Main Method
int main()
{
    //Created the a large string array to compensate for the size of the expression
    char expression[100];

    printf("Please enter your expression:  "); fflush(stdout);

    //fgets function is used to store characters from input into a given array
    fgets (expression, 100, stdin);

    //finds the length of the string that was input
    int exp_length = strlen(expression);

    int i, answer; // i is for counter and answer is used to store the result

    bool firstRun = true; //This is used in the beginning of the array

    // This if statement is used if only a single number is entered as an expression.
    if(exp_length == 2){
        printf("Answer: %c\n",expression[0]); fflush(stdout);}

        //This else statement carries out the calculations
    else{

        //for loop is used to run through the array until the program gets to the end
        for(i = 0; i < exp_length;){

            // Ignore space method is called
            i = ignore_space(expression, i, exp_length);
            if(i >= exp_length) // If the counter reaches the end of the array, the loop breaks
                break;

            int int1;

            //This if statement is only used once to get the first number
            //When the for loop comes back around, the new int1 would have been the previous result
            if(firstRun){

                // Subtracting the number character number from the '0' character returns the value of the actual character
                int1 = expression[i++] - '0';
                firstRun = false; //This is only intended to run once
            }else{
                int1=answer;
            }

            // Ignore space method is called
            i = ignore_space(expression, i, exp_length);
            if(i >= exp_length) // If the counter reaches the end of the array, the loop breaks
                break;

            char operator = expression[i++]; // After the first number is attained, an operator is expected
            //the operator is stored as a character

            // Ignore space method is called
            i = ignore_space(expression, i, exp_length);
            if(i >= exp_length)  // If the counter reaches the end of the array, the loop breaks
                break;


            // Subtracting the number character number from the '0' character returns the value of the actual character
            int int2 = expression[i++] - '0';

            // The calculator method is called and the result is returned into the variable answer.
            answer = calculator(int1, int2, operator);
        }

        //Prints the answer
        printf("Result: %d\n",answer); fflush(stdout);
    }

    return 0; //Return 0 for method return type
}
//End of main


int ignore_space(char expression[], int i, int exp_length){
    while(expression[i]==' '|| expression[i]=='\t')
        i++;
    return i;
}

//Method that uses the character operator to carry out calculations
int calculator(int int1, int int2, char operator){


    int result;
    switch(operator){

        //Case for addition.
        case '+':
            result= int1+int2;
            break;

            //Case for subtraction
        case '-':
            result= int1-int2;
            break;

            //Case for multiplication
        case '*':
            result= int1*int2;
            break;

            //Case for division
        case '/':
            result= int1/int2;
    }
    return result;
}