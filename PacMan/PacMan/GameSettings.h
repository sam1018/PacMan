#pragma once

#include <string>

class GameSettings
{
private:
    int m_nFPS = 60;
    std::string m_strPath = "../Settings.ini";

private:
    GameSettings();
    void Load();
    void Save();
    static GameSettings& GetInstance();

public:
    static int FPS();
};
