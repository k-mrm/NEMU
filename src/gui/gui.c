#include "gui/gui.h"

void gui_init(int *argc, char **argv) {
    glutInitWindowSize(256, 240);
    glutInit(argc, argv);
    glutCreateWindow(argv[0]);
}

