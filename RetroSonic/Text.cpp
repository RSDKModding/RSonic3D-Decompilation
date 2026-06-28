#include "RetroEngine.hpp"

void TextMeDo(const char *text, TextMenu *menu, int rowNo, int rowID, int length)
{
    menu->rowCount = rowNo;
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

void DrawMenuText(const char *text, int length, int xpos, int ypos, byte sprite)
{
    D3DLVERTEX vertices[20][4];
    ushort indices[20][6];

    ZeroMemory(vertices, sizeof(vertices));

    D3DCOLOR color = (sprite == 8) ? 0xFFFFFF00 : 0xBFFFFFFF;

    for (int i = 0; i < length; ++i) {
        float texUTL = 0.0f;
        float texVTL = 0.0f;
        float texUBR = 0.0f;
        float texVBR = 0.0f;

        if (text[i] != '\0') {
            texUTL = (text[i] - 1) % 6;
            texVTL = (text[i] - 1) / 6;
            texUTL *= (42.0f / 256.0f);
            texVTL *= (42.0f / 256.0f);

            texUBR = texUTL;
            texVBR = texVTL;
            texUBR += (42.0f / 256.0f);
            texVBR += (42.0f / 256.0f);
        }

        float x = (xpos - 160) + (10 * i);
        float y = (120 - ypos);

        x /= SCREEN_XCENTER_F;
        y /= SCREEN_YCENTER_F;

        vertices[i][0] = { { x + 0.00f, y - 0.00f, 1.0f }, color, 0, texUTL, texVTL };
        vertices[i][1] = { { x + 0.08f, y - 0.00f, 1.0f }, color, 0, texUBR, texVTL };
        vertices[i][2] = { { x + 0.00f, y - 0.08f, 1.0f }, color, 0, texUTL, texVBR };
        vertices[i][3] = { { x + 0.08f, y - 0.08f, 1.0f }, color, 0, texUBR, texVBR };

        indices[i][0] = (4 * i) + 0;
        indices[i][1] = (4 * i) + 1;
        indices[i][2] = (4 * i) + 2;
        indices[i][3] = (4 * i) + 1;
        indices[i][4] = (4 * i) + 3;
        indices[i][5] = (4 * i) + 2;
    }

    D3DDevice->BeginScene();
    D3DDevice->SetTexture(0, surfaceMText);
    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices[0], 4 * length, indices[0], 6 * length, 0);
    D3DDevice->EndScene();
}

void DrawText_2(const char *text, int length, int xpos, int ypos)
{
    D3DLVERTEX vertices[20][4];
    ushort indices[20][6];

    ZeroMemory(vertices, sizeof(vertices));

    for (int i = 0; i < length; ++i) {
        float texUTL = 0.0f;
        float texVTL = 0.0f;
        float texUBR = 0.0f;
        float texVBR = 0.0f;

        if (text[i] != '\0') {
            texUTL = (text[i] - 1) % 6;
            texVTL = (text[i] - 1) / 6;
            texUTL *= (42.0f / 256.0f);
            texVTL *= (42.0f / 256.0f);

            texUBR = texUTL;
            texVBR = texVTL;
            texUBR += (42.0f / 256.0f);
            texVBR += (42.0f / 256.0f);
        }

        float x = (xpos - 160) + (10 * i);
        float y = (120 - ypos);

        x /= SCREEN_XCENTER_F;
        y /= SCREEN_YCENTER_F;

        if (i == length - 1) {
            vertices[i][0] = { { x + 0.00f, y - 0.00f, 1.0f }, 0xBFFFFFFF, 0, texUTL, texVTL };
            vertices[i][1] = { { x + 0.08f, y - 0.00f, 1.0f }, 0xBFFFFFFF, 0, texUBR, texVTL };
            vertices[i][2] = { { x + 0.00f, y - 0.08f, 1.0f }, 0xBFFFFFFF, 0, texUTL, texVBR };
            vertices[i][3] = { { x + 0.08f, y - 0.08f, 1.0f }, 0xBFFFFFFF, 0, texUBR, texVBR };
        }
        else {
            vertices[i][0] = { { x + 0.00f, y - 0.00f, 1.0f }, 0xFFFFFFFF, 0, texUTL, texVTL };
            vertices[i][1] = { { x + 0.08f, y - 0.00f, 1.0f }, 0xFFFFFFFF, 0, texUBR, texVTL };
            vertices[i][2] = { { x + 0.00f, y - 0.08f, 1.0f }, 0xFFFFFFFF, 0, texUTL, texVBR };
            vertices[i][3] = { { x + 0.08f, y - 0.08f, 1.0f }, 0xFFFFFFFF, 0, texUBR, texVBR };
        }

        indices[i][0] = (4 * i) + 0;
        indices[i][1] = (4 * i) + 1;
        indices[i][2] = (4 * i) + 2;
        indices[i][3] = (4 * i) + 1;
        indices[i][4] = (4 * i) + 3;
        indices[i][5] = (4 * i) + 2;
    }

    D3DDevice->BeginScene();
    D3DDevice->SetTexture(0, surfaceMText);
    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices[0], 4 * length, indices[0], 6 * length, 0);
    D3DDevice->EndScene();
}

