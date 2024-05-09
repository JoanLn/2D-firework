//
//  Fireworks 
//	Author: Joanna Lin
//       (This code is a reference to https://github.com/dancoen/opengl-labs)
//	Date: 12/11/2023
//	This project is simulating the firework activity using openGL in C++.
//

// standard
#include <assert.h>
#include <math.h>
#include <cstdlib>

// glut
#include <GL/glut.h>

#include <stdio.h>
#include <vector>
#include <time.h>
#include <string>
using namespace std;


//set num particles here
#define MAXPARTICLES 500

#define WIDTH 1600
#define HEIGHT 900

class Particle {
public:
	float pos[3];
	float vel[3];
	float color[4];
	double gravity;				//apply to explosion but not fire/smoke
	double life;		//lifetime
	double size;
	
	Particle() {}

	Particle(double angle, double xpos, double ypos, double zpos, double xvelocity, double yvelocity, double zvelocity, double time, double grav, float r, float g, float b) {
		pos[0] = xpos; 
		pos[1] = ypos; 
		pos[2] = zpos;
		if(angle == -1){
			vel[0] = xvelocity;
			vel[1] = yvelocity;
		}else{
			vel[0] = sin(angle) * xvelocity;
			vel[1] = cos(angle) * yvelocity;
		}
		color[0] = r;
		color[1] = g;
		color[2] = b;
		life = time;
		gravity = grav;
		size = .08;
		angle = angle;
	}

	void update() {
		pos[0] += vel[0];
		vel[1] -= gravity;		//gravity - if any
		pos[1] += vel[1];
		//glLoadIdentity();
		//glTranslatef(pos[0], pos[1], pos[2]);
		//glutSolidCube(size);
		life--;
	}
};

class Tail{
	public:
	float pos[3];
	float color[4];
	double size;
	int life;
	float heightTarget;
	
	Tail() {}

	Tail(double xpos, double ypos, double zpos, float r, float g, float b, float height) {
		pos[0] = xpos; 
		pos[1] = ypos; 
		pos[2] = zpos;
		color[0] = r;
		color[1] = g;
		color[2] = b;
		size = .1;
		life = 1;
		heightTarget = height;
	}

	void update() {
		//update position depending on velocity
		pos[1] += 0.2;
		if(pos[1] >= heightTarget){
			life = 0;
		}
		//glLoadIdentity();
		//glTranslatef(pos[0], pos[1], pos[2]);
		//glutSolidSphere(size, 20, 20);
		//glutSolidCube(size);
	}
};

//Particle *particles;	//particle array
std::vector<Particle*> particleSets;
std::vector<Tail> TailSets;
std::vector<std::vector<float>> position;

int num;
std::vector<float> timeIndex;

int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

bool start = false;		//check if 's' key has been pressed to start animation


double fRand(double fMin, double fMax)		//random double generator
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

std::vector<std::vector<float>> ColorSet = {
	{1.0, 0, 0}, //R
	{0, 1.0, 0}, //G
	{0, 0, 1.0}, //B
	{1.0, 1.0, 0.0}, //Y
	{0.0, 1.0, 1.0}, //C
	{1.0, 0.0, 1.0}, //M
	{1.0, 1.0, 1.0}, //W
};

