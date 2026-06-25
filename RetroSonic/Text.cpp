#include "RetroEngine.hpp"

void TextMeDo(const char *text, TextMenu *menu, int rowCount, int rowID, int length)
{
    menu->rowCount = rowCount;
    menu->labels   = new TextMenuEntry[menu->rowCount];
    menu->selMode  = new int[menu->rowCount];

    SetGameText(text, menu, rowID, length);
}

void SetGameText(const char *text, TextMenu *menu, int rowID, int length)
{
    TextMenuEntry *entry = &menu->labels[rowID];

    entry->text   = new char[length];
    entry->length = length;

    for (int i = 0; i < length; ++i) {
        entry->text[i] = text[i];
        if (entry->text[i] == ' ')
            entry->text[i] = '\0';

        if (entry->text[i] > '/' && entry->text[i] < ':')
            entry->text[i] -= '\x15';

        if (entry->text[i] > '9' && entry->text[i] < 'f')
            entry->text[i] -= '@';
    }
}