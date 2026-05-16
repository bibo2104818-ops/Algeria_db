#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "list.h"
#include "stack.h"
#include "binary_tree.h"

// ─── MASTER WIDGET COUPLING PROTOTYPES ─────────────────────────────────────
void init_algeria_database_gui(int argc, char *argv[]);
void apply_theme(const char *css_file_name);
void update_details_card(const char *name, const char *lifespan, const char *status, const char *biography);

#endif