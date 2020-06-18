/*
*CS101 Project
*Team Id:231
*Author List:1. Pushkaraj Dhake-----140070019
             2. Mohammed Rafi-------140070049
             3. Gaurav Sinha--------140040082
             4. Maddela Sai karthik-140040102
* Filename: chain_reaction.cpp
* Project: Chain Reaction
*/

#include <iostream>
#include <GL/glut.h>
//#include <cstdint>

using namespace std;
//*******************************************************************************************************************************//
//*******************************************************************************************************************************//

float angle=0.0;                               //initializing angle to 0.0 used for rotation.

int current_player=0,i_1,j_1,max_players,window=0;
//initializing current_player and window to 0.

//current_player keeps the value of the player whose turn it is to play.
//If it is player x's turn to play, current_player = x-1.

//window is a variable which determines which window will be drawn on calling display function.
//If window = 0, initial window with menu buttons will be drawn.
//If window = 2, the window to choose no. of players will be drawn
//If window = 3, the window showing instructions to play the game will be drawn.
//If window = 1, the window where actual game can be played will be drawn.
//If window = 4, the window showing the win will be drawn if someone has won.

const int max_balls[8][6]= {{2,3,3,3,3,2},        //matrix with values as maximum no. balls each cell can occupy.
    {3,4,4,4,4,3},
    {3,4,4,4,4,3},
    {3,4,4,4,4,3},
    {3,4,4,4,4,3},
    {3,4,4,4,4,3},
    {3,4,4,4,4,3},
    {2,3,3,3,3,2}
};

int ball_count[8][6];
//matrix which stores no. of balls present in a cell.

int ball_colour[8][6];
//msatrix which stores which colour's balls are present in a cell
//ball_colour[i][j] = -1 if no ball is present in a cell in i'th row and j'th column.
//ball_colour[i][j] = x-1 if x'th palyer's coloured ball is present in that cell.

int player[4]= {0,0,0,0};
//player[i] = 0 if i+1'th player is in the game
//player[i] = 1 if i+1'th player is eliminated from the game.

//const int font_1=(int)GLUT_BITMAP_9_BY_15;
//const int font_2=(int)GLUT_BITMAP_TIMES_ROMAN_24;
//fonts that are used to write text on the window.

bool is_chance=false;
//is_chance = true, whenever a player clicks in a valid cell to place his ball, else it is false.

//initializes current_player, matrices ball_count[8][6], ball_coulour[8][6], player[4] to start new game.
void initialize();

//draws everything to display on the window according to the value of the variable window.
void display(void);

//draws initial window with menu buttons.
void mainscrean(void);

//Function that prints text on screan.
void textf(float x, float y, const char *string);

void textf1(float x, float y, const char *string);

//draws window for showing instructions to play game.
void instructions(void);

// draws window to choose no. of players to play the game.
void choose_players(void);

//draws grid of the colour whose turn is to play and displays the turn.
void draw_grid(void);

//updates the angle of rotation of spheres with time.
void update(int value);

//places i spheres at coordinates (x_1,y_1) on the window.
void sphere_mainscreen(float x_1,float y_1,int i,int j);

//draws spheres in the cells according to placements of balls on board.
void sphere(int i, int j);

//gets the mouse coordinates and perform changes required to perform on every mouse click.
void mouse_click(int button,int state,int x,int y);

//checks for explosion and explodes the balls.
void rearrange();

//eliminates players after loosing.
void eliminate();

//checks win and calls to display win.
void win();

//shows the win on the window if someone has won.
void won_player();

//closes application if 'ESC' button on the keyboard is pressed.
void keyboard(unsigned char key, int x, int y);

//A class Point which takes input x and y values to store x and y coordinates of a point respectively.
class Point
{
public:
    float x,y;
    Point()
    {
        x=0;
        y=0;
    }
    Point(float p,float q)
    {
        x=p;
        y=q;
    }
};

Point mouse_point; // variable of class Point to store coordinates where mouse is clicked.

//*******************************************************************************************************************************//
//*******************************************************************************************************************************//

//Function to convert coordinates of point where the mouse is clicked into window coordinates.
Point mouse(int x, int y)
{
    int windowWidth = 800, windowHeight = 600;
    return Point((float(x)/windowWidth)*2-1.0, 1.0- 2*(float(y)/windowHeight));
}//end

