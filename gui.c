#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"

#define MAX_RECORDS 500

// ─── MULTILINGUAL TRANSLATION DICTIONARY (EN / AR / FR) ───────────────────
typedef enum { LANG_EN, LANG_AR, LANG_FR } AppLanguage;
static AppLanguage current_language = LANG_EN;

enum {
    TXT_SIDEBAR_TITLE, TXT_SEARCH_TAB, TXT_MANAGE_TAB, TXT_TREE_TAB, TXT_OPTIONS_TAB,
    TXT_SEARCH_PLACEHOLDER, TXT_VIEW_ALL_BTN, TXT_NAME_LBL, TXT_DOB_LBL, TXT_DOD_LBL,
    TXT_BIO_LBL, TXT_INSERT_BTN, TXT_ERASE_BTN, TXT_IMPORT_BTN, TXT_THEME_LBL,
    TXT_TREE_HEADER, TXT_CARD_INITIAL, TXT_LANG_LBL, TXT_COUNT
};

static const char *translations[3][TXT_COUNT] = {
    // [LANG_EN] - English Translation Vectors
    {
        "Algerian Personalities Database", "Search Engine", "Manage Database", "Tree Explorer", "App Options",
        "Query record system prefix (e.g. Larbi)...", "View All", "Personality Name:", "Date of Birth:", "Date of Death:",
        "Historical Biography:", "Insert Record", "Erase Record", "Import File", "Interface Color Profile Theme:",
        "Binary Search Tree Node Graph Vis", "Select a Profile Node", "System Interface Language:"
    },
    // [LANG_AR] - Arabic Translation Vectors
    {
        "قاعدة بيانات الشخصيات الجزائرية", "محرك البحث", "إدارة البيانات", "مستكشف الشجرة", "خيارات التطبيق",
        "ابحث عن اسم الشخصية (مثال: العربي)...", "عرض الكل", "اسم الشخصية التاريخية:", "تاريخ الميلاد:", "تاريخ الوفاة:",
        "السيرة الذاتية والتاريخية:", "إدخال السجل", "مسح السجل", "استيراد ملف", "مظهر واجهة النظام:",
        "عرض هيكلي تفرعي لشجرة البحث الثنائية", "اختر ملف شخصية لعرض التفاصيل", "لغة واجهة النظام الإداري:"
    },
    // [LANG_FR] - French Translation Vectors
    {
        "Base de Données des Personnalités Algériennes", "Moteur de Recherche", "Gérer la Base", "Explorateur d'Arbre", "Options de l'App",
        "Rechercher une personnalité (ex: Larbi)...", "Voir Tout", "Nom de la Personnalité:", "Date de Naissance:", "Date de Décès:",
        "Biographie Historique:", "Insérer un Enregistrement", "Effacer l'Enregistrement", "Importer un Fichier", "Thème du Profil de Couleur:",
        "Visualisation de l'Arbre de Recherche Binaire", "Sélectionnez un nœud de profil", "Langue de l'Interface Système:"
    }
};

#define _(key) translations[current_language][key]

// ─── BACKGROUND STRUCTURE REFERENCES ──────────────────────────────────────
extern TList *s;              
extern void *a;               
extern TStack *stk;           
extern TTree *tree; 

// ─── CORE DISPLAY LAYOUT LABELS ──────────────────────────────────────────
GtkWidget *name_label = NULL;  
GtkWidget *dates_label = NULL; 
GtkWidget *bio_label = NULL;   

// ─── CORE ENTRY WIDGET CONTROLLERS ───────────────────────────────────────
static GtkWidget *ent_name = NULL;    
static GtkWidget *ent_dob = NULL;     
static GtkWidget *ent_dod = NULL;     
static GtkWidget *main_content_stack = NULL;
static GtkWidget *ent_def = NULL;     
static GtkWidget *main_search_entry = NULL; 

