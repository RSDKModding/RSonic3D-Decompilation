#ifndef TEXTSYSTEM_H
#define TEXTSYSTEM_H

enum TextMenuAlignments {
    MENU_ALIGN_LEFT,
    MENU_ALIGN_RIGHT,
    MENU_ALIGN_CENTER,
};

struct TextMenuEntry {
    char *text;
    int length;
};

struct TextMenu {
    TextMenuEntry *labels;
    int *selMode;
    int rowCount;
    byte alignment;
    byte selectionCount;
    int selection1;
    int selection2;
};

void TextMeDo(const char *text, TextMenu *menu, int rowNo, int rowID, int length);
void SetGameText(const char *text, TextMenu *menu, int rowID, int length);

void DrawMenuText(const char *text, int length, int xpos, int ypos, byte sprite);
void DrawMenuTextFaded(const char *text, int length, int xpos, int ypos);
void DrawMenuTextClipped(const char *text, int length, int xpos, int ypos, byte sprite, byte alphaStrengthA, byte alphaStrengthB);

#endif