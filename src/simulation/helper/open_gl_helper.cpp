#include "open_gl_helper.h"


namespace OpenGLHelper{

    void drawText(Color color, Position projectionPosition, PositionXY position, std::string text){
        glPushAttrib(GL_CURRENT_BIT);
        glColor3f(color.r,color.g,color.b);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(projectionPosition.left, projectionPosition.right, projectionPosition.bottom, projectionPosition.top);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glRasterPos2f(position.x, position.y);

        char        buf[15] = "FPS : ";
        const char* p(buf);

        strcat(buf, text.c_str());
        do glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p); while(*(++p));

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glPopAttrib();
    }

}