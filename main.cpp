#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include<bits/stdc++.h>
#include "BmpLoader.h"

const int width = 1024;
const int height = 768;

unsigned int ID, floor_tex, wall_tex, door_tex, beam_tex, poolborder_tex;

float aspect_ratio = 1.0*width/height ;

GLfloat eyeX = 0;
GLfloat eyeY = 25;
GLfloat eyeZ = -45;

GLfloat lookX = 0;
GLfloat lookY = 20;
GLfloat lookZ = 0;

float ambient_val = .25;
float diffuse_val = .8;
float specular_val = 1;

GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_ambient[]  = {ambient_val, ambient_val, ambient_val, 1.0};
GLfloat light_diffuse[]  = { diffuse_val, diffuse_val, diffuse_val, 1.0 };
GLfloat light_specular[] = { specular_val, specular_val, specular_val, 1.0 };

float cutoff_val = 40;
GLfloat splight_ambient[]  = {.5*specular_val, .5*specular_val, 0, 1.0};
GLfloat splight_diffuse[]  = {1*specular_val, 1*specular_val, 0, 1.0 };
GLfloat splight_specular[] = { specular_val, specular_val, specular_val, 1.0 };

const float floor_width = 90.0, floor_length = 120.0, wall_height = 50 ;

bool flagScale = true, flagrotate = false, fullscreen = false, door_cntrl = false ;
bool light0_switch = false, light1_switch = true, light2_switch = true;
bool ambient_switch = false, diffusion_switch = false, specular_switch = false ;

float angle = 0, angle_rotate=0, door_angle = 0;
float rot_right_left = 0, x_axis = 0, y_axis = 1, z_axis = 0;

static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {0,2,6,4},
    {1,5,7,3},

    {0,4,5,1},
    {2,6,7,3},

    {0,1,3,2},
    {4,6,7,5}
};

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void light()
{
    GLfloat light0_position[] = {0.0, wall_height-3, 0, 1.0 };
    glEnable( GL_LIGHT0);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT0, GL_POSITION, light0_position);

    GLfloat light1_position[] = {0.0, wall_height-3, 0.0, 1.0 };
    glEnable(GL_LIGHT1);
    glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT1, GL_POSITION, light1_position);

    GLfloat light2_position[] = {0.0, wall_height-3, 0, 1.0 };
    glEnable( GL_LIGHT2);
    glLightfv( GL_LIGHT2, GL_AMBIENT, splight_ambient);
    glLightfv( GL_LIGHT2, GL_DIFFUSE, splight_diffuse);
    glLightfv( GL_LIGHT2, GL_SPECULAR, splight_specular);
    glLightfv( GL_LIGHT2, GL_POSITION, light2_position);

    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, cutoff_val);
}

void cube(float colR=0.5, float colG=0.5, float colB=0.5, float alpha=1)
{
    float ambient_percent = 1.0 ;
    float diffuse_percent = 1.0 ;
    float specular_percent = 1.0 ;

    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat material_ambient[]  = {ambient_percent*colR, ambient_percent*colG, ambient_percent*colB, 1.0 };
    GLfloat material_diffuse[]  = {diffuse_percent*colR, diffuse_percent*colG, diffuse_percent*colB, 1.0};
    GLfloat material_specular[] = { specular_percent, specular_percent, specular_percent, 1.0 };

    GLfloat mat_shininess[] = {50};
    ///GLfloat mat_emission[] = {100};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION , no_light);
    ///glMaterialfv( GL_FRONT, GL_EMISSION, mat_emission);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        glColor4f(colR,colG,colB,alpha);

        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glTexCoord2f(0,2);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(2,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(2,2);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
    }
    glEnd();
}

