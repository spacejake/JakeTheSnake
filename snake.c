/* Jake The Snake game, a snake clone
 * Created by Jacob Morton
 * Version 1.0
 * Date: 03/29/07
 */


#include<stdlib.h>
#include<stdio.h>
#include<glut.h>
#include<math.h>
#include <ctime>

/*Window height and width*/
GLint WIDTH = 440, HIGHT = 480;

/*Segment polygon for snake*/
GLfloat vertices[][3] = {{-2,-2,0},{2,-2,0},{2,2,0},{-2,2,0}};
/*Polygon to place texture of Snake picture on to in main menu*/
GLfloat vert[][3] = {{-200,-200,0},{200,-200,0},{200,200,0},{-200,200,0}};
/*Wall or boundry Polygon*/
GLfloat wallvert[][3] = {{0,0,0},{440,0,0},{440,10,0},{0,10,0},{440,440,0},{430,440,0},{430,10,0},{0,440,0},{0,430,0},{430,430,0},{10,10,0},{10,430,0}};
/*Button indicator*/
GLfloat button[][3] = {{-30,-15,0},{30,-15,0},{30,15,0},{-30,15,0}};
/*main menu Snake Texture Coordinates*/
GLfloat tex[] = {1,0, 0,0, 0,1, 1,1};

/*RGBA colors for snake: Red*/
GLfloat colors[][4] = {{1,0,0,1.0},{1,0,0,1.0},
{1,0,0,1.0}, {1,0,0,1.0}};

/*Colors for main menu polygon*/
GLfloat colors2[][4] = {{1,1,1,0.5},{1,1,1,0.5},
{1,1,1,0.5}, {1,1,1,0.5}};


GLfloat foodcolors[][4] = {{0,1,0,1.0},{0,1,0,1.0},
{0,1,0,1}, {0,1,0,1.0}};


GLfloat wallcolors[][4] = {{0,0,1,1.0},{0,0,1,1.0},
{0,0,1,1}, {0,0,1,1.0},{0,0,1,1.0},{0,0,1,1.0},
{0,0,1,1}, {0,0,1,1.0},{0,0,1,1.0},{0,0,1,1.0},
{0,0,1,1}, {0,0,1,1.0}};

/*Snake Segment polygon indices*/
GLubyte cubeIndices[] = {0,1,2,3};

/*Complete 2D model indicies for boundry*/
GLubyte wallIndices[] = {0,1,2,3, 2,4,5,6, 5,7,8,9, 8,3,10,11};

/*Array of snake segments of x and y coordinates*/
int pos[1000][2];

/*food coordinates*/
int food[2] = {240,120};

/*initial Snake sarting position*/
int initx = 120, inity = 20;

/*score buffer*/
char turnstr[20];

/*Porinter to images that will be read in from image files*/
GLubyte *image;
GLubyte *play;
GLubyte *play2;
GLubyte *quit;
GLubyte *quit2;

int iw, ih, maxV, jiw, jih, jmaxV;
float s;
int i = 0;

/*Screen refreash rate per second*/
int n = 16;

/*Flag determining if game begins or ends, 1 for game end and 0 for play*/
int end = 1;

/*Direction of snake's movement*/
int direction = 0;

/*number of segments the snake has*/
int seg = 25;
int opt = 1;
int score = 0;
int refreash = 1;

/*Handles window's shape, preventing user from changing window size*/
void myReshape(int x, int y){
	glutReshapeWindow(WIDTH,HIGHT);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH,HIGHT);
	glViewport(0,0,WIDTH,HIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,WIDTH,-40.0,440);
	glMatrixMode(GL_MODELVIEW);
}

