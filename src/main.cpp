#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

class $modify(MyProfilePage, ProfilePage) {
    void setupPage() {
        ProfilePage::setupPage();

        // Obtenemos el ID de la cuenta del usuario que estamos viendo en el perfil
        int accountID = m_accountID;

        // Creamos un botón con el estilo clásico de Geometry Dash (puedes cambiar el texto o el sprite)
        auto shareCrpBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("CRP", "goldFont.fnt", "GJ_button_01.png"),
            this,
            menu_selector(MyProfilePage::onOpenShareCRP)
        );
        shareCrpBtn->setID("share-crp-profile-button"_spr);

        // Buscamos el menú de la derecha donde están los botones de mensaje, bloquear, etc.
        // En ProfilePage, este menú suele encontrarse o añadirse al contenedor principal de la derecha.
        // Vamos a buscar el menú lateral de la derecha (suele llamarse "right-menu" o estar dentro de las capas del perfil).
        
        // Una forma segura en ProfilePage es añadirlo al menú de botones que está al lado del avatar:
        if (auto menu = m_mainLayer->getChildByID("right-menu")) {
            menu->addChild(shareCrpBtn);
            menu->updateLayout();
        } else {
            // Si por estructura el ID varía en tu versión de GDPS, lo añadimos de manera manual al menú principal de botones
            // Buscando el nodo de botones lateral:
            if (auto leftMenu = this->getChildByID("other-menu")) { // Ajustamos si es necesario
                leftMenu->addChild(shareCrpBtn);
                leftMenu->updateLayout();
            }
        }
    }

    void onOpenShareCRP(CCObject*) {
        // Abrimos la ventana emergente pasándole el ID de la cuenta del usuario actual
        ShareCRPPopup::create(m_accountID)->show();
    }
};
