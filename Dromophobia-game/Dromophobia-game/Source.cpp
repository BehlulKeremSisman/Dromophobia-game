/* ///////////////////////////////////
/									 /
/	Created by: BEHLUL KEREM SISMAN  /
/									 /
*/ ///////////////////////////////////

#include <stdlib.h>	
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <list>
#include <string>
#include <ctime>

GLint windowHeight, windowWidth;
GLint Tx = 250, Ty = 24, Lx = 242, Ly = 0.0, Rx = 258, Ry = 0.0;	// T=Top edge, R=right edge, L=left edge (agent)
GLint agentDirectionControl;		//0=up	1=down,
int yForCoinsArray[19] = { 36,60,84,108,156,180,204,252,276,300,324,372,396,420,444,492,516,540,564 };  //lanes' y coordinates for coins
int yForSideWalks[5] = { 132,228,348,468,588 };	//y coordinates of sidewalks
int coinY1 = rand() % 19, coinY2 = rand() % 19, coinY3 = rand() % 19, coinY4 = rand() % 19, coinY5 = rand() % 19; //to generate 5 y coordinate for 5 coins
int coinX1 = rand() % 476 + 12, coinX2 = rand() % 476 + 12, coinX3 = rand() % 476 + 12, coinX4 = rand() % 476 + 12, coinX5 = rand() % 476 + 12;	//to generate 5 x coordinate for 5 coins
int coinsY[5] = { yForCoinsArray[coinY1],yForCoinsArray[coinY2],yForCoinsArray[coinY3],yForCoinsArray[coinY4],yForCoinsArray[coinY5] };	//y coordinates for 5 coins
int coinsX[5] = { coinX1,coinX2,coinX3,coinX4,coinX5 };	//x coordinates for 5 coins
int point = 0;
int lockSideWalksForExtraPoint = 0; //0=not locked, 1=locked
bool paused = false, gameOver = false;
char pointString[30];	//for render the integer point on the screen
int laneY = 36; //lanes' y coordinates
bool oneStep = true;
int makeYouWaitForStart = 0; // this makes you dont let you go up approximately 8-10 seconds for waiting the vehicles to come lanes.
int whichList = 1;

class Lanes {
public:
	typedef struct {
		GLint x;
		GLint y;
	} Point2D;

	typedef struct {
		GLubyte r;
		GLubyte g;
		GLubyte b;
	} Color;

	typedef struct vehicle {	// vehicle struct
		Point2D center;
		Color color;
		int carOrTruck; // 0 = car, 1 = truck
		struct vehicle *next;
	}Vehicle;

	typedef struct liste {		//lanes struct. Lanes = liste1, liste2, liste3, ......, liste19
		int laneFromRightorLeft;	//left=0, right=1. Vehicles come from which way to the lane
		int rate;	//rate for lanes
		Vehicle* head;
		int numberOfVehicle;
	}Liste;

	Liste liste1, liste2, liste3, liste4, liste5, liste6, liste7, liste8, liste9, liste10, liste11, liste12, liste13, liste14, liste15, liste16, liste17, liste18, liste19; // linklists for each lane

	void initVehicle(Liste *liste) {
		liste->head = (Vehicle*)malloc(sizeof(Vehicle));
		liste->head->next = NULL;
		liste->laneFromRightorLeft = rand() % 2;
		liste->numberOfVehicle = 0;
		liste->rate = rand() % 30;
	}

	void insert(Liste *liste) {
		Vehicle* newVehicle = (Vehicle*)malloc(sizeof(Vehicle));
		newVehicle->carOrTruck = rand() % 2;
		if (liste->laneFromRightorLeft == 0)
			newVehicle->center.x = 10;
		if (liste->laneFromRightorLeft == 1)
			newVehicle->center.x = 590;
		newVehicle->center.y = laneY;
		newVehicle->color.r = (GLubyte)rand() % 256;
		newVehicle->color.g = (GLubyte)rand() % 256;
		newVehicle->color.b = (GLubyte)rand() % 256;
		newVehicle->next = NULL;

		if (liste->numberOfVehicle == 0)	// if list is empty
			liste->head->next = newVehicle;
		else {
			Vehicle* ptr = liste->head;
			int i = 0;
			while (i < liste->numberOfVehicle) {
				ptr = ptr->next;
				i++;
			}
			ptr->next = newVehicle;
		}
		newVehicle->next = NULL;
		liste->numberOfVehicle++;
	}

