#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funcs.h"


//The purpose of this function is to safely read a float from the user.


static float get_float_input(const char* prompt) { 
    char buf[128];
    float value;
    int valid = 0;

    while (!valid) {
        printf("%s", prompt); //displaying prompt message, e.g. resistor value:
        if(!fgets(buf, sizeof(buf), stdin)){ //
            printf("Input error. Exiting.\n"); //if theres an input error, user is notified
            exit(1); //program is exited
        }
        //this section of the fuction is to manually remove newline from the string, 
        //before it is converted to a float using atof
        int i=0; 
        while (buf[i] != '\0') {
            if (buf[i] == '\n'){
                buf[i] = '\0';
                break;

            }
            i++;
        }
        value = atof(buf); //convert string to a float.
        // atof returns 0.0 if inout is invalid
        //allow the user to enter 0, checking if input is actually 0
        if (value !=0.0f || (buf[0] == '0' && buf[1]== '\0'))
        valid=1 ;
        else
        printf("Invalid input, please enter a number!\n");



    }
    return value;
}

//this function is to allow user to save their results in a txt file
static void save_string(const char* text){ 
    FILE*fp = fopen("results.txt","a"); //opens a txt file in append mode
    if(!fp){
        printf("Error, unable to open file"); //this is a check to see if file opened successfully
        return;
    }

    fprintf(fp,"%s\n", text); //write the specified text, with a newline
    fclose(fp); //closes file to save changes
    printf("calculations saved successfully");

}
//this function allows the user to choose whether or not the user would like to save their values.
static int ask_to_save(void){
    char choice;
    printf("Save this calculation? (Y/N): ");
    scanf(" %c", &choice);
    while(getchar()!='\n'); //this is to clear the leftover input in the buffer
    if(choice=='Y' || choice=='y')
    return 1; //yes

    return 0; //no
    
}

//It was often necessary to avoid negative inputs for things like resistors, so I made this function.
//It works with get_float_input and ensures value is greater than zero
static float get_positive_input(const char* prompt){
    float value;
    while(1){
        value = get_float_input(prompt);
        if (value>0.0f)
        return value;

        printf("value must be greater than 0.\n");
    }
}

//my first menu item is an inverting amplifier calculator
void menu_item_1(void) {
    printf("\n>> Inverting Amplifier Calculator\n");
    float Rin = get_positive_input("Enter Rin (ohms): ");
    float Rf = get_positive_input("Enter Rf value (ohms): ");
    float Vin = get_float_input("Enter input voltage Vin (V): ");
    float Av = -(Rf/Rin);
    float Vout = Av*Vin;
    printf("\nGain = %.3f\n", Av);
    printf("Vout = %.3fV\n", Vout);
    if(Vout>15 || Vout<-15) //check if Vout is within rails.
        printf("Output exceeds rails\n");

    if(ask_to_save()) { //asks user to save, then uses save_string to append the text file with info
    char buffer[256];
    sprintf(buffer, "Inverting Amplifier: Vin=%.3f, Rin=%.3f, Rf=%.3f, Av=%.3f, Vout=%.3f", Vin, Rin, Rf,Av, Vout);
    //sprintf formats the text into a character array
    save_string(buffer);
    
    }

    

  
}

//my second menu option is a non-inverting amplifier calculator
//very similar to menu option 1, but of course formulas are different
void menu_item_2(void) {
    printf("\n>> Non-Inverting Amplifier Calculator\n");
    float Rg = get_positive_input("Please enter Rg (in ohms) ");
    float Rf = get_positive_input("Please enter Rf ");
    float Vin = get_float_input("Please enter Vin ");
    float Av = 1.0f + (Rf/Rg);
    float Vout = Av * Vin;
    printf("\nGain = %.3f\n", Av);
    printf("Vout = %.3fV\n", Vout);
    if(Vout>15 || Vout<-15)
        printf("Output exceeds rails\n");

    if(ask_to_save()) {
    char buffer[256];
    sprintf(buffer, "Non-Inverting Amplifier: Vin=%.3f, Rg=%.3f, Rf=%.3f, Av=%.3f, Vout=%.3f", Vin, Rg, Rf, Av, Vout);
    save_string(buffer);
    
    }
    
}

