#include<stdio.h>
#include<stdlib.h>
#include<stack>
#include<list>
#include<sstream>
#include<algorithm>
#include<map>
#include<Windows.h>
#include<string.h>
#include<queue>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include<utility>
#include<iostream>
using namespace std;

int* color; // holds the colors of nodes
map<int, vector<int> > adjlist; // maps node number to a vector containing node numbers of neighbours
int vt = 0; // no of vertices drawn
size_t n = 0, e = 0; // no of nodes and edges
pair< int, int>* coordinates; // array of coordinates of nodes
vector <int> final;
string str;
GLvoid* font_style = GLUT_BITMAP_HELVETICA_18;
GLubyte strcol[3] = { 230,100,80 }; // string color of node number (red)

#define nl cout<<"\n";
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define sign(x) ((x>0)?1:((x<0)?-1:0))

extern string str; // to hold the node number to print on nodes
extern GLvoid* font_style;
extern GLubyte fillCol[3]; //interior color of node (white)
extern GLubyte borderCol[3]; // border color of node (black)
extern GLubyte fillColt[3];
extern GLubyte strcol[3]; // string color of node number (red)


void display(); // prototype

void delay(int i) {

	while (i--);
}

void drawstr(GLuint x, GLuint y, const char* format, int length)// draw string on screen
{
	glRasterPos2i(x, y);
	for (int i = 0; i < length; ++i)
	{
		glutBitmapCharacter(font_style, *(format + i));
	}
}

void setPixel(GLint x, GLint y) // set pixel as white
{
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(30.0);
	glVertex2i(x, y);
	glEnd();
}

void setPixel(GLint x, GLint y, int clr) // set pixel as white
{
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(30.0);
	glVertex2i(x, y);
	glEnd();
}

void drawCircle(int color, pair <int, int> center) // circle drawing algo
{
	int i;
	int x = center.first, y = center.second;
	GLfloat radius = 16;
	GLfloat PI = 3.14f;
	int triangleAmount = 1000;
	GLfloat twicePi = 2.0f * PI;

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0);

	glBegin(GL_LINES);
	if (color == 0)
		glColor4f(1.0, 1.0, 1.0, 1.0);
	else if (color == 1)
		glColor4f(1, 1, 0, 0.5);
	else
		glColor4f(0, 120/255.0, 0, 0);
	for (i = 0; i <= triangleAmount; i++)
	{
		glVertex2f(x, y);
		glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
	}
	glEnd();
	drawstr(x - 32, y, str.c_str(), str.length());
}

void drawEdge(pair<int, int> p, pair<int, int> q) // line drawing algo
{
	int x1, x2, y1, y2;
	x1 = p.first;
	x2 = q.first;
	y1 = p.second;
	y2 = q.second;

	int dx, dy, x, y, d, s1, s2, swap = 0, temp;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	s1 = sign(x2 - x1);
	s2 = sign(y2 - y1);

	if (dy > dx)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		swap = 1;
	}

	d = 2 * dy - dx;
	x = x1;
	y = y1;

	int i;

	for (i = 1; i < dx; i++)
	{
		setPixel(x, y);

		while (d >= 0)
		{
			d = d - 2 * dx;
			if (swap)
				x += s1;
			else
				y += s2;
		}

		d = d + 2 * dy;
		if (swap)
			y += s2;
		else
			x += s1;
	}
}
void drawEdge(pair<int, int> p, pair<int, int> q, int clr) // line drawing algo
{
	int x1, x2, y1, y2;
	x1 = p.first;
	x2 = q.first;
	y1 = p.second;
	y2 = q.second;

	int dx, dy, x, y, d, s1, s2, swap = 0, temp;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	s1 = sign(x2 - x1);
	s2 = sign(y2 - y1);

	if (dy > dx)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		swap = 1;
	}

	d = 2 * dy - dx;
	x = x1;
	y = y1;

	int i;

	for (i = 1; i < dx; i++)
	{
		if (clr == 0)
			setPixel(x, y);
		else
			setPixel(x, y, 1);
		while (d >= 0)
		{
			d = d - 2 * dx;
			if (swap)
				x += s1;
			else
				y += s2;
		}

		d = d + 2 * dy;
		if (swap)
			y += s2;
		else
			x += s1;
	}
	drawstr((x1 + x2) / 2 - 5, (y1 + y2) / 2, str.c_str(), str.length());
}
void handleResize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);   // Transformations being applied to Projection Matrix
	glLoadIdentity();   // Always call after changing matrix
	glViewport(0, 0, w, h);
	gluOrtho2D(0.0, 1000.0, 0.0, 700.0); // Here object is being moved along with window, wx & hy window size. Here clipping range is zNear=1,zFar=-200
	glMatrixMode(GL_MODELVIEW); // Transformations on ModelView Matrix(default)
}