	void remove(Liste *liste) {
		liste->numberOfVehicle--;
		Vehicle *ptr = liste->head;
		while (ptr->next != NULL) {
			if (ptr->next->center.x > 800 || ptr->next->center.x < -300) {
				Vehicle *tmp = ptr->next;
				ptr->next = ptr->next->next;
				free(tmp);
			}
			ptr = ptr->next;
		}
	}

	void drawVehicle(Liste *liste) {
		Vehicle *ptr = liste->head;
		while (ptr->next != NULL) {
			if (ptr->carOrTruck == 0) {	//draw cars 20 weight 20 height
				glColor3ub(ptr->next->color.r, ptr->next->color.g, ptr->next->color.b);
				glRectf(ptr->next->center.x - 10, ptr->next->center.y - 10, ptr->next->center.x + 10, ptr->next->center.y + 10);
			}
			if (ptr->carOrTruck == 1) {	//draw truck 40 weight 20 height
				glColor3ub(ptr->next->color.r, ptr->next->color.g, ptr->next->color.b);
				glRectf(ptr->next->center.x - 20, ptr->next->center.y - 10, ptr->next->center.x + 20, ptr->next->center.y + 10);
			}
			ptr = ptr->next;
		}
	}

	void updateVehicle(Liste *liste) {
		Vehicle *p = liste->head;
		while (p->next != NULL) {
			if (liste->laneFromRightorLeft == 0)	//if vehicles comes from left move them right
				p->next->center.x += 2;
			if (liste->laneFromRightorLeft == 1)	//if vehicles comes from right move them left
				p->next->center.x -= 2;
			if (((p->next->center.x == 150 + liste->rate || p->next->center.x == 151 + liste->rate) && liste->laneFromRightorLeft == 0) || ((p->next->center.x == 450 + liste->rate || p->next->center.x == 451 + liste->rate) && liste->laneFromRightorLeft == 1))
				insert(liste);
			p = p->next;
		}
		Vehicle *p2 = liste->head;
		while (p2->next != NULL) {
			if (p2->next->center.x > 800 || p2->next->center.x < -300)
				remove(liste);
			p2 = p2->next;
		}
	}

	void collision(Liste *liste) {
		Vehicle *p = liste->head;
		while (p->next != NULL) {
			if (agentDirectionControl == 0) {	// if agent's way to up
				if (p->carOrTruck == 0) {	// if the vehicle is car
					if (Lx - p->next->center.x < 10 && Lx - p->next->center.x >= 0 && p->next->center.y == Ty - 12 || p->next->center.x - Rx < 10 && p->next->center.x - Rx >= 0 && p->next->center.y == Ty - 12)
						gameOver = true;
				}
				if (p->carOrTruck == 1) {	// if the vehicle is truck
					if (Lx - p->next->center.x < 20 && Lx - p->next->center.x >= 0 && p->next->center.y == Ty - 12 || p->next->center.x - Rx < 20 && p->next->center.x - Rx >= 0 && p->next->center.y == Ty - 12)
						gameOver = true;
				}
			}
			if (agentDirectionControl == 1) {	// if agent's way to down
				if (p->carOrTruck == 0) {	// if the vehicle is car
					if (Lx - p->next->center.x < 10 && Lx - p->next->center.x >= 0 && p->next->center.y == Ty + 12 || p->next->center.x - Rx < 10 && p->next->center.x - Rx >= 0 && p->next->center.y == Ty + 12)		// Agent is lefter than car
						gameOver = true;
				}
				if (p->carOrTruck == 1) {	// if the vehicle is truck
					if (Lx - p->next->center.x < 20 && Lx - p->next->center.x >= 0 && p->next->center.y == Ty + 12 || p->next->center.x - Rx < 20 && p->next->center.x - Rx >= 0 && p->next->center.y == Ty + 12)		// Agent is lefter than truck
						gameOver = true;
				}
			}
			p = p->next;
		}
	}