//*******************************************************************************************************************************//

//initializes current_player, matrices ball_count[8][6], ball_coulour[8][6], player[4] to start new game.
void initialize()
{
    current_player=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<6; j++)
        {
            ball_count[i][j]=0;   //makes no. of balls in each cell to zero.
            ball_colour[i][j]=-1; //sets colour of ball in each cell to none as there are no balls in any cell.
        }
    }
    for(int i=0; i<4; i++)
    {
        player[i]=0; // sets every playerto be able to play the game with no player eliminated.
    }
}//end.

//*******************************************************************************************************************************//

//draws everything to display on the window according to the value of the variable window.
void display()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); //Set the background clolour of the window as black.
    glClear (GL_COLOR_BUFFER_BIT);
    switch(window)
    {
    case 0:           // To show initial window.
        mainscrean(); //call to  draw initial window.
        break;
    case 1:           // To show the window where players can play.
        draw_grid(); // call to draw grid.
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<6; j++)
            {
                sphere(i,j);
            }
        }
        break;
    case 2:           // To show the window where no. of players are chosen before starting the game.
        choose_players();
        break;
    case 3:           // To show the window which displays instructions to play the game.
        instructions();
        break;
    case 4:           //  To show the win if someone has won.
        won_player();
    default :
        ;
    }
    glFlush();
    glutSwapBuffers();
}//end.

//*******************************************************************************************************************************//

//draws initial window with menu buttons.
void mainscrean(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int k;
    for(k=-1; k<2; k++) //draws rectangle around text.
    {
        if(k!=0)
        {
            glColor3f(1.0,0.28,0.0); // sets the colour in terms of R,G,B coeeficients to draw whatever follows this.

            glBegin(GL_QUAD_STRIP);  // //draws quadrilateral through given vertices and filled with given colour.
            glVertex3f(k*0.9,k*0.9,0);
            glVertex3f(k*1,k*1,0.5);
            glVertex3f(k*-0.9,k*0.9,0.5);
            glVertex3f(k*-1,k*1,0);
            glEnd();

            glBegin(GL_QUAD_STRIP);
            glVertex3f(k*0.45,0.4,0);
            glVertex3f(k*0.9,0.4,0);
            glVertex3f(k*0.45,-0.4,0);
            glVertex3f(k*0.9,-0.4,0);
            glEnd();

            glColor3f(0.0,0.0,1.0);
            glBegin(GL_QUAD_STRIP);
            glVertex3f(k*0.9,k*0.9,0);
            glVertex3f(k*1,k*1,0);
            glVertex3f(k*0.9,k*-0.9,0);
            glVertex3f(k*1,k*-1,0);
            glEnd();
        }
    }

    glColor3f(0.5,0.5,0.5);
    int i,j;
    for(i=-1; i<2; i++)
    {
        if(i!=0)
        {
            for(j=-1; j<2; j++)
            {
                if(j!=0)
                {
                    glBegin(GL_QUAD_STRIP);
                    glVertex3f(i*0.45,j*0.4,0);
                    glVertex3f(i*0.9,j*0.4,0);
                    glVertex3f(i*0.45,j*0.9,0);
                    glVertex3f(i*0.9,j*0.9,0.9);
                    glEnd();
                }
            }
        }
    }

    glColor3f(0.82,0.647,0.125);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-.45,0.9,0);
    glVertex3f(0.45,0.9,0);
    glVertex3f(-0.45,-0.9,0);
    glVertex3f(0.45,-0.9,0);
    glEnd();

    for(i=-1; i<2; i++)  //draws rectangle around text.
    {
        glBegin(GL_QUAD_STRIP); //draws quadrilateral through given vertices and filled with given colour.
        glColor3f(0.5,0.5,0.5);
        glVertex2f(-0.2,(i*0.3)-0.05);
        glVertex2f(+0.2,(i*0.3)-0.05);
        glVertex2f(-0.2,(i*0.3)+0.0875);
        glVertex2f(+0.2,(i*0.3)+0.0875);
        glEnd();

        glBegin(GL_LINE_STRIP); //draws lines joining given vertices
        glColor3f(1.0,1.0,1.0);
        glVertex2f(-0.2,(i*0.3)-0.05);
        glVertex2f(+0.2,(i*0.3)-0.05);
        glVertex2f(+0.2,(i*0.3)+0.0875);
        glVertex2f(-0.2,(i*0.3)+0.0875);
        glVertex2f(-0.2,(i*0.3)-0.05);
        glEnd();
    }
    sphere_mainscreen(-0.35,+0.68,3,3); // places 3 spheres at the position given in terms of x,y coordinates.
    sphere_mainscreen(0.333,0.68,3,3);

    glColor3f(0.0,0.0,1.0);
    textf1(-0.265,+0.65,"CHAIN REACTION");

    glColor3d(0.0, 0.0, 0.0);
    textf(-0.09,+0.3,"NEW GAME"); // prints the text given in the form of string
    textf(-0.135,0,"INSTRUCTIONS");//with left bottom of the text at x,y inputs given to the function textf.
    textf(-0.045,-0.3,"EXIT");

    textf(-0.40,-0.5,"DEVELOPED BY-");
    textf(-0.175,-0.60,"PUSHKARAJ DHAKE");
    textf(-0.175,-0.68,"MOHAMMED RAFI");
    textf(-0.175,-0.76,"GAURAV SINHA");
    textf(-0.175,-0.84,"MADDELA SAI KARTHIK");
    textf1(-0.67,-1.0,"CREDITS TO OUR TA Ms.MAHIMA CHOUDHARY");

    sphere_mainscreen(-0.21,-0.58,1,1);// places 1 sphere at the position given in terms of x,y coordinates.
    sphere_mainscreen(-0.21,-0.66,1,3);
    sphere_mainscreen(-0.21,-0.74,1,4);
    sphere_mainscreen(-0.21,-0.82,1,5);
}

