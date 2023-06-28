void drawPixel(RGFW_window* win, unsigned int x, unsigned int y, char color[3]) {
    int i;

    for (i = 0; i < 4; i++)
        win->buffer[4 * win->h * x + y * 4 + i] = color[
                                                        i == 2 ? 0 : 
                                                        i == 0 ? 2 :
                                                        i
                                                       ];
}

void drawRect(RGFW_window* win, unsigned int x, unsigned int y, unsigned int w, unsigned int h, char color[3]) {
    unsigned int mx, my, z;

    for (my = y; my < y + h; my++)
        for (mx = x; mx < x + w; mx++)
            drawPixel(win, mx, my, color);
}

void clearScreen(RGFW_window* win) {
    RGFW_swapBuffers(win);

    drawRect(win, 0, 0, win->w, win->h, (char[]){0, 0, 0});
}

void drawMono(RGFW_window* window, unsigned int* pixels /* monochrome bitmap */ ) {
    clearScreen(window);
    drawRect(window, 0, 0, window->w, window->h, (char[]){0, 0, 0});
    int x, y;
    
    for (y = 0; y < SCREEN_HEIGHT; y++)
        for (x = 0; x < SCREEN_WIDTH; x++)
            if (pixels[SCREEN_HEIGHT * x + y])
                drawRect(window, x, y, 1, 1, (char[]){255, 255, 255});
}
