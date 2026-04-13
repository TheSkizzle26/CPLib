/*
 * Basically just raylib for the classpad II.
 * Can't do much at the moment.
 * Edit: can actually do quite a bit now.
 *
 * Also doesn't require the hollyhock-2 SDK, as it is included in this lib.
 *
 * If you would like to use this in another project,
 * just copy the cplib folder to your project and take a look
 * at this example script. You may also want to copy linker.ld and Makefile.
 */


#include "cplib/cplib.h"

APP_NAME("CPLib test")
APP_DESCRIPTION("CPLib test program")
APP_AUTHOR("TheSkizzle")
APP_VERSION("0.0.0")


static bool isRunning;
static int tick;

cpTexture texture;
int texX, texY;
int texVx, texVy;

cpMesh mesh;
cpCamera3d camera;


void init() {
    // init texture
    const uint8_t data[] = {0, 248, 0, 248, 0, 248, 0, 248, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 160, 7, 224, 7, 224, 7, 224, 7, 224, 7, 0, 240, 0, 248, 0, 248, 0, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 138, 82, 138, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 7, 224, 7, 224, 7, 224, 7, 0, 248, 0, 248, 0, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 121, 206, 121, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 198, 77, 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 7, 224, 7, 224, 7, 0, 248, 0, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 173, 117, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 251, 222, 174, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 7, 224, 7, 0, 248, 0, 144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 173, 117, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 173, 117, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 7, 0, 232, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 125, 239, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 173, 117, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 117, 173, 117, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 239, 123, 239, 123, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 251, 222, 251, 222, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 125, 239, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 222, 96, 173, 0, 0, 0, 0, 0, 0, 0, 0, 142, 115, 219, 222, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 219, 222, 142, 115, 0, 0, 0, 0, 0, 0, 0, 0, 23, 0, 192, 255, 160, 247, 224, 222, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 219, 222, 142, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 142, 115, 125, 239, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 0, 31, 0, 224, 255, 224, 255, 224, 255, 96, 173, 0, 0, 0, 0, 0, 0, 0, 0, 142, 115, 219, 222, 219, 222, 117, 173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 142, 115, 186, 214, 219, 222, 219, 222, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 31, 0, 31, 0, 224, 255, 224, 255, 224, 255, 192, 255, 128, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 142, 115, 117, 173, 125, 239, 125, 239, 125, 239, 219, 222, 219, 222, 219, 222, 125, 239, 125, 239, 219, 222, 186, 214, 142, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 31, 0, 31, 0, 31, 0, 224, 255, 224, 255, 224, 255, 224, 255, 192, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 142, 115, 142, 115, 142, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 30, 0, 31, 0, 31, 0, 31, 0, 224, 255, 224, 255, 192, 255, 224, 255, 224, 255, 224, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 31, 0, 31, 0, 31, 0, 31, 0, };
    texture = (cpTexture) {
        32, 16,
        CP_PIXEL_FORMAT_RGB565,
        malloc(sizeof(data))
    };
    memset(texture.data, 0, sizeof(data));
    memcpy(texture.data, data, sizeof(data));

    texX = 100;
    texY = 100;
    texVx = 5;
    texVy = 5;

    // init mesh
    const cpVector3i vertices[] = {
        {fix16_from_int(-1), fix16_from_int(-1), fix16_from_int(0)},
        {fix16_from_int(1), fix16_from_int(-1), fix16_from_int(0)},
        {fix16_from_int(0), fix16_from_int(1), fix16_from_int(0)},
    };
    const cpMeshEdge edges[] = {
        {0, 1},
        {1, 2},
        {2, 0},
    };

    mesh = (cpMesh) {
        sizeof(vertices) / sizeof(vertices[0]),
        malloc(sizeof(vertices)),
        sizeof(edges) / sizeof(edges[0]),
        malloc(sizeof(edges)),
    };

    memcpy(mesh.vertices, vertices, sizeof(vertices));
    memcpy(mesh.edges, edges, sizeof(edges));

    // init camera
    camera = (cpCamera3d) {0};
    camera.position.z = fix16_from_int(-3);
    camera.fovY = fix16_from_int(90);
}

void update() {
    if (cpIsKeyPressed(CP_KEY_CLEAR)) {
        isRunning = false;
    }

    texX += texVx;
    texY += texVy;

    // bouncy \(^-^)/
    if (texX < 0) {
        texX = 0;
        texVx *= -1;
    }
    if (texX+texture.width >= cpGetScreenWidth()) {
        texX = cpGetScreenWidth()-texture.width;
        texVx *= -1;
    }
    if (texY < 0) {
        texY = 0;
        texVy *= -1;
    }
    if (texY+texture.height >= cpGetScreenHeight()) {
        texY = cpGetScreenHeight()-texture.height;
        texVy *= -1;
    }

    camera.position.z += fix16_div(
        fix16_from_int(1),
        fix16_from_int(20)
    );

    fix16_t yaw, pitch;
    cpVector3ToRotation(cpGetCamera3dRotation(camera), &yaw, &pitch);
    yaw = fix16_add(yaw, fix16_from_str("0.1"));
    cpSetCamera3dRotation(&camera, cpRotationToVector3(yaw, pitch));
}

void render() {
    cpBeginDrawing();
    cpClearBackground(BLACK);

    // basic shapes
    cpDrawLine(0, 0, 100, 100, RED);
    cpDrawRectangle(tick % cpGetScreenWidth(), 250, 100, 50, SKYBLUE);
    cpDrawLine(0, 0, tick, 0, WHITE);
    cpDrawCircle(150, 100, 30, WHITE);

    // texture drawing, empty texture for now since it crashes on the cp?!?
    cpDrawTexture(texture, texX, texY);

    // draw mesh
    cpRegisterCamera3d(camera);
    cpDrawMesh(mesh, (cpVector3){}, WHITE);

    // draw 3d dot
    cpDrawPixel3d((cpVector3){
        fix16_from_int(0),
        fix16_from_str("0.25"),
        fix16_from_int(0),
    }, PURPLE);

    // draw 3d line
    cpDrawLine3d(
        (cpVector3){
            fix16_from_int(0),
            fix16_from_str("0.3"),
            fix16_from_int(-1),
        },
        (cpVector3){
            fix16_from_str("0.5"),
            fix16_from_str("0.5"),
            fix16_from_int(0),
        },
        RED
    );

    cpEndDrawing();
}

int main() {
    cpInit();
    cpSetTargetFPS(20);
    cpSetOverclock(CP_OC_MUL_48);

    init();
    isRunning = true;

    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (isRunning) {
        update();
        render();

        tick++;
    }

    cpQuit();
    return 0;
}