void window(float xpos = floor_width/2)
{
    float window_width = .2, window_height = 15, window_length = 30;

    glPushMatrix();
    if(xpos>=0)
        glTranslatef(xpos-window_width,wall_height/2,-floor_length/4);
    else
        glTranslatef(xpos+window_width,wall_height/2,-floor_length/4);
    glScalef(window_width,window_height,window_length);
    glTranslatef(0,-0.5,-0.5);
    cube(0.690, 0.878, 0.902);
    glPopMatrix();

    /// horizontal line

    for(int line=window_height/2 ; line>=-window_height/2 ; line-=2)
    {

        glPushMatrix();
        if(xpos>=0)
            glTranslatef(xpos-window_width-0.5,wall_height/2+line,-floor_length/4);
        else
            glTranslatef(xpos+window_width,wall_height/2+line,-floor_length/4);
        glScalef(window_width+.5,window_width,window_length);
        glTranslatef(0,-0.5,-0.5);
        cube(0.35, 0.35, 0.35);
        glPopMatrix();
    }

    /// vertical line
    for(int line=window_length/2 ; line>=-window_length/2 ; line-=3)
    {

        glPushMatrix();
        if(xpos>=0)
            glTranslatef(xpos-window_width-0.5,wall_height/2,-floor_length/4+line);
        else
            glTranslatef(xpos+window_width,wall_height/2,-floor_length/4+line);
        glScalef(window_width+.5,window_height,window_width);
        glTranslatef(0,-0.5,-0.5);
        cube(0.35, 0.35, 0.35);
        glPopMatrix();
    }
}

