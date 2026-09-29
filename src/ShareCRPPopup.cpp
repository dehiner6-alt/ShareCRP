#include "ShareCRPPopup.hpp"

// Método para crear el popup
ShareCRPPopup* ShareCRPPopup::create(int levelID) {
    auto ret = new ShareCRPPopup();
    if (ret && ret->init(levelID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ShareCRPPopup::init(int levelID) {
    if (!FLAlertLayer::init(300.f, 200.f)) {
        return false;
    }

    m_targetLevelID = levelID;

    // Fondo del popup
    auto bg = CCScale9Sprite::create("GJ_square01.png", { 0.f, 0.f, 80.f, 80.f });
    bg->setContentSize({ 300.f, 200.f });
    bg->setPosition(m_size / 2);
    m_mainLayer->addChild(bg);

    // Título en inglés
    auto title = CCLabelBMFont::create("Share Creator Points", "goldFont.fnt");
    title->setPosition(m_size.width / 2, m_size.height - 30.f);
    title->setScale(0.7f);
    m_mainLayer->addChild(title);

    // Campo de texto para los puntos
    m_pointsInput = TextInput::create(150.f, "Points", "chatFont.fnt");
    m_pointsInput->setPosition(m_size / 2);
    m_pointsInput->setAllowedChars("0123456789-");
    m_mainLayer->addChild(m_pointsInput);

    // Botón de enviar (Submit)
    auto submitBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Submit", "goldFont.fnt", "GJ_button_01.png"),
        this,
        menu_selector(ShareCRPPopup::onSubmitButton)
    );
    
    auto menu = CCMenu::create();
    menu->addChild(submitBtn);
    menu->setPosition(m_size.width / 2, 45.f);
    m_mainLayer->addChild(menu);

    return true;
}

void ShareCRPPopup::onSubmitButton(CCObject* sender) {
    std::string pointsStr = m_pointsInput->getString();
    if (pointsStr.empty()) return;

    // Obtenemos automáticamente el Account ID del usuario logueado en el juego
    auto gm = GameManager::sharedState();
    int accountID = gm->m_accountID;

    // URL directa de tu panel en el FHGDPS
    std::string url = "https://choyhomero.ps.fhgdps.com/dashboard/levels/shareCP.php"; 
    
    geode::utils::web::WebRequest req;
    // Enviamos el levelID, el accountID y los puntos escritos
    req.body(
        "levelID=" + std::to_string(m_targetLevelID) + 
        "&accountID=" + std::to_string(accountID) + 
        "&points=" + pointsStr
    );
    req.header("Content-Type: application/x-www-form-urlencoded");

    req.post(url, [this](geode::utils::web::WebResponse* response) {
        if (response->ok()) {
            std::string res = response->string().unwrapOr("");
            
            // Validaciones de respuesta en inglés
            if (res.find("success") != std::string::npos || res == "1" || res.empty()) {
                FLAlertLayer::create("Success", "Creator points successfully added to the level!", "OK")->show();
                this->onClose(nullptr);
            } 
            else if (res.find("permissions") != std::string::npos || res.find("unauthorized") != std::string::npos || res == "0") {
                FLAlertLayer::create("Access Denied", "You do not have permissions", "OK")->show();
            } 
            else {
                FLAlertLayer::create("Notice", "Server response: " + res, "OK")->show();
            }
        } else {
            FLAlertLayer::create("Network Error", "Could not connect to the server.", "OK")->show();
        }
    });
}

void ShareCRPPopup::onClose(CCObject* sender) {
    this->removeFromAndCleanup(true);
}