/*Function displays polygons on to screen*/
void display(){
	int i;
	char* buf;
	int m = 10;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode( GL_FRONT, GL_FILL );
	glPolygonMode( GL_BACK, GL_LINE );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	/*Main Menu*/
	if (end == 1){
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glColorPointer(4,GL_FLOAT, 0, colors2);
		glVertexPointer(3, GL_FLOAT, 0, vert);		
		glTexCoordPointer(2,GL_FLOAT, 0, tex); 

		glPushMatrix();
		glTranslated(220,250,0);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iw,ih,0,GL_RGB,GL_UNSIGNED_BYTE, image);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, cubeIndices);
		glPopMatrix();

		glVertexPointer(3, GL_FLOAT, 0, button);
		glPushMatrix();
		glTranslated(220,90,1);
		if(opt > 0)	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,jiw,jih,0,GL_RGB,GL_UNSIGNED_BYTE, play2);
		else glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,jiw,jih,0,GL_RGB,GL_UNSIGNED_BYTE, play);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, cubeIndices);
		glPopMatrix();

		glPushMatrix();
		glTranslated(220,60,1);
		if (opt < 0) glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,jiw,jih,0,GL_RGB,GL_UNSIGNED_BYTE, quit2);
		else glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,jiw,jih,0,GL_RGB,GL_UNSIGNED_BYTE, quit);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, cubeIndices);
		glPopMatrix();

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
	
	/*Set Snake attributes*/
	glColorPointer(4,GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	
	/*Draw Snake*/
	for(i = 0; i < seg; i++){
	glPushMatrix();
	glTranslated(pos[i][0],pos[i][1],0);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, cubeIndices);
	glPopMatrix();
	} 

	/*Draw Food*/
	glColorPointer(4,GL_FLOAT, 0, foodcolors);
	glPushMatrix();
	glTranslated(food[0],food[1],0);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, cubeIndices);
	glPopMatrix();
	
	/*Draw boundry walls*/
	glVertexPointer(3, GL_FLOAT, 0, wallvert);
	glColorPointer(4,GL_FLOAT, 0, wallcolors);
	glDrawElements(GL_QUADS, 20, GL_UNSIGNED_BYTE, cubeIndices);

	/*Display Score*/
	sprintf(turnstr,"Score: %d", score);

	for (buf=turnstr; *buf != '\0'; buf++){
		glRasterPos2f(m,-30);
    	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *buf);
		m = m + glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24,*buf) + 3;
	}

	/*Flush I/O and Using duel display buffer support*/
	glFlush(); 
	glutSwapBuffers();
}

/*Animation function controls locations of polygons between refreash*/
void animate(int v){
	int i;
	int check = 0;
	int temp;
	if (end != 1){
	/*Update Snake*/
	if (direction == 0){// newx+=4;
		temp = pos[0][0] + 4;
		for (i = seg - 1; i > 0; i--){
			pos[i][0] = pos[i-1][0];
			pos[i][1] = pos[i-1][1];
			if (temp == pos[i][0] && pos[0][1] == pos[i][1]){
				end = 1;
			}
		}
		pos[0][0] = temp;
	}
	if (direction == 1){// newy+=4;
		temp = pos[0][1] + 4;
		for (i = seg - 1; i > 0; i--){
			pos[i][0] = pos[i-1][0];
			pos[i][1] = pos[i-1][1];
			if (pos[0][0] == pos[i][0] && temp == pos[i][1]){
				end = 1;
			}
		}
		pos[0][1] = temp;
	}
	if (direction == 2){// newx-=4;
		temp = pos[0][0] - 4;
		for (i = seg - 1; i > 0; i--){
			pos[i][0] = pos[i-1][0];
			pos[i][1] = pos[i-1][1];
			if (temp == pos[i][0] && pos[0][1] == pos[i][1]){
				end = 1;
			}
		}
		pos[0][0] = temp;
	}
	if (direction == 3){// newy-=4;
		temp = pos[0][1] - 4;
		for (i = seg - 1; i > 0; i--){
			pos[i][0] = pos[i-1][0];
			pos[i][1] = pos[i-1][1];
			if (pos[0][0] == pos[i][0] && temp == pos[i][1]){
				end = 1;
			}
		}
		pos[0][1] = temp;
	}

	/*Update Food*/
	if (pos[0][0] == food[0] && pos[0][1] == food[1]){
		seg+=25;
		score+=25;
		do{
		food[0] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/104)) + 3) * 4;
		food[1] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/104)) + 3) * 4;

		/*Make sure food doesn't spawn where snake exists*/
		for (i = seg - 1; i >= 0; i--){
			if (pos[i][0] == food[0] && food[1] == pos[i][1]){
				check = 1;
				break;
			}
			else check = 0;
		}
		}while(check == 1);
	}
	/*Ends Game if snakes Segments grow to 1000, point to change level in future*/
	if (seg >= 1000){
		end = 1;
	}
	/*Check if snake collides with wall*/
	if (pos[0][0] >= 430 || pos[0][0] <= 10 || pos[0][1] >= 430 || pos[0][1] <= 10) end = 1;
	refreash = 1;
	}

	glutPostRedisplay();

	/*Control Refresh rate*/
	glutTimerFunc(1000/n,animate,v);
}

