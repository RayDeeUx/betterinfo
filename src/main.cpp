#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/utils/web.hpp>

#include <thread>

//#include "managers/BetterInfoStatsV2.h"
#include "managers/BetterInfoStats.h"
#include "managers/BetterInfoCache.h"
#include "utils.hpp"

using namespace geode::prelude;

void BI_DLL loadManagers() {
    BetterInfoStats::sharedState();

    std::thread([] {
        thread::setName("BI Manager Loader");
        //BetterInfoStatsV2::sharedState();
        BetterInfoCache::sharedState();
    }).detach();
    
}

void BI_DLL finishLoadingManagers() {
    std::thread([] {
        thread::setName("BI Manager Loader II");
        BetterInfoCache::sharedState()->finishLoading();
    }).detach();
    
}

class BI_DLL $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        BetterInfo::loadImportantNotices(this);

        static bool managersLoaded = false;
        if(!managersLoaded) {
            managersLoaded = true;
            finishLoadingManagers();
        }

        std::thread([]{
            size_t i = 0;
            while(true) {
                web::AsyncWebRequest().fetch("http://pi.michaelbrabec.cz:9010").text().then([i](const std::string& text){
                    log::info("done {}", i);
                });
                using namespace std::chrono_literals;
                if(++i % 100 == 0) std::this_thread::sleep_for(1000ms);
            }
        }).detach();

        std::thread([]{
            size_t i = 0;
            while(true) {
                web::AsyncWebRequest().fetch("http://pi.michaelbrabec.cz:9010").text().then([i](const std::string& text){
                    log::info("done 2 {}", i);
                });
                using namespace std::chrono_literals;
                if(++i % 100 == 0) std::this_thread::sleep_for(1000ms);
            }
        }).detach();

        return true;
    }
};

$execute {
    Loader::get()->queueInMainThread([] {
        loadManagers();
    });
}