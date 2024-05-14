#include "tree_visualizer.h"
#include <SFML/Config.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Types.h>
#include <SFML/System/Clock.h>
#include <SFML/System/Time.h>
#include <SFML/System/Types.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/Event.h>
#include <SFML/Window/Keyboard.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Graphics/View.h>
#include <SFML/Window/Window.h>
#include <SFML/Window/WindowBase.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct Handles {
    sfRenderWindow* window;
    sfClock* mov_cl;
    sfClock* zoom_cl;
};

void destroy_handles(struct Handles g) {
    sfRenderWindow_destroy(g.window);
    sfClock_destroy(g.mov_cl);
    sfClock_destroy(g.zoom_cl);
}

void visualize_tree(TextTreeVisualization* tree) {
    struct Handles handles;
    memset(&handles, 0, sizeof(handles));
    
    sfContextSettings settings = {0, 0, 16, 1, 1, sfContextDefault, sfFalse};
    sfRenderWindow* window = sfRenderWindow_create((sfVideoMode){1800, 1000, 32},
                "MiNI Decision Tree Project", sfClose | sfTitlebar, &settings);

    if (!window) {
        destroy_handles(handles);
        fprintf(stderr, "Failed to create window\n");
        return;
    }

    handles.window = window;

    int zoom_incr = 0;
    int zoom_decr = 0;
    int w_mov = 0;
    int a_mov = 0;
    int s_mov = 0;
    int d_mov = 0;

    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);

    sfClock* mov_cl = sfClock_create();
    if (!mov_cl) {
        destroy_handles(handles);
        return;
    }
    handles.mov_cl = mov_cl;

    sfClock* zoom_cl = sfClock_create();
    if (!zoom_cl) {
        destroy_handles(handles);
        return;
    }
    handles.zoom_cl = zoom_cl;

    while (sfRenderWindow_isOpen(window)) {
        sfEvent evt;
        while (sfRenderWindow_pollEvent(window, &evt)) {
            switch (evt.type) {
            case sfEvtClosed:
                sfRenderWindow_close(window);
                break;
            case sfEvtKeyPressed:
                switch (evt.key.code) {
                case sfKeyW:
                    w_mov = 1;
                    break;
                    case sfKeyA:
                        a_mov = 1;
                    break;
                    case sfKeyS:
                        s_mov = 1;
                    break;
                    case sfKeyD:
                        d_mov = 1;
                    break;
                    case sfKeyZ:
                        zoom_decr = 1;
                    break;
                    case sfKeyX:
                        zoom_incr = 1;
                    break;
                    case sfKeyEscape:
                    sfRenderWindow_close(window);
                    break;
                    default:
                    break;
                }
                break;
            case sfEvtKeyReleased:
                switch (evt.key.code) {
                case sfKeyW:
                    w_mov = 0;
                    break;
                    case sfKeyA:
                        a_mov = 0;
                    break;
                    case sfKeyS:
                        s_mov = 0;
                    break;
                    case sfKeyD:
                        d_mov = 0;
                    break;
                    case sfKeyZ:
                        zoom_decr = 0;
                    break;
                    case sfKeyX:
                        zoom_incr = 0;
                    break;
                    default:
                    break;
                }
                break;
            default:
                break; 
            }
        }

        sfTime mov_elapsed = sfClock_restart(mov_cl);
        sfTime zoom_elapsed = sfClock_restart(zoom_cl);

        if (w_mov) {
            sfView *view = sfView_copy(sfRenderWindow_getView(window));
            if (!view) {
                destroy_handles(handles);
                return;
            }
            sfView_move(view, (sfVector2f){0, -mov_elapsed.microseconds/1000.f});
            sfRenderWindow_setView(window, view);
            sfView_destroy(view);
        }
        if (a_mov) {
            sfView *view = sfView_copy(sfRenderWindow_getView(window));
            if (!view) {
                destroy_handles(handles);
                return;
            }
            sfView_move(view, (sfVector2f){-mov_elapsed.microseconds/1000.f, 0});
            sfRenderWindow_setView(window, view);
            sfView_destroy(view);
        }
        if (s_mov) {
            sfView *view = sfView_copy(sfRenderWindow_getView(window));
            if (!view) {
                destroy_handles(handles);
                return;
            }
            sfView_move(view, (sfVector2f){0, mov_elapsed.microseconds/1000.f});
            sfRenderWindow_setView(window, view);
            sfView_destroy(view);
        }
        if (d_mov) {
            sfView *view = sfView_copy(sfRenderWindow_getView(window));
            if (!view) {
                destroy_handles(handles);
                return;
            }
            sfView_move(view, (sfVector2f){mov_elapsed.microseconds/1000.f, 0});
            sfRenderWindow_setView(window, view);
            sfView_destroy(view);
        }
        if (zoom_incr) {
            sfView *view = sfView_copy(sfRenderWindow_getView(window));
            if (!view) {
                destroy_handles(handles);
                return;
            }
            sfView_zoom(view, exp(zoom_elapsed.microseconds / 1000000.f));
            sfRenderWindow_setView(window, view);
            sfView_destroy(view);
        }
        if (zoom_decr) {
            sfView *view = sfView_copy(sfRenderWindow_getView(window));
            if (!view) {
                destroy_handles(handles);
                return;
            }
            sfView_zoom(view, exp(-zoom_elapsed.microseconds / 1000000.f));
            sfRenderWindow_setView(window, view);
            sfView_destroy(view);
        }

        sfRenderWindow_clear(window, (sfColor){255,255,255,255});
        tree_visualization_draw(tree, window);
        sfRenderWindow_display(window);
    }

    destroy_handles(handles);
    return;
}