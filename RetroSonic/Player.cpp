#include "RetroEngine.hpp"

Matrix3D MatrixObject;
Matrix3D MatrixSonicNodeRotation[36];
Matrix3D MatrixSonicNodeTransform[36];

float PlayerTargetRotationZ;
float PlayerTargetRotationX;

float PlayerRotationZ;
float PlayerRotationX;

float PlayerPrevRotationZ;
float PlayerPrevRotationX;

float PlayerJumpRotationX;

int PlayerRotationTimerX;
int PlayerRotationTimerZ;

byte PNumber;
PlayerObject Player[2];

#if RETRO_USE_ORIGINAL_CODE
void LoadPlayerGfx(const char *textureName, sbyte characterID)
{
    char name[64];
    lstrcpy(name, "Data/Characters/");
    lstrcat(name, textureName);

    if (CharacterTexture[characterID] != NULL)
        CharacterTexture[characterID]->Release();

    CharacterTexture[characterID] = (Texture *)(CreateTexture(D3DDevice, name));

    if (characterID == CHARACTER_SONIC_TAILS)
        DDLoadBitmap(name, 0);
}
#endif

void ProcessPlayerInput()
{
    int pressed = 0;

    PlayerObject *player  = &Player[PNumber];
    PlayerObject *player1 = &Player[0];

    player->up        = false;
    player->jumpPress = false;

    if (!player->disableControl) {
        player->targetAngle = 0;

        CheckInput(&MGameInput);

        if (Debug) {
            if (MGameInput.left == true) {
                player1->position.x -= Cos(CameraRotateY);
                CameraPosition.x -= Cos(CameraRotateY);

                player1->position.z -= Sin(CameraRotateY);
                CameraPosition.z -= Sin(CameraRotateY);
            }

            if (MGameInput.right == true) {
                player1->position.x += Cos(CameraRotateY);
                CameraPosition.x += Cos(CameraRotateY);

                player1->position.z += Sin(CameraRotateY);
                CameraPosition.z += Sin(CameraRotateY);
            }

            if (MGameInput.Z) {
                if (MGameInput.up == true) {
                    player1->position.y += 1.0f;
                    CameraPosition.y += 1.2f;
                }

                if (MGameInput.down == true) {
                    player1->position.y -= 1.0f;
                    CameraPosition.y -= 1.2f;
                }
            }
            else {
                if (MGameInput.up == true) {
                    player1->position.x -= Cos(CameraRotateY - RSDK_PI_H);
                    CameraPosition.x -= Cos(CameraRotateY - RSDK_PI_H);

                    player1->position.z -= Sin(CameraRotateY - RSDK_PI_H);
                    CameraPosition.z -= Sin(CameraRotateY - RSDK_PI_H);
                }

                if (MGameInput.down == true) {
                    player1->position.x += Cos(CameraRotateY - RSDK_PI_H);
                    CameraPosition.x += Cos(CameraRotateY - RSDK_PI_H);

                    player1->position.z += Sin(CameraRotateY - RSDK_PI_H);
                    CameraPosition.z += Sin(CameraRotateY - RSDK_PI_H);
                }
            }

            CheckKeyPress(&MGameInput, INPUT_START, INPUT_ONCE);

            if (MGameInput.control == true)
                CreateObject(OBJ_TYPE_SPRING, 0, player1->position.x, player1->position.y + 1.8f, player1->position.z);

            if (MGameInput.X == true)
                CreateObject(OBJ_TYPE_RING, 0, player1->position.x, player1->position.y + 4.0f, player1->position.z);
        }
        else {
            if (MGameInput.left == true) {
                player->targetAngle += 64;
                if (player->angle > 192)
                    player->angle -= 256;

                ++pressed;
            }

            if (MGameInput.right == true) {
                if (player->angle < 64)
                    player->angle += 256;
                player->targetAngle += 192;

                ++pressed;
            }

            if (MGameInput.up == true) {
                if (player->angle > 128)
                    player->targetAngle += 256;

                ++pressed;
            }

            if (MGameInput.down == true) {
                player->targetAngle += 128;

                ++pressed;
            }

            if (pressed > 0) {
                player->up = true;
                player->targetAngle /= pressed;
            }

            player->z = MGameInput.Z == true;
            CheckKeyPress(&MGameInput, INPUT_START, INPUT_Z);
        }

        if (MGameInput.control == true)
            player->jumpPress = true;

        if (MGameInput.shift == true && DebugEn == true)
            Debug ^= true;
    }

    CheckInput(&MGameInput);
}