//*******************************************************************************************************************************//

//Function that prints text on screan.
void textf(float x, float y, const char *string)
{
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);//c is the pointer to character string.
    }
}

void textf1(float x, float y, const char *string)
{
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);//c is the pointer to character string.
    }
}

//*******************************************************************************************************************************//

//draws window for showing instructions to play game.
void instructions(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.5,0.5,0.5);
    glVertex2f(-0.2,-0.85);
    glVertex2f(+0.2,-0.85);
    glVertex2f(-0.2,-0.7125);
    glVertex2f(+0.2,-0.7125);
    glEnd();

    glColor3f(0.0,1.0,0.0);
    textf1(-0.25,+0.8,"INSTRUCTIONS");

    glColor3d(1.0, 0.0, 0.0);
    textf(-0.8,+0.6,"1.  WITH THE HELP OF MOUSE CLICK ON THE REQUIRED CELL PLAYER CAN PLACE  ");
    textf(-0.8,0.52,"    BALL OF HIS COLOUR ON THE BOARD.");

    glColor3d(0.0,0.0,1.0);
    textf(-0.8,0.42,"2.  THE PLAYER CAN CLICK ONLY ON THE CELL CONTAINING SAME COLOUR BALL OR ");
    textf(-0.8,0.34,"    ON AN EMPTY CELL WITH NO BALLS IN IT.");

    glColor3d(1.0,1.0,0.0);
    textf(-0.8,0.24,"3.  THERE IS A LIMIT ON THE MAXIMUM NUMBER OF BALLS THAT CAN BE CONTAINED ");
    textf(-0.8,0.16,"    BY A CELL AND IF THE NUMBER OF BALLS IN A PARTICULAR CELL EXCEED THAT ");
    textf(-0.8,0.08,"    LIMIT THEN EXPLOSION TAKES PLACE");

    glColor3d(0.0,1.0,1.0);
    textf(-0.8,-0.02,"4.  WHILE EXPLODING IF ANOTHER PLAYER'S BALLL IS PRESENT IN THE ADJACENT ");
    textf(-0.8,-0.10,"    CELL IT ALSO BECOMES THE COLOUR OF THE PLAYER WHO HAS DONE EXPLOSION.");

    glColor3d(0.0,1.0,0.0);
    textf(-0.8,-0.20,"5.  WHILE EXPLODING IF ADJACENT CELL ALSO CONTAINS MAXIMUM NUMBER OF BALLS");
    textf(-0.8,-0.28,"    THEN AFTER ADDITION OF BALL FROM EXPLODING CELL IT ALSO EXPLODES AND ");
    textf(-0.8,-0.36,"    THIS CHAIN FURTHER CONTINUES.");

    glColor3d(1.0,0.0,1.0);
    textf(-0.8,-0.46,"6.  IN THIS WAY THE PLAYER HAS TO ELIMINATE THE OPPONENTS'S BALLS AND HAS ");
    textf(-0.8,-0.54,"    TO TAKE CONTROL OF THE BOARD.");

    glColor3d(1.0,1.0,0.0);
    textf(-0.8,-0.62,"#   You can press 'ESC' key on keyboard anytime to exit the game.");

    glColor3d(1.0, 0.0, 0.0);
    textf(-0.045,-0.8,"BACK");

    glBegin(GL_LINE_STRIP);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(-0.2,-0.85);
    glVertex2f(+0.2,-0.85);
    glVertex2f(+0.2,-0.7125);
    glVertex2f(-0.2,-0.7125);
    glVertex2f(-0.2,-0.85);
    glEnd();
}