void wall()
{
    /// floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,floor_tex);
    glPushMatrix();
    glScalef(floor_width,.2,floor_length);
    glTranslatef(-0.5,-1,-0.5);
    cube(1.000, 1.000, 1.000);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D) ;


    ///back wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,wall_tex);
    glPushMatrix();
    glTranslatef(0,wall_height/2,floor_length/2-.2);
    glScalef(floor_width,wall_height,.2);
    glTranslatef(-0.5,-0.5,0);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D) ;

    ///spot light on back wall
    float slight_width = .5, slight_height = .5, slight_length = 1;
    glPushMatrix();
    glTranslatef(0,wall_height-slight_height-2,(floor_length/2-slight_length));
    glScalef(slight_width,slight_height,slight_length);
    glTranslatef(-0.5,0,-0.5);
    //cube(1.0, 1.0, 1.0);
    glPopMatrix();

    ///====================sphere======================
    glPushMatrix();
    glTranslatef(0,wall_height-slight_height-2,(floor_length/2-slight_length));
    glScalef(slight_width,slight_height,slight_length);
    glTranslatef(-0.5,0,-0.5);
    //glutSolidSphere(2,360,8);
    glPopMatrix();

    ///front wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,wall_tex);
    glPushMatrix();
    glTranslatef(0,wall_height/2,(-floor_length/2));
    glScalef(floor_width,wall_height,.2);
    glTranslatef(-0.5,-0.5,0);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D) ;

    ///right wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,wall_tex);

    glPushMatrix() ;

    glPushMatrix();
    glTranslatef(-floor_width/2,wall_height/2,0);
    glScalef(.2,wall_height,floor_length);
    glTranslatef(0,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;

    ///right beam
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,beam_tex);

    float beam_width = 3, beam_height = wall_height, beam_length = 4;

    glPushMatrix();
    glTranslatef(-(floor_width/2-beam_width/2-.2),wall_height/2,0);
    glScalef(beam_width,beam_height,beam_length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;

    ///=====right window========
    window(-floor_width/2);

    glPushMatrix();
    glTranslated(0,0,floor_length/2);
    glScalef(1,1,1);
    window(-floor_width/2);
    glPopMatrix();

    glPopMatrix() ;

    ///left wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,wall_tex);

    glPushMatrix();

    glPushMatrix();
    glTranslatef(floor_width/2,wall_height/2,0);
    glScalef(.2,wall_height,floor_length);
    glTranslatef(0,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    ///left beam
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,beam_tex);

    beam_width = 3, beam_height = wall_height, beam_length = 4;

    glPushMatrix();
    glTranslatef(floor_width/2-beam_width/2,wall_height/2,0);
    glScalef(beam_width,beam_height,beam_length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;

    ///=======window==========
    window();

    ///==========leftside door=========

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,door_tex);

    float door_width = 0.3, door_height = 25, door_length = 13 ;

    glPushMatrix();
    glTranslatef(0,0,-10) ;

    glPushMatrix();
    glPushMatrix();
    //glTranslatef(floor_width/2-door_width,0,(floor_length/2-door_length)-10);
    //glRotatef(door_angle,0,1,0) ;

    glPushMatrix();
    glTranslatef(floor_width/2-door_width,0,(floor_length/2-door_length)-10);
    glRotatef(door_angle,0,1,0) ;
    glTranslatef(0,0,door_length/2);
    glScalef(door_width,door_height,door_length);
    glTranslatef(0,0,-0.5);
    cube(0.871, 0.722, 0.529);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;

    ///===========door handle=============
    glPushMatrix();
    float handle_height = 2 ;
    glTranslatef(floor_width/2-door_width,door_height/2,(floor_length/2-door_length)-10);
    glRotatef(door_angle,0,1,0) ;
    glTranslatef(0,0,door_length-1);
    glScalef(door_width+.5,handle_height,door_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    glPopMatrix();

    ///outside door
    glPushMatrix();
    glTranslatef(floor_width/2-door_width/2,0,(floor_length/2-door_length)-10);
    glTranslatef(0,0,door_length/2);
    glScalef(door_width+0.2,door_height,door_length);
    glTranslatef(0,0,-0.5);
    cube(0.0, 0.0, 0.0);
    glPopMatrix();

    ///left border
    glPushMatrix();
    glTranslatef(floor_width/2-door_width-0.2,0,(floor_length/2-door_length)-10);
    glTranslatef(0,0,door_length);
    glScalef(door_width+0.2,door_height,door_width);
    glTranslatef(0,0,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    ///right border
    glPushMatrix();
    glTranslatef(floor_width/2-door_width-0.2,0,(floor_length/2-door_length)-10);
    glScalef(door_width+0.2,door_height,door_width);
    glTranslatef(0,0,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    ///top border
    glPushMatrix();
    glTranslatef(floor_width/2-door_width-0.2,door_height-door_width,(floor_length/2-door_length/2)-10);
    glScalef(door_width+0.2,door_width,door_length);
    glTranslatef(0,0,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    glPopMatrix() ;

    glPopMatrix() ;

    /// roof
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,wall_tex);

    glPushMatrix() ;

    glPushMatrix();
    glTranslatef(0,wall_height,0);
    glScalef(floor_width,.2,floor_length);
    glTranslatef(-0.5,-1,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;

    ///top beam
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,beam_tex);

    beam_width = floor_width, beam_height = 3, beam_length = 4;

    glPushMatrix();
    glTranslatef(0,wall_height-beam_height,0);
    glScalef(beam_width,beam_height,beam_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;
    ///bulb attached to top beam
    float bulb_width = 15, bulb_height = 1, bulb_length = .5;

    glPushMatrix();
    //glEnable(GL_TEXTURE_2D);
    glTranslatef(0,wall_height-beam_height-bulb_height,0);
    glScalef(bulb_width,bulb_height,bulb_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1.0, 1.0, 1.0);
    glPopMatrix();

    ///bulb attached to front wall
    bulb_width = 15, bulb_height = 1, bulb_length = 1;

    glPushMatrix();
    glTranslatef(0,wall_height-10, floor_length/2-bulb_length);
    glScalef(bulb_width,bulb_height,bulb_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1.0, 1.0, 1.0);
    glPopMatrix();

    /// bulb attached to back wall
    glPushMatrix();
    glTranslatef(0,wall_height-10, -floor_length/2+bulb_length);
    glScalef(bulb_width,bulb_height,bulb_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1.0, 1.0, 1.0);
    glPopMatrix();

    glPopMatrix() ;
}

void balls()
{
    float height=10;
    float width=15;
    float length=2*width;

    float base_height=.5;
    float leg_height=height-base_height;


    ///8 balls
    ///glColor3d(1,1,1);
    glPushMatrix();
    glTranslatef(0,leg_height+base_height,0);

    glPushMatrix();
    glTranslatef(0,.5,0);
    glutSolidSphere(.5,100,100);
    glPopMatrix();

    glPopMatrix() ;
}

void table()
{
    float height=10;
    float width=15;
    float length=2*width;

    float base_height=.5;
    float leg_height=height-base_height;
    float leg_width=0.7;

    /// whole table
    glPushMatrix();
    glTranslatef(0,leg_height,0);

    /// base
    glPushMatrix();
    glScalef(width-.1,base_height,length-.1);
    glTranslatef(-0.5,0,-0.5);
    cube(0, 0.722, 0.529);
    glPopMatrix();

    float border_height = .75+base_height;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,poolborder_tex);

    /// upper border
    glPushMatrix();
    glTranslatef(0,0,length/2-(.05*length/2));
    glScalef(width,border_height,.05*length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1);
    glPopMatrix();

    /// lower border
    glPushMatrix();
    glTranslatef(0,0,-(length/2-(.05*length/2)));
    glScalef(width,border_height,.05*length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1);
    glPopMatrix();

    /// left border
    glPushMatrix();
    glTranslatef(width/2-(.05*width/2),0,0);
    glScalef(.05*width,border_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1);
    glPopMatrix();

    /// right border
    glPushMatrix();
    glTranslatef(-(width/2-(.05*width/2)),0,0);
    glScalef(.05*width,border_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;
    // legs
    //glColor3f(0.85, 0.7, 0.50);


    int leg_adjust_factor = 2 ;

    /// upper left leg
    glPushMatrix();
    glTranslatef((width/2-leg_adjust_factor*leg_width),0,(length/2-leg_adjust_factor*leg_width));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube();
    glPopMatrix();

    /// lower left leg
    glPushMatrix();
    glTranslatef((width/2-leg_adjust_factor*leg_width),0,-(length/2-leg_adjust_factor*leg_width));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube();
    glPopMatrix();

    /// upper right leg
    glPushMatrix();
    glTranslatef(-(width/2-leg_adjust_factor*leg_width),0,(length/2-leg_adjust_factor*leg_width));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube();
    glPopMatrix();

    ///lower right leg
    glPushMatrix();
    glTranslatef(-(width/2-leg_adjust_factor*leg_width),0,-(length/2-leg_adjust_factor*leg_width));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube();
    glPopMatrix();

    glPopMatrix();
}


void fan()
{
    float hanger_height=5;
    float hanger_width=.15;
    float hanger_length=.15;

    float blade_height=0.1;
    float blade_width=.4;
    float blade_length=4;

    /// whole fan
    glPushMatrix();
    glRotatef(angle,0,1,0) ;
    glScalef(2,1,2) ;

    /// blade 1
    glPushMatrix();
    glTranslatef(0,wall_height-hanger_height,0);
    glScalef(blade_length,blade_height,blade_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.941, 0.902, 0.549);
    glPopMatrix();

    /// blade 2
    glPushMatrix();
    glTranslatef(0,wall_height-hanger_height,0);
    glScalef(blade_width,blade_height,blade_length);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.941, 0.902, 0.549);
    glPopMatrix();

    glPopMatrix();

    /// hanger
    glPushMatrix();
    glTranslatef(0,wall_height,0);
    glScalef(hanger_width,hanger_height,hanger_length);
    glTranslatef(-0.5,-1,-0.5);///-3
    cube(0.467, 0.533, 0.600);
    glPopMatrix();

}

void door()
{
    float door_width = 13, door_height = 25, door_length = 0.3 ;

    glPushMatrix();
    glPushMatrix();

    glTranslatef(-door_width/2,0,-(floor_length/2-door_length));
    glRotatef(door_angle,0,1,0) ;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,door_tex);

    glPushMatrix();
    glTranslatef(0,door_height/2,0);
    glScalef(door_width,door_height,door_length);
    glTranslatef(0,-0.5,-0.5);
    cube(0.871, 0.722, 0.529);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D) ;

    ///door handle
    glPushMatrix();
    float handle_height = 2 ;
    glTranslatef(door_width-door_length/2-1,door_height/2,0);
    glScalef(door_length,handle_height,door_length+.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    glPopMatrix();

    ///outside door
    glPushMatrix();
    glTranslatef(-door_width/2,door_height/2,-(floor_length/2));
    glScalef(door_width,door_height,door_length+.2);
    glTranslatef(0,-0.5,-0.5);
    cube(0.0, 0.0, 0.0);
    glPopMatrix();

    ///left border
    glPushMatrix();
    glTranslatef(-(door_width/2-door_length/2),door_height/2,-(floor_length/2-door_length-0.5));
    glScalef(door_length,door_height,door_length+.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    ///right border
    glPushMatrix();
    glTranslatef(door_width/2-door_length/2,door_height/2,-(floor_length/2-door_length-0.5));
    glScalef(door_length,door_height,door_length+.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    ///top border
    glPushMatrix();
    glTranslatef(0,door_height-door_length,-(floor_length/2-door_length-0.5));
    glScalef(door_width,door_length,door_length+.5);
    glTranslatef(-0.5,0,-0.5);
    cube(0.671, 0.522, 0.329);
    glPopMatrix();

    glPopMatrix();

}

static void pool(float view_x = 0, float view_y = 0)
{
    float windowsize = 3.5, near_val = 3.5, far_val = 150.0 ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-windowsize, windowsize, -windowsize, windowsize, near_val, far_val);
    //glOrtho(-30, 30, -30, 30, 2, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);
    glViewport(view_x, view_y, width, height);

    glRotatef(rot_right_left, x_axis,y_axis,z_axis);
    glRotatef(angle_rotate, 0,1,0);

    ///================wall====================
    wall();

    ///===============main door===================

    door();

    ///=================table=====================
    //glTranslatef(floor_width/2,0,floor_length/2);
    table();

    float z = 2, x = 2.75, thresh = .5 ;
    ///==================8 balls==================
    for(z = 6 ; z >= 2 ; z-=1)
    {
        x = x-thresh ;
        for(float i=x; i>=(-x+thresh) ; i-=1)
        {
            glPushMatrix();
            glTranslatef(i,0,z);
            balls();
            glPopMatrix();
        }
        //x = x-1 ;

    }
    //balls();

    ///==================fan======================

    glPushMatrix() ;
    glTranslatef(0,0,40);
    fan();
    glPopMatrix() ;

    for(int x = 15 ; x>=-15 ; x-=30)
    {
        for(int z=25 ; z>=-25 ; z-=25)
        {
            glPushMatrix() ;
            glTranslatef(x,0,z);
            fan();
            glPopMatrix() ;
        }
    }

    glutSwapBuffers();
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    case 'a':
        //eyeX += 1;
        //lookX += 1;
        /*eyeY = 15;
        eyeZ = -10;
        x_axis = 0 ;
        y_axis = 1 ;
        z_axis = 0 ;*/

        x_axis = 0 ;
        y_axis = 1 ;
        z_axis = 0 ;
        rot_right_left+=1;
        break;
    case 'd':
        //eyeX -= 1;
        //lookX -= 1;
        /*
        eyeX = 0;
        eyeY = 15;
        eyeZ = -10;
        x_axis = 0 ;
        y_axis = 1 ;
        z_axis = 0 ;*/

        x_axis = 0 ;
        y_axis = 1 ;
        z_axis = 0 ;
        rot_right_left-=1;
        break;
    case 'w':
        /*
        eyeX = 0;
        eyeY = 15;
        eyeZ = -10;
        x_axis = 1 ;
        y_axis = 0 ;
        z_axis = 0 ;
        */
        //lookY++;
        x_axis = 1 ;
        y_axis = 0 ;
        z_axis = 0 ;
        rot_right_left+=1;
        break;
    case 's':
        /*
        eyeX = 0;
        eyeY = 15;
        eyeZ = -10;
        x_axis = 1 ;
        y_axis = 0 ;
        z_axis = 0 ;
        */
        x_axis = 1 ;
        y_axis = 0 ;
        z_axis = 0 ;
        rot_right_left-=1;
        break;
    case 'z':
        eyeX += 1;
        lookX +=1 ;
        break;
    case 'x':
        eyeX -= 1;
        lookX -=1 ;
        break;
    case 'r':
        flagScale = !flagScale ;
        break ;
    case 'b':
        flagrotate = !flagrotate ;
        break ;
    case 'o':
        door_cntrl = !door_cntrl ;
        break;
    case '0':
        light0_switch = !light0_switch ;
        break;
    case '1':
        light1_switch = !light1_switch ;
        break;
    case '2':
        light2_switch = !light2_switch ;
        break;
    case '4':
        ambient_switch = !ambient_switch ;
        if(ambient_switch==false)
        {
            ambient_val = .25 ;

            for(int i=0 ; i<3 ; i++)
            {
                light_ambient[i] = ambient_val ;
            }
            glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
            glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
        }
        else
        {
            ambient_val = 0 ;

            for(int i=0 ; i<3 ; i++)
            {
                light_ambient[i] = ambient_val ;
            }
            glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
            glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
        }
        break;
    case '5':
        diffusion_switch = !diffusion_switch ;
        if(diffusion_switch==false)
        {
            diffuse_val = 1 ;

            for(int i=0 ; i<3 ; i++)
            {
                light_diffuse[i] = diffuse_val ;
            }

            glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
            glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        }
        else
        {
            diffuse_val = 0 ;

            for(int i=0 ; i<3 ; i++)
            {
                light_diffuse[i] = diffuse_val ;
            }

            glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
            glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);

        }
        break;
    case '6':
        specular_switch = !specular_switch ;
        if(specular_switch==false)
        {
            specular_val = 1 ;

            for(int i=0 ; i<3 ; i++)
            {
                light_specular[i] = specular_val ;
            }

            glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
            glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
        }
        else
        {
            specular_val = 0 ;

            for(int i=0 ; i<3 ; i++)
            {
                light_specular[i] = specular_val ;
            }

            glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
            glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
        }
        break;
    case '+':
        //eyeX = 0;
        //eyeY = 15;
        eyeZ += 1;
        lookZ += 1;
        break ;
    case '-':
        //eyeX = 0;
        //eyeY = 15;
        eyeZ -= 1;
        //lookZ -= 1;
        break ;
    case 'f': /* Fullscreen mode (Additional) : f/F */
        fullscreen = !fullscreen;
        if (fullscreen)
        {
            glutFullScreen();                /* Go to full screen */
        }
        else
        {
            glutReshapeWindow(800, 600);        /* Restore us */
            glutPositionWindow(10,30);
        }
    }

    glutPostRedisplay();
}

static void display()
{
    pool();
}

void animate()
{
    ///============fan rotate=============
    if (flagScale == true)
    {
        angle+= 10;
        if(angle > 360)
            angle = 0;
    }

    ///===========room rotate==============
    if (flagrotate == true)
    {
        angle_rotate+= .25;
        if(angle_rotate > 360)
            angle_rotate = 0;
    }

    ///==========door control=============
    if (door_cntrl == true)
    {
        if(door_angle>=-135)
            door_angle -= 1.5 ;
    }
    else if(door_cntrl==false)
    {
        if(door_angle<0)
            door_angle += 1.5 ;
    }

    ///===========light0 control===============
    if (light0_switch == true)
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
    }
    else
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    }
    ///==========light1 control================
    if (light1_switch == true)
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
    }
    else
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
    }
    ///============spotlight control=============
    if(light2_switch==true)
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
    }
    else
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, splight_ambient);
        glLightfv( GL_LIGHT2, GL_DIFFUSE, splight_diffuse);
        glLightfv( GL_LIGHT2, GL_SPECULAR, splight_specular);

        GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
        glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, cutoff_val);
    }

    glutPostRedisplay();
}