void ProcessPlayerCamera()
{
    PlayerObject *player = &Player[PNumber];

    if (MGameInput.Z == true) {
        CameraPosition.x -= player->position.x;
        CameraPosition.z -= player->position.z;

        float x = CameraPosition.x;
        float z = CameraPosition.z;

        CameraPosition.x = Cos(0.02f) * x;
        CameraPosition.z = -Sin(0.02f) * x;

        CameraPosition.x += Sin(0.02f) * z;
        CameraPosition.z += Cos(0.02f) * z;

        CameraPosition.x += player->position.x;
        CameraPosition.z += player->position.z;
    }

    if (MGameInput.X == true) {
        CameraPosition.x -= player->position.x;
        CameraPosition.z -= player->position.z;

        float x = CameraPosition.x;
        float z = CameraPosition.z;

        CameraPosition.x = Cos(-0.02f) * x;
        CameraPosition.z = -Sin(-0.02f) * x;

        CameraPosition.x += Sin(-0.02f) * z;
        CameraPosition.z += Cos(-0.02f) * z;

        CameraPosition.x += player->position.x;
        CameraPosition.z += player->position.z;
    }

    if (CameraPosition.x == player->position.x) {
        CameraRotateY = 0.0f;
        if (CameraPosition.z >= player->position.z)
            CameraRotateY = RSDK_PI;
    }
    else {
        if (CameraPosition.x <= player->position.x)
            CameraRotateY = ATan((CameraPosition.z - player->position.z) / (CameraPosition.x - player->position.x)) - (RSDK_PI * 0.5f);
        else
            CameraRotateY = ATan((CameraPosition.z - player->position.z) / (CameraPosition.x - player->position.x)) + (RSDK_PI * 0.5f);
    }

    if (Fabs((player->position.x - CameraPosition.x) / Sin(CameraRotateY)) < 32.0f) {
        CameraPosition.x = player->position.x - Sin(CameraRotateY) * -32.0f;
        CameraPosition.z = Cos(CameraRotateY) * -32.0f + player->position.z;
    }

    if (Fabs((player->position.x - CameraPosition.x) / Sin(CameraRotateY)) > 60.0f) {
        CameraPosition.x = player->position.x - Sin(CameraRotateY) * -60.0f;
        CameraPosition.z = Cos(CameraRotateY) * -60.0f + player->position.z;
    }
}

