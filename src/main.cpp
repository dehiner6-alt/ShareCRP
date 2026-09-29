#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

// Función auxiliar para verificar si estamos en tu GDPS o en los servidores oficiales de RobTop
bool isMyGDPS() {
    // Obtenemos la URL del servidor actual configurada en el juego
    std::string gameServer = GJAccountManager::sharedState()->m_serverUrl;
    // Si contiene tu dominio, estamos en tu FHGDPS
    if (gameServer.find("choyhomero.ps.fhgdps.com") != std::string::npos) {
        return true;
    }
    // Si usas otro método o está vacío apuntando a roptop por defecto:
    // (Puedes ajustar esta condición según cómo tu cliente detecte el servidor privado)
    return false; 
}

// 1. Alerta al iniciar el juego si NO estás en tu GDPS
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

// 2. Control del botón en la información del nivel
class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) {
            return false;
        }

        // Si no estamos en tu GDPS, no creamos ni mostramos el botón
        if (!isMyGDPS()) {
            return true; 
        }

        // Creamos el fondo circular para el botón
        auto bgCircle = CCSprite::createWithSpriteFrameName("GJ_square01.png");
        bgCircle->setScale(0.7f);

        // Cargamos el ícono de Creator Points
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

        // Por ahora lo ubicamos en el menú lateral. En cuanto me mandes la captura, 
        // ajustaremos las coordenadas exactas donde lo quieras poner.
        if (auto menu = m_mainLayer->getChildByID("other-menu")) {
            menu->addChild(shareCrpBtn);
            menu->updateLayout();
        } else {
            auto sideMenu = CCMenu::create();
            sideMenu->addChild(shareCrpBtn);
            sideMenu->setPosition({ m_uiLayer->getContentSize().width - 35, 170 });
            sideMenu->setLayout(ColumnLayout::create());
            m_uiLayer->addChild(sideMenu, 10);
        }

        return true;
    }

    void onOpenShareCRP(CCObject*) {
        int currentLevelID = m_level->m_levelID;
        ShareCRPPopup::create(currentLevelID)->show();
    }
};
