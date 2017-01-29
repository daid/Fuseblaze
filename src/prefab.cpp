#include "prefab.h"
#include "main.h"
#include "floor.h"
#include "wall.h"
#include "door.h"
#include "trigger.h"

#include "editor/prototype.h"

#include <json11/json11.hpp>
#include <fstream>

void Prefab::load(sp::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return;
    std::stringstream data;
    data << file.rdbuf();
    sp::string err;
    
    json11::Json json = json11::Json::parse(data.str(), err);
    
    for(const json11::Json& entry : json.array_items())
    {
        Part part;
        sp::string type_string = entry["type"].string_value();
        if (type_string == "Wall")
            part.type = Part::Type::Wall;
        else if (type_string == "Floor")
            part.type = Part::Type::Floor;
        else if (type_string == "Door")
            part.type = Part::Type::Door;
        else if (type_string == "Trigger")
            part.type = Part::Type::Trigger;
        else if (type_string == "PrefabConnection")
            part.type = Part::Type::PrefabConnection;
        else
        {
            LOG(Warning, "Unknown part type", type_string);
            part.type = Part::Type::Floor;
        }
        part.position.x = entry["position_x"].number_value();
        part.position.y = entry["position_y"].number_value();
        part.rotation = entry["rotation"].number_value();
        part.size.x = entry["size_x"].number_value();
        part.size.y = entry["size_y"].number_value();
        part.color.hue = entry["color_h"].number_value();
        part.color.saturation = entry["color_s"].number_value();
        part.color.value = entry["color_v"].number_value();
        part.id = entry["id"].string_value();
        
        parts.push_back(part);
    }
}

void Prefab::save(sp::string filename)
{
    json11::Json::array prefab_data;
    for(Part& part : parts)
    {
        json11::Json::object data;
        switch(part.type)
        {
        case Prefab::Part::Type::Wall:
            data["type"] = "Wall";
            break;
        case Prefab::Part::Type::Floor:
            data["type"] = "Floor";
            break;
        case Prefab::Part::Type::Door:
            data["type"] = "Door";
            break;
        case Prefab::Part::Type::Trigger:
            data["type"] = "Trigger";
            break;
        case Prefab::Part::Type::PrefabConnection:
            data["type"] = "PrefabConnection";
            break;
        }
        data["position_x"] = part.position.x;
        data["position_y"] = part.position.y;
        data["rotation"] = part.rotation;
        data["size_x"] = part.size.x;
        data["size_y"] = part.size.y;
        data["color_h"] = part.color.hue;
        data["color_s"] = part.color.saturation;
        data["color_v"] = part.color.value;
        if (part.id.length() > 0)
            data["id"] = part.id;
        
        prefab_data.push_back(data);
    }
    json11::Json json = prefab_data;
    
    std::ofstream file(filename);
    file << json.dump();
}

std::map<sp::string, sp::P<sp::SceneNode>> Prefab::spawn(sp::Vector2d position, double rotation)
{
    std::map<sp::string, sp::P<sp::SceneNode>> id_map;
    sp::Quaterniond q = sp::Quaterniond::fromAngle(rotation);
    for(Part& part : parts)
    {
        sp::Vector2d p = position + q * part.position;
        double r = rotation + part.rotation;
        
        sp::P<sp::SceneNode> node;
        switch(part.type)
        {
        case Part::Type::Floor:
            node = new Floor(p, r, part.size, part.color);
            break;
        case Part::Type::Wall:
            node = new Wall(p, r, part.size, part.color);
            break;
        case Part::Type::Door:
            node = new Door(p, r, part.size, part.color);
            break;
        case Part::Type::Trigger:
            node = new Trigger(p, r, part.size);
            break;
        case Part::Type::PrefabConnection:
            break;
        }
        if (node && part.id.length() > 0)
            id_map[part.id] = node;
    }
    if (id_map["DoorTrigger"])
    {
        sp::P<Door> door1 = id_map["Door1"];
        sp::P<Door> door2 = id_map["Door2"];
        (sp::P<Trigger>(id_map["DoorTrigger"]))->onTriggerEnter = [door1, door2]()
        {
            door1->open();
            door2->open();
        };
        (sp::P<Trigger>(id_map["DoorTrigger"]))->onTriggerExit = [door1, door2]()
        {
            door1->close();
            door2->close();
        };
    }
    return id_map;
}

std::vector<Prefab::Part> Prefab::getConnections()
{
    std::vector<Part> connections;
    for(Part& part : parts)
    {
        if (part.type == Part::Type::PrefabConnection)
            connections.push_back(part);
    }
    return connections;
}

void Prefab::createPrototypes()
{
    for(Part& part : parts)
    {
        Prototype* p = new Prototype(part.type, part.position, part.rotation, part.size);
        p->setColor(part.color);
    }
}

void Prefab::updateFromPrototypes()
{
    parts.clear();
    
    for(sp::SceneNode* node : scene->getRoot()->getChildren())
    {
        sp::P<Prototype> prototype = sp::P<sp::SceneNode>(node);
        if (!prototype)
            continue;

        Part part;
        part.type = prototype->type;
        part.position = prototype->getGlobalPosition2D();
        part.rotation = prototype->getGlobalRotation2D();
        part.size = prototype->size;
        part.color = prototype->color;
        part.id = prototype->id;
        
        parts.push_back(part);
    }
}
