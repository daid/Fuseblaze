#include "weaponInfo.h"
#include <sp2/io/keyValueTreeLoader.h>
#include <unordered_map>

std::unordered_map<sp::string, WeaponInfo> weapons;

void WeaponInfo::init()
{
    sp::P<sp::KeyValueTree> tree = sp::io::KeyValueTreeLoader::load("weapons/definitions.txt");
    for(auto data : tree->getFlattenNodesByIds())
    {
        WeaponInfo info;
        info.name = data.first;
        info.auto_fire = data.second["auto_fire"] == "true";
        
        info.ammo_per_clip = data.second["ammo_per_clip"].toInt();
        info.cooldown_delay = data.second["cooldown_delay"].toFloat();
        info.reload_delay = data.second["reload_delay"].toFloat();

        info.projectile_count = data.second["projectile_count"].toInt();
        info.projectile_spread = data.second["projectile_spread"].toFloat();
        info.trace_shot = data.second["trace_shot"] == "true";
        info.pass_trough_enemies = data.second["pass_trough_enemies"] == "true";
        
        info.damage_per_hit = data.second["damage_per_hit"].toFloat();
        info.range = data.second["range"].toFloat();
    
        info.auto_aim = data.second["auto_aim"] == "true";
        
        float average_time_per_shot = (info.reload_delay + info.cooldown_delay * float(info.ammo_per_clip - 1)) / float(info.ammo_per_clip);
        float damage_per_shot = float(info.projectile_count) * info.damage_per_hit;
        float raw_dps = damage_per_shot / average_time_per_shot;
        if (!info.auto_fire)
            average_time_per_shot += 0.07; //when not using auto fire, the user still needs to press the button which adds some delay between shots.
        if (info.pass_trough_enemies)
            damage_per_shot *= 1.5;
        float guessed_dps = damage_per_shot / average_time_per_shot;
        float burst_dps = damage_per_shot / info.cooldown_delay;
        LOG(Debug, "Weapon:", info.name, "Raw DPS:", raw_dps, "Guessed DPS:", guessed_dps, "Burst DPS:", burst_dps);
        
        weapons[info.name] = info;
    }
    delete *tree;
}

WeaponInfo& WeaponInfo::get(sp::string name)
{
    return weapons[name];
}