//================================
// init
//================================
void init( void ) {
	srand( (unsigned)time(NULL) );
	int setup;

	printf("Setup firework sequences from: 0:file, 1:input: ");
	scanf("%d", &setup);
	if(setup == 0){
		std::vector<float> setting;
		FILE * setupFile;
		setupFile = fopen("fireworks.txt", "r");
		fscanf(setupFile, "%d\n", &num);
		printf("%d fireworks\n",num);
		for(int i=0; i<num; i++){
			float x, y, time;
			int color, type;
			fscanf(setupFile, "%f,%f,%f,%d,%d\n", &x, &y, &time, &color, &type);
			setting = {x, y, ColorSet[color][0], ColorSet[color][1], ColorSet[color][2], (float)type};
			printf("%d: %f, %f, %f, %d, %d\n", i, x, y, time, color, type);
			timeIndex.push_back(time*60);
			position.push_back(setting);	
		}

	}else{
		printf("How many fireworks do you want? (greater than 0): \n");
		scanf("%d", &num);
		while(!(num>0)){
			printf("How many fireworks do you want? (greater than 0): \n");
			scanf("%d", &num);
		}
		for(int i=0; i<num; i++){
			float x, y;
			int color;
			float sec;
			int type;
			float angle;
			
			printf("enter the position of firework %d: \nx (-14 to 14):", i);
			scanf("%f", &x);
			printf("y (-8 to 8):");
			scanf("%f", &y);
			
			printf("choose the type of firework: \n\t0:peony\n\t1:ring\n\t2:willow\n\t3:2 color\n\t4:2 color(surround)\n\t5:2 rings\n\t6:palm\n");
			scanf("%d", &type);
			while(!(type>=0 && type<7)){
				printf("choose the type of firework: \n\t0:peony\n\t1:ring\n\t2:willow\n\t3:2 color\n\t4:2 color(surround)\n\t5:2 rings\n\t6:palm\n");
				scanf("%d", &type);
			}
			
			printf("choose the color of firework: \n\t0:red\n\t1:green\n\t2:blue\n\t3:yellow\n\t4:cyan\n\t5:magenta\n");
			scanf("%d", &color);
			while(!(color>=0 && color<6)){
				printf("choose the color of firework: \n\t0:red\n\t1:green\n\t2:blue\n\t3:yellow\n\t4:cyan\n\t5:magenta\n");
				scanf("%d", &color);
			}
			printf("When the firework start?");
			scanf("%f", &sec);
			timeIndex.push_back(sec*60);
			std::vector<float> pos;
			pos = {x, y, ColorSet[color][0], ColorSet[color][1], ColorSet[color][2], (float)type};
			position.push_back(pos);
		}
	}

	for(int j=0; j<num; j++){
		Tail tail;
		tail = Tail(position[j][0], -10, -20, position[j][2], position[j][3], position[j][4], position[j][1]);
		TailSets.push_back(tail);
	}

	for(int j=0; j<num; j++){
		printf("num: %d\n", j);
		Particle *particles = new Particle[MAXPARTICLES];
		float vx, vy;
		float angle = fRand(0, 2*M_PI);
		float vvx[5] = {-0.2, -0.1, 0, 0.2, 0.2};
		float vv[5][2] = {{-0., -0.2},{-0.2, -0.1},{0.01, 0.2},{0.1, -0.2},{0.2, 0.2} };

		for (int i = 0; i < MAXPARTICLES; i++) {
			int t = (int)position[j][5];
			//6		
			int index = rand()%5;
			float vel = fRand(0.1, 0.9);
			switch(t){
				case 0:
				default:
					vx = fRand(-.2, .2);
					vy = vx;
					particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, position[j][2], position[j][3], position[j][4]);
					break;
				case 1: //ring
					vx = fRand(0.15, 0.2) * pow(-1, i%2);
					vy = vx;
					particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, position[j][2], position[j][3], position[j][4]);
					break;
				case 2: //willow
					vx = fRand(-.2, .2);
					vy = fRand(0, 0.05);
					particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, position[j][2], position[j][3], position[j][4]);
					break;
				case 3: //2 color 0
					vx = fRand(-.2, .2);
					vy = vx;
					if(i%2==0)
						particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, position[j][2], position[j][3], position[j][4]);
					else	
						particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, 1, 1, 1);
					break;
				case 4: //2 color 1
					vx = fRand(-.2, .2);
					vy = vx;
					if(vx<-0.1 or vx>0.1)
						particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, position[j][2], position[j][3], position[j][4]);
					else	
						particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, 1, 1, 1);
					break;
				case 5: //two ringd
					vx = vvx[rand()%5]/2;
					vy = vx;
					particles[i] = Particle(fRand(0, 2*M_PI), position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, position[j][2], position[j][3], position[j][4]);
					break;
				case 6: //palm
					vx = vv[index][0]*vel+fRand(-.01, .01);
					vy = vv[index][1]*vel+fRand(-.01, .01);
					//printf("vv: %f, vy: %f", vv[4][1], vy);
					particles[i] = Particle(-1, position[j][0], position[j][1], -20, vx, vy, fRand(-.1, .1), rand() % 40+20, 0.01, position[j][2], position[j][3], position[j][4]);
					break;
			}
		}
		particleSets.push_back(particles);
	}
}


//================================
// render
//================================
void render( void ) {
	// clear buffer
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth (1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[]	= { 0.11f, 0.06f, 0.11f, 1.0f };
	GLfloat material_Kd[]	= { 0.43f, 0.47f, 0.54f, 1.0f };
	GLfloat material_Ks[]	= { 0.33f, 0.33f, 0.52f, 1.0f };
	GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
	GLfloat material_Se		= 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
	glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);

	// modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glEnable(GL_COLOR_MATERIAL);
	if (start) {
		//draw tail
		for(int s=0; s<TailSets.size(); s++){	
			if(g_frameIndex>=timeIndex[s] && TailSets[s].life == 1 ){
				glColor3f(TailSets[s].color[0], TailSets[s].color[1], TailSets[s].color[2]); 
				TailSets[s].update();
				glLoadIdentity();
				glTranslatef(TailSets[s].pos[0], TailSets[s].pos[1], TailSets[s].pos[2]);
				glutSolidSphere(TailSets[s].size, 20, 20);
			}
		}
		//draw explosion
		for(int s=0; s<particleSets.size(); s++){	
			if(g_frameIndex>=timeIndex[s]  && TailSets[s].life == 0){
				for (int i = 0; i < MAXPARTICLES; i++) {
					if (particleSets[s][i].life > 0) {
						if (particleSets[s][i].life < 20) {
							float ratio = particleSets[s][i].life/20;
							glColor3f(particleSets[s][i].color[0]*ratio, particleSets[s][i].color[1]*ratio, particleSets[s][i].color[2]*ratio); 
						}
						else { 
							glColor3f(particleSets[s][i].color[0], particleSets[s][i].color[1], particleSets[s][i].color[2]); 
						}
						particleSets[s][i].update();
						glLoadIdentity();
						glTranslatef(particleSets[s][i].pos[0], particleSets[s][i].pos[1], particleSets[s][i].pos[2]);
						glutSolidCube(particleSets[s][i].size);

					}
				}
			}
		}
	}

	// disable lighting
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// swap back and front buffers
	glutSwapBuffers();
}


//================================
// keyboard input
//================================
void keyboard( unsigned char key, int x, int y ) {
	switch(key){
		case 's': 	//start animation
			start = true;
			break;
		case 'r':  //restart
			start = false;
			particleSets.clear();
			TailSets.clear();
			position.clear();
			timeIndex.clear();
			g_frameIndex = 0;
			init();
			break;
		case 27: 	 //quit
			exit(0);
			break;
		
	}
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
	// screen size
	g_screenWidth  = w;
	g_screenHeight = h;	
	
	// viewport
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {	
	// increase frame index
	g_frameIndex++;

	//update();
	
	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 32, timer, 0 );
}



//================================
// main
//================================
int main( int argc, char** argv ) {
	// init
	init();
	
	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( WIDTH, HEIGHT ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( argv[0] );

	
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutTimerFunc( 32, timer, 0 );
	
	// main loop
	glutMainLoop();

	return 0;
}