/*Main menu enter KeyBoard input handling function*/
void myKey(unsigned char key, int x, int y){
	/*Enter Key, address returned by enter key because it is not initially supported by API*/
	if(key == 0x0D){
			/*resets game if "Play" is selected*/
			if (opt > 0){
				end = 0;
				food[0] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/104)) + 3) * 4;
				food[1] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/100)) + 7) * 4;
				for(i = 0; i < 1000; i++){
					pos[i][0] = 0;
					pos[i][1] = 0;
				}
	
				for(i = 0; i < 25; i++){
					pos[i][0] = initx - (i*4);
					pos[i][1] = inity;
				}
				seg = 25;
				direction = 0;
				opt = 1;
				score = 0;
				refreash = 1;
			}
			else exit(0);
	}


	glutPostRedisplay();
}

/*Game Key handleing funcion*/
void processSpecialKeys(int key, int x, int y) {
	int i;
	switch(key) {
	case GLUT_KEY_RIGHT:
		if (direction !=2 && end != 1 && refreash == 1){ 
			direction = 0;
			refreash = 0;
		}
		break;
	case GLUT_KEY_UP:
		if(end == 1) opt*=-1;
		else if (direction !=3 && refreash == 1){
			direction = 1;
			refreash = 0;
		}
		break;
	case GLUT_KEY_LEFT:
		if (direction !=0 && end != 1 && refreash == 1){
			direction = 2;
			refreash = 0;
		}
		break;
	case GLUT_KEY_DOWN:
		if(end == 1) opt*=-1;
		else if (direction !=1 && refreash == 1){
			direction = 3;
			refreash = 0;
		}
		break;
	case GLUT_KEY_INSERT:
		/*Insert Key does same as enter key befor support was added for enter key*/
		if(end == 1){
			if (opt > 0){
				end = 0;
				food[0] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/104)) + 3) * 4;
				food[1] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/100)) + 7) * 4;
				for(i = 0; i < 1000; i++){
					pos[i][0] = 0;
					pos[i][1] = 0;
				}
	
				for(i = 0; i < 25; i++){
					pos[i][0] = initx - (i*4);
					pos[i][1] = inity;
				}
				seg = 25;
				direction = 0;
				opt = 1;
				score = 0;
				refreash = 1;
			}
			else exit(0);
		}
		//else seg+=25;
		break;
	}
	glutPostRedisplay();
}