	void debug(Liste *liste) {
		Vehicle *p = liste->head;
		while (p->next != NULL) {
			if (p->next->center.y != 36 && p->next->center.x > 0 && p->center.x < 500)
				printf("\n %d. Lane's vehicles' center x coordinates=%d and %d. Lane's vehicles' center y coordinates=%d", whichList, p->next->center.x, whichList, p->next->center.y);
			p = p->next;
		}
		printf("\n Agent's Top edge of x coordinate=%d and Top edge of y coordinate=%d \n Agent's Left edge of x coordinate=%d and Left edge of y coordinate=%d \n Agent's Right edge of x coordinate=%d and Right edge of y coordinate=%d \n ", Tx, Ty, Lx, Ly, Rx, Ry);

		paused == false;
		whichList++;
		glutPostRedisplay();
	}

};

void drawCoin(float x, float y, float r, int segments) {
	glBegin(GL_TRIANGLE_FAN);	// to draw filled square..
	glVertex2f(x, y);
	for (int n = 0; n <= segments; ++n) {
		glColor3f(0.8, 0.7, 0.1);
		float const t = 2 * 3.14 * (float)n / (float)segments;
		glVertex2f(x + sin(t) * r, y + cos(t) * r);
	}
	glEnd();
}

void calculatePoints() {
	for (int j = 0; j < 5; j++) {
		if (Ty - 12 == yForSideWalks[j] && agentDirectionControl == 0 && lockSideWalksForExtraPoint == 0) { // agent's direction is up
			point++;
			lockSideWalksForExtraPoint = 1;
		}
		if (Ty + 12 == yForSideWalks[j] && agentDirectionControl == 1 && lockSideWalksForExtraPoint == 0) { //agent's direction is down
			point++;
			lockSideWalksForExtraPoint = 1;
		}
	}
	for (int i = 0; i < 5; i++) {
		if (Ty - 12 == coinsY[i] && agentDirectionControl == 0 && Tx - coinsX[i] <= 24 && Tx - coinsX[i] >= 0 || Ty - 12 == coinsY[i] && agentDirectionControl == 0 && coinsX[i] - Tx <= 24 && coinsX[i] - Tx >= 0) { // agent's direction is up
			point += 5;
			int a = rand() % 19;
			coinsY[i] = yForCoinsArray[a];
			coinsX[i] = rand() % 476 + 12;
			drawCoin(coinsX[i], coinsY[i], 12, 1000);
		}
		if (Ty + 12 == coinsY[i] && agentDirectionControl == 1 && Tx - coinsX[i] <= 24 && Tx - coinsX[i] >= 0 || Ty + 12 == coinsY[i] && agentDirectionControl == 1 && coinsX[i] - Tx <= 24 && coinsX[i] - Tx >= 0) { //agent's direction is down
			point += 5;
			int b = rand() % 19;
			coinsY[i] = yForCoinsArray[b];
			coinsX[i] = rand() % 476 + 12;
			drawCoin(coinsX[i], coinsY[i], 12, 1000);
		}
		sprintf(pointString, "TIME : %2d Sec", point);
	}
}

void RenderString(GLdouble x, GLdouble y, const std::string &string) {	// to render texts on screen
	glRasterPos2d(x, y);
	for (int n = 0; n < string.size(); ++n)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[n]);
}

Lanes L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, L13, L14, L15, L16, L17, L18, L19; //Lanes