// ─── TREE EXPLORER VIEW CONTROLLERS ──────────────────────────────────────
static GtkTreeStore *tree_store = NULL;
static GtkWidget *tree_view_widget = NULL;

// ─── LIVE RE-RENDERING INTERFACE LABEL REFERENCES ─────────────────────────
static GtkWidget *lbl_sidebar_title = NULL;
static GtkWidget *lbl_mgmt_name = NULL;
static GtkWidget *lbl_mgmt_dob = NULL;
static GtkWidget *lbl_mgmt_dod = NULL;
static GtkWidget *lbl_mgmt_bio = NULL;
static GtkWidget *lbl_tree_title = NULL;
static GtkWidget *lbl_theme_title = NULL;
static GtkWidget *lbl_lang_title = NULL;

static GtkWidget *btn_view_all_widget = NULL;
static GtkWidget *btn_add_widget = NULL;
static GtkWidget *btn_del_widget = NULL;
static GtkWidget *btn_import_widget = NULL;

// ─── LOCAL DISPLAY LAYOUT BUFFER CACHE MATRIX ─────────────────────────────
static char gui_names[MAX_RECORDS][100];
static char gui_dobs[MAX_RECORDS][20];
static char gui_dods[MAX_RECORDS][20];
static char gui_bios[MAX_RECORDS][500];
static int gui_record_count = 0;

// ─── AUTOCOMPLETE SUGGESTION BUILDER ENGINE ───────────────────────────────
void update_search_completion() {
    if (!main_search_entry) return;

    GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;

    for (int i = 0; i < gui_record_count; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, gui_names[i], -1);
    }

    GtkEntryCompletion *completion = gtk_entry_completion_new();
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));
    gtk_entry_completion_set_text_column(completion, 0);
    
    gtk_entry_completion_set_inline_completion(completion, TRUE);
    gtk_entry_completion_set_popup_completion(completion, TRUE);

    gtk_entry_set_completion(GTK_ENTRY(main_search_entry), completion);
    g_object_unref(store);
    g_object_unref(completion);
}

// ─── VISUAL TREE VIEW EXPLORER RECURSIVE POPULATOR ────────────────────────
void populate_gtk_tree_store(TTree *node, GtkTreeIter *parent) {
    if (node == NULL) return;

    GtkTreeIter current_iter;
    char node_display_buffer[250];
    
    snprintf(node_display_buffer, sizeof(node_display_buffer), 
             "Node: %s (%s - %s)", node->name, node->DoB, strlen(node->DoD) > 0 ? node->DoD : "Present");

    gtk_tree_store_append(tree_store, &current_iter, parent);
    gtk_tree_store_set(tree_store, &current_iter, 0, node_display_buffer, -1);

    if (node->left)  populate_gtk_tree_store(node->left, &current_iter);
    if (node->right) populate_gtk_tree_store(node->right, &current_iter);
}

void refresh_visual_tree_explorer() {
    if (!tree_store) return;
    
    gtk_tree_store_clear(tree_store);
    
    if (tree != NULL) {
        populate_gtk_tree_store(tree, NULL);
    } else {
        GtkTreeIter root;
        gtk_tree_store_append(tree_store, &root, NULL);
        gtk_tree_store_set(tree_store, &root, 0, "BST Map Empty. Seed data to build structural nodes.", -1);
    }
    
    if (tree_view_widget) {
        gtk_tree_view_expand_all(GTK_TREE_VIEW(tree_view_widget));
    }
}

