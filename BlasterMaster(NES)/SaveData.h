#pragma once
class SaveData 
{
public:
	float sophiaHP, sophiaX, sophiaY, jasonHP;
	int sophiaSection, sophiaState;
    int mainLives, lastScene, lastSection;
    SaveData operator=(const SaveData& s) {
        SaveData newS;
        newS.sophiaHP = s.sophiaHP;
        newS.sophiaX = s.sophiaX;
        newS.sophiaY = s.sophiaY;
        newS.jasonHP = s.jasonHP;
        newS.sophiaSection = s.sophiaSection;
        newS.sophiaState = s.sophiaState;
        newS.mainLives = s.mainLives;
        newS.lastScene = s.lastScene;
        newS.lastSection = s.lastSection;
        return newS;
    }
    SaveData() {
        this->jasonHP = -1;
        this->sophiaHP = -1;
        this->sophiaX = -1;
        this->sophiaY = -1;
        this->sophiaSection = -1;
        this->sophiaState = -1;
        this->mainLives = 3;
        this->lastScene = -1;
        this->lastSection = -1;
    }
};