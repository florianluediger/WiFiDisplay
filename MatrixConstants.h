#ifndef MatrixConstants_h
#define MatrixConstants_h

/*
 * 3-pixel wide mapping of the numbers 0-9 on arrays that can be drawn on the display
 */
int numbers[10][3] =  	{{126,66,126},    //0
						{0,0,126},        //1
						{114,82,94},      //2
						{82,82,126},      //3
						{30,16,126},      //4
						{94,82,114},      //5
						{126,82,114},     //6
						{98,26,6},        //7
						{126,82,126},     //8
						{94,82,126}};     //9

/*
 * 5-pixel wide mapping of the most important part of the ASCII alphabet 
 * on arrays that can be drawn on the display
 */
int letters[96][5] = 	{{0,0,0,0,0},			//SPACE
						{0,0,79,0,0},			    //!
						{0,3,0,3,0},			    //"
						{20,127,20,127,20},		//#
						{36,42,127,42,18},		//$
						{35,19,8,100,98},		  //%
						{54,73,86,32,80},		  //&
						{0,0,3,0,0},			    //'
						{0,28,34,65,0},			  //(
						{0,65,34,28,0},			  //)
						{42,28,127,28,42},		//*
						{8,8,62,8,8},			    //+
						{128,224,96,0,0},		  //,
						{8,8,8,8,8},			    //-
						{0,96,96,0,0},			  //.
						{64,48,8,6,1},			  ///
						{62,81,73,69,62},		  //0
						{68,66,127,64,64},		//1
						{66,97,81,73,70},		  //2
						{34,65,73,73,54},		  //3
						{24,20,18,127,16},		//4
						{47,73,73,73,49},		  //5
						{60,74,73,73,48},		  //6
						{3,1,121,5,3}, 			  //7
						{54,73,73,73,54},		  //8
						{6,73,73,41,30},		  //9
						{0,54,54,0,0},			  //:
						{64,118,54,0,0},		  //;
						{8,20,34,65,0},			  //<
						{20,20,20,20,20},		  //=
						{65,34,20,8,0},			  //>
						{2,1,81,9,6},			    //=
						{62,73,85,89,78},		  //@
						{124,10,9,10,124}, 		//A
						{65,127,73,73,54},		//B
						{62,65,65,65,34},		  //C
						{65,127,65,65,62},		//D
						{127,73,73,73,65},		//E
						{127,9,9,9,1},			  //F
						{62,65,73,73,58},		  //G
						{127,8,8,8,127},		  //H
						{0,65,127,65,0},		  //I
						{32,64,65,63,1},		  //J
						{127,8,20,34,65},		  //K
						{127,64,64,64,64},		//L
						{127,2,4,2,127},		  //M
						{127,4,8,16,127},		  //N
						{62,65,65,65,62},		  //O
						{127,9,9,9,6},			  //P
						{62,65,81,33,94},		  //Q
						{127,9,25,41,70},		  //R
						{38,73,73,73,50},		  //S
						{1,1,127,1,1},			  //T
						{63,64,64,64,63},		  //U
						{31,32,64,32,31},		  //V
						{127,32,16,32,127},		//W
						{99,20,8,20,99},		  //X
						{7,8,120,8,7},			  //Y
						{97,81,73,69,67},		  //Z
						{0,127,65,65,0},		  //[
						{1,6,8,48,64},			  //\
						{64,64,64,64,64},		  //_
						{0,65,65,127,0},		  //]
						{4,2,1,2,4},			    //^
						{64,64,64,64,64},		  //_
						{0,1,2,4,0},			    //`
						{32,84,84,56,64},		  //a
						{127,40,68,68,56},    //b
						{56,68,68,68,68},     //c
						{56,68,68,40,127},    //d
						{56,84,84,84,24},     //e
						{8,126,9,9,2},        //f
						{24,164,164,152,124}, //g
						{127,8,4,4,120}, 		  //h
						{0,68,125,64,0},      //i
						{64,128,128,132,125}, //j
						{127,32,16,40,68},    //k
						{0,65,127,64,0},      //l
						{124,4,24,4,120},     //m
						{124,4,4,4,120},		  //n
						{56,68,68,68,56},     //o
						{252,24,36,36,24},    //p
						{24,36,36,24,252},    //q
						{124,8,4,4,8},        //r
						{72,84,84,84,36},     //s
						{4,63,68,68,32},      //t
						{60,64,64,32,124},    //u
						{28,32,64,32,28},     //v
						{124,64,48,64,124},   //w
						{68,40,16,40,68},     //x
						{28,160,160,144,124}, //y
						{68,100,84,76,68},		//z
						{8,62,65,65,65},		  //{
						{0,0,127,0,0},			  //|
						{65,65,65,62,8},		  //}
						{8,4,8,16,8}};			  //~

/*
 * Array that can be used to turn all LEDs off
 */
int empty[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                      

#endif
