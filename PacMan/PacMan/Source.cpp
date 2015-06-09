// C headers
#include <math.h>
#include <stdlib.h>
#include <time.h>

// C++ headers
#include <queue>
#include <iostream>
#include <fstream>

// boost headers
#include <boost\geometry.hpp>
#include <boost\geometry\geometries\point_xy.hpp>

// glut header
#include <glut.h>

// Windows headers
#include <Windows.h>
#include <tchar.h>

// 
#include "GameSettings.h"

// using declarations
using namespace std;
using namespace boost::geometry;

//const double FPS = 60;
const double AMMO_SPIN_PER_SEC = 1;
const double FACE_CIRCLE_DRAW_TIME_IN_SEC = 2;

bool edge[20][20][4], foundf;
double herox, heroy, enemyx, enemyy;
int totalh, totale, denemy, dhero = -1, dfire = -1, win = -1, shots;
int pathSize, speedh, speede, speedf;
double spin, p1, p2, firex, firey;
model::d2::point_xy<int> hero, enemy, fire[10];

void end()
{
    double center = -1 + (pathSize + 2) / 2.0, r = .5, temp = 0;
    double pi = 2 * acos(0.0), angle;
    int i;

    if (win)
    {
        glColor3f(0, 1, 0);
    }
    else
    {
        glColor3f(1, 0, 0);
    }

    glLineWidth(7);

    glPushMatrix();

    glTranslatef(center, center, 0);

    glBegin(GL_LINE_STRIP);

    for (i = 0; i < p1; i++)
    {
        angle = 2 * pi * i / 1000;
        glVertex2f(cos(angle) / r, sin(angle) / r);
    }

    glEnd();

    glPopMatrix();

    r *= 2;

    glPushMatrix();

    if (!win)
    {
        glTranslatef(center, center - r, 0);
    }
    else
    {
        glTranslatef(center, center, 0);
    }

    if (win)
    {
        glRotatef(180, 0, 0, 1);
    }

    glBegin(GL_LINE_STRIP);

    for (i = 0; i < p2; i++)
    {
        angle = 2 * pi * i / 1000;
        glVertex2f(cos(angle) / r, sin(angle) / r);
    }

    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslatef(center, center, 0);
    glRotatef(180, 0, 0, 1);

    if (p2 >= 490)
    {
        glPointSize(7);

        glBegin(GL_POINTS);

        angle = 2 * pi * 832 / 1000;
        glVertex2f(cos(angle) / r, sin(angle) / r);

        if (p2 >= 495)
        {
            angle = 2 * pi * 666 / 1000;
            glVertex2f(cos(angle) / r, sin(angle) / r);
        }

        glEnd();
    }

    glPopMatrix();
}

void drawMaze()
{
    int i, j;

    glLineWidth(3);
    glColor3f(1, 1, 1);

    glBegin(GL_LINES);

    glVertex2i(0, pathSize);
    glVertex2i(pathSize, pathSize);

    glVertex2i(0, pathSize);
    glVertex2i(0, 0);

    for (i = 0; i < pathSize; i++)
    {
        for (j = 0; j < pathSize; j++)
        {
            if (edge[i][j][0])
            {
                glVertex2i(j + 1, pathSize - i);
                glVertex2i(j + 1, pathSize - 1 - i);
            }
            if (edge[i][j][1])
            {
                glVertex2i(j, pathSize - 1 - i);
                glVertex2i(j + 1, pathSize - 1 - i);
            }
        }
    }

    glEnd();
}

void circle(double r, int points)
{
    double pi = 2 * acos(0.0), angle;
    int i;

    glBegin(GL_POLYGON);

    for (i = 0; i < points; i++)
    {
        angle = 2 * pi * i / points;
        glVertex2f(cos(angle) / r, sin(angle) / r);
    }

    glEnd();
}

void drawHero()
{
    glColor3f(0, 0, 1);

    glPushMatrix();

    glTranslatef(herox, heroy, 0);
    circle(3, 100);

    glPopMatrix();
}

void drawEnemy()
{
    glColor3f(1, 0, 0);

    glPushMatrix();

    glTranslatef(enemyx, enemyy, 0);
    circle(3, 100);

    glPopMatrix();
}

void drawGoal()
{
    glColor3f(0, 1, 0);

    glPushMatrix();

    glTranslatef(pathSize - .5, .5, 0);
    circle(3, 100);

    glPopMatrix();
}

