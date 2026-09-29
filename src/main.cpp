#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

bool isMyGDPS() {
    // Verificación segura de la URL del servidor actual en Geometry Dash
    std::string gameServer = GJAccountManager::sharedState()->m_serverIP;
    if (gameServer.find("choyhomero.ps.fhgdps.com") != std::string::npos) {
        return true;
    }
    // Puedes retornar true temporalmente aquí si quieres probar sin importar el servidor:
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
                    "ShareCRP Notice",
                    "ShareCRP mod is disabled because you are not connected to your official GDPS.",
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

        // Añadir de forma segura al menú lateral derecho de la pantalla de info del nivel
        if (auto sideMenu = this->getChildByID("right-side-menu")) {
            sideMenu->addChild(shareCrpBtn);
            sideMenu->updateLayout();
        } else if (auto menu = m_uiLayer->getChildByID("right-menu")) {
            menu->addChild(shareCrpBtn);
            menu->updateLayout();
        }

        return true;
    }

    void onOpenShareCRP(CCObject*) {
        int currentLevelID = m_level->m_levelID;
        auto popup = ShareCRPPopup::create(currentLevelID);
        popup->show();
    }
};