void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Pool 3D");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(animate) ;
    glutReshapeFunc(resize);

    glEnable(GL_DEPTH_TEST);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);


    light();
    LoadTexture("G:\\KUET\\Welcome 2K16\\4-2\\LAB\\CSE 4208\\Pool Project\\floor 1.bmp");
    floor_tex = ID ;
    LoadTexture("G:\\KUET\\Welcome 2K16\\4-2\\LAB\\CSE 4208\\Pool Project\\wall 4.bmp");
    wall_tex = ID ;
    LoadTexture("G:\\KUET\\Welcome 2K16\\4-2\\LAB\\CSE 4208\\Pool Project\\wall.bmp");
    beam_tex = ID ;
    LoadTexture("G:\\KUET\\Welcome 2K16\\4-2\\LAB\\CSE 4208\\Pool Project\\door.bmp");
    door_tex = ID ;
    LoadTexture("G:\\KUET\\Welcome 2K16\\4-2\\LAB\\CSE 4208\\Pool Project\\pool border.bmp");
    poolborder_tex = ID ;

    printf("Use 'w' to look up\n's' to look down\n'd' to look right\n'a' to look left.\n");
    printf("Use 'b' to animate classroom\n'r' to off/on fan\n");
    printf("Use '+' to zoom in\n'-' to zoom out.\n");
    printf("Use 'o' to open or close the door.\n");
    printf("Use 'o' to open or to close the door.\n");
    printf("Use 'z' to shift left and 'x' to shift right\n");
    printf("Use '0,1,2' to on/off light0,light1 and spotlight\n");
    printf("Use '3,4,5' to on/off ambient,diffuse and specular effect\n\n\n");

    glutMainLoop();

    return EXIT_SUCCESS;
}