//*******************************************************************************************************************************//

// draws window to choose no. of players to play the game.
// No. of players can be 2, 3 or 4.
void choose_players(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int k;
    for(k=-1; k<2; k++) //draws rectangle around text.
    {
        if(k!=0)
        {
            glColor3f(1.0,0.28,0.0);
            glBegin(GL_QUAD_STRIP);
            glVertex3f(k*0.9,k*0.9,0);
            glVertex3f(k*1,k*1,0.5);
            glVertex3f(k*-0.9,k*0.9,0.5);
            glVertex3f(k*-1,k*1,0);
            glEnd();

            glColor3f(0.0,0.0,1.0);
            glBegin(GL_QUAD_STRIP);
            glVertex3f(k*0.9,k*0.9,0);
            glVertex3f(k*1,k*1,0);
            glVertex3f(k*0.9,k*-0.9,0);
            glVertex3f(k*1,k*-1,0);
            glEnd();
        }
    }

    int i;
    for(i=-1; i<2; i++)  //draws rectangle around text.
    {
        glBegin(GL_QUAD_STRIP);
        glColor3f(0.5,0.5,0.5);
        glVertex2f(-0.2,(i*0.3)-0.05);
        glVertex2f(+0.2,(i*0.3)-0.05);
        glVertex2f(-0.2,(i*0.3)+0.0875);
        glVertex2f(+0.2,(i*0.3)+0.0875);
        glEnd();
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0,1.0,1.0);
        glVertex2f(-0.2,(i*0.3)-0.05);
        glVertex2f(+0.2,(i*0.3)-0.05);
        glVertex2f(+0.2,(i*0.3)+0.0875);
        glVertex2f(-0.2,(i*0.3)+0.0875);
        glVertex2f(-0.2,(i*0.3)-0.05);
        glEnd();
    }
    glColor3d(1.0, 1.0, 0.0);
    textf1(-0.4125,+0.6,"CHOOSE NUMBER OF PLAYERS :");

    glColor3f(0.0,0.0,0.0);
    textf(-0.03375,+0.3,"TWO");
    textf(-0.05625,0,"THREE");
    textf(-0.045,-0.3,"FOUR");
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(-0.2,-0.85);
    glVertex2f(+0.2,-0.85);
    glVertex2f(+0.2,-0.7125);
    glVertex2f(-0.2,-0.7125);
    glVertex2f(-0.2,-0.85);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.5,0.5,0.5);
    glVertex2f(-0.2,-0.85);
    glVertex2f(+0.2,-0.85);
    glVertex2f(-0.2,-0.7125);
    glVertex2f(+0.2,-0.7125);
    glEnd();

    glColor3d(1.0, 0.0, 0.0);
    textf(-0.045,-0.8,"BACK");
}

//*******************************************************************************************************************************//