/*Main function initializes all images and holds main loop*/
int main(int argc, char** argv){
	int i, j, r, g, b;

	/*Main Menu Picture Loading*/
	FILE *fp;
	if((fp = fopen("image.ppm", "r")) == NULL){
		printf("ERROR: Invalid Filename");
	}
	fscanf(fp,"%i %i %i",&iw,&ih, &maxV);
	image = malloc(3*sizeof(GLuint)*(iw * ih));
	s=255./maxV;
	
	for(j=0;j<=(iw * ih);j++){
		fscanf(fp,"%d %d %d",&r,&g,&b);
		image[3*(iw * ih)-3*j-3]=r;
		image[3*(iw * ih)-3*j-2]=g;
		image[3*(iw * ih)-3*j-1]=b;
	}

	glPixelTransferf(GL_RED_SCALE, s);
	glPixelTransferf(GL_GREEN_SCALE, s);
	glPixelTransferf(GL_BLUE_SCALE, s);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	fclose(fp);

	/*Play button option image load*/
	if((fp = fopen("play.ppm", "r")) == NULL){
		printf("ERROR: Invalid Filename");
	}
	fscanf(fp,"%i %i %i",&jiw,&jih, &jmaxV);
	play = malloc(3*sizeof(GLuint)*(jiw * jih));
	s=255./jmaxV;
	
	for(j=0;j<=(jiw * jih);j++){
		fscanf(fp,"%d %d %d",&r,&g,&b);
		play[3*(jiw * jih)-3*j-3]=r;
		play[3*(jiw * jih)-3*j-2]=g;
		play[3*(jiw * jih)-3*j-1]=b;
	}

	glPixelTransferf(GL_RED_SCALE, s);
	glPixelTransferf(GL_GREEN_SCALE, s);
	glPixelTransferf(GL_BLUE_SCALE, s);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	fclose(fp);
	
	if((fp = fopen("play2.ppm", "r")) == NULL){
		printf("ERROR: Invalid Filename");
	}
	fscanf(fp,"%i %i %i",&jiw,&jih, &jmaxV);
	play2 = malloc(3*sizeof(GLuint)*(jiw * jih));
	s=255./jmaxV;
	
	for(j=0;j<=(jiw * jih);j++){
		fscanf(fp,"%d %d %d",&r,&g,&b);
		play2[3*(jiw * jih)-3*j-3]=r;
		play2[3*(jiw * jih)-3*j-2]=g;
		play2[3*(jiw * jih)-3*j-1]=b;
	}

	glPixelTransferf(GL_RED_SCALE, s);
	glPixelTransferf(GL_GREEN_SCALE, s);
	glPixelTransferf(GL_BLUE_SCALE, s);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	fclose(fp);

	/*Quit Option button load*/
	if((fp = fopen("quit.ppm", "r")) == NULL){
		printf("ERROR: Invalid Filename");
	}
	fscanf(fp,"%i %i %i",&jiw,&jih, &jmaxV);
	quit = malloc(3*sizeof(GLuint)*(jiw * jih));
	s=255./jmaxV;
	
	for(j=0;j<=(jiw * jih);j++){
		fscanf(fp,"%d %d %d",&r,&g,&b);
		quit[3*(jiw * jih)-3*j-3]=r;
		quit[3*(jiw * jih)-3*j-2]=g;
		quit[3*(jiw * jih)-3*j-1]=b;
	}

	glPixelTransferf(GL_RED_SCALE, s);
	glPixelTransferf(GL_GREEN_SCALE, s);
	glPixelTransferf(GL_BLUE_SCALE, s);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	fclose(fp);

	if((fp = fopen("quit2.ppm", "r")) == NULL){
		printf("ERROR: Invalid Filename");
	}
	fscanf(fp,"%i %i %i",&jiw,&jih, &jmaxV);
	quit2 = malloc(3*sizeof(GLuint)*(jiw * jih));
	s=255./jmaxV;
	
	for(j=0;j<=(jiw * jih);j++){
		fscanf(fp,"%d %d %d",&r,&g,&b);
		quit2[3*(jiw * jih)-3*j-3]=r;
		quit2[3*(jiw * jih)-3*j-2]=g;
		quit2[3*(jiw * jih)-3*j-1]=b;
	}

	glPixelTransferf(GL_RED_SCALE, s);
	glPixelTransferf(GL_GREEN_SCALE, s);
	glPixelTransferf(GL_BLUE_SCALE, s);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	fclose(fp);

	/*Randomly generate initial food location*/
	srand(time(0));
	food[0] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/104)) + 3) * 4;
	food[1] = (int)((int)(rand()/(((double)RAND_MAX +1.0)/100)) + 7) * 4;
	
	/*Initialize Snake segment Array*/
	for(i = 0; i < 1000; i++){
		pos[i][0] = 0;
		pos[i][1] = 0;
	}
	
	/*Load Initial snake segments (25)*/
	for(i = 0; i < 25; i++){
		pos[i][0] = initx - (i*4);
		pos[i][1] = inity;
	}

	/*Main loop initializations*/
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH,HIGHT);
	glutCreateWindow("Jake The Snake");
	glutReshapeFunc(myReshape);
	glutDisplayFunc((void *) display);
	glutTimerFunc(100,animate,n);
	glutKeyboardFunc(myKey);
	glutSpecialFunc(processSpecialKeys);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glutMainLoop();
}
