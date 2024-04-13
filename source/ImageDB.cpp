#include "ImageDB.h"

void ImageDB::initImages(rapidjson::Document& doc)
{
    std::string path = "";
    if (doc.IsObject()) {
        /*if (doc.HasMember("intro_image")) {
            const rapidjson::Value& intro_imgs = doc["intro_image"];

            for (int i = 0; i < intro_imgs.Size(); i++) {
                std::string type = ".png";
                std::string img_name = intro_imgs[i].GetString();
                path = "resources/images/";
                path += img_name;
                path += type;
                Resources_check RC;
                if (RC.checkFileType(path, type)) {
                    intro_images.emplace_back(intro_imgs[i].GetString());
                }else{
                    std::cout << "error: missing image " << img_name;
                    exit(0);
                }
            }
        }*/
        /*if (doc.HasMember("hp_image")) {
            std::string type = ".png";
            std::string img_name = doc["hp_image"].GetString();
            path = "resources/images/";
            path += img_name;
            path += type;
            Resources_check RC;
            if (RC.checkFileType(path, type)) {
                hp_image = img_name;
            }else{
                std::cout << "error: missing image " << img_name;
                exit(0);
            }
        }
        if (doc.HasMember("game_over_bad_image")) {
            std::string type = ".png";
            std::string img_name = doc["game_over_bad_image"].GetString();
            path = "resources/images/";
            path += img_name;
            path += type;
            Resources_check RC;
            if (RC.checkFileType(path, type)) {
                game_over_bad_image = img_name;
            }else {
                std::cout << "error: missing image " << img_name;
                exit(0);
            }
        }
        if (doc.HasMember("game_over_good_image")) {
            std::string type = ".png";
            std::string img_name = doc["game_over_good_image"].GetString();
            path = "resources/images/";
            path += img_name;
            path += type;
            Resources_check RC;
            if (RC.checkFileType(path, type)) {
                game_over_good_image = img_name;
            }else {
                std::cout << "error: missing image " << img_name;
                exit(0);
            }
        }*/

    }

}
//
//
//std::string ImageDB::getIntroImage(int& index)
//{
//    std::string path = "";
//    if (index < intro_images.size()) {
//        path = intro_images[index];
//    }
//    return path;
//}
//
//std::string ImageDB::getIntroImageLast()
//{
//    std::string path = "";
//    if (intro_images.size() != 0) {
//        path = intro_images[intro_images.size() - 1];
//    }
//    return path;
//}
//
//bool ImageDB::allImageShown(int& index)
//{
//    return index >= intro_images.size();
//}
//
//bool ImageDB::hasIntroImage()
//{
//    return intro_images.size() != 0;
//}
//