//draws grid of the colour whose turn is to play the game.
//also shows whose turn it is as a text above the grid.
void draw_grid(void)
{
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.5,0.5,0.5);;
    glVertex2f(-0.4,-0.9625);
    glVertex2f(+0.4,-0.9625);
    glVertex2f(-0.4,-0.825);
    glVertex2f(+0.4,-0.825);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(-0.4,-0.9625);
    glVertex2f(+0.4,-0.9625);
    glVertex2f(+0.4,-0.825);
    glVertex2f(-0.4,-0.825);
    glVertex2f(-0.4,-0.9625);
    glEnd();
    glColor3d(0.0, 0.0, 0.0);
    textf(-0.36,-0.9125,"LEAVE THE GAME & GO TO MAIN MENU");

    if (current_player==0) glColor3f(1.0,1.0,0.0);                                      //sets the colour of grid and text
    else glColor3f(int(current_player/4)%2, int(current_player/2)%2, current_player%2); //as the colour of whose turn it is.

    textf(-0.18,+0.89,"PLAYER");

    if (current_player==0)
        textf(-0.0450,+0.89, " 1");
    else if (current_player==1)
        textf(-0.0450,+0.89, " 2");
    else if (current_player==2)
        textf(-0.0450,+0.89, " 3");
    else if (current_player==3)
        textf(-0.0450,+0.89, " 4");


    textf(+0.00,+0.89,"'S TURN");

    glBegin(GL_LINE_STRIP);  //draws grid.
    float i,j;
    for(i=-6; i<6; i=i+2)
    {
        for(j=-8; j<8; j=j+2)
        {
            glVertex2f(i/10,j/10);
            glVertex2f(i/10+0.2,j/10);
            glVertex2f(i/10+0.2,j/10+0.2);
            glVertex2f(i/10,j/10+0.2);
            glVertex2f(i/10,j/10);
        }
        glVertex2f(i/10+0.2,j/10-0.2);
    }
    glEnd();
    win();     //Call to check if someone has won.

}//end.

//*******************************************************************************************************************************//

//updates the angle of rotation of spheres/balls with time.
void update(int value)
{
    angle+=10.0;
    if(angle>360)
    {
        angle-=360;
    }
    glutTimerFunc(20,update,0);
    glutPostRedisplay(); // calls display function again to redisplay everything.
}//end.

//*******************************************************************************************************************************//

//draws combination of 'i' no. of spheres on scrreen with centre of combination of spheres as x_1,y_1 .
//i denotes no. of spheres in combination to be drawn.
//varying j can change the colour of combination of spheres.
void sphere_mainscreen(float x_1,float y_1,int i,int j)
{
    glColor3f(int(j/4)%2, int(j/2)%2,j%2); //setting colour of spheres with the value of j.

    if(i==1)  //draws one sphere as a combination.
    {
        glTranslatef(x_1, y_1, 0);
        glutSolidSphere(0.02, 200, 300);
        glTranslatef(-x_1,-y_1,0);
    }

    else if(i==2)  //draws two spheres as a combination.
    {
        glRotatef(angle,x_1,y_1,0.0);
        glTranslatef(x_1-0.03, y_1, 0);
        glutSolidSphere(0.04, 200, 300);
        glTranslatef(0.06, 0, 0 );
        glutSolidSphere(0.04, 200, 300);
        glTranslatef(-x_1-0.03,-y_1,0);
        glRotatef(-angle,x_1,y_1,0.0);
    }

    else if(i==3)  //draws three spheres as a combination.
    {
        glRotatef(angle,x_1,y_1,0.0);
        glTranslatef(x_1, y_1+0.02, 0);
        glutSolidSphere(0.03, 200, 300);

        glTranslatef(-0.03,-0.04 , 0);
        glutSolidSphere(0.03, 200, 300);
        glTranslatef(+0.06, 0, 0);
        glutSolidSphere(0.03, 200, 300);

        glTranslatef(-x_1-0.03,-y_1+0.02,0);
        glRotatef(-angle,x_1,y_1,0.0);
    }
    else {
        glRotatef(angle,x_1,y_1,0.0);
        glTranslatef(x_1, y_1+0.02, 0);
        glutSolidSphere(0.03, 200, 300);

        glTranslatef(-0.03,-0.04 , 0);
        glutSolidSphere(0.03, 200, 300);
        glTranslatef(+0.06, 0, 0);
        glutSolidSphere(0.03, 200, 300);

        glTranslatef(-x_1-0.03,-y_1+0.02,0);
        glRotatef(-angle,x_1,y_1,0.0);
    	}
}

//*******************************************************************************************************************************//