//my third menu item is a 3 input summing amplifier.
void menu_item_3(void) {
    printf("\n>> Summing Amplifier\n");
    printf("1. Inverting Summing Amplifier\n"); //I have allowed the user to choose between inverting and non-inverting summing amplifiers
    printf("2. Non-Inverting Summing Amplifier\n");
    printf("Selection: ");

    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n'); //reads upto newline
    if (choice!=1 && choice != 2){
        printf("Invalid selection.\n");
        return;
    }
    int count;
    count = (int)get_positive_input("How many inputs? (1-10)? ");
    while (count<1 || count>10){
        printf("Please enter a number between 1 and 10.\n");
        count = (int)get_positive_input("How many inputs? (1-10)? ");
    }
        float V[10];
        float R[10];
        for(int i = 0; i< count; i++){
            char promptv[100];
            sprintf(promptv, "Enter V%d (V) ", i+1);
            V[i] = get_float_input(promptv);

            char promptr[100];
            sprintf(promptr, "Enter R%d (ohms): ", i+1);
            R[i] = get_positive_input(promptr);

        }
        float Rf = get_positive_input("Enter Rf (ohms): ");
        float Vout;

        if (choice == 1){
            float sum = 0.0f;
            for(int i=0; i<count; i++){
                sum += V[i]/R[i];
            }

            Vout = -Rf * sum;

            printf("\nInverting Summing Amplifier Output:\n");
            printf("Vout = %.3f V\n", Vout);

            if(Vout>15 || Vout<-15)
            printf("Warning, Vout is outside of typical rails.\n");


        if(ask_to_save()) {
        char buffer[256];
        sprintf(buffer, "Inverting Sum: (%d inputs): Vout=%.3f", count, Vout);
        save_string(buffer);
        }
        return;
        
    }
    if (choice==2){
        float Rg = get_positive_input("Enter Rg (ohms): ");
        float numerator = 0.0f;
        float denominator = 0.0f;

        for(int i=0; i<count; i++){
            numerator += V[i] / R[i];
            denominator += 1.0f/R[i];
        }
        
        float Vtemp = numerator/denominator;
        float Av = 1.0f+(Rf/Rg);
        Vout = Av * Vtemp;

        printf("\nNon-Inverting Summing Amplifier Output:\n");
        printf("Vout=%.3f V\n", Vout);
        if(Vout>15 || Vout<-15)
        printf("Warning, Vout is outside of typical rails.\n");

        if(ask_to_save()) {
        char buffer[256];
        sprintf(buffer, "Non-Inverting Summing Amplifier (%d inputs): Vout=%.3f", count, Vout);
        save_string(buffer);
        }
        return;



    }

}


//my fourth meny item is a bandiwdth and slew rate calculator
void menu_item_4(void) {
    printf("\n>> Bandwidth & Slew Rate Calculator\n");

    float GBW = get_positive_input("Enter Gain Bandwidth Product (Hz)\n");
    float Gain = get_positive_input("Enter closed-loop gain: ");
    float BW = GBW / Gain;
    printf("Closed-Loop bandwidth = %.3f Hz\n", BW);
    float freq = get_positive_input("Enter signal frequency (Hz) ");
    float Vpeak = get_positive_input("Enter peak output voltage (V): ");
    float SR = 2*3.141519f*freq*Vpeak;
    printf("Required slew rate = %.3f V/s\n", SR);
//this segment helps the user to compare their SR value with a typical op-amp SR value.
    float typicalSR = 0.5e6f;
    if(SR>typicalSR)
    printf("Warning: Required slew rate exceeds typical op-amp limits!\n");

    if(ask_to_save()) {
        char buffer[256];
        sprintf(buffer, "Bandwidth and SR: GBW=%.3f, Gain=%.3f, BW=%.3f, f=%.3f, Vpeak=%.3f, SR=%.3f", GBW, Gain, BW, freq, Vpeak, SR);
        save_string(buffer);
        }

    /* you can call a function from here that handles menu 4 */
}

void menu_item_5(void) {
    printf("\n>> Noise and Tolerance Calculator\n");
    const float k = 1.38e-23f; //boltzmann constant
    float R = get_positive_input("Enter R (ohms)");
    float T = get_positive_input("Enter T (kelvin)");
    float B = get_positive_input("Enter B (Hz)");
    float Vnoise = sqrt(4*k*T*R*B); //computes noise voltage
    printf("Thermal noise voltage = %.9f V RMS\n", Vnoise);
    float Av = get_float_input("Enter nominal gain Av: "); //this segment is to calculate gain uncertainty
    float Rf = get_float_input("Enter Rf (ohms) ");
    float dRf = get_float_input("Enter ∆Rf: ");
    float Rin = get_positive_input("Enter Rin: ");
    float dRin = get_float_input("Enter ∆Rin: ");

    float dAv = Av*sqrt((dRf/Rf)*(dRf/Rf) + (dRin/Rin)*(dRin/Rin));
    printf("Gain uncertainty ∆Av = %.9f\n", dAv);
    if(ask_to_save()) {
        char buffer[256];
        sprintf(buffer, "Noise and Tolerance: Vn=%.9f, ∆Av=%.9f", Vnoise, dAv);
        save_string(buffer);
        }
    
}
//my last menu item is a loop gain calculator
void menu_item_6(void) {
    printf("\n>> Stability (Loop Gain) Calculator\n");
    
    float A0 = get_positive_input("Enter A0 (open-loop gain): ");
    float wp = get_positive_input("Enter wp (rad/s): ");
    float freq = get_float_input("Enter frequency (Hz) ");
    while(freq<0){
        printf("Frequency must be >= 0.\n");
        freq = get_float_input("Enter frequency (Hz) ");
    }
    float beta = get_float_input("Enter feedback factor β (0<β<=1): "); 
    while(beta<=0 || beta>1){ //get_positive_input would not work here since I need to also allow 0.
        printf("0<β<=1\n");
        beta = get_float_input("Enter feedback factor β (0<β<=1): ");
    }

    float Tmag = (A0/sqrt(1+pow((2*3.14159f*freq/wp), 2)))* beta;
    printf("Loop gain magntitude |T| = %.3f\n", Tmag);
    if(Tmag>1)
    printf("Warning: System may be unstable.\n");
else
    printf("loop gain acceptable.\n");

    if(ask_to_save()) {
        char buffer[256];
        sprintf(buffer, "Stability: A0=%.3f, wp=%.3f, f=%.3f, β=%.3f, |T|=%.3f", A0, wp, freq, beta, Tmag);
        save_string(buffer);
        }


}