void ProcessPlayerMovement()
{
    PlayerObject *player = &Player[PNumber];

    if (player->state == STATE_AIR && player->up == true) {
        if (player->angle < player->targetAngle) {
            player->angle += 6;

            if (player->speed > 0.0f)
                player->speed -= 0.02f;

            if (player->angle > player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->angle > player->targetAngle) {
            player->angle -= 6;

            if (player->speed > 0.0f)
                player->speed -= 0.02;

            if (player->angle < player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->speed < 2.4f)
            player->speed += 0.012f;
    }
    else if (player->state == STATE_GROUND && player->up == true) {
        if (player->angle < player->targetAngle) {
            player->angle += 8;

            if (player->speed > 0.0f)
                player->speed -= 0.01f;

            if (player->angle > player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->angle > player->targetAngle) {
            player->angle -= 8;

            if (player->speed > 0.0f)
                player->speed -= 0.01f;

            if (player->angle < player->targetAngle)
                player->angle = player->targetAngle;
        }

        if (player->speed < 2.4f)
            player->speed += 0.012f;
    }
    else {
        if (player->speed > 0.0f)
            player->speed -= 0.012f;

        if (player->speed > -0.05f && player->speed < 0.0f)
            player->speed = 0.0f;
    }

    if (player->state != STATE_STATIC) {
        if (player->gravity == GRAVITY_AIR) {
            player->state = STATE_AIR;

            player->velocity.y -= 0.05f;
            if (player->velocity.y > 4.0f) {
                player->velocity.y = 4.0f;
                ResetPlayerRotation();
            }

            SetPlayerAnimation(ANI_JUMPING, (player->speed * 0.4f) + 0.4f);
        }
        else {
            PlayerObject *player1 = &Player[0];
            if (player1->speed < 0.01f)
                SetPlayerAnimation(ANI_STOPPED, 0.0f);
            else
                SetPlayerAnimation(ANI_WALKING, player->speed * 0.4f);

            player->state = STATE_GROUND;
            if (player->speed == 0.0f)
                player->unused5 = 0;

            if (player->jumpPress == true) {
                player->gravity    = GRAVITY_AIR;
                player->velocity.y = 2.0f;
                player->state      = STATE_AIR;
                ResetPlayerRotation();
            }
        }
    }

    player->velocity.x = -Sin(player->rotationY) * player->speed;
    player->velocity.z = Cos(player->rotationY) * player->speed;

    player->collisionPos = { 0.0f, -4.0f, 0.0f };
    if (player->gravity == GRAVITY_AIR) {
        if (player->velocity.y < 0.0f) {
            if (ObjectFloorCollision(&player->position, { player->velocity.x, -player->velocity.y, player->velocity.z }) == true) {
                player->velocity.y = 0.0f;
                player->gravity    = GRAVITY_GROUND;
                player->state      = STATE_GROUND;
            }
        }
    }
    else {
        player->velocity.y = 0.0f;
        HandlePlayerRotation(&player->velocity);

        player->collisionPos = { 0.0f, -3.5f, 0.0f };
        HandlePlayerRotation(&player->collisionPos);

        player->position.x -= player->collisionPos.x * 0.75f;
        player->position.y -= player->collisionPos.y * 0.75f;
        player->position.z -= player->collisionPos.z * 0.75f;
        switch (ObjectFloorCollision(&player->position, player->collisionPos)) {
            case 1:
            case 2:
                player->gravity = GRAVITY_GROUND;
                player->state   = STATE_GROUND;
                break;

            case 0:
                player->position.x += player->collisionPos.x * 0.75f;
                player->position.y += player->collisionPos.y * 0.75f;
                player->position.z += player->collisionPos.z * 0.75f;
                player->gravity = GRAVITY_AIR;
                break;

            default: break;
        }
    }

    player->position += player->velocity;
    ProcessPlayerCamera();

    if (player->up == true)
        player->rotationY = (player->angle * RSDK_PI / 128.0f) + CameraRotateY;

    if (player->gravity == GRAVITY_AIR) {
        if (CameraAirTimer < 30)
            CameraAirTimer++;

        if (CameraRotateX < player->velocity.y) {
            CameraRotateX += 0.1f;
            if (CameraRotateX > player->velocity.y)
                CameraRotateX = player->velocity.y;
        }

        if (CameraRotateX > player->velocity.y) {
            CameraRotateX -= 0.1f;
            if (CameraRotateX < player->velocity.y)
                CameraRotateX = player->velocity.y;
        }
    }
    else {
        if (CameraAirTimer > 0)
            CameraAirTimer--;

        if (CameraRotateX > 0.0f) {
            CameraRotateX -= 0.1f;
            if (CameraRotateX < 0.0f)
                CameraRotateX = 0.0f;
        }

        if (CameraRotateX < 0.0f) {
            CameraRotateX += 0.1f;
            if (CameraRotateX > 0.0f)
                CameraRotateX = 0.0f;
        }
    }

    CameraPosition.y = player->position.y + 15.0f - (CameraAirTimer * 0.25f * CameraRotateX);
}

void ProcessDebugMode()
{
    // chillin
}

void ProcessPlayerAnimation()
{
    Animator *animator = &SonicAni;

    ushort id = animator->animationID;
    if (animator->animationID != animator->nextAnimation)
        id = animator->nextAnimation;

    animator->frameTimer += animator->states[id].frameDuration;
    if (animator->frameTimer >= 240) {
        animator->frameTimer -= 240;

        if (++animator->frameID >= animator->states[animator->animationID].frameCount)
            animator->frameID = animator->states[animator->animationID].loopIndex;

        if (animator->animationID != animator->nextAnimation) {
            animator->animationID = animator->nextAnimation;

            animator->frameID = 0;
        }

        animator->nextFrame = animator->frameID + 1;
        if (animator->nextFrame >= animator->states[animator->animationID].frameCount)
            animator->nextFrame = animator->states[animator->animationID].loopIndex;
    }

    float timer = animator->frameTimer / 240.0f;
    for (int i = 0; i < 36; ++i) {
        AnimatorPart *node = &animator->nodes[i];

        AnimatorState *state     = &animator->states[animator->animationID];
        AnimatorState *stateNext = &animator->states[animator->nextAnimation];

        memcpy(&MatrixSonicNodeRotation[i], &MatrixIdentity, sizeof(MatrixSonicNodeRotation[i]));

        float ZPosingAnim = node->ZPosing[state->indices[animator->frameID]];
        float ZPosingNext = node->ZPosing[stateNext->indices[animator->nextFrame]];
        MatrixWorldRotateZ((1.0f - timer) * ZPosingAnim + (1.0f * timer) * ZPosingNext);
        MatrixMultiply(&MatrixSonicNodeRotation[i], &MatrixWorld);

        float YPosingAnim = node->YPosing[state->indices[animator->frameID]];
        float YPosingNext = node->YPosing[stateNext->indices[animator->nextFrame]];
        MatrixWorldRotateY((1.0f - timer) * YPosingAnim + (1.0f * timer) * YPosingNext);
        MatrixMultiply(&MatrixSonicNodeRotation[i], &MatrixWorld);

        float XPosingAnim = node->XPosing[state->indices[animator->frameID]];
        float XPosingNext = node->XPosing[stateNext->indices[animator->nextFrame]];
        MatrixWorldRotateX((1.0f - timer) * XPosingAnim + (1.0f * timer) * XPosingNext);
        MatrixMultiply(&MatrixSonicNodeRotation[i], &MatrixWorld);

        memcpy(&MatrixSonicNodeTransform[i], &MatrixIdentity, sizeof(MatrixSonicNodeTransform[i]));

        MatrixWorldTranslateXYZ(-node->x, -node->y, -node->z);
        MatrixMultiply(&MatrixSonicNodeTransform[i], &MatrixWorld);

        MatrixMultiply(&MatrixSonicNodeTransform[i], &MatrixSonicNodeRotation[i]);

        MatrixWorldTranslateXYZ(node->x, node->y, node->z);
        MatrixMultiply(&MatrixSonicNodeTransform[i], &MatrixWorld);
    }

    bool parented = false;
    for (int i = 0; i < animator->nodeCount; ++i) {
        if (parented != false) {
            if (animator->nodeIndices[i] == 0xFE || animator->nodeIndices[i] == 0xFF) {
                parented = (animator->nodeIndices[i] == 0xFE);
                continue;
            }

            memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));
            for (int k = i; animator->nodeIndices[k] < 0xFE; ++k) {
                MatrixMultiply(&MatrixObject, &MatrixSonicNodeTransform[animator->nodeIndices[k]]);
            }
            SetPlayerVertexPositions(animator->nodeIndices[i]);

            memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));
            for (int k = i; animator->nodeIndices[k] < 0xFE; ++k) {
                MatrixMultiply(&MatrixObject, &MatrixSonicNodeRotation[animator->nodeIndices[k]]);
            }
            SetPlayerVertexNormals(animator->nodeIndices[i]);
        }
        else {
            if (animator->nodeIndices[i] == 0xFE || animator->nodeIndices[i] == 0xFF) {
                parented = (animator->nodeIndices[i] == 0xFE);
                continue;
            }

            memcpy(&MatrixObject, &MatrixSonicNodeTransform[animator->nodeIndices[i]], sizeof(MatrixObject));
            SetPlayerVertexPositions(animator->nodeIndices[i]);

            memcpy(&MatrixObject, &MatrixSonicNodeRotation[animator->nodeIndices[i]], sizeof(MatrixObject));
            SetPlayerVertexNormals(animator->nodeIndices[i]);
        }
    }
}

