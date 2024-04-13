#include "TemplateDB.h"


void TemplateDB::loadTemplate(std::string& actor_template, Actor& actorT)
{
	std::string path = "resources/actor_templates/";
	std::string type = ".template";
	path += actor_template;
	path += type;

	if (!Resources_check::checkFileType(path, type)) {
		std::cout << "error: template " << actor_template << " is missing";
		exit(0);
	}

	rapidjson::Document actor;
	Game_initialize::Read_Jsonfile(path, actor);

	actorT.setActor(actor);

}