void myReshape(int winWidth, int winHeight) {
	windowHeight = winHeight;
	windowWidth = winWidth;
	glViewport(0, 0, winWidth, winHeight);  // update the viewport
	glMatrixMode(GL_PROJECTION);              // update the projection
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 600);           // map unit square to viewport
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();                      // request redisplay
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);       // clear the window
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0, 0, 500, 24); 	//sidewalk-1
	glColor3f(1.0, 1.0, 1.0);
	glRectf(0, 24, 500, 120);	//road-1
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0, 120, 500, 144);	//sidewalk-2
	glColor3f(1.0, 1.0, 1.0);
	glRectf(0, 144, 500, 216);	//road-2
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0, 216, 500, 240);	//sidewalk-3
	glColor3f(1.0, 1.0, 1.0);
	glRectf(0, 240, 500, 336); 	//road-3
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0, 336, 500, 360);	//sidewalk-4
	glColor3f(1.0, 1.0, 1.0);
	glRectf(0, 360, 500, 456);	//road-4
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0, 456, 500, 480);	//sidewalk-5
	glColor3f(1.0, 1.0, 1.0);
	glRectf(0, 480, 500, 576);	//road-5
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0, 576, 500, 600);	//sidewalk-6
	glColor3f(1.0, 0.0, 0.0);
	// to draw lane boundaries
	int f[14] = { 48,72,96,168,192,264,288,312,384,408,432,504,528,552 };  // lane boundaries' y coordinates array
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINES);
	for (int y = 0; y < 14; y++)
		for (float x = 0.0; x < 500; x = x + 10)
			glVertex2f(x, f[y]);
	glEnd();
	// to draw coins
	glColor3f(1.0, 0.8, 0.1);
	for (int i = 0; i < 5; i++)
		drawCoin(coinsX[i], coinsY[i], 12, 1000);
	// to draw POINTS text
	glColor3f(1.0, 0.0, 0.0);
	sprintf(pointString, "POINTS : %d", point);
	RenderString(380, 585, pointString);
	// to draw vehicles
	L1.drawVehicle(&L1.liste1);
	L2.drawVehicle(&L2.liste2);
	L3.drawVehicle(&L3.liste3);
	L4.drawVehicle(&L4.liste4);
	L5.drawVehicle(&L5.liste5);
	L6.drawVehicle(&L6.liste6);
	L7.drawVehicle(&L7.liste7);
	L8.drawVehicle(&L8.liste8);
	L9.drawVehicle(&L9.liste9);
	L10.drawVehicle(&L10.liste10);
	L11.drawVehicle(&L11.liste11);
	L12.drawVehicle(&L12.liste12);
	L13.drawVehicle(&L13.liste13);
	L14.drawVehicle(&L14.liste14);
	L15.drawVehicle(&L15.liste15);
	L16.drawVehicle(&L16.liste16);
	L17.drawVehicle(&L17.liste17);
	L18.drawVehicle(&L18.liste18);
	L19.drawVehicle(&L19.liste19);
	//to draw agent
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(Lx, Ly);
	glVertex2f(Rx, Ry);
	glVertex2f(Tx, Ty);
	glEnd();
	// to draw pause gameover and points text on the screen
	if (paused == true && gameOver == false) {
		glColor3f(1.0, 0.0, 0.0);
		RenderString(200, 270, "-PAUSED-");
	}
	if (gameOver == true) {
		glColor3f(1.0, 0.0, 0.0);
		RenderString(200, 270, "-GAME OVER-");
	}

	glFlush();				// force OpenGL to render now
	glutSwapBuffers();			// swap buffers
}

void myKeyboard(unsigned char c, int x, int y) {
	if ((c == 'Q') || (c == 'q'))		// hit q when you want to quit
		exit(0);
	if ((c == 'R') || (c == 'r')) 	// hit r when you want to continue(god mod)
		gameOver = false;

	glutPostRedisplay();		// request redisplay
}

