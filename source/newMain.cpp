#include "raylib.h"
#include "raymath.h"
#include <string>
#include <vector>
#include <iostream>
#include "Player.h"
#include "newEnemy.h"
#include "StateMachine.h"
#include "ConcreteStates.h"

int allTreasureCollected = 0;
int totalTreasureOnMap = 3;
bool gameOver;

void renderMap(std::vector< std::vector<int> >& map, const  std::vector<Texture2D>& tileTextures, int renderScale) {
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            int tileId = map[y][x];

            float mapX = x * renderScale * tileTextures[tileId].width; 
            float mapY = y * renderScale * tileTextures[tileId].height;

            DrawTextureEx(tileTextures[tileId], Vector2{ mapX, mapY }, 0.0f, renderScale, WHITE);
        }
    }
}

bool canMoveToSpace(Vector2 pos, std::vector< std::vector<int> >& map, auto* gameObject) {

    if (pos.x < 0.f)
        return false; //vasemmasta reunasta yli
    if (pos.y < 0.f)
        return false; //yläreunasta yli
    if (pos.y >= map.size())
        return false; // alareunasta yli
    if (pos.x >= map[pos.y].size())
        return false; // oikeasta reunasta yli

    if (map[pos.y][pos.x] == 1) //spaceen ei voi liikkua
    {
        return false;
    }
    else if (map[pos.y][pos.x] == 2) //aarre!!
    {
        gameObject->AddTreasure();

        map[pos.y][pos.x] = 0; //muutetaan aarreruutu normi ruuduksi
    }

    return true;
}


/*bool isMapCollision(auto& gameObject, const auto& map)
{
    auto goPos = gameObject.GetGridPos();
    auto size = gameObject.GetSize();

    if (!canMoveToSpace(goPos, map)) //vasen ylakulma
        return true;

    Vector2 pos1{
        goPos.x + size.x,
        goPos.y + size.y
    };

    if (!canMoveToSpace(pos1, map)) // oikea alakulma
        return true;

    Vector2 pos2{
        goPos.x + size.x,
        goPos.y
    };

    if (!canMoveToSpace(pos2, map)) //oikea ylakulma
        return true;

    Vector2 pos3{
        goPos.x,
        goPos.y + size.y
    };

    if (!canMoveToSpace(pos3, map)) //vasen alakulma
        return true;

    return false;
}*/

bool isMapCollision(Vector2 nextPos, std::vector< std::vector<int> >& map, auto* gameObject)
{
    auto size = gameObject->GetSize();

    size.x *= 0.95; //skaalataan hahmoa hieman pienemmaksi, jottei peli luo virheellisia collisioneita
    size.y *= 0.95;

    // vasen ylakulma
    if (!canMoveToSpace(nextPos, map, gameObject))
        return true;

    // oikea ylakulma
    Vector2 topRight = Vector2{ nextPos.x + size.x, nextPos.y };
    if (!canMoveToSpace(topRight, map, gameObject))
        return true;

    // vasen alakulma
    Vector2 bottomLeft = Vector2{ nextPos.x, nextPos.y + size.y };
    if (!canMoveToSpace(bottomLeft, map, gameObject))
        return true;

    // oikea alakulma
    Vector2 bottomRight = Vector2{ nextPos.x + size.x, nextPos.y + size.y };
    if (!canMoveToSpace(bottomRight, map, gameObject))
        return true;

    // If all corners are valid, return false (no collision)
    return false;
}

//2 ulotteinen taulukko täynnä tileId:tä.
std::vector< std::vector<int> > MAP = { //0 is walkable, 1 is unwalkable, 2 is treasure
   //0  1  2  3  4  5  6  7
    {1, 1, 1, 1, 1, 1, 1, 1}, //0
    {1, 1, 0, 0, 0, 0, 0, 1}, //1
    {1, 0, 0, 0, 1, 0, 0, 1}, //2
    {1, 0, 2, 1, 0, 0, 0, 1}, //3
    {1, 0, 0, 0, 0, 0, 0, 1}, //4
    {1, 0, 0, 2, 0, 0, 1, 1}, //5
    {1, 0, 0, 0, 0, 0, 2, 1}, //6
    {1, 1, 1, 1, 1, 1, 1, 1}  //7
};

