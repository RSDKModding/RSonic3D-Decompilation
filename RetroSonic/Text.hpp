#ifndef TEXTSYSTEM_H
#define TEXTSYSTEM_H

struct TextMenuEntry {
    char *text;
    int length;
};

struct TextMenu {
    TextMenuEntry *labels;
    int *selMode;
    int rowCount;
    byte field_C;
    byte field_D;
    int field_10;
    int field_14;
};

void TextMeDo(const char *text, TextMenu *menu, int rowCount, int rowID, int length);
void SetGameText(const char *text, TextMenu *menu, int rowID, int length);

#endif