void ProcessPlayerAnimationLMC()
{
    Animator *animator = &SonicAni;

    bool parented = false;
    for (int i = 0; i < 36; ++i) {
        for (int k = 0; k < 36; ++k) {
            AnimatorPart *node = &animator->nodes[k];

            memcpy(&MatrixSonicNodeRotation[k], &MatrixIdentity, sizeof(MatrixSonicNodeRotation[k]));

            MatrixWorldRotateZ(node->ZPosing[i]);
            MatrixMultiply(&MatrixSonicNodeRotation[k], &MatrixWorld);

            MatrixWorldRotateY(node->YPosing[i]);
            MatrixMultiply(&MatrixSonicNodeRotation[k], &MatrixWorld);

            MatrixWorldRotateX(node->XPosing[i]);
            MatrixMultiply(&MatrixSonicNodeRotation[k], &MatrixWorld);

            memcpy(&MatrixSonicNodeTransform[k], &MatrixIdentity, sizeof(MatrixSonicNodeTransform[k]));

            MatrixWorldTranslateXYZ(-node->x, -node->y, -node->z);
            MatrixMultiply(&MatrixSonicNodeTransform[k], &MatrixWorld);

            MatrixMultiply(&MatrixSonicNodeTransform[k], &MatrixSonicNodeRotation[k]);

            MatrixWorldTranslateXYZ(node->x, node->y, node->z);
            MatrixMultiply(&MatrixSonicNodeTransform[k], &MatrixWorld);
        }

        for (int k = 0; k < animator->nodeCount; ++k) {
            if (parented != false) {
                if (animator->nodeIndices[k] == 0xFE || animator->nodeIndices[k] == 0xFF) {
                    parented = (animator->nodeIndices[k] == 0xFE);
                    continue;
                }

                memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));
                for (int m = k; animator->nodeIndices[m] < 0xFE; ++m) {
                    MatrixMultiply(&MatrixObject, &MatrixSonicNodeTransform[animator->nodeIndices[m]]);
                }
                memcpy(&SonicNodeMatrixPositionStore[k][i], &MatrixObject, sizeof(SonicNodeMatrixPositionStore[k][i]));

                memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));
                for (int n = k; animator->nodeIndices[n] < 0xFE; ++n) {
                    MatrixMultiply(&MatrixObject, &MatrixSonicNodeRotation[animator->nodeIndices[n]]);
                }
                memcpy(&SonicNodeMatrixNormalStore[k][i], &MatrixObject, sizeof(SonicNodeMatrixNormalStore[k][i]));
            }
            else {
                if (animator->nodeIndices[k] == 0xFE || animator->nodeIndices[k] == 0xFF) {
                    parented = (animator->nodeIndices[k] == 0xFE);
                    continue;
                }

                memcpy(&MatrixObject, &MatrixSonicNodeTransform[animator->nodeIndices[k]], sizeof(MatrixObject));
                SetPlayerVertexPositions(animator->nodeIndices[k]);

                memcpy(&MatrixObject, &MatrixSonicNodeRotation[animator->nodeIndices[k]], sizeof(MatrixObject));
                SetPlayerVertexNormals(animator->nodeIndices[k]);
            }
        }
    }
}

