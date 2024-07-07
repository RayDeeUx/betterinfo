#include <Geode/Geode.hpp>
#include <Geode/modify/DailyLevelPage.hpp>

#include "../utils.hpp"
#include "../layers/DailyHistory/DailyViewLayer.h"
#include "Geode/Enums.hpp"

using namespace geode::prelude;

class BI_DLL $modify(BIDailyLevelPage, DailyLevelPage) {
    /*
     * Callbacks
     */
    void onDailyHistory(CCObject* sender){
        auto layer = DailyViewLayer::scene(m_type);
        auto transitionFade = CCTransitionFade::create(0.5, layer);
        CCDirector::sharedDirector()->pushScene(transitionFade);
    }

    /*
     * Hooks
     */
    void updateTimers(float something) {
        DailyLevelPage::updateTimers(something);

        CCLayer* layer = this->m_mainLayer;

        if(layer->getChildByID("current-daily-text"_spr)) return;

        auto GM = GameLevelManager::sharedState();
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        //2.21: event levels
        std::ostringstream currentDaily;
        int id = m_type == GJTimedLevelType::Event ? GM->m_eventID : 
                 m_type == GJTimedLevelType::Weekly ? GM->m_weeklyID % 100000 :
                    GM->m_dailyID;
        auto currentDailyNode = CCLabelBMFont::create(fmt::format("Current: #{}", id).c_str(), "chatFont.fnt");
        currentDailyNode->setPosition({(winSize.width / 2) + 183, (winSize.height / 2) + 51});
        currentDailyNode->setAnchorPoint({1,0});
        currentDailyNode->setScale(0.6f);
        currentDailyNode->setColor({255,255,255});
        currentDailyNode->setOpacity(200);
        layer->addChild(currentDailyNode);
        currentDailyNode->setID("current-daily-text"_spr);
    }

    bool init(GJTimedLevelType type) {
        if(!DailyLevelPage::init(type)) return false;

        CCMenu* menu = this->m_buttonMenu;

        auto historySprite = BetterInfo::createWithBISpriteFrameName("BI_historyBtn_001.png");
        historySprite->setScale(0.8f);
        auto historyBtn = CCMenuItemSpriteExtra::create(
            historySprite,
            this,
            menu_selector(BIDailyLevelPage::onDailyHistory)
        );
        historyBtn->setPosition({13, -247});
        menu->addChild(historyBtn);
        historyBtn->setID("history-button"_spr);

        return true;
    }
};