void BFS(int src) // bfs algo
{
	vector<int> neighbours = adjlist[src];
	cout << "exploring neighbors of " << src; nl
		queue<int> q;
	delay(1100000000);
	for (int i : neighbours)
	{
		if (color[i] == 0)
		{
			color[i] = 1;
			cout << "processing node " << i; nl
				final.push_back(i);
			display();
			delay(1100000000);
			q.push(i);
		}
	}
	while (!q.empty())
	{
		BFS(q.front());
		q.pop();
	}
	cout << "backing. completely visited " << src; nl
		color[src] = 2;
	display();
	delay(110000000);
	if (final.size() == n && src == 0)
	{
		cout << "\n";
		for (int i = 0; i < n; i++)
		{
			cout << final[i] << " ";
		}
		cout << endl;
	}
}


void mouse(int button, int state, int x, int y)
{
	if (vt < n)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			coordinates[vt].first = x;
			coordinates[vt].second = (700 - y);
			cout << coordinates[vt].first << '\t' << coordinates[vt].second << '\n';
			vt++;
			display();
			delay(600);
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		cout << "\nRunning BFS\n";
		for (int i = 0; i < n; i++)
		{
			if (color[i] == 0)
			{
				cout << "processing node " << i; nl;
				color[i] = 1;
				final.push_back(i);
				display();
				BFS(i);
				delay(600);
			}
		}
	}
}
void print(int x, int y, string str1, void* font)
{
	int length, k;
	glRasterPos2f(x, y);
	length = str1.length();
	k = 0;
	while (k < length) {
		glutBitmapCharacter(font, str1[k]);
		k++;
	}

}

void welcomeDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.000, 0.749, 1.000);
	glColor3f(0.000, 0.749, 1.000);
	print(290, 650, " VIDYAVARDHAKA COLLEGE OF ENGINEERING ", GLUT_BITMAP_TIMES_ROMAN_24);
	//print(23, 486, " (Affiliated to Visvesvaraya Technological University, Belagavi | Recognized by Govt. of Karnataka | Approved by AICTE, New Delhi) ", GLUT_BITMAP_HELVETICA_12);
	print(400, 600, " GOKULAM, MYSURU- 570 002 ", GLUT_BITMAP_HELVETICA_18);

	print(300, 550, " DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING ", GLUT_BITMAP_HELVETICA_18);
	glColor3f(1, 1, 1);
	print(329, 450, " BREADTH FIRST SEARCH ALGORITHM ", GLUT_BITMAP_TIMES_ROMAN_24);
	glColor3f(0.000, 0.749, 1.000);
	glColor3f(0, 1, 1);
	print(700, 300, " By: ", GLUT_BITMAP_HELVETICA_18);
	print(700, 250, " SULONI PRAVEEN	 - 4VV19CS161 ", GLUT_BITMAP_HELVETICA_18);
	print(700, 200, " SWATHI MEGHANA K R   - 4VV19CS165 ", GLUT_BITMAP_HELVETICA_18);
	print(700, 150, " YUVIKA BALAJI  - 4VV19CS186 ", GLUT_BITMAP_HELVETICA_18);

	//print(5, 120, " Press 'x' to continue!!!  ", GLUT_BITMAP_TIMES_ROMAN_24);

	glutSwapBuffers();
}
void keys(unsigned char key, int x, int y)
{
	if (key == 'x')
	{
		glutDisplayFunc(display);
	}
	glutPostRedisplay();
}
void display()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(32.0/255.0, 178.0/255.0, 170.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < vt; i++)
	{
		str = to_string(i);
		drawCircle(color[i], coordinates[i]);
	}
	if (vt == n)
	{
		for (int i = 0; i < n; i++)
		{
			vector<int> neighbours = adjlist[i];
			for (int j : neighbours)
			{
				drawEdge(coordinates[i], coordinates[j]);
			}
		}
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	str = "-----------------------------------------------------------";
	drawstr(10, 110, str.c_str(), str.length());
	str = "Breadth First Search Algorithm";
	drawstr(10, 90, str.c_str(), str.length());
	str = "White - Initial color of the node";
	drawstr(10, 70, str.c_str(), str.length());
	str = "Yellow - The node is being processed";
	drawstr(10, 50, str.c_str(), str.length());
	str = "Green - All of its neighbours are reached or the final processing is done";
	drawstr(10, 30, str.c_str(), str.length());
	str = "-----------------------------------------------------------";
	drawstr(10, 10, str.c_str(), str.length());
	glutSwapBuffers();
}


int main(int argc, char** argv)
{

	cout << "Enter the number of nodes in the graph"; nl;
	cin >> n;
	color = new int[n];
	coordinates = new pair< int, int >[n];
	memset(color, 0, n * sizeof(int));
	cout << "Enter the number of edges in the graph"; nl;
	cin >> e;
	cout << "Enter the edges of the graph (u --> v) pairs"; nl;
	size_t u, v;
	for (int i = 0; i < e; i++)
	{
		cin >> u >> v;
		adjlist[u].push_back(v);
		adjlist[v].push_back(u);
	}
	/**/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1500, 1000);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Breadth First Search");
	glutDisplayFunc(welcomeDisplay);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	glutReshapeFunc(handleResize);
	glutMainLoop();
	return 0;
}