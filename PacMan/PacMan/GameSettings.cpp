#include "GameSettings.h"

// boost headers
#include <boost\filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
using namespace boost::filesystem;

GameSettings::GameSettings()
{
    path p(m_strPath);

    if (exists(p))
    {
        Load();
    }
    else
    {
        Save();
    }
}

void GameSettings::Load()
{
    boost::property_tree::ptree tree;

    boost::property_tree::read_ini(m_strPath, tree);

    m_nFPS = tree.get("FPS", m_nFPS);
}

void GameSettings::Save()
{
    boost::property_tree::ptree tree, subtree;

    subtree.put("FPS", m_nFPS);

    tree.push_back(make_pair("All", subtree));

    boost::property_tree::write_ini(m_strPath, tree);
}

GameSettings& GameSettings::GetInstance()
{
    static GameSettings ob;
    return ob;
}

int GameSettings::FPS()
{
    return GetInstance().m_nFPS;
}
