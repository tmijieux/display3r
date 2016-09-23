#include <string>
#include <ncurses.h>

#include "display3r/Window.hpp"
#include "display3r/Exception.hpp"
#include "display3r/PluginDetails.hpp"
#include "display3r/Camera.hpp" // for directions
#include "display3r/ncurses/backend_ncurses.hpp"

DISPLAY3R_PLUGIN(backend_ncurses_factory, "ncurses window backend", "0.1");

using namespace display3r;
using std::string;
using std::cout;
using std::endl;

backend_ncurses *backend_ncurses_factory::sWindowInstance = NULL;

display3r::Window *
backend_ncurses_factory::MakeWindow(int width, int height, Color c)
{
    try {
        if (sWindowInstance == NULL)
            sWindowInstance = new backend_ncurses(width, height, c);
    } catch (exception &e) {
        cout << e.what() << endl;
        return NULL;
    }
    return sWindowInstance;
}

/* --- */

int backend_ncurses::GetWidth()
{
    return stdscr->_maxx;
}

int backend_ncurses::GetHeight()
{
    return stdscr->_maxy;
}

void backend_ncurses::Clear()
{
    clear();
}

void backend_ncurses::Update()
{
    refresh();
}

void backend_ncurses::SetPixel(ivec2 const &A, Color const &c)
{
    int maxW = 0;
    int maxH = 0;

    getmaxyx(stdscr, maxH, maxW);
    if (A.x >= 0 && A.x < maxW / 2 && A.y >= 0 && A.y < maxH) {
	int downer, upper;
	GetIntervalID(c, &downer, &upper);
	attron(COLOR_PAIR(downer));
	mvprintw(A.y, 2 * A.x, " ");
	attroff(COLOR_PAIR(downer));
	attron(COLOR_PAIR(upper));
	mvprintw(A.y, 2 * A.x + 1, " ");
	attroff(COLOR_PAIR(upper));
    }
}

Color backend_ncurses::GetPixel(ivec2 const&)
{
    return Color(0,0,0);
}

backend_ncurses::backend_ncurses(int width, int height, Color bg):
    Window(width, height, bg)
{
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    curs_set(0);

    if (!InitColor(bg)) {
	endwin();
	throw exception("Unable to change colors."
                        " Try <export TERM=xterm-256color\n");
    }
}

backend_ncurses::~backend_ncurses()
{
    endwin();
    backend_ncurses_factory::sWindowInstance = NULL;
}

bool backend_ncurses::PollEvent(Event &event)
{
    int c;
    nodelay(stdscr, TRUE);
    if ((c = getch()) == ERR)
        return false;
    ParseEvent(event, c);
    return true;
}

void backend_ncurses::WaitEvent(Event &event)
{
    int c;
    nodelay(stdscr, FALSE);
    while ((c = getch()) == ERR)
    {
        /* WAIT */
    }
    ParseEvent(event, c);
}

void backend_ncurses::ParseEvent(Event &event, int c)
{
    event.type = Event::OTHER;
    switch (c) {
    case KEY_LEFT:
        event.type = Event::ROTATE;
        event.direction = LEFT;
        break;
    case KEY_RIGHT:
        event.type = Event::ROTATE;
        event.direction = RIGHT;
        break;
    case KEY_UP:
        event.type = Event::ROTATE;
        event.direction = UP;
        break;
    case KEY_DOWN:
        event.type = Event::ROTATE;
        event.direction = DOWN;
        break;
    case 'r':
    case KEY_RESIZE: {
        getmaxyx(stdscr, event.height, event.width);
        resize_term(event.height, event.width);
        event.type = Event::RESIZE;
        event.width /= 2;
        break;
    }
    case 'q':
        event.type = Event::TRANSLATE;
        event.direction = LEFT;
        break;
    case 'z':
        event.type = Event::TRANSLATE;
        event.direction = FORWARD;
        break;
    case 'd':
        event.type = Event::TRANSLATE;
        event.direction = RIGHT;
        break;
    case 's':
        event.type = Event::TRANSLATE;
        event.direction = BACKWARD;
        break;
    case 'a':
        event.type = Event::TRANSLATE;
        event.direction = DOWN;
        break;
    case 'e':
        event.type = Event::TRANSLATE;
        event.direction = UP;
        break;
    case 'p':
        event.type = Event::QUIT;
        break;
    case 'o':
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::FACE;
        break;
    case 'w':
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::WIREFRAME;
        break;
    case 'n':
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::NORMAL;
        break;
    case 'v':
        event.type = Event::DRAW;
        event.drawState = Renderer::DrawState::VERTEX;
        break;
    case 'f':
        event.type = Event::HIDE_FRAME;
        break;
    case 'l':
        event.type = Event::LOAD;
        break;
    case 'u':
        event.type = Event::UNLOAD;
        break;
    default:
        break;
    }
}
