#include <iostream>

// Test includes for GLEW, GLFW, GLUT, GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <GL/glu.h>

// Texture IDs
static GLuint texBark =0;
static GLuint texLeaves =0;

// Create simple procedural textures for bark and leaves
void createProceduralTextures()
{
 const int TS =256;
 unsigned char bark[TS][TS][4];
 unsigned char leaf[TS][TS][4];

 // Bark: vertical stripes with slight variation
 for (int y =0; y < TS; ++y) {
 for (int x =0; x < TS; ++x) {
 int stripe = (x /16) %2;
 float v = (stripe) ?0.55f :0.35f;
 // add subtle noise based on y
 float noise = ((x *7 + y *13) %23) /115.0f;
 float r = (0.45f + noise) * v;
 float g = (0.26f + noise *0.5f) * v;
 float b = (0.07f + noise *0.2f) * v;
 bark[y][x][0] = (unsigned char)(fminf(1.0f, r) *255);
 bark[y][x][1] = (unsigned char)(fminf(1.0f, g) *255);
 bark[y][x][2] = (unsigned char)(fminf(1.0f, b) *255);
 bark[y][x][3] =255;
 }
 }

 // Leaves: radial gradient with green variations
 for (int y =0; y < TS; ++y) {
 for (int x =0; x < TS; ++x) {
 float nx = (x - TS/2) / (float)(TS/2);
 float ny = (y - TS/2) / (float)(TS/2);
 float d = sqrtf(nx*nx + ny*ny);
 float alpha = (d <1.0f) ? (1.0f - d) :0.0f;
 float base =0.45f +0.25f * ((x*y) %7) /7.0f;
 float r = base *0.2f;
 float g = base;
 float b = base *0.4f;
 leaf[y][x][0] = (unsigned char)(fminf(1.0f, r) *255);
 leaf[y][x][1] = (unsigned char)(fminf(1.0f, g) *255);
 leaf[y][x][2] = (unsigned char)(fminf(1.0f, b) *255);
 leaf[y][x][3] = (unsigned char)(alpha *255);
 }
 }

 glGenTextures(1, &texBark);
 glBindTexture(GL_TEXTURE_2D, texBark);
 glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, TS, TS,0, GL_RGBA, GL_UNSIGNED_BYTE, bark);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

 glGenTextures(1, &texLeaves);
 glBindTexture(GL_TEXTURE_2D, texLeaves);
 glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, TS, TS,0, GL_RGBA, GL_UNSIGNED_BYTE, leaf);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

 // Unbind
 glBindTexture(GL_TEXTURE_2D,0);
}

// Draw a simple axis-aligned textured cube centered at (0,0,0) with given half-sizes
void drawTexturedCube(float hx, float hy, float hz)
{
 glBegin(GL_QUADS);
 // +X face
 glNormal3f(1,0,0);
 glTexCoord2f(0,0); glVertex3f(hx, -hy, -hz);
 glTexCoord2f(1,0); glVertex3f(hx, -hy, hz);
 glTexCoord2f(1,1); glVertex3f(hx, hy, hz);
 glTexCoord2f(0,1); glVertex3f(hx, hy, -hz);
 // -X face
 glNormal3f(-1,0,0);
 glTexCoord2f(0,0); glVertex3f(-hx, -hy, hz);
 glTexCoord2f(1,0); glVertex3f(-hx, -hy, -hz);
 glTexCoord2f(1,1); glVertex3f(-hx, hy, -hz);
 glTexCoord2f(0,1); glVertex3f(-hx, hy, hz);
 // +Y face
 glNormal3f(0,1,0);
 glTexCoord2f(0,0); glVertex3f(-hx, hy, -hz);
 glTexCoord2f(1,0); glVertex3f(hx, hy, -hz);
 glTexCoord2f(1,1); glVertex3f(hx, hy, hz);
 glTexCoord2f(0,1); glVertex3f(-hx, hy, hz);
 // -Y face
 glNormal3f(0,-1,0);
 glTexCoord2f(0,0); glVertex3f(-hx, -hy, hz);
 glTexCoord2f(1,0); glVertex3f(hx, -hy, hz);
 glTexCoord2f(1,1); glVertex3f(hx, -hy, -hz);
 glTexCoord2f(0,1); glVertex3f(-hx, -hy, -hz);
 // +Z face
 glNormal3f(0,0,1);
 glTexCoord2f(0,0); glVertex3f(-hx, -hy, hz);
 glTexCoord2f(1,0); glVertex3f(-hx, hy, hz);
 glTexCoord2f(1,1); glVertex3f(hx, hy, hz);
 glTexCoord2f(0,1); glVertex3f(hx, -hy, hz);
 // -Z face
 glNormal3f(0,0,-1);
 glTexCoord2f(0,0); glVertex3f(hx, -hy, -hz);
 glTexCoord2f(1,0); glVertex3f(hx, hy, -hz);
 glTexCoord2f(1,1); glVertex3f(-hx, hy, -hz);
 glTexCoord2f(0,1); glVertex3f(-hx, -hy, -hz);
 glEnd();
}