//draws sphere(s)/ball(s) in the cell present in i'th row and j'th column of the grid.
//no. of spheres/balls to be drawn is decided by ball_count[i][j].
//colour of sphere(s)/ball(s) to be drawn is decided by ball_colour[i][j].
void sphere(int i, int j)
{
    float x_1=-0.5+0.2*j, y_1=0.7-0.2*i;
    if (ball_colour[i][j]==0) glColor3f(1.0,1.0,0.0);
    else glColor3f((ball_colour[i][j]/4)%2, (ball_colour[i][j]/2)%2, ball_colour[i][j]%2);

    if(ball_count[i][j]==1)  //places one ball.
    {
        glTranslatef(x_1, y_1, 0);
        glutSolidSphere(0.05, 200, 300);
        glTranslatef(-x_1,-y_1,0);
    }

    if(ball_count[i][j]==2)  //places two balls.
    {
        glRotatef(angle,x_1,y_1,0.0);
        glTranslatef(x_1-0.03, y_1, 0);
        glutSolidSphere(0.04, 200, 300);
        glTranslatef(0.06, 0, 0 );
        glutSolidSphere(0.04, 200, 300);
        glTranslatef(-x_1-0.03,-y_1,0);
        glRotatef(-angle,x_1,y_1,0.0);
    }

    if(ball_count[i][j]==3)   //places three balls.
    {
        glRotatef(angle,x_1,y_1,0.0);
        glTranslatef(x_1, y_1+0.02, 0);
        glutSolidSphere(0.03, 200, 300);
        glTranslatef(-0.03,-0.04 , 0);
        glutSolidSphere(0.03, 200, 300);
        glTranslatef(+0.06, 0, 0);
        glutSolidSphere(0.03, 200, 300);
        glTranslatef(-x_1-0.03,-y_1+0.02,0);
        glRotatef(-angle,x_1,y_1,0.0);

    }
}//end.

//*******************************************************************************************************************************//

//If someone clicks the mouse on the present window, this function decides whwther it is a valid click or not.
//If it is valid then this function makes changes in the variables according to the position of mouse click on present windoow.
//After making changes, it calls display function to redisplay everything with changes implemented.
void mouse_click(int button,int state,int x,int y)
{
    mouse_point=mouse(x,y);   // takes the coordinates of mouse in the form of window coordinates
    if ((button==GLUT_LEFT_BUTTON)&&(state==GLUT_UP))
    {
        switch(window)  //denotes which window is being displayed at present.
        {
        case 0 :        //if current window being displayed is initial window with menu buttons.
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(0.25<mouse_point.y)&&(mouse_point.y<0.3875))
            {
                //if clicked within the region of on 'NEW GAME' button on the window,
                window=2;
                glutPostRedisplay();
            }
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(-0.05<mouse_point.y)&&(mouse_point.y<0.0875))
            {
                //if clicked within the region of on 'INSTRUCTIONS' button on the window,
                window=3;
                glutPostRedisplay();
            }
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(-0.35<mouse_point.y)&&(mouse_point.y<-0.2125))
            {
                //if clicked within the region of on 'EXIT' button on the window,
                exit(0);
            }
            break;
        case 1 :        //if current window being displayed is the window where the game can be played.
            if((-0.4<mouse_point.x)&&(mouse_point.x<0.4)&&(-0.9625<mouse_point.y)&&(mouse_point.y<-0.825))
            {
                //if clicked within the region of on 'LEAVE THE GAME & GO TO MAIN MENU' button on the window,
                initialize();
                window=0;
                glutPostRedisplay();
            }
            if((-0.6<mouse_point.x)&&(mouse_point.x<0.6)&&(-0.8<mouse_point.y)&&(mouse_point.y<0.8))
            {
                //if clicked within the region of GRID ON THE WINDOW
                j_1=(mouse_point.x+0.6)/0.2;  //gets the cell in which mouse is clicked from mouse coordinates
                i_1=(-mouse_point.y+0.8)/0.2; //in terms of i_1'th row and j_1'th column.
                if(ball_colour[i_1][j_1]==-1||ball_colour[i_1][j_1]==current_player)
                {
                    //if the cell where mouse is clicked has no balls or balls of the same player whose turn it is.
                    is_chance=true;
                    ball_count[i_1][j_1]++;
                    ball_colour[i_1][j_1]=current_player; 
                }
                else
                {
                    is_chance=false;
                }; 
                rearrange();
                glutPostRedisplay();

                if(is_chance)   //switching the current_player to next player if he has played a valid chance.
                {
                    while(true)
                    {
                        current_player=(current_player+1)%max_players;
                        if(player[current_player]==0)
                            break;
                    }
                }
            }
            break;
        case 2:        //if current window being displayed is the window to choose no. of players to play.
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(0.25<mouse_point.y)&&(mouse_point.y<0.3875))
            {
                //if clicked within the region of on 'TWO' button on the window,
                window=1;
                max_players=2;
                glutPostRedisplay();
            }
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(-0.05<mouse_point.y)&&(mouse_point.y<0.0875))
            {
                //if clicked within the region of on 'THREE' button on the window,
                window=1;
                max_players=3;
                glutPostRedisplay();
            }
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(-0.35<mouse_point.y)&&(mouse_point.y<-0.2125))
            {
                //if clicked within the region of on 'FOUR' button on the window,
                window=1;
                max_players=4;
                glutPostRedisplay();
            }
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(-0.85<mouse_point.y)&&(mouse_point.y<-0.7125))
            {
                //if clicked within the region of on 'BACK' button on the window,
                window=0;
                glutPostRedisplay();
            }
            break;
        case 3:        //if current window being displayed is the window to show instructions to play the game.
            if((-0.2<mouse_point.x)&&(mouse_point.x<0.2)&&(-0.85<mouse_point.y)&&(mouse_point.y<-0.7125))
            {
                //if clicked within the region of on 'BACK' button on the window,
                window=0;
                glutPostRedisplay();
            }
            break;
        case 4:        //if current window being displayed is the window showing win, if someone has won.
            if((-0.7<mouse_point.x)&&(mouse_point.x<-0.3)&&(-0.9625<mouse_point.y)&&(mouse_point.y<-0.825))
            {
                //if clicked within the region of on 'MAIN MENU' button on the window,
                initialize();
                window=0;
                glutPostRedisplay();
            }
            if((+0.3<mouse_point.x)&&(mouse_point.x<0.7)&&(-0.9625<mouse_point.y)&&(mouse_point.y<-0.825))
            {
                //if clicked within the region of on 'EXIT' button on the window,
                exit(0);
            }
            break;
        default:
            ;
        }
    }
}//end.