int main()
{
    const int windowX = 1100;
    const int windowY = 800;

    InitWindow(windowX, windowY, "Third time's the charm lol");

    gameOver = false;

    const int renderScale = 6;

    Player player{ LoadTexture("textures/characters/player_idle.png"), LoadTexture("textures/characters/player_run.png"), renderScale, Vector2{1, 6}};

    newEnemy enemy{ LoadTexture("textures/characters/slime_idle.png"), LoadTexture("textures/characters/slime_run.png"), renderScale, Vector2{6, 1} };

    Texture treasureTex = LoadTexture("textures/nature_tileset/chest.png");

    std::vector<Texture2D> mapTileTextures = {
        LoadTexture("textures/nature_tileset/grass.png"),
        LoadTexture("textures/nature_tileset/rock.png"),
        treasureTex
    };

    Rectangle upButton = { windowX - 200, 100, 80, 80 };
    Rectangle downButton = { windowX - 200, 200, 80, 80 };
    Rectangle leftButton = { windowX - 300, 130, 80, 80 };
    Rectangle rightButton = { windowX - 100, 130, 80, 80 };

    Rectangle treasureCounter = { windowX - 200, 300, 80, 80};

    SetTargetFPS(60);

    player.SetWorldPos();
    enemy.SetWorldPos();

    while (!WindowShouldClose())
    { 
        if (!gameOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && enemy.getCurrentState() != &MyTurn::getInstance())
        {
            Vector2 oldPlayerPos = player.GetGridPos();
            Vector2 mousePos = GetMousePosition();

            if (CheckCollisionPointRec(mousePos, upButton))
            {
                Vector2 nextPos = Vector2{ player.GetGridPos().x, player.GetGridPos().y - 1 };
                if(!isMapCollision(nextPos, MAP, &player))
                    player.MoveCharacter(Vector2{ 0, -1 }); //ylos
            }
            if (CheckCollisionPointRec(mousePos, downButton))
            {
                Vector2 nextPos = Vector2{ player.GetGridPos().x, player.GetGridPos().y + 1 };
                if (!isMapCollision(nextPos, MAP, &player))
                    player.MoveCharacter(Vector2{ 0, 1 }); //alas
            }
            if (CheckCollisionPointRec(mousePos, leftButton))
            {
                Vector2 nextPos = Vector2{ player.GetGridPos().x - 1, player.GetGridPos().y};
                if (!isMapCollision(nextPos, MAP, &player))
                    player.MoveCharacter(Vector2{ -1, 0 }); //vasemmalle
            }
            if (CheckCollisionPointRec(mousePos, rightButton))
            {
                Vector2 nextPos = Vector2{ player.GetGridPos().x + 1, player.GetGridPos().y};
                if (!isMapCollision(nextPos, MAP, &player))
                    player.MoveCharacter(Vector2{ 1, 0 }); //oikealle
            }

            if (player.GetGridPos().x != oldPlayerPos.x || player.GetGridPos().y != oldPlayerPos.y) //Siirra vihua pelaajan jalkeen
            {
                enemy.Toggle(); //Enemy MyTurn
                enemy.MoveTowardsTreasure(MAP, player.GetGridPos());
            }
        }

        allTreasureCollected = player.GetTreasure() + enemy.GetTreasure();

        if (allTreasureCollected >= totalTreasureOnMap && gameOver == false) //muutetaan game over vain kerran
        {
            std::cout << "game over!";
            gameOver = true;
        }

        //start drawing
        BeginDrawing();
        ClearBackground(DARKGREEN);

        renderMap(MAP, mapTileTextures, renderScale);

        player.Tick(GetFrameTime());
        enemy.Tick(GetFrameTime());

        //Rectangle source{ frame * width, 0.f, rightLeft * width, height };
        //Rectangle dest{ worldPos.x, worldPos.y, scale * width, scale * height };

        //DrawTexturePro(activeTexture, source, dest, Vector2{}, 0.f, WHITE);
        DrawTexturePro(treasureTex, Rectangle{0.f, 0.f, (float)treasureTex.width, (float)treasureTex.height}, treasureCounter, {}, 0.0f, WHITE);
        std::string treasureAmount = "Treasure: ";
        treasureAmount.append(std::to_string(player.GetTreasure()), 0, 1);
        DrawText(treasureAmount.c_str(), treasureCounter.x - 20, treasureCounter.y + 80, 20, BLACK);

        if (gameOver)
        {
            //std::cout << "Game over reached";

            std::string winText;
            if (player.GetTreasure() > enemy.GetTreasure())
            {
                winText = "You win!";
            }
            else if (player.GetTreasure() == enemy.GetTreasure())
            {
                winText = "It's a tie!";
            }
            else if (player.GetTreasure() < enemy.GetTreasure())
            {
                winText = "You lose!";
            }
            DrawText(winText.c_str(), windowX / 2 - 100, windowY / 2 - 100, 40, RED);

            EndDrawing();
            continue;
        }
        
        DrawRectangleRec(upButton, GRAY);
        DrawText("UP", upButton.x + 15, upButton.y + 10, 20, BLACK);

        DrawRectangleRec(downButton, GRAY);
        DrawText("DOWN", downButton.x + 5, downButton.y + 10, 20, BLACK);

        DrawRectangleRec(leftButton, GRAY);
        DrawText("LEFT", leftButton.x + 5, leftButton.y + 10, 20, BLACK);

        DrawRectangleRec(rightButton, GRAY);
        DrawText("RIGHT", rightButton.x + 5, rightButton.y + 10, 20, BLACK);


        EndDrawing();
    }

    for (auto mapTex : mapTileTextures)
    {
        UnloadTexture(mapTex);
    }
    player.UnloadTextures();
    enemy.UnloadTextures();
    UnloadTexture(treasureTex);
}