// Draw a textured pyramid (square base) centered on base at y=0
void drawTexturedPyramid(float halfSize, float height)
{
 // base
 glBegin(GL_QUADS);
 glNormal3f(0,-1,0);
 glTexCoord2f(0,0); glVertex3f(-halfSize,0, -halfSize);
 glTexCoord2f(1,0); glVertex3f(halfSize,0, -halfSize);
 glTexCoord2f(1,1); glVertex3f(halfSize,0, halfSize);
 glTexCoord2f(0,1); glVertex3f(-halfSize,0, halfSize);
 glEnd();

 // triangular faces with simple texcoords
 glBegin(GL_TRIANGLES);
 // front
 {
 float ax = -halfSize, ay =0, az = halfSize;
 float bx = halfSize, by =0, bz = halfSize;
 float cx =0, cy = height, cz =0;
 // normal calculation as before
 float ux = bx - ax, uy = by - ay, uz = bz - az;
 float vx = cx - ax, vy = cy - ay, vz = cz - az;
 float nx = uy * vz - uz * vy;
 float ny = uz * vx - ux * vz;
 float nz = ux * vy - uy * vx;
 float len = sqrtf(nx*nx + ny*ny + nz*nz);
 if (len >0) { nx/=len; ny/=len; nz/=len; }
 glNormal3f(nx, ny, nz);
 glTexCoord2f(0,0); glVertex3f(ax, ay, az);
 glTexCoord2f(1,0); glVertex3f(bx, by, bz);
 glTexCoord2f(0.5f,1); glVertex3f(cx, cy, cz);
 }
 // right
 {
 float ax = halfSize, ay =0, az = halfSize;
 float bx = halfSize, by =0, bz = -halfSize;
 float cx =0, cy = height, cz =0;
 float ux = bx - ax, uy = by - ay, uz = bz - az;
 float vx = cx - ax, vy = cy - ay, vz = cz - az;
 float nx = uy * vz - uz * vy;
 float ny = uz * vx - ux * vz;
 float nz = ux * vy - uy * vx;
 float len = sqrtf(nx*nx + ny*ny + nz*nz);
 if (len >0) { nx/=len; ny/=len; nz/=len; }
 glNormal3f(nx, ny, nz);
 glTexCoord2f(0,0); glVertex3f(ax, ay, az);
 glTexCoord2f(1,0); glVertex3f(bx, by, bz);
 glTexCoord2f(0.5f,1); glVertex3f(cx, cy, cz);
 }
 // back
 {
 float ax = halfSize, ay =0, az = -halfSize;
 float bx = -halfSize, by =0, bz = -halfSize;
 float cx =0, cy = height, cz =0;
 float ux = bx - ax, uy = by - ay, uz = bz - az;
 float vx = cx - ax, vy = cy - ay, vz = cz - az;
 float nx = uy * vz - uz * vy;
 float ny = uz * vx - ux * vz;
 float nz = ux * vy - uy * vx;
 float len = sqrtf(nx*nx + ny*ny + nz*nz);
 if (len >0) { nx/=len; ny/=len; nz/=len; }
 glNormal3f(nx, ny, nz);
 glTexCoord2f(0,0); glVertex3f(ax, ay, az);
 glTexCoord2f(1,0); glVertex3f(bx, by, bz);
 glTexCoord2f(0.5f,1); glVertex3f(cx, cy, cz);
 }
 // left
 {
 float ax = -halfSize, ay =0, az = -halfSize;
 float bx = -halfSize, by =0, bz = halfSize;
 float cx =0, cy = height, cz =0;
 float ux = bx - ax, uy = by - ay, uz = bz - az;
 float vx = cx - ax, vy = cy - ay, vz = cz - az;
 float nx = uy * vz - uz * vy;
 float ny = uz * vx - ux * vz;
 float nz = ux * vy - uy * vx;
 float len = sqrtf(nx*nx + ny*ny + nz*nz);
 if (len >0) { nx/=len; ny/=len; nz/=len; }
 glNormal3f(nx, ny, nz);
 glTexCoord2f(0,0); glVertex3f(ax, ay, az);
 glTexCoord2f(1,0); glVertex3f(bx, by, bz);
 glTexCoord2f(0.5f,1); glVertex3f(cx, cy, cz);
 }
 glEnd();
}

