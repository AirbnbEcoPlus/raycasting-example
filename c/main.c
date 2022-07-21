#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3,1415926535
float px,py,pdx,pdy,pa;
int mapX=8,mapY=8,mapS=64;
float degToRad(int a) { return a*M_PI/180.0;}
int FixAng(int a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}
int map[] = {
        1,1,1,1,1,1,1,1,
        1,0,1,0,0,0,0,1,
        1,0,0,0,0,1,1,1,
        1,0,1,0,0,0,0,1,
        1,0,1,0,0,0,0,1,
        1,0,1,0,1,0,0,1,
        1,0,0,0,0,1,0,1,
        1,1,1,1,1,1,1,1,
};
void drawRays2D(){

    int r,mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH;

    ra=FixAng(pa+30);                                                              //ray set back 30 degrees

    for(r=0;r<60;r++)
    {
        //---Vertical---
        dof=0; side=0; disV=100000;
        float Tan=tan(degToRad(ra));
        if(cos(degToRad(ra))> 0.001){ rx=(((int)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
        else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>6)<<6) -0.0001; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
        else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit

        while(dof<8)
        {
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
        }
        vx=rx; vy=ry;

        //---Horizontal---
        dof=0; disH=100000;
        Tan=1.0/Tan;
        if(sin(degToRad(ra))> 0.001){ ry=(((int)py>>6)<<6) -0.0001; rx=(py-ry)*Tan+px; yo=-64; xo=-yo*Tan;}//looking up
        else if(sin(degToRad(ra))<-0.001){ ry=(((int)py>>6)<<6)+64;      rx=(py-ry)*Tan+px; yo= 64; xo=-yo*Tan;}//looking down
        else{ rx=px; ry=py; dof=8;}                                                   //looking straight left or right

        while(dof<8)
        {
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit
            else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
        }

        glColor3f(1.0f, 0.5f, 0.0f);
        if(disV<disH){ rx=vx; ry=vy; disH=disV;glColor3f(0.5f, 1.0f, 1.0f);}                  //horizontal hit first

        int ca=FixAng(pa-ra); disH=disH*cos(degToRad(ca));                            //fix fisheye
        int lineH = (mapS*640)/(disH); if(lineH>640){ lineH=640;}                     //line height and limit
        int lineOff = 260 - (lineH>>1);                                               //line offset

        glLineWidth(10);glBegin(GL_LINES);glVertex2i(r*8+300,lineOff);glVertex2i(r*8+300,lineOff+lineH);glEnd();//draw vertical wall

        ra=FixAng(ra-1);                                                              //go to next ray
    }
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawRays2D();
    glutSwapBuffers();
}
void init(){
    glClearColor(0.3,0.3,0.3,0);
    gluOrtho2D(0,1024,1024,0);
    px=300; py=300; pdx=cos(pa)*5; pdy=sin(pa)*5;
}
void controls(unsigned char key, int x, int y){
    if(key=='q'){ pa+=5; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
    if(key=='d'){ pa-=5; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
    if(key=='z'){ px+=pdx*5; py+=pdy*5;}
    if(key=='s'){ px-=pdx*5; py-=pdy*5;}
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024,512);
    glutCreateWindow("game");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(controls);
    glutMainLoop();
}
