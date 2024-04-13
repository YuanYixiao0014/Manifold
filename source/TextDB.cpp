#include "TextDB.h"

void TextDB::initTextDB(rapidjson::Document& doc)
{
    //init texts
    if (doc.IsObject()) {
        //if (doc.HasMember("intro_text")) {
        //    const rapidjson::Value& intro_txts = doc["intro_text"];

        //    for (int i = 0; i < intro_txts.Size(); i++) {
        //        intro_texts.emplace_back(intro_txts[i].GetString());
        //    }

        //    //init font
        //    if (doc.HasMember("font")) {
        //        std::string font_name = doc["font"].GetString();
        //        std::string path = "resources/fonts/" + font_name + ".ttf";
        //        std::string type = ".ttf";
        //        if (!Resources_check::checkFileType(path, type)) {
        //            std::cout << "error: font " << font_name << " missing";
        //            exit(0);
        //        }
        //        font = TTF_OpenFont(path.c_str(), 16);
        //    }
        //    else {
        //        std::cout << "error: text render failed. No font configured";
        //        exit(0);
        //    }
        //}
    }

}
TTF_Font* TextDB::getFont(std::string& font_name, int& font_size)
{
    //find a font already initialized
    if (text_Fonts.find(font_name) != text_Fonts.end()) {
        std::unordered_map<int, TTF_Font*>& fonts = text_Fonts.at(font_name);

        if (fonts.find(font_size) != fonts.end()) {

            return fonts.at(font_size);
        }
    }

    //initialize a font if not found
    return initFont(font_name, font_size);
}

TTF_Font* TextDB::initFont(std::string& font_name, int& font_size)
{
    TTF_Font* font = nullptr;
    std::string path = "resources/fonts/" + font_name + ".ttf";
    std::string type = ".ttf";
    if (!Resources_check::checkFileType(path, type)) {
        std::cout << "error: font " << font_name << " missing";
        exit(0);
    }
    font = TTF_OpenFont(path.c_str(), font_size);
    text_Fonts[font_name][font_size] = font;

    return font;
}


