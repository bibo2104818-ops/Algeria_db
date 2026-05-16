Algerian Personalities Database Management System
Ecole Nationale Supérieure en Cybersécurité (ENSC) — Advanced Data Structures Project
An administrative, high-performance C-based desktop application designed to archive, index, and query the profiles of Algerian national heroes. This system couples decoupled core data structures with an asymmetric, localized graphical interface powered by the GTK+ 3.0 object framework.

Technical Architecture & Design Implementation
Asymmetric Translation & RTL Layout Redirection
The application isolates system components from translation strings via a multi-dimensional array lookup macro mapping token identifiers against active language offsets.

C
#define _(key) translations[current_language][key]
When toggled to LANG_AR, the translation engine triggers runtime layout direction updates using the gtk_widget_set_direction interface. This forces an immediate rewrite of the GTK widget tree properties, reconfiguring structural entry box widgets and Pango layouts to use Right-to-Left (RTL) text flows without state drops or binary restarts.

Signal Inversion & Event Filtering
Text-entry boundaries implement proactive validation mechanisms to block malformed inputs. By connecting custom handlers to the insert-text signal on GtkEditable widgets, keystroke events are filtered before standard internal buffer allocation occurs:

C
g_signal_connect(G_OBJECT(ent_dob), "insert-text", G_CALLBACK(on_date_entry_insert_text), NULL);
If an input violates the timestamp signature [0-9/], the handler suppresses the character injection by calling g_signal_stop_emission_by_name, protecting memory blocks from parsing crashes.

Automated UI Autocomplete Hooks
Live query hinting leverages the native GtkEntryCompletion module coupled to a dedicated GtkListStore model. When the local memory array layout updates via entry updates or text file deserialization, update_search_completion() runs a clean buffer cycle:

Instantiates a temporary GtkListStore matrix populated with cache pointers.

Binds the data table to the search controller using text-column indices.

Triggers inline match lookups and automated drop-down popups directly against user string prefix sequences.

Recursive Memory Mapping & Visual BST Introspection
To inspect live node configurations, the system implements an interactive GtkTreeView element tied to an underlying GtkTreeStore model topology. The dashboard utilizes a tailored pointer-traversal routine to map binary node offsets into visual graphical representations:

C
void populate_gtk_tree_store(TTree *node, GtkTreeIter *parent);
The tree explorer steps through active memory branches using standard left/right leaf recursive iterations. Each visited structural pointer generates a child iteration token (GtkTreeIter) nested inside its parent tree node layout. This enables live visualization of leaf balances, node structures, and allocation depth properties directly from active heap space.

Underlying Data Structures Architecture
The execution layer structures profiles concurrently across three distinct abstract data allocation formats to balance algorithmic lookup efficiency, operational safety, and historical serialization capabilities:

Linked List (TList): Serves as the primary data repository for individual records, maintaining sequential metadata definitions and lifespan bounds.

Stack Grid (TStack): Tracks biography strings and change tracking records, using a Last-In, First-Out (LIFO) alignment pattern to preserve historical logs.

Binary Search Tree (TTree): Optimizes lookup performance by organizing personality names alphabetically. By maintaining a structured tree topology, search complexities avoid linear degradation O(n) and achieve logarithmic performance guarantees O(logn).

File Input Parsing Specifications
The filesystem module reads unstructured raw flat files (.txt) sequentially using a semicolon-delimited token parser. Extracted string tokens are sanitized of carriage returns (\r\n) and run through a validation algorithm before data structures accept them:

Validates month ranges against a standard calendar matrix [1-12].

Validates daily upper bounds against month configurations.

Handles leap year constraints on February values dynamically.

Parsing Signature Matrix
Plaintext
Name;Date_of_Birth;Date_of_Death;Detailed_Biography
Raw Buffer Mapping Vector Example
Plaintext
Larbi Ben M'hidi;26/01/1923;04/03/1957;Prominent FLN strategist and member of the CRUA who directed the autonomous zone during the Battle of Algiers.
Djamila Bouhired;04/06/1935;;Nationalist militant whose arrest and trial galvanized international public opinion.
Note: For living personalities, the structural death boundary must remain completely empty between the specified semicolon delimiters (;;) to notify the parser to assign a present active layout status pointer.

Compilation & Subsystem Deployment Guide
Environment Configuration
Ensure your build station (Ubuntu or Zorin OS development environments) has the GNU Compiler Collection (GCC) toolchain and the proper GTK+ 3.0 development shared library objects installed:

Bash
sudo apt update
sudo apt install build-essential libgtk-3-dev
Technical Build Command
Compile all decoupled code segments concurrently while passing flags for GTK configuration and enabling the GUI preprocessor mode switch:

Bash
gcc -DGUI_MODE main.c gui.c stack.c recursion.c list.c binary_tree.c -o algeria_history_app `pkg-config --cflags --libs gtk+-3.0`
Launching the Application
Execute the resulting binary target file from your terminal layout workspace:

Bash
./algeria_history_app
Academic Verification
Developed as an Advanced Data Structures (ALDDS) semester project at the National School of Cybersecurity (ENSC), Algiers, Algeria. 🇩🇿

ENSC Node Safety Status: COMPILED & LOCKED SECURE
