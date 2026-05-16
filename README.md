# Algerian Personalities Database Management System
### Ecole Nationale Supérieure en Cybersécurité (ENSC) — Advanced Data Structures Project

An administrative desktop management system designed to archive and explore the legacies of Algerian national heroes. This application bridges complex low-level data structures with a localized graphical user interface (GUI) powered by GTK+ 3.0.

---

## Core Engineering Features

* **Multilingual Translation Engine:** Real-time interface toggling between English, Arabic, and French. Switching to Arabic automatically updates the interface layout to native Right-to-Left (RTL) text alignment.
* **Live Search Auto-Suggestions:** Utilizes a native `GtkEntryCompletion` system to intercept keyboard inputs and display instant query hints in a drop-down list.
* **Recursive BST Memory Mapping Explorer:** Features an interactive `GtkTreeView` dashboard that recursively traverses active Binary Search Tree (BST) allocations to visualize parent-child node relationships and tree balancing.
* **Native File Chooser Engine:** Integrates a `GtkFileChooserDialog` with customized `.txt` filters to dynamically browse, select, and parse custom data files on runtime.
* **Dual Asset Color Themes:** * `style.css` (National Light): Clean white canvas base paired with Algerian Deep Green headers and a sharp Crimson Red sidebar separation border.
    * `dark_style.css` (Sahara Desert Night): Matte charcoal background featuring rich Golden Sand dune accents and silver text blocks.

---

## Underlying Data Structures Architecture

The backend engine coordinates data across three distinct memory structures to balance search efficiency and history tracking:
1.  **Linked List (`TList`):** Manages primary sequential metadata definitions and baseline profile configurations.
2.  **Stack Grid (`TStack`):** Handles narrative biography string blocks, text limits, and temporal tracking logs.
3.  **Binary Search Tree (`TTree`):** Structures historical personality names alphabetically to optimize searches to logarithmic time complexity ($O(\log n)$).

---

## File Input Format Specifications

Source text files (`.txt`) must utilize semicolons (`;`) as delimiters. Missing death dates for living personalities should remain empty between the designated delimiters (`;;`).

### Format Template
```text
Name;Date_of_Birth;Date_of_Death;Detailed_Biography
