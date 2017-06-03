#include "window.h"

int main(int argc, char **argv)
{
    owl::forms::window win;
    owl::forms::control child(&win);
    child.backcolor(255,0,0);
    child.size(300,300);

    win.create();
    win.title("oeoe");
    child.create();

    owl::forms::image img;

    if (img.load("../../media/photo.png"))
    {
        child.draw(&img, 320,200);
    }

    while (win.created())
    {
        owl::forms::window::update();
    }

    return 0;
}