void myMouse(int b, int s, int x, int y) {
	if (b == GLUT_MIDDLE_BUTTON && s == GLUT_DOWN)
		paused = true;
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN)
		paused = false;
	if (b == GLUT_RIGHT_BUTTON && s == GLUT_DOWN) {
		paused = true;
		L1.debug(&L1.liste1);
		L2.debug(&L2.liste2);
		L3.debug(&L3.liste3);
		L4.debug(&L4.liste4);
		L5.debug(&L5.liste5);
		L6.debug(&L6.liste6);
		L7.debug(&L7.liste7);
		L8.debug(&L8.liste8);
		L9.debug(&L9.liste9);
		L10.debug(&L10.liste10);
		L11.debug(&L11.liste11);
		L12.debug(&L12.liste12);
		L13.debug(&L13.liste13);
		L14.debug(&L14.liste14);
		L15.debug(&L15.liste15);
		L16.debug(&L16.liste16);
		L17.debug(&L17.liste17);
		L18.debug(&L18.liste18);
		L19.debug(&L19.liste19);
		whichList = 1;

		L1.updateVehicle(&L1.liste1);
		laneY += 24;
		L2.updateVehicle(&L2.liste2);
		laneY += 24;
		L3.updateVehicle(&L3.liste3);
		laneY += 24;
		L4.updateVehicle(&L4.liste4);
		laneY += 48;
		L5.updateVehicle(&L5.liste5);
		laneY += 24;
		L6.updateVehicle(&L6.liste6);
		laneY += 24;
		L7.updateVehicle(&L7.liste7);
		laneY += 48;
		L8.updateVehicle(&L8.liste8);
		laneY += 24;
		L9.updateVehicle(&L9.liste9);
		laneY += 24;
		L10.updateVehicle(&L10.liste10);
		laneY += 24;
		L11.updateVehicle(&L11.liste11);
		laneY += 48;
		L12.updateVehicle(&L12.liste12);
		laneY += 24;
		L13.updateVehicle(&L13.liste13);
		laneY += 24;
		L14.updateVehicle(&L14.liste14);
		laneY += 24;
		L15.updateVehicle(&L15.liste15);
		laneY += 48;
		L16.updateVehicle(&L16.liste16);
		laneY += 24;
		L17.updateVehicle(&L17.liste17);
		laneY += 24;
		L18.updateVehicle(&L18.liste18);
		laneY += 24;
		L19.updateVehicle(&L19.liste19);
		laneY = 36;
	}
	glutPostRedisplay();
}

void myTimeOut(int id) {
	if (Ty == 600) {	// when the agent came to the top
		Ty = Ty - 24;
		Ry = Ry + 24;
		Ly = Ly + 24;
		agentDirectionControl = 1;	// agent's direction to down
	}
	if (Ty == 0) {		// when the agent came to the bottom
		Ty = Ty + 24;
		Ry = Ry - 24;
		Ly = Ly - 24;
		agentDirectionControl = 0;	// agent's direction to up
	}
	if (paused == false && gameOver == false) {
		L1.updateVehicle(&L1.liste1);
		laneY += 24;
		L2.updateVehicle(&L2.liste2);
		laneY += 24;
		L3.updateVehicle(&L3.liste3);
		laneY += 24;
		L4.updateVehicle(&L4.liste4);
		laneY += 48;
		L5.updateVehicle(&L5.liste5);
		laneY += 24;
		L6.updateVehicle(&L6.liste6);
		laneY += 24;
		L7.updateVehicle(&L7.liste7);
		laneY += 48;
		L8.updateVehicle(&L8.liste8);
		laneY += 24;
		L9.updateVehicle(&L9.liste9);
		laneY += 24;
		L10.updateVehicle(&L10.liste10);
		laneY += 24;
		L11.updateVehicle(&L11.liste11);
		laneY += 48;
		L12.updateVehicle(&L12.liste12);
		laneY += 24;
		L13.updateVehicle(&L13.liste13);
		laneY += 24;
		L14.updateVehicle(&L14.liste14);
		laneY += 24;
		L15.updateVehicle(&L15.liste15);
		laneY += 48;
		L16.updateVehicle(&L16.liste16);
		laneY += 24;
		L17.updateVehicle(&L17.liste17);
		laneY += 24;
		L18.updateVehicle(&L18.liste18);
		laneY += 24;
		L19.updateVehicle(&L19.liste19);
		laneY = 36;

		makeYouWaitForStart++;
	}
	glutPostRedisplay();			  // request redisplay
	glutTimerFunc(20, myTimeOut, 0);  // request next timer event
}

