#include <io/pad.h>
#include <tiny3d.h>

// Minimal PS3 startup test based on the Tiny3D sample initialization order.
// This isolates package/SELF/Tiny3D startup from the unfinished game code.
static void drawRect(float x, float y, float w, float h, unsigned int color) {
    tiny3d_SetPolygon(TINY3D_QUADS);
    tiny3d_VertexPos(x, y, 65535);
    tiny3d_VertexColor(color);
    tiny3d_VertexPos(x + w, y, 65535);
    tiny3d_VertexPos(x + w, y + h, 65535);
    tiny3d_VertexPos(x, y + h, 65535);
    tiny3d_End();
}

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    // Tiny3D sample initializes graphics before controller I/O.
    tiny3d_Init(1024 * 1024);
    ioPadInit(7);

    padInfo padinfo;
    padData paddata;

    while (1) {
        tiny3d_Clear(0xFF080D20, TINY3D_CLEAR_ALL);
        tiny3d_Project2D();

        // Visible diagnostic screen: cyan frame and blue center panel.
        drawRect(0, 0, 848, 512, 0xFF102040);
        drawRect(24, 24, 800, 464, 0xFF00BFFF);
        drawRect(32, 32, 784, 448, 0xFF102040);
        drawRect(180, 190, 488, 132, 0xFF087FFF);

        tiny3d_Flip();

        ioPadGetInfo(&padinfo);
        for (int i = 0; i < MAX_PADS; ++i) {
            if (padinfo.status[i]) {
                ioPadGetData(i, &paddata);
                if (paddata.BTN_CROSS) {
                    ioPadEnd();
                    return 0;
                }
            }
        }
    }

    return 0;
}