void SetPlayerVertexPositions(int nodeID)
{
    Animator *animator = &SonicAni;
    AnimatorPart *node = &animator->nodes[nodeID];

    TMF *model     = &SonicModel;
    TMF *baseModel = &SonicBaseModel;

    Matrix3D *matrix = &MatrixObject;
    for (int i = 0; i < node->numIndices; ++i) {
        Vertex *vert     = &model->vertices[node->indices[i]];
        Vertex *baseVert = &baseModel->vertices[node->indices[i]];

        vert->x = matrix->m[0][0] * baseVert->x + matrix->m[1][0] * baseVert->y + matrix->m[2][0] * baseVert->z + matrix->m[3][0];
        vert->y = matrix->m[0][1] * baseVert->x + matrix->m[1][1] * baseVert->y + matrix->m[2][1] * baseVert->z + matrix->m[3][1];
        vert->z = matrix->m[0][2] * baseVert->x + matrix->m[1][2] * baseVert->y + matrix->m[2][2] * baseVert->z + matrix->m[3][2];
    }
}

void SetPlayerVertexNormals(int nodeID)
{
    Animator *animator = &SonicAni;
    AnimatorPart *node = &animator->nodes[nodeID];

    TMF *model     = &SonicModel;
    TMF *baseModel = &SonicBaseModel;

    Matrix3D *matrix = &MatrixObject;
    for (int i = 0; i < node->numIndices; ++i) {
        Vertex *vert     = &model->vertices[node->indices[i]];
        Vertex *baseVert = &baseModel->vertices[node->indices[i]];

        vert->nx = matrix->m[0][0] * baseVert->nx + matrix->m[1][0] * baseVert->ny + matrix->m[2][0] * baseVert->nz + matrix->m[3][0];
        vert->ny = matrix->m[0][1] * baseVert->nx + matrix->m[1][1] * baseVert->ny + matrix->m[2][1] * baseVert->nz + matrix->m[3][1];
        vert->nz = matrix->m[0][2] * baseVert->nx + matrix->m[1][2] * baseVert->ny + matrix->m[2][2] * baseVert->nz + matrix->m[3][2];
    }
}

