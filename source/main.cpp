#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <vector>
#include <iostream>

static bool isFirstRender = false;

void renderMap(const std::vector< std::vector<int> >& map, const  std::vector<Texture2D>& tileTextures, float tileSize, Vector2 mapPos) {
    if (isFirstRender)
    {
        //std::cout << "Map origin x: " << mapPos.x << ", y: " << mapPos.y;
        isFirstRender = false;
    }
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            int tileId = map[y][x];

            float mapY = y * tileSize + mapPos.y;
            float mapX = x * tileSize + mapPos.x;
            DrawTextureEx(tileTextures[tileId], Vector2{ mapX, mapY }, 0.0f, tileSize / 16, WHITE); //skaalataan tile tileSize / 16
        }
    }
}

bool canMoveToSpace(Vector2 pos, const std::vector< std::vector<int> >& map) {
    if (pos.x < 0.f)
        return false; //vasemmasta reunasta yli
    if (pos.y < 0.f)
        return false; //yläreunasta yli
    if (pos.y >= map.size())
        return false; // alareunasta yli
    if (pos.x >= map[pos.y].size())
        return false; // oikeasta reunasta yli

    if (map[pos.x][pos.y] == 1) //spaceen ei voi liikkua
    {
        //std::cout << "x = " << pos.x << ", y = " << pos.y;
        return false;
    }

    return true;
}

bool isMapCollision(auto gameObject, const auto& map, float tileSize)
{
    auto goPos = gameObject->GetPosition(tileSize);
    auto size = gameObject->GetSize();

    if (!canMoveToSpace(goPos, map)) //vasen ylakulma
        return true;

    Vector2 pos1{
        goPos.x + size.x / tileSize,
        goPos.y + size.y / tileSize
    };

    if (!canMoveToSpace(pos1, map)) // oikea alakulma
        return true;

    Vector2 pos2{
        goPos.x + size.x / tileSize,
        goPos.y
    };

    if (!canMoveToSpace(pos2, map)) //oikea ylakulma
        return true;

    Vector2 pos3{
        goPos.x,
        goPos.y + size.y / tileSize
    };

    if (!canMoveToSpace(pos3, map)) //vasen alakulma
        return true;

    return false;
}

//2 ulotteinen taulukko täynnä tileId:tä.
const std::vector< std::vector<int> > MAP = { //0 is walkable, 1 is unwalkable
   //0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, //1
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, //2
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, //3
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, //4
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0}, //5
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //6
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //7
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //8
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0}, //9
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, //10
    {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, //11
    {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0}, //12
    {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}, //13
    {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}, //14
    {0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0}, //15
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, //16
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, //17
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, //18
    {0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0}, //19
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //20
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //21
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, //22
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  //23
};

int main()
{
    isFirstRender = true;

    const int windowDimensions[2] {800, 600}; //width, height
    InitWindow(windowDimensions[0], windowDimensions[1], "The World's Most Epic Top Down RPG Ever Created During This Lifetime Patent Pending TM!");

    Texture2D mapTexture = LoadTexture("textures/nature_tileset/map.png");
    Vector2 mapPos = {0.0f, 0.0f};
    const float mapScale = 4.f;
    const float tileSize = 16.f * mapScale; //tile on 16x16px, skaalataan se mapScalella

    std::vector<Texture2D> mapTileTextures = {
        LoadTexture("textures/nature_tileset/grass.png"),
        LoadTexture("textures/nature_tileset/rock.png")
    };

    Character player{ windowDimensions[0], windowDimensions[1], LoadTexture("textures/characters/player.png"), Vector2 {} };

    /*Prop props[2]{
        Prop{Vector2{600.f, 200.f}, LoadTexture("textures/objects/rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("textures/objects/stump.png")}
    };*/

    Enemy slime{Vector2{700.f, 400.f}, LoadTexture("textures/characters/slime.png"), 13, 7};
    Enemy slimeTwo{Vector2{500.f, 700.f}, LoadTexture("textures/characters/slime.png"), 13, 7};
    Enemy slimeThree{ Vector2{600.f, 600.f}, LoadTexture("textures/characters/slime.png"), 13, 7 };

    Enemy* enemies[]{
        &slime,
        &slimeTwo,
        &slimeThree
    };
    
    for (auto Enemy : enemies)
    {
        Enemy->SetTarget(&player);
    }

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //Draw the map
        mapPos = Vector2Scale(player.GetWorldPos(), -1.f);
        renderMap(MAP, mapTileTextures, tileSize, mapPos);

        /*for (auto prop : props) // loop through the props array and draw them
        {
            prop.Render(player.GetWorldPos());
        }*/

        if(!player.GetAlive()) //player not alive
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);

            EndDrawing();
            continue;
        }
        else
        {
            std::string playerHealth = "Health: ";
            playerHealth.append(std::to_string(player.GetHealth()), 0, 5);
            DrawText(playerHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        player.Tick(GetFrameTime(), 0, 0);

        //check for map collisions
        if (isMapCollision(&player, MAP, tileSize))
        {
            player.UndoMovement();
        }

        for (auto enemy : enemies)
        {
            if (isMapCollision(enemy, MAP, tileSize))
                enemy->UndoMovement();
        }

        /*for(auto prop : props)
        {
            if(CheckCollisionRecs(prop.GetCollisionRec(player.GetWorldPos()), player.GetCollisionRec())) //check prop collisions
            {
                player.UndoMovement();
            }
        }*/

        for(auto enemy : enemies)
        {
            enemy->Tick(GetFrameTime(), 0, 0);
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for(auto enemy : enemies)
            {
                if(CheckCollisionRecs(enemy->GetCollisionRec(), player.GetWeaponCollisionRec()))
                {
                    enemy->SetAlive(false);
                }
            }
        }
        
        EndDrawing();
    }

    UnloadTexture(mapTexture);
    CloseWindow();
}