// ─── SOULFUL DETAILS CARD UPDATE ENGINE FUNCTION ──────────────────────────
void update_details_card(const char *name, const char *lifespan, const char *status, const char *biography) {
    if (!name_label || !dates_label || !bio_label) return;

    gtk_label_set_text(GTK_LABEL(name_label), name);

    char structured_header[250];
    if (strstr(status, "Located") != NULL || strstr(status, "Success") != NULL) {
        snprintf(structured_header, sizeof(structured_header), 
                 "%s   •   SECURE REGISTRY: ACTIVE PROFILE", lifespan);
    } else {
        snprintf(structured_header, sizeof(structured_header), "%s", lifespan);
    }
    gtk_label_set_text(GTK_LABEL(dates_label), structured_header);

    char soul_bio_buffer[2500];
    if (strlen(biography) > 0 && strcmp(biography, "—") != 0 && strcmp(biography, "") != 0) {
        snprintf(soul_bio_buffer, sizeof(soul_bio_buffer),
                 "HISTORICAL ANTHOLOGY RECORD\n"
                 "═════════════════════════════════════════════════════════════════════\n\n"
                 "“ %s ”\n\n"
                 "═════════════════════════════════════════════════════════════════════\n"
                 "National School of Cybersecurity • ENSC Archive Node, Algiers", 
                 biography);
    } else {
        strncpy(soul_bio_buffer, biography, sizeof(soul_bio_buffer) - 1);
        soul_bio_buffer[sizeof(soul_bio_buffer) - 1] = '\0';
    }
    
    gtk_label_set_text(GTK_LABEL(bio_label), soul_bio_buffer);
}

void refresh_interface_language() {
    if (lbl_sidebar_title) gtk_label_set_text(GTK_LABEL(lbl_sidebar_title), _(TXT_SIDEBAR_TITLE));
    if (main_search_entry) gtk_entry_set_placeholder_text(GTK_ENTRY(main_search_entry), _(TXT_SEARCH_PLACEHOLDER));
    if (btn_view_all_widget) gtk_button_set_label(GTK_BUTTON(btn_view_all_widget), _(TXT_VIEW_ALL_BTN));
    
    if (lbl_mgmt_name) gtk_label_set_text(GTK_LABEL(lbl_mgmt_name), _(TXT_NAME_LBL));
    if (lbl_mgmt_dob) gtk_label_set_text(GTK_LABEL(lbl_mgmt_dob), _(TXT_DOB_LBL));
    if (lbl_mgmt_dod) gtk_label_set_text(GTK_LABEL(lbl_mgmt_dod), _(TXT_DOD_LBL));
    if (lbl_mgmt_bio) gtk_label_set_text(GTK_LABEL(lbl_mgmt_bio), _(TXT_BIO_LBL));
    
    if (btn_add_widget) gtk_button_set_label(GTK_BUTTON(btn_add_widget), _(TXT_INSERT_BTN));
    if (btn_del_widget) gtk_button_set_label(GTK_BUTTON(btn_del_widget), _(TXT_ERASE_BTN));
    if (btn_import_widget) gtk_button_set_label(GTK_BUTTON(btn_import_widget), _(TXT_IMPORT_BTN));
    
    if (lbl_tree_title) gtk_label_set_text(GTK_LABEL(lbl_tree_title), _(TXT_TREE_HEADER));
    if (lbl_theme_title) gtk_label_set_text(GTK_LABEL(lbl_theme_title), _(TXT_THEME_LBL));
    if (lbl_lang_title) gtk_label_set_text(GTK_LABEL(lbl_lang_title), _(TXT_LANG_LBL));

    // FIXED: Dynamically re-label the sidebar menu container tab page titles in real-time
    if (main_content_stack) {
        GList *children = gtk_container_get_children(GTK_CONTAINER(main_content_stack));
        if (g_list_length(children) >= 4) {
            // Index mappings mirror exactly the sequence of stack insertion calls below
            gtk_container_child_set(GTK_CONTAINER(main_content_stack), GTK_WIDGET(g_list_nth_data(children, 0)), "title", _(TXT_SEARCH_TAB), NULL);
            gtk_container_child_set(GTK_CONTAINER(main_content_stack), GTK_WIDGET(g_list_nth_data(children, 1)), "title", _(TXT_MANAGE_TAB), NULL);
            gtk_container_child_set(GTK_CONTAINER(main_content_stack), GTK_WIDGET(g_list_nth_data(children, 2)), "title", _(TXT_TREE_TAB), NULL);
            gtk_container_child_set(GTK_CONTAINER(main_content_stack), GTK_WIDGET(g_list_nth_data(children, 3)), "title", _(TXT_OPTIONS_TAB), NULL);
        }
        g_list_free(children);
    }

    if (current_language == LANG_AR) {
        gtk_widget_set_direction(name_label, GTK_TEXT_DIR_RTL);
        gtk_widget_set_direction(bio_label, GTK_TEXT_DIR_RTL);
    } else {
        gtk_widget_set_direction(name_label, GTK_TEXT_DIR_LTR);
        gtk_widget_set_direction(bio_label, GTK_TEXT_DIR_LTR);
    }
}