void drawFire()
{
    int i;

    glColor3f(1, 1, 0);

    if (foundf)
    {
        firex = herox;
        firey = heroy;
    }

    if (foundf || dfire != -1)
    {
        glPushMatrix();

        glTranslatef(firex, firey, 0);
        glRotatef(spin, 0, 1, 0);
        circle(7, 4);

        glPopMatrix();
    }

    for (i = 0; i < shots; i++)
    {
        glPushMatrix();

        glTranslatef(fire[i].y() + .5, pathSize - .5 - fire[i].x(), 0);
        glRotatef(spin, 0, 1, 0);
        circle(7, 4);

        glPopMatrix();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (win == -1)
    {
        drawMaze();
        drawHero();
        drawGoal();
        drawEnemy();
        drawFire();
    }
    else
    {
        end();
    }

    glutSwapBuffers();
}

void LoadMaze1()
{
    edge[0][0][0] = 1;
    edge[1][0][0] = 1;
    edge[2][1][0] = 1;
    edge[2][3][0] = 1;
    edge[3][0][0] = 1;
    edge[3][2][0] = 1;
    edge[3][3][0] = 1;
    edge[4][0][0] = 1;

    edge[0][1][1] = 1;
    edge[0][2][1] = 1;
    edge[0][3][1] = 1;
    edge[1][2][1] = 1;
    edge[1][3][1] = 1;
    edge[3][1][1] = 1;
    edge[3][2][1] = 1;
    edge[3][4][1] = 1;
}

void LoadMaze2()
{
    int i;

    for (i = 0; i < 10; i++)
    {
        edge[i][0][0] = 1;
        edge[i][1][0] = 1;
    }
    edge[2][0][0] = 0;

    edge[6][0][0] = 0;
    edge[0][1][0] = 0;
    edge[6][1][0] = 0;

    for (i = 2; i < 6; i++)
    {
        edge[i][2][0] = 1;
    }

    edge[1][3][0] = 1;
    edge[2][3][0] = 1;
    edge[3][3][0] = 1;
    edge[4][3][0] = 1;

    edge[1][4][0] = 1;
    edge[3][4][0] = 1;
    edge[4][4][0] = 1;
    edge[5][4][0] = 1;

    edge[0][5][0] = 1;
    edge[2][5][0] = 1;
    edge[3][5][0] = 1;

    edge[1][6][0] = 1;
    edge[3][6][0] = 1;
    edge[4][6][0] = 1;
    edge[6][6][0] = 1;
    edge[7][6][0] = 1;
    edge[8][6][0] = 1;

    edge[6][7][0] = 1;
    edge[7][7][0] = 1;
    edge[9][7][0] = 1;
    edge[0][7][0] = 1;

    edge[3][8][0] = 1;
    edge[5][8][0] = 1;
    edge[6][8][0] = 1;
    edge[7][8][0] = 1;
    edge[8][8][0] = 1;
    edge[9][8][0] = 1;

    edge[0][2][1] = 1;

    edge[1][3][1] = 1;
    edge[1][5][1] = 1;
    edge[1][6][1] = 1;
    edge[1][7][1] = 1;
    edge[1][8][1] = 1;
    edge[1][9][1] = 1;

    edge[3][7][1] = 1;
    edge[3][8][1] = 1;

    for (i = 4; i < 9; i++)
    {
        edge[4][i][1] = 1;
    }
    edge[4][5][1] = 0;

    edge[5][2][1] = 1;
    edge[5][5][1] = 1;
    edge[5][6][1] = 1;
    edge[5][7][1] = 1;

    edge[6][2][1] = 1;
    edge[6][3][1] = 1;

    edge[7][3][1] = 1;
    edge[7][5][1] = 1;
    edge[7][6][1] = 1;

    for (i = 3; i < 7; i++)
    {
        edge[8][i][1] = 1;
    }
}

void setMaze()
{
    int i, j;

    for (i = 0; i < pathSize; i++)
    {
        edge[i][0][2] = 1;
    }
    for (i = 0; i < pathSize; i++)
    {
        edge[0][i][3] = 1;
    }
    for (i = 0; i < pathSize; i++)
    {
        edge[i][pathSize - 1][0] = 1;
    }
    for (i = 0; i < pathSize; i++)
    {
        edge[pathSize - 1][i][1] = 1;
    }

    if (pathSize == 5)
    {
        LoadMaze1();
    }
    if (pathSize == 10)
    {
        LoadMaze2();
    }


    for (i = 0; i < pathSize; i++)
    {
        for (j = 0; j < pathSize; j++)
        {
            if (edge[i][j][0] && j + 1 < pathSize)
            {
                edge[i][j + 1][2] = 1;
            }
            if (edge[i][j][1] && i + 1 < pathSize)
            {
                edge[i + 1][j][3] = 1;
            }
        }
    }
}

void bfs()
{
    int i;
    model::d2::point_xy<int> u, v, temp;
    queue <model::d2::point_xy<int>> q;
    bool d[20][20];
    int p[20][20][2];

    memset(d, 0, sizeof(d));

    q.push(enemy);
    while (!q.empty())
    {
        u = q.front();
        q.pop();
        for (i = 0; i < 4; i++)
        {
            if (!edge[u.x()][u.y()][i])
            {
                v = u;
                if (i == 0)
                {
                    v.y(v.y() + 1);
                }
                if (i == 1)
                {
                    v.x(v.x() + 1);
                }
                if (i == 2)
                {
                    v.y(v.y() - 1);
                }
                if (i == 3)
                {
                    v.x(v.x() - 1);
                }

                if (!d[v.x()][v.y()])
                {
                    q.push(v);
                    d[v.x()][v.y()] = 1;
                    p[v.x()][v.y()][0] = u.x();
                    p[v.x()][v.y()][1] = u.y();
                }

                v.x(hero.x());
                v.y(hero.y());
            }
        }
    }

    u = hero;
    while (p[u.x()][u.y()][0] != enemy.x() || p[u.x()][u.y()][1] != enemy.y())
    {
        temp.x(p[u.x()][u.y()][0]);
        temp.y(p[u.x()][u.y()][1]);
        u = temp;
    }

    if (u.x() == enemy.x())
    {
        if (u.y() == enemy.y() + 1)
        {
            denemy = 0;
        }
        else
        {
            denemy = 2;
        }
    }
    else
    {
        if (u.x() == enemy.x() + 1)
        {
            denemy = 1;
        }
        else
        {
            denemy = 3;
        }
    }
}

void menemy()
{
    if (denemy == 0)
    {
        enemyx += 1.0 / speede;
    }
    if (denemy == 1)
    {
        enemyy -= 1.0 / speede;
    }
    if (denemy == 2)
    {
        enemyx -= 1.0 / speede;
    }
    if (denemy == 3)
    {
        enemyy += 1.0 / speede;
    }

    totale++;

    if (totale == speede)
    {
        if (denemy == 1)
        {
            enemy.x(enemy.x() + 1);
        }
        if (denemy == 2)
        {
            enemy.y(enemy.y() - 1);
        }
        if (denemy == 0)
        {
            enemy.y(enemy.y() + 1);
        }
        if (denemy == 3)
        {
            enemy.x(enemy.x() - 1);
        }

        totale = 0;
        bfs();
    }

    if (enemy.x() == hero.x() && enemy.y() == hero.y())
    {
        win = 0;
    }
}

void mhero()
{
    int i;

    if (dhero != -1)
    {
        if (dhero == 0)
        {
            herox += 1.0 / speedh;
        }
        if (dhero == 1)
        {
            heroy -= 1.0 / speedh;
        }
        if (dhero == 2)
        {
            herox -= 1.0 / speedh;
        }
        if (dhero == 3)
        {
            heroy += 1.0 / speedh;
        }

        totalh++;
    }

    if (totalh == speedh)
    {
        if (dhero == 1)
        {
            hero.x(hero.x() + 1);
        }
        if (dhero == 2)
        {
            hero.y(hero.y() - 1);
        }
        if (dhero == 0)
        {
            hero.y(hero.y() + 1);
        }
        if (dhero == 3)
        {
            hero.x(hero.x() - 1);
        }

        totalh = 0;
        dhero = -1;

        for (i = 0; i < shots; i++)
        {
            if (hero.x() == fire[i].x() && hero.y() == fire[i].y())
            {
                foundf = 1;
            }
        }

        if (hero.x() == pathSize - 1 && hero.y() == pathSize - 1)
        {
            win = 1;
        }
    }
}

void mfire()
{
    int offx = 0, offy = 0;

    if (dfire != -1)
    {
        if (dfire == 0)
        {
            firex += 1.0 / speedf;
            offx = -1;
        }
        if (dfire == 1)
        {
            firey -= 1.0 / speedf;
            offy += 1;
        }
        if (dfire == 2)
        {
            firex -= 1.0 / speedf;
            offx += 1;
        }
        if (dfire == 3)
        {
            firey += 1.0 / speedf;
            offy -= 1;
        }

        if (abs(firex + offx - enemyx) < 0 && abs(firey + offy - enemyy) < 0)
        {
            dfire = -1;

            totale = 0;

            enemyx = pathSize - .5;
            enemyy = pathSize - .5;

            enemy.x(0);
            enemy.y(pathSize - 1);

            bfs();
        }
    }
}

void move()
{
    static double prevClock = 0;
    double newClock = clock();
    if ((newClock - prevClock) / CLOCKS_PER_SEC > 1.0 / GameSettings::FPS())
    {
        prevClock = clock();
    }
    else
    {
        return;
    }

    int i;

    if (win == -1)
    {
        menemy();
        mhero();
        mfire();

        spin += (AMMO_SPIN_PER_SEC * 360) / GameSettings::FPS();
        if (spin == 360)
        {
            spin = 0;
        }
    }

    else
    {
        p1 += (1000 / FACE_CIRCLE_DRAW_TIME_IN_SEC) / GameSettings::FPS();
        if (p1 >= 1000)
        {
            p2 += (1000 / FACE_CIRCLE_DRAW_TIME_IN_SEC) / GameSettings::FPS();
        }

        if (p2 >= 495)
        {
            for (i = 0; i < 100000000; i++)
            {
            }
        }

        if (p2 >= 500)
        {
            glutIdleFunc(NULL);

            if (win)
            {
                printf("You Win!!!\n");
            }
            else
            {
                printf("You Lose!!!\n");
            }
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's':
    case 'k':
        if (!edge[hero.x()][hero.y()][1] && !totalh)
        {
            dhero = 1;
            glutIdleFunc(move);
        }
        break;
    case 'a':
    case 'j':
        if (!edge[hero.x()][hero.y()][2] && !totalh)
        {
            dhero = 2;
            glutIdleFunc(move);
        }
        break;
    case 'd':
    case 'l':
        if (!edge[hero.x()][hero.y()][0] && !totalh)
        {
            dhero = 0;
            glutIdleFunc(move);
        }
        break;
    case 'w':
    case 'i':
        if (!edge[hero.x()][hero.y()][3] && !totalh)
        {
            dhero = 3;
            glutIdleFunc(move);
        }
        break;
    case 'q':
        if (foundf && (hero.x() == enemy.x() || hero.y() == enemy.y()))
        {
            foundf = 0;
            if (hero.x() < enemy.x())
            {
                dfire = 1;
            }
            if (hero.x() > enemy.x())
            {
                dfire = 3;
            }
            if (hero.y() < enemy.y())
            {
                dfire = 0;
            }
            if (hero.y() > enemy.y())
            {
                dfire = 2;
            }
        }
        break;
    }
}

void initwindow()
{
    glClearColor(0, 0, 0, 0);
    glLoadIdentity();
    gluOrtho2D(-1, pathSize + 1, -1, pathSize + 1);
}

void init()
{
    int x;

    cout << "1)Easy\n2)Hard\nGive your choice:\n";
    cin >> x;

    if (x == 1)
    {
        pathSize = 5;
        speedh = 100;
        speede = 200;
        shots = 1;
    }
    else
    {
        pathSize = 10;
        speedh = 30;
        speede = 30;
        shots = 4;
    }
    speedf = speedh;

    hero.x(0);
    hero.y(0);
    enemy.x(0);
    enemy.y(pathSize - 1);

    herox = .5;
    heroy = pathSize - .5;

    enemyx = pathSize - .5;
    enemyy = pathSize - .5;

    fire[0].x(4);
    fire[0].y(0);

    fire[1].x(4);
    fire[1].y(7);

    fire[2].x(2);
    fire[2].y(5);

    fire[3].x(2);
    fire[3].y(3);
}

int main(int argc, char** argv)
{
    printf("Keys:\nw or i to move up\na or j to move left\nd or l to move right\ns or k to move down\nq to fire\n\nTask:\nYour job is to take the blue ball to the green ball. But if red ball reaches you first you lose. You can fire the red ball once you have taken one yellow square. The red ball has to be on the same vertical or horizontal line with the blue ball when you fire.\n\n\n");

    init();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize((pathSize + 2) * 50, (pathSize + 2) * 50);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PacMan");
    initwindow();
    setMaze();
    bfs();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    HWND hWnd = FindWindow(NULL, _T("PacMan"));
    SetForegroundWindow(hWnd);


    glutMainLoop();

    return 0;
}
