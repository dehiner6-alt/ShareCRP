#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

bool isMyGDPS() {
    std::string gameServer = GJAccountManager::sharedState()->m_DS_Server_Path;
    if (gameServer.find("choyhomero.ps.fhgdps.com") != std::string::npos) {
        return true;
    }
    // Si prefieres omitir la restricción estricta mientras pruebas, puedes retornar true temporalmente aquí:
    return true; 
}

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        static bool initialCheck = false;
        if (!initialCheck) {
            initialCheck = true;
            if (!isMyGDPS()) {
                FLAlertLayer::create(
                    "ShareCRP",
                    "ShareCRP mod is disabled because you are in RobTop Servers.",
                    "OK"
                )->show();
            }
        }

        return true;
    }
};

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) {
            return false;
        }

        if (!isMyGDPS()) {
            return true; 
        }

        auto bgCircle = CCSprite::createWithSpriteFrameName("GJ_square01.png");
        bgCircle->setScale(0.7f);

        auto crpIcon = CCSprite::createWithSpriteFrameName("GJ_creatorIcon_001.png");
        if (!crpIcon) {
            crpIcon = CCSprite::createWithSpriteFrameName("difficulty_06_btn_001.png");
        }
        
        if (crpIcon) {
            crpIcon->setPosition(bgCircle->getContentSize() / 2);
            bgCircle->addChild(crpIcon);
        }

        auto shareCrpBtn = CCMenuItemSpriteExtra::create(
            bgCircle,
            this,
            menu_selector(MyLevelInfoLayer::onOpenShareCRP)
        );
        shareCrpBtn->setID("level-share-crp-button"_spr);

        if (auto menu = m_otherMenu) {
            menu->addChild(shareCrpBtn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenShareCRP(CCObject*) {
        int currentLevelID = m_level->m_levelID;
        ShareCRPPopup::create(currentLevelID)->show();
    }
};