// Draw a simple tree at world position (x,z) using textures
void drawTree3D(float x, float z, float trunkW, float trunkH, float foliageSize, float foliageH)
{
 // trunk: textured cube
 glPushMatrix();
 glTranslatef(x, trunkH/2.0f, z);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, texBark);
 glColor3f(1.0f,1.0f,1.0f);
 drawTexturedCube(trunkW/2.0f, trunkH/2.0f, trunkW/2.0f);
 glBindTexture(GL_TEXTURE_2D,0);
 glDisable(GL_TEXTURE_2D);
 glPopMatrix();

 // foliage: textured pyramid
 glPushMatrix();
 glTranslatef(x, trunkH, z);
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, texLeaves);
 glColor3f(1.0f,1.0f,1.0f);
 drawTexturedPyramid(foliageSize/2.0f, foliageH);
 glBindTexture(GL_TEXTURE_2D,0);
 glDisable(GL_TEXTURE_2D);
 glPopMatrix();
}

int main(int argc, char** argv)
{
 // Initialize GLFW
 if (!glfwInit()) {
 std::cerr << "GLFW init failed" << std::endl;
 return 1;
 }

 // Create a window with depth buffer
 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
 glfwWindowHint(GLFW_DEPTH_BITS,24);
 GLFWwindow* window = glfwCreateWindow(1024,768, "Simple3D Scene: Sky, Ground, Trees", nullptr, nullptr);
 if (!window) {
 std::cerr << "Failed to create GLFW window" << std::endl;
 glfwTerminate();
 return 1;
 }
 glfwMakeContextCurrent(window);

 // Initialize GLEW
 glewExperimental = GL_TRUE;
 GLenum glewErr = glewInit();
 if (glewErr != GLEW_OK) {
 std::cerr << "GLEW init failed: " << (const char*)glewGetErrorString(glewErr) << std::endl;
 glfwDestroyWindow(window);
 glfwTerminate();
 return 1;
 }

 // Create procedural textures
 createProceduralTextures();

 // Enable depth test and basic backface culling
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LEQUAL);
 glEnable(GL_CULL_FACE);
 glCullFace(GL_BACK);

 // Simple lighting
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 GLfloat lightPos[] = { -1.0f,1.0f,0.5f,0.0f }; // directional
 GLfloat lightColor[] = {1.0f,1.0f,0.95f,1.0f };
 glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
 glEnable(GL_COLOR_MATERIAL);
 glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

 // Camera parameters
 float camAngle =0.0f; // around Y
 float camDist =80.0f;
 float camHeight =25.0f;

 // Main loop
 while (!glfwWindowShouldClose(window)) {
 int w, h;
 glfwGetFramebufferSize(window, &w, &h);
 glViewport(0,0, w, h);

 // Setup perspective
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 double aspect = (h >0) ? (double)w / (double)h :1.0;
 gluPerspective(45.0, aspect,0.1,1000.0);

 // Camera
 float eyeX = sinf(camAngle) * camDist;
 float eyeZ = cosf(camAngle) * camDist;
 float eyeY = camHeight;

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(eyeX, eyeY, eyeZ, // eye
0.0f,5.0f,0.0f, // center (look at slightly above ground)
0.0f,1.0f,0.0f); // up

 // Clear
 glClearColor(0.529f,0.808f,0.922f,1.0f); // sky color
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Draw ground plane (large quad)
 glPushMatrix();
 glColor3f(0.2f,0.6f,0.2f);
 glBegin(GL_QUADS);
 glNormal3f(0,1,0);
 float g =500.0f;
 glVertex3f(-g,0.0f, -g);
 glVertex3f(g,0.0f, -g);
 glVertex3f(g,0.0f, g);
 glVertex3f(-g,0.0f, g);
 glEnd();
 glPopMatrix();

 // Draw several trees at different positions
 drawTree3D(-30.0f, -10.0f,2.0f,8.0f,8.0f,12.0f);
 drawTree3D(-10.0f,5.0f,1.8f,7.0f,7.0f,10.0f);
 drawTree3D(5.0f, -20.0f,2.2f,9.0f,9.0f,13.0f);
 drawTree3D(20.0f,10.0f,1.6f,6.0f,6.0f,9.0f);
 drawTree3D(35.0f, -5.0f,2.0f,8.0f,8.0f,12.0f);

 // Simple ground grid lines for depth perception
 glDisable(GL_LIGHTING);
 glColor3f(0.12f,0.45f,0.12f);
 glBegin(GL_LINES);
 for (int i = -10; i <=10; ++i) {
 glVertex3f(i*5.0f,0.01f, -50.0f);
 glVertex3f(i*5.0f,0.01f,50.0f);
 glVertex3f(-50.0f,0.01f, i*5.0f);
 glVertex3f(50.0f,0.01f, i*5.0f);
 }
 glEnd();
 glEnable(GL_LIGHTING);

 // Swap buffers and poll events
 glfwSwapBuffers(window);
 glfwPollEvents();

 // Camera controls
 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camAngle -=0.02f;
 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camAngle +=0.02f;
 if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { camDist -=1.0f; if (camDist <5.0f) camDist =5.0f; }
 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camDist +=1.0f;
 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camHeight +=0.5f;
 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camHeight -=0.5f;
 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window,1);
 }

 glfwDestroyWindow(window);
 glfwTerminate();
 return 0;
}
