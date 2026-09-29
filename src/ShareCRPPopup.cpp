#include "ShareCRPPopup.hpp"

ShareCRPPopup* ShareCRPPopup::create(int levelID) {
    auto ret = new ShareCRPPopup();
    if (ret && ret->initAnchored(300.f, 200.f, levelID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ShareCRPPopup::setup(int levelID) {
    m_targetLevelID = levelID;

    // Título
    auto title = CCLabelBMFont::create("Share Creator Points", "goldFont.fnt");
    title->setPosition(m_size.width / 2, m_size.height - 25.f);
    title->setScale(0.7f);
    m_mainLayer->addChild(title);

    // Campo de texto para los puntos
    m_pointsInput = TextInput::create(150.f, "Points", "chatFont.fnt");
    m_pointsInput->setPosition(m_size / 2);
    m_pointsInput->setAllowedChars("0123456789-");
    m_mainLayer->addChild(m_pointsInput);

    // Botón de enviar
    auto submitBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Submit", "goldFont.fnt", "GJ_button_01.png"),
        this,
        menu_selector(ShareCRPPopup::onSubmitButton)
    );
    
    auto menu = CCMenu::create();
    menu->addChild(submitBtn);
    menu->setPosition(m_size.width / 2, 40.f);
    m_mainLayer->addChild(menu);

    return true;
}

void ShareCRPPopup::onSubmitButton(CCObject* sender) {
    std::string pointsStr = m_pointsInput->getString();
    if (pointsStr.empty()) return;

    auto gm = GameManager::sharedState();
    // Usamos el accountID mediante el GameStatsManager o métodos seguros de la cuenta activa
    int accountID = GJAccountManager::sharedState()->m_accountID;

    std::string url = "https://choyhomero.ps.fhgdps.com/dashboard/levels/shareCP.php"; 
    
    geode::utils::web::WebRequest req;
    req.body(
        "levelID=" + std::to_string(m_targetLevelID) + 
        "&accountID=" + std::to_string(accountID) + 
        "&points=" + pointsStr
    );
    req.header("Content-Type", "application/x-www-form-urlencoded");

    // Pasamos la solicitud web de forma compatible con la versión actual de Geode
    req.post(url, [this](geode::utils::web::WebResponse* response) {
        if (response->ok()) {
            std::string res = response->string().unwrapOr("");
            
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