void on_language_profile_changed(GtkComboBox *combo, gpointer user_data) {
    current_language = (AppLanguage)gtk_combo_box_get_active(combo);
    refresh_interface_language();
    update_details_card(_(TXT_CARD_INITIAL), "— — —", "System Sync", "");
}

// ─── CALENDAR DATE VALIDATION ENGINE ───────────────────────────────────────
int is_valid_calendar_date(const char *date_str) {
    int day, month, year;
    if (sscanf(date_str, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0; 
    }
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return 0;
    }
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2) {
        int is_leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (is_leap) days_in_month[1] = 29;
    }
    if (day > days_in_month[month - 1]) return 0;
    return 1;
}

// ─── INPUT SANITIZATION FILTER CHARACTERS ──────────────────────────────────
void on_date_entry_insert_text(GtkEditable *editable, const char *new_text, 
                               int new_text_length, int *position, gpointer user_data) {
    for (int i = 0; i < new_text_length; i++) {
        if ((new_text[i] < '0' || new_text[i] > '9') && new_text[i] != '/') {
            g_signal_stop_emission_by_name(editable, "insert-text");
            return;
        }
    }
}

// ─── CARD RENDER VIEW ALL HANDLER ──────────────────────────────────────────
void on_view_all_clicked(GtkButton *button, gpointer user_data) {
    if (gui_record_count == 0) {
        update_details_card(_(TXT_CARD_INITIAL), "— — —", "Empty Matrix", "");
        return;
    }

    char global_list_buffer[4000] = "";
    for (int i = 0; i < gui_record_count; i++) {
        char entry_line[250];
        snprintf(entry_line, sizeof(entry_line), "• %s (Lifespan: %s - %s)\n", 
                 gui_names[i], gui_dobs[i], strlen(gui_dods[i]) > 0 ? gui_dods[i] : "Present");
        strcat(global_list_buffer, entry_line);
    }

    update_details_card("Registry System Index", "Complete Registry View", "Active Node Mapping", global_list_buffer);
}

