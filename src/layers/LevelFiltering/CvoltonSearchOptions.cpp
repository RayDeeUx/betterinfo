#include "CvoltonSearchOptions.h"
#include "IDRangePopup.h"
#include "../../enums.hpp"
#include "../../ui/EnumSelectNode.h"

const int completedMax = 7;

CvoltonSearchOptions* CvoltonSearchOptions::create(){
    auto ret = new CvoltonSearchOptions();
    if (ret && ret->init()) {
        //robert 1 :D
        ret->autorelease();
    } else {
        //robert -1
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void CvoltonSearchOptions::onClose(cocos2d::CCObject* sender)
{
    destroyToggles();
    CvoltonAlertLayerStub::onClose(sender);
}

void CvoltonSearchOptions::onInfo(cocos2d::CCObject* sender)
{
    FLAlertLayer::create(nullptr, "Advanced options", "<cl>Trim Spaces:</c> Removes spaces from the beginning and end of your search query\n\n<co>Completed Mode:</c> Changes which levels are sent to the completed/uncompleted filters", "OK", nullptr, 480)->show();
}

void CvoltonSearchOptions::onPrev(cocos2d::CCObject* sender)
{
    auto layer = MoreSearchLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
    onClose(sender);
}

bool CvoltonSearchOptions::init(){
    if(!CvoltonAlertLayerStub::init({440.0f, 290.0f}, 1.f, 0x96)) return false;
    m_closeBtn->setPositionX(m_closeBtn->getPositionX() + 0.5f);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto label = createTextLabel("Advanced Options", {(winSize.width / 2), (winSize.height / 2) + 127}, .7f, m_mainLayer, "goldFont.fnt");
    label->setID("title"_spr);
    auto prevBtn = createButton("GJ_arrow_03_001.png", {- (winSize.width / 2) + 30, 0}, menu_selector(CvoltonSearchOptions::onPrev));
    prevBtn->setID("prev-button"_spr);
    auto infoBtn = createButton("GJ_infoIcon_001.png", {203, 128}, menu_selector(CvoltonSearchOptions::onInfo));
    infoBtn->setID("info-button"_spr);

    auto plus = createButton("GJ_plusBtn_001.png", {196, -120}, menu_selector(CvoltonSearchOptions::onPercentageRange), .75f);
    plus->setID("percentage-plus-button"_spr);

    auto completedModeSelect = EnumSelectNode<CompleteMode>::create({
        {CompleteMode::modeDefault, "Default"},
        {CompleteMode::completed, "Completed Levels"},
        {CompleteMode::completed21, "Completed Orbs"},
        {CompleteMode::completed211, "Completed Leaderboard"},
        {CompleteMode::allCoins, "Completed With Coins"},
        {CompleteMode::noCoins, "Completed Without Coins"},
        {CompleteMode::percentage, "Percentage"},
    },
    [this, plus](CompleteMode mode) -> void {
        setOptionInt("search_completed", static_cast<int>(mode));
        GameLevelManager::sharedState()->m_timerDict->removeAllObjects();
        plus->setVisible(mode == CompleteMode::percentage);
    });
    completedModeSelect->setCurrentValue(static_cast<CompleteMode>(getOptionInt("search_completed")));
    completedModeSelect->setPosition({winSize.width / 2, (winSize.height / 2) - 120});
    m_mainLayer->addChild(completedModeSelect);

    drawToggles();

    return true;
}

void CvoltonSearchOptions::drawToggles(){
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    //createToggle("search_contains", "Name Contains", -170, 75); //40 -60, 170 -60, 300 -60, 40 -110
    //createToggle("search_no_id", "No ID Search", -40, 75);
    //createToggle("search_surround_percent", "No Forced Star", 90, 75);
    createToggle("search_trim", "Trim Spaces");

    auto completedMode = createTextLabel("Completed Mode:", {0, -95}, 0.5f, m_buttonMenu, "goldFont.fnt");
    completedMode->setID("completed-mode"_spr);
    m_toggles.push_back(completedMode);
}

void CvoltonSearchOptions::onPercentageRange(CCObject* sender) {
    IDRangePopup::create(this, getOptionInt("search_completed_percentage_min"), getOptionInt("search_completed_percentage_max"), "Percentage")->show();
    GameLevelManager::sharedState()->m_timerDict->removeAllObjects();
}

std::string CvoltonSearchOptions::getCompletedString(){
    int type = getOptionInt("search_completed");
    const char* types[] = {"Default", "Completed Levels", "Completed Orbs", "Completed Leaderboard", "Completed With Coins", "Completed Without Coins", "Percentage"};
    return std::to_string(type) + ": " + types[type % completedMax];
}

void CvoltonSearchOptions::onIDRangeFinished(int min, int max, int additional) {
    setOptionInt("search_completed_percentage_min", min);
    setOptionInt("search_completed_percentage_max", max);
}