void SetPlayerAnimation(byte animation, float speed)
{
    Animator *animator = &SonicAni;

    if (animation != animator->nextAnimation) {
        animator->nextAnimation = animation;

        animator->frameTimer = 0;
        animator->nextFrame  = 0;
        if (animator->animationID == ANI_JUMPING) {
            animator->animationID = animator->nextAnimation;
            animator->frameID     = 0;

            PlayerJumpRotationX = 0.0f;
        }
    }

    switch (animation) {
        case ANI_WALKING: {
            AnimatorState *state = &animator->states[ANI_WALKING];

            state->frameDuration = (byte)(speed * 128.0f);
            break;
        }

        case ANI_JUMPING: {
            AnimatorState *state = &animator->states[ANI_JUMPING];

            PlayerJumpRotationX = PlayerJumpRotationX - speed * 0.3f;
            if (PlayerJumpRotationX < 0.0f)
                PlayerJumpRotationX = 2 * RSDK_PI;

            state->frameDuration = (byte)(speed * 128.0f);
            break;
        }

        default: break;
    }
}

void DrawModelSonic(float x, float y, float z, float ry)
{
    Animator *animator = &SonicAni;

    TMF *model = &SonicModel;
    TMF *ball  = &BallModel;

    SetRenderTexture(0, SonicTexture);
    SetRenderState(RENDER_STATE_SPECULARENABLE, true);
    memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

    if (animator->animationID == ANI_JUMPING) {
        if (animator->frameID != 0) {
            MatrixWorldRotateY(ry);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            MatrixWorldTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
            DrawFace(RENDER_FVF_VERTEX, ball->vertices, ball->numVertices, ball->indices, ball->numIndices);
        }
        else {
            MatrixWorldTranslateXYZ(0.0f, -5.4f, 0.0f);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            MatrixWorldRotateZ(PlayerJumpRotationX);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            MatrixWorldTranslateXYZ(0.0f, 3.8f, 0.0f);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            MatrixWorldRotateY(ry);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            MatrixWorldTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
            DrawFace(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);

            RenderMaterial.diffuse.a = 0.25f;
            SetRenderMaterial(&RenderMaterial);

            memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

            MatrixWorldSetRotateY(ry);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            MatrixWorldTranslateXYZ(x, y, z);
            MatrixMultiply(&MatrixObject, &MatrixWorld);

            SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
            DrawFace(RENDER_FVF_VERTEX, ball->vertices, ball->numVertices, ball->indices, ball->numIndices);

            RenderMaterial.diffuse.a = 1.0f;
            SetRenderMaterial(&RenderMaterial);
        }
    }
    else {
        // The fact that this uses an ry parameter instead
        // of a global PlayerRotationY bothers me, and now
        // it will bother you as well.
        MatrixWorldRotateY(ry);
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        MatrixWorldRotateZ(PlayerRotationZ);
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        MatrixWorldRotateX(PlayerRotationX);
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        MatrixWorldTranslateXYZ(x, y, z);
        MatrixMultiply(&MatrixObject, &MatrixWorld);

        SetRenderTransform(RENDER_TRANSFORM_WORLD, &MatrixObject);
        DrawFace(RENDER_FVF_VERTEX, model->vertices, model->numVertices, model->indices, model->numIndices);
    }

    SetRenderState(RENDER_STATE_SPECULARENABLE, false);
    DrawModelShadow(x, y, z, -30.0f, 2.5f, 3.0f, ry);
}

void HandlePlayerRotation(float *x, float *y, float *z)
{
    Matrix3D *matrix = &MatrixObject;

    float sx = *x;
    float sy = *y;
    float sz = *z;

    memcpy(&MatrixObject, &MatrixIdentity, sizeof(MatrixObject));

    MatrixWorldRotateZ(PlayerTargetRotationZ);
    MatrixMultiply(&MatrixObject, &MatrixWorld);

    MatrixWorldRotateX(PlayerTargetRotationX);
    MatrixMultiply(&MatrixObject, &MatrixWorld);

    *x = matrix->m[0][0] * sx + matrix->m[1][0] * sy + matrix->m[2][0] * sz + matrix->m[3][0];
    *y = matrix->m[0][1] * sx + matrix->m[1][1] * sy + matrix->m[2][1] * sz + matrix->m[3][1];
    *z = matrix->m[0][2] * sx + matrix->m[1][2] * sy + matrix->m[2][2] * sz + matrix->m[3][2];
}

void ResetPlayerRotation()
{
    PlayerTargetRotationZ = 0.0f;
    PlayerTargetRotationX = 0.0f;
}