void DrawText_3(const char *text, int length, int xpos, int ypos, byte sprite, byte alphaStrengthT, byte alphaStrengthB)
{
    D3DLVERTEX vertices[20][4];
    ushort indices[20][6];

    ZeroMemory(vertices, sizeof(vertices));

    if (alphaStrengthT >= 0x10 || alphaStrengthB >= 0x10)
        return;

    D3DCOLOR colorT = 0xFFFFFFFF;
    D3DCOLOR alphaT = 0x00;

    D3DCOLOR colorB = 0xFFFFFFFF;
    D3DCOLOR alphaB = 0x00;

    if (alphaStrengthT <= 8 && alphaStrengthB <= 8) {
        alphaT = 0x08 - alphaStrengthT;
        alphaB = 0x08 - alphaStrengthB;
    }
    else if (alphaStrengthT <= 8) {
        alphaT = 0x10 - alphaStrengthB;
        alphaB = 0x00;
    }
    else {
        alphaT = 0x00;
        alphaB = 0x10 - alphaStrengthT;
    }

    alphaT = (alphaT * 255) / 16;
    alphaB = (alphaB * 255) / 16;

    colorT = TO_ARGB(alphaT, 255, 255, 255);
    colorB = TO_ARGB(alphaB, 255, 255, 255);

    for (int i = 0; i < length; ++i) {
        float texUTL = 0.0f;
        float texVTL = 0.0f;
        float texUBR = 0.0f;
        float texVBR = 0.0f;

        if (text[i] != '\0') {
            texUTL = (text[i] - 1) % 6;
            texVTL = (text[i] - 1) / 6;
            texUTL *= (42.0f / 256.0f);
            texVTL *= (42.0f / 256.0f);

            texUBR = texUTL;
            texVBR = texVTL;
            texUBR += (42.0f / 256.0f);
            texVBR += (42.0f / 256.0f);
        }

        float x = (xpos - 160) + (10 * i);
        float y = (120 - ypos);

        x /= SCREEN_XCENTER_F;
        y /= SCREEN_YCENTER_F;

        vertices[i][0] = { { x + 0.00f, y - 0.00f, 1.0f }, colorT, 0, texUTL, texVTL };
        vertices[i][1] = { { x + 0.08f, y - 0.00f, 1.0f }, colorT, 0, texUBR, texVTL };
        vertices[i][2] = { { x + 0.00f, y - 0.08f, 1.0f }, colorB, 0, texUTL, texVBR };
        vertices[i][3] = { { x + 0.08f, y - 0.08f, 1.0f }, colorB, 0, texUBR, texVBR };

        indices[i][0] = (4 * i) + 0;
        indices[i][1] = (4 * i) + 1;
        indices[i][2] = (4 * i) + 2;
        indices[i][3] = (4 * i) + 1;
        indices[i][4] = (4 * i) + 3;
        indices[i][5] = (4 * i) + 2;
    }

    D3DDevice->BeginScene();
    D3DDevice->SetTexture(0, surfaceMText);
    D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vertices[0], 4 * length, indices[0], 6 * length, 0);
    D3DDevice->EndScene();
}