//*******************************************************************************************************************************//

//checks whether explosion of balls ocuur and makes changes in variables accordingly.
//this check is performed repeatedly untill no new explosion is occuring after previous explosion.
void   rearrange()
{
   while(true)
    {
        int check=0;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<6; j++)
            {
                if(max_balls[i][j]<=ball_count[i][j])
                {
                    if(i<7)
                    {
                        ball_count[i+1][j]++;
                        ball_count[i][j]--;
                        ball_colour[i+1][j]=current_player;
                        ball_colour[i][j]=-1;
                    }
                    if(i>0)
                    {
                        ball_count[i-1][j]++;
                        ball_count[i][j]--;
                        ball_colour[i-1][j]=current_player;
                        ball_colour[i][j]=-1;
                    }
                    if(j>0)
                    {
                        ball_count[i][j-1]++;
                        ball_count[i][j]--;
                        ball_colour[i][j-1]=current_player;
                        ball_colour[i][j]=-1;
                    }
                    if(j<5)
                    {
                        ball_count[i][j+1]++;
                        ball_count[i][j]--;
                        ball_colour[i][j+1]=current_player;
                        ball_colour[i][j]=-1;
                    }
                }
            }
        }
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<6; j++)
            {
                if(max_balls[i][j]>ball_count[i][j])
                {
                    check++;
                }
            }
        }
        if (check==48) //stops checking for explosion and rearranging variables if each of the cells
            break;     // have less no. of balls than max. no. of balls that corresponding cell can have. 
    }
    eliminate(); // Call to check if some player is eliminated.
} //end.

//*******************************************************************************************************************************//

