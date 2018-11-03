#include "window.h"

int main(int argc, char **argv)
{
    owl::forms::window win;
    owl::forms::control child(&win);
    child.backcolor(200,200,200);
    child.location(100,100);
    child.size(300,300);

    win.create();
    win.title("oeoe");
    child.create();

    owl::forms::image img;

    if (img.load("../../media/test.png"))
    {
        child.draw(&img, 10,0);

    }
    win.refresh();
    win.draw(&img, 0,0);

    while (win.created())
    {

        owl::forms::window::update();
    }

    return 0;
}