// ─── CARD RENDER SEARCH ENGINE HANDLER ───────────────────────────────
void on_search_activated(GtkEntry *entry, gpointer user_data) {
    const char *query = gtk_entry_get_text(entry);
    if (strlen(query) == 0) return;

    int found = 0;
    for (int i = 0; i < gui_record_count; i++) {
        if (strcasecmp(gui_names[i], query) == 0 || strncasecmp(gui_names[i], query, strlen(query)) == 0) {
            char lifespan_buffer[100];
            if (strlen(gui_dods[i]) > 0) {
                snprintf(lifespan_buffer, sizeof(lifespan_buffer), "Lifespan: %s — %s", gui_dobs[i], gui_dods[i]);
            } else {
                snprintf(lifespan_buffer, sizeof(lifespan_buffer), "Lifespan: %s — Present", gui_dobs[i]);
            }
            update_details_card(gui_names[i], lifespan_buffer, "Success Located", gui_bios[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        update_details_card("Profile Node Offline", "— — —", "Search Failed", "");
    }
}

// ─── CARD RENDER INSERT DATA HANDLER ───────────────────────────────────────
void on_add_entry_clicked(GtkButton *button, gpointer user_data) {
    const char *name = gtk_entry_get_text(GTK_ENTRY(ent_name));
    const char *dob = gtk_entry_get_text(GTK_ENTRY(ent_dob));
    const char *dod = gtk_entry_get_text(GTK_ENTRY(ent_dod));
    const char *def = gtk_entry_get_text(GTK_ENTRY(ent_def));

    if (strlen(name) == 0 || strlen(dob) == 0) return;

    if (!is_valid_calendar_date(dob) || (strlen(dod) > 0 && !is_valid_calendar_date(dod))) {
        update_details_card("Validation Error", "— — —", "Invalid Calendar Properties", "");
        return;
    }

    s = addPersonality(s, a, (char *)name, (char *)dob, (char *)dod);
    stk = addNameStack(stk, (char *)name, (char *)def, (char *)dob, (char *)dod);
    tree = addNameBST(tree, (char *)name, (char *)dob, (char *)dod);

    if (gui_record_count < MAX_RECORDS) {
        strncpy(gui_names[gui_record_count], name, 99);
        strncpy(gui_dobs[gui_record_count], dob, 19);
        strncpy(gui_dods[gui_record_count], dod, 19);
        strncpy(gui_bios[gui_record_count], def, 499);
        gui_record_count++;
    }

    update_search_completion(); 
    refresh_visual_tree_explorer(); 

    gtk_entry_set_text(GTK_ENTRY(ent_name), "");
    gtk_entry_set_text(GTK_ENTRY(ent_dob), "");
    gtk_entry_set_text(GTK_ENTRY(ent_dod), "");
    gtk_entry_set_text(GTK_ENTRY(ent_def), "");
    
    update_details_card("Success", "— — —", "Success Saved", "Personality profile node locked successfully.");
}
void on_import_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *dialog;
    
    // Create a native GTK File Chooser explorer dialog window
    dialog = gtk_file_chooser_dialog_new(
        current_language == LANG_AR ? "اختر ملف البيانات" : 
        (current_language == LANG_FR ? "Sélectionner le fichier" : "Select Personality Data File"),
        GTK_WINDOW(window),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        current_language == LANG_AR ? "إلغاء" : (current_language == LANG_FR ? "Annuler" : "Cancel"), GTK_RESPONSE_CANCEL,
        current_language == LANG_AR ? "افتح" : (current_language == LANG_FR ? "Ouvrir" : "Open"), GTK_RESPONSE_ACCEPT,
        NULL
    );

    // Filter to only show .txt files in the selection browser window
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_filter_set_name(filter, "Text Files (*.txt)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        FILE *file = fopen(filename, "r");
        
        if (!file) {
            update_details_card("Import Error", "— — —", "File Link Offline", "");
            g_free(filename);
            gtk_widget_destroy(dialog);
            return;
        }

        char line[1024];
        int imported_count = 0;

        while (fgets(line, sizeof(line), file) && gui_record_count < MAX_RECORDS) {
            line[strcspn(line, "\r\n")] = '\0';
            if (strlen(line) < 5) continue;

            char *name = strtok(line, ";");
            char *dob = strtok(NULL, ";");
            char *dod = strtok(NULL, ";");
            char *bio = strtok(NULL, ";");

            if (name && dob && bio) {
                if (!is_valid_calendar_date(dob)) continue;
                if (dod && strlen(dod) > 0 && !is_valid_calendar_date(dod)) continue;

                char clear_dod[20] = "";
                if (dod) strncpy(clear_dod, dod, 19);

                s = addPersonality(s, a, name, dob, clear_dod);
                stk = addNameStack(stk, name, bio, dob, clear_dod);
                tree = addNameBST(tree, name, dob, clear_dod); 

                strncpy(gui_names[gui_record_count], name, 99);
                strncpy(gui_dobs[gui_record_count], dob, 19);
                strncpy(gui_dods[gui_record_count], clear_dod, 19);
                strncpy(gui_bios[gui_record_count], bio, 499);
                gui_record_count++;
                imported_count++;
            }
        }
        fclose(file);
        g_free(filename);

        update_search_completion(); 
        refresh_visual_tree_explorer(); 

        char alert_msg[200];
        if (current_language == LANG_AR) {
            snprintf(alert_msg, sizeof(alert_msg), "تم استيراد %d سجلات بنجاح إلى قاعدة البيانات.", imported_count);
            update_details_card("نجاح الاستيراد", "اكتمل تحميل الملف", "مزامنة ناجحة", alert_msg);
        } else if (current_language == LANG_FR) {
            snprintf(alert_msg, sizeof(alert_msg), "%d profils historiques importés avec succès.", imported_count);
            update_details_card("Import Réussi", "Chargement Terminé", "Sync Succès", alert_msg);
        } else {
            snprintf(alert_msg, sizeof(alert_msg), "Successfully synchronized %d national archive nodes from selected file.", imported_count);
            update_details_card("Import Success", "Database Seeding Complete", "Success Sync", alert_msg);
        }
    }
    
    gtk_widget_destroy(dialog);
}
// ─── ERASE RECORD CALLBACK PLACEHOLDER ─────────────────────────────────────
void on_delete_entry_clicked(GtkButton *button, gpointer user_data) {
    update_details_card("Erase Record", "— — —", "Pending Operation", "");
}

// ─── THEME CHANGER PROFILE SWITCHER ─────────────────────────────────
void on_theme_profile_changed(GtkComboBox *combo, gpointer user_data) {
    gint active_index = gtk_combo_box_get_active(combo);
    if (active_index == 0) {
        apply_theme("style.css");
    } else if (active_index == 1) {
        apply_theme("dark_style.css");
    }
}

// ─── MAIN GRAPHICAL INTERFACE SETUP ENGINE ─────────────────────────────────
void init_algeria_database_gui(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "History of Algeria Database Management System");
    gtk_window_set_default_size(GTK_WINDOW(window), 980, 680);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    apply_theme("style.css");

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(sidebar), "sidebar");
    gtk_widget_set_size_request(sidebar, 240, -1);
    gtk_box_pack_start(GTK_BOX(main_box), sidebar, FALSE, FALSE, 0);

    lbl_sidebar_title = gtk_label_new(_(TXT_SIDEBAR_TITLE));
    gtk_widget_set_name(lbl_sidebar_title, "sidebar-title");
    gtk_box_pack_start(GTK_BOX(sidebar), lbl_sidebar_title, FALSE, FALSE, 15);

    GtkWidget *content_stack = gtk_stack_new();
    main_content_stack = content_stack;
    gtk_box_pack_start(GTK_BOX(main_box), content_stack, TRUE, TRUE, 0);

    GtkWidget *stack_sidebar = gtk_stack_sidebar_new();
    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(stack_sidebar), GTK_STACK(content_stack));
    gtk_box_pack_start(GTK_BOX(sidebar), stack_sidebar, TRUE, TRUE, 0);

    // ─── PANEL 1: SEARCH ENGINE DASHBOARD ───
    GtkWidget *search_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(search_view), 25);
    gtk_stack_add_titled(GTK_STACK(content_stack), search_view, "SearchPanel", _(TXT_SEARCH_TAB));
    GtkWidget *search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    main_search_entry = gtk_search_entry_new(); 
    gtk_entry_set_placeholder_text(GTK_ENTRY(main_search_entry), _(TXT_SEARCH_PLACEHOLDER));
    g_signal_connect(main_search_entry, "activate", G_CALLBACK(on_search_activated), NULL);
    gtk_box_pack_start(GTK_BOX(search_box), main_search_entry, TRUE, TRUE, 0);

    btn_view_all_widget = gtk_button_new_with_label(_(TXT_VIEW_ALL_BTN));
    g_signal_connect(btn_view_all_widget, "clicked", G_CALLBACK(on_view_all_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(search_box), btn_view_all_widget, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(search_view), search_box, FALSE, FALSE, 0);

    GtkWidget *details_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_style_context_add_class(gtk_widget_get_style_context(details_card), "card-view");
    gtk_container_set_border_width(GTK_CONTAINER(details_card), 20);
    
    name_label = gtk_label_new(_(TXT_CARD_INITIAL));
    gtk_widget_set_name(name_label, "details-title");
    gtk_label_set_xalign(GTK_LABEL(name_label), 0.0);

    dates_label = gtk_label_new("");
    gtk_widget_set_name(dates_label, "details-subtitle");
    gtk_label_set_xalign(GTK_LABEL(dates_label), 0.0);

    bio_label = gtk_label_new("");
    gtk_widget_set_name(bio_label, "details-narrative");
    gtk_label_set_xalign(GTK_LABEL(bio_label), 0.0);
    gtk_label_set_justify(GTK_LABEL(bio_label), GTK_JUSTIFY_FILL); 
    gtk_label_set_line_wrap(GTK_LABEL(bio_label), TRUE);
    gtk_label_set_line_wrap_mode(GTK_LABEL(bio_label), PANGO_WRAP_WORD);

    gtk_box_pack_start(GTK_BOX(details_card), name_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(details_card), dates_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(details_card), bio_label, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(search_view), details_card, TRUE, TRUE, 10);

    // ─── PANEL 2: MANAGE SYSTEM DATABASE ───
    GtkWidget *mgmt_view = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(mgmt_view), 15);
    gtk_grid_set_column_spacing(GTK_GRID(mgmt_view), 15);
    gtk_container_set_border_width(GTK_CONTAINER(mgmt_view), 30);
    gtk_stack_add_titled(GTK_STACK(content_stack), mgmt_view, "MgmtPanel", _(TXT_MANAGE_TAB));
    lbl_mgmt_name = gtk_label_new(_(TXT_NAME_LBL));
    ent_name = gtk_entry_new();

    lbl_mgmt_dob = gtk_label_new(_(TXT_DOB_LBL));
    ent_dob = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(ent_dob), 10); 
    gtk_entry_set_placeholder_text(GTK_ENTRY(ent_dob), "DD/MM/YYYY");
    g_signal_connect(ent_dob, "insert-text", G_CALLBACK(on_date_entry_insert_text), NULL);

    lbl_mgmt_dod = gtk_label_new(_(TXT_DOD_LBL));
    ent_dod = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(ent_dod), 10);
    gtk_entry_set_placeholder_text(GTK_ENTRY(ent_dod), "DD/MM/YYYY");
    g_signal_connect(ent_dod, "insert-text", G_CALLBACK(on_date_entry_insert_text), NULL);

    lbl_mgmt_bio = gtk_label_new(_(TXT_BIO_LBL));
    ent_def = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(mgmt_view), lbl_mgmt_name, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), ent_name, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), lbl_mgmt_dob, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), ent_dob, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), lbl_mgmt_dod, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), ent_dod, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), lbl_mgmt_bio, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), ent_def, 1, 3, 2, 1);

    btn_add_widget = gtk_button_new_with_label(_(TXT_INSERT_BTN));
    btn_del_widget = gtk_button_new_with_label(_(TXT_ERASE_BTN));
    btn_import_widget = gtk_button_new_with_label(_(TXT_IMPORT_BTN));

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_add_widget), "btn-success");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_del_widget), "btn-danger");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_import_widget), "btn-primary");

    g_signal_connect(btn_add_widget, "clicked", G_CALLBACK(on_add_entry_clicked), NULL);
    g_signal_connect(btn_del_widget, "clicked", G_CALLBACK(on_delete_entry_clicked), NULL);
    g_signal_connect(btn_import_widget, "clicked", G_CALLBACK(on_import_clicked), NULL);

    gtk_grid_attach(GTK_GRID(mgmt_view), btn_add_widget, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), btn_del_widget, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(mgmt_view), btn_import_widget, 2, 4, 1, 1);

    // ─── PANEL 3: NATIVE BST TREE EXPLORER VIEW ────────────────────────────
    GtkWidget *tree_panel_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(tree_panel_box), 20);
    gtk_stack_add_titled(GTK_STACK(content_stack), tree_panel_box, "TreePanel", _(TXT_TREE_TAB));
    lbl_tree_title = gtk_label_new(_(TXT_TREE_HEADER));
    gtk_widget_set_name(lbl_tree_title, "settings-header");
    gtk_label_set_xalign(GTK_LABEL(lbl_tree_title), 0.0);
    gtk_box_pack_start(GTK_BOX(tree_panel_box), lbl_tree_title, FALSE, FALSE, 5);

    GtkWidget *tree_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(tree_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tree_panel_box), tree_scrolled_window, TRUE, TRUE, 0);

    tree_store = gtk_tree_store_new(1, G_TYPE_STRING);
    tree_view_widget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_store));
    g_object_unref(tree_store);

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Hierarchical Memory Mapping Nodes", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view_widget), column);
    gtk_container_add(GTK_CONTAINER(tree_scrolled_window), tree_view_widget);

    refresh_visual_tree_explorer();

    // ─── PANEL 4: APP OPTIONS & THEMES ───
    GtkWidget *settings_view = gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
    gtk_container_set_border_width(GTK_CONTAINER(settings_view), 35);
    gtk_stack_add_titled(GTK_STACK(content_stack), settings_view, "SettingsPanel", _(TXT_OPTIONS_TAB));
    GtkWidget *settings_title = gtk_label_new("System Configuration");
    gtk_widget_set_name(settings_title, "settings-header");
    gtk_box_pack_start(GTK_BOX(settings_view), settings_title, FALSE, FALSE, 0);

    // Sub-Layout Section A: Visual Color Schemes
    GtkWidget *theme_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    lbl_theme_title = gtk_label_new(_(TXT_THEME_LBL));
    GtkWidget *theme_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(theme_combo), "Algerian National Light");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(theme_combo), "Sahara Night Dark");
    gtk_combo_box_set_active(GTK_COMBO_BOX(theme_combo), 0); 
    g_signal_connect(theme_combo, "changed", G_CALLBACK(on_theme_profile_changed), NULL);

    gtk_box_pack_start(GTK_BOX(theme_box), lbl_theme_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theme_box), theme_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_view), theme_box, FALSE, FALSE, 10);

    // Sub-Layout Section B: Language Localizations Selector
    GtkWidget *lang_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    lbl_lang_title = gtk_label_new(_(TXT_LANG_LBL));
    GtkWidget *lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "English");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "العربية (Arabic)");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(lang_combo), "Français (French)");
    gtk_combo_box_set_active(GTK_COMBO_BOX(lang_combo), 0);
    g_signal_connect(lang_combo, "changed", G_CALLBACK(on_language_profile_changed), NULL);

    gtk_box_pack_start(GTK_BOX(lang_box), lbl_lang_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(lang_box), lang_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_view), lang_box, FALSE, FALSE, 10);

    // Initialize core structural configurations
    update_search_completion();

    gtk_widget_show_all(window);
    gtk_main();
}

// ─── THEME LAYOUT PROVIDER RENDER DEFINITION ──────────────────────────────
void apply_theme(const char *css_file_name) {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    GdkScreen *screen = gdk_screen_get_default();
    if (gtk_css_provider_load_from_path(css_provider, css_file_name, NULL)) {
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    g_object_unref(css_provider);
}