void idle() {
	L1.collision(&L1.liste1);
	L2.collision(&L2.liste2);
	L3.collision(&L3.liste3);
	L4.collision(&L4.liste4);
	L5.collision(&L5.liste5);
	L6.collision(&L6.liste6);
	L7.collision(&L7.liste7);
	L8.collision(&L8.liste8);
	L9.collision(&L9.liste9);
	L10.collision(&L10.liste10);
	L11.collision(&L11.liste11);
	L12.collision(&L12.liste12);
	L13.collision(&L13.liste13);
	L14.collision(&L14.liste14);
	L15.collision(&L15.liste15);
	L16.collision(&L16.liste16);
	L17.collision(&L17.liste17);
	L18.collision(&L18.liste18);
	L19.collision(&L19.liste19);
	glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y) {
	// moves for way keys
	if (paused == false && gameOver == false) {
		if (key == GLUT_KEY_RIGHT && Rx < 484) {	// to preserve the agent out of window Rx < 476
			Tx = Tx + 16;
			Rx = Rx + 16;
			Lx = Lx + 16;
			lockSideWalksForExtraPoint = 1;
		}
		if (key == GLUT_KEY_LEFT && Lx > 16) {		// to preserve the agent out of window Lx > 24
			Tx = Tx - 16;
			Rx = Rx - 16;
			Lx = Lx - 16;
			lockSideWalksForExtraPoint = 1;
		}
		if (key == GLUT_KEY_UP && agentDirectionControl == 0 && makeYouWaitForStart > 500) {		// to preserve going back when the agent's direction is up
			Ty = Ty + 24;
			Ry = Ry + 24;
			Ly = Ly + 24;
			lockSideWalksForExtraPoint = 0;
			if (Ty == 576 || Ty == 456 || Ty == 336 || Ty == 216 || Ty == 120)	//to preserve ekstra points while trying to go opposite way in sidewalks
				lockSideWalksForExtraPoint = 1;
		}
		if (key == GLUT_KEY_DOWN && agentDirectionControl == 1) {		// to preserve going up when the agent's direction is down
			Ty = Ty - 24;
			Ry = Ry - 24;
			Ly = Ly - 24;
			lockSideWalksForExtraPoint = 0;
			if (Ty == 144 || Ty == 240 || Ty == 360 || Ty == 480 || Ty == 24)	//to preserve ekstra points while trying to go opposite way in sidewalks
				lockSideWalksForExtraPoint = 1;
		}
		calculatePoints();
		glutPostRedisplay();
	}
}

int main(int argc, char **argv) {
	srand(time(NULL));
	glutInit(&argc, argv);   //Initialize glut and gl
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	// Double buffering and RGB color mode
	glutInitWindowSize(500, 600);   // 500x600 window
	glutInitWindowPosition(0, 0);    // upper left
	glutCreateWindow("My First Game"); 	// create the window

										//register callbacks
	glutReshapeFunc(myReshape);     // call myReshape if window is resized
	glutMouseFunc(myMouse);         // call in mouse event 
	glutDisplayFunc(myDisplay);		//  call myDisplay to redraw window
	glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit	
	glutSpecialFunc(specialKeyInput);
	glutTimerFunc(20, myTimeOut, 0);
	glutIdleFunc(idle);

	// Initializations for each lanes(lists)
	L1.initVehicle(&L1.liste1);
	L1.insert(&L1.liste1);
	L2.initVehicle(&L2.liste2);
	L2.insert(&L2.liste2);
	L3.initVehicle(&L3.liste3);
	L3.insert(&L3.liste3);
	L4.initVehicle(&L4.liste4);
	L4.insert(&L4.liste4);
	L5.initVehicle(&L5.liste5);
	L5.insert(&L5.liste5);
	L6.initVehicle(&L6.liste6);
	L6.insert(&L6.liste6);
	L7.initVehicle(&L7.liste7);
	L7.insert(&L7.liste7);
	L8.initVehicle(&L8.liste8);
	L8.insert(&L8.liste8);
	L9.initVehicle(&L9.liste9);
	L9.insert(&L9.liste9);
	L10.initVehicle(&L10.liste10);
	L10.insert(&L10.liste10);
	L11.initVehicle(&L11.liste11);
	L11.insert(&L11.liste11);
	L12.initVehicle(&L12.liste12);
	L12.insert(&L12.liste12);
	L13.initVehicle(&L13.liste13);
	L13.insert(&L13.liste13);
	L14.initVehicle(&L14.liste14);
	L14.insert(&L14.liste14);
	L15.initVehicle(&L15.liste15);
	L15.insert(&L15.liste15);
	L16.initVehicle(&L16.liste16);
	L16.insert(&L16.liste16);
	L17.initVehicle(&L17.liste17);
	L17.insert(&L17.liste17);
	L18.initVehicle(&L18.liste18);
	L18.insert(&L18.liste18);
	L19.initVehicle(&L19.liste19);
	L19.insert(&L19.liste19);

	glutMainLoop();
	return 0;

}