//after each player has played at least one chance, then this function checks if there is a player
//with no balls left on the grid. If so, that player has lost, then this function eliminates that player from the game.
void eliminate()
{
    int sum=0,i,j,a;
    for(i=0; i<8; i++)
    {
        for(j=0; j<6; j++)
        {
            sum+=ball_count[i][j];
        }
    }
    if(sum>max_players)
    {
        for(a=0; a<max_players; a=a+1)
        {
            int count1=0;
            if(player[a]==0)
            {
                for(i=0; i<8; i++)
                {
                    for(j=0; j<6; j++)
                    {
                        if(ball_colour[i][j]==a)
                        {
                            count1++;
                        }
                    }
                }
            }
            if(count1==0)
            {
                player[a]=1;
            }
        }
    }
}//end.

//*******************************************************************************************************************************//

//checks if all the players playing except one player are eliminated i.e. if someone has won,
//If so, then this function calls the display function to show who has won.
void win()
{
    int a,sum=0;
    for(a=0; a<max_players; a++)
    {
        sum+=player[a];
    }
    if(sum==max_players-1)
    {
        window=4;
        glutPostRedisplay;
    }
}//end.

//*******************************************************************************************************************************//

//If some player has won the game, then this function shows who has won on the window.
void won_player()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (current_player==0) glColor3f(1.0,1.0,0.0);
    else glColor3f(int(current_player/4)%2, int(current_player/2)%2, current_player%2);
    textf1(-0.28,+0.89,"PLAYER");

    if (current_player==0)
        textf1(-0.0450,+0.89, " 1");
    else if (current_player==1)
        textf1(-0.0450,+0.89," 2");
    else if (current_player==2)
        textf1(-0.0450,+0.89," 3");
    else if (current_player==3)
        textf1(-0.0450,+0.89, " 4");

    textf1(+0.00,+0.89," HAS WON");
    int k;
    for(k=-1; k<2; k++) //draws rectangle around text.
    {
        if(k!=0)
        {
            glBegin(GL_QUAD_STRIP);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(k*0.7,-0.9625);
            glVertex2f(k*0.3,-0.9625);
            glVertex2f(k*0.7,-0.825);
            glVertex2f(k*0.3,-0.825);
            glEnd();

            glColor3f(1.0,1.0,1.0);
            glBegin(GL_LINE_STRIP);
            glVertex2f(k*0.7,-0.9625);
            glVertex2f(k*0.3,-0.9625);
            glVertex2f(k*0.3,-0.825);
            glVertex2f(k*0.7,-0.825);
            glVertex2f(k*0.7,-0.9625);
            glEnd();
        }
    }

    glColor3d(0.0, 0.0, 0.0);
    textf(-0.60125,-0.9125,"MAIN MENU");
    textf(+0.455,-0.9125,"EXIT");

    if (current_player==0) glColor3f(1.0,1.0,0.0);
    else glColor3f(int(current_player/4)%2, int(current_player/2)%2, current_player%2);
    glBegin(GL_LINE_STRIP);
    float i,j;
    for(i=-6; i<6; i=i+2)
    {
        for(j=-8; j<8; j=j+2)
        {
            glVertex2f(i/10,j/10);
            glVertex2f(i/10+0.2,j/10);
            glVertex2f(i/10+0.2,j/10+0.2);
            glVertex2f(i/10,j/10+0.2);
            glVertex2f(i/10,j/10);
        }
        glVertex2f(i/10+0.2,j/10-0.2);
    }
    glEnd();
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<6; j++)
            {
                sphere(i,j);
            }
        }
    }
}//end.

//*******************************************************************************************************************************//

//exits the application if 'ESC' button on the keyb oard is pressed.
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0); // Exit the application if 'Esc' key is pressed
    }
}//end.

//*******************************************************************************************************************************//

//main program.
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);                //Initializes glut.
    initialize();                         //initializes variables used in program.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize(800,600);          //Makes the window size to 800 x 600 pixels.
    glutInitWindowPosition(0,0);          //Makes the position of top left corner of the window to the top left
    //corner of computer's screen.
    glutCreateWindow("Chain Reaction");   //Creates window with title "Chain Reaction".
    glutDisplayFunc(display);             //Call to display function.
    glutKeyboardFunc (keyboard);          //call to keyboard function for keyboard input.
    glutMouseFunc(mouse_click);           //call to mouse function for keyboard input.
    glutTimerFunc(20,update,0);           //call to timer function to update angle rotated with time to show rotation of balls
    glutMainLoop();
    return 0;
}

//*******************************************************************************************************************************//
//*************************************************************END***************************************************************//
//*******************************************************************